#include "CROAMTerrain.h"
#include "CImage.h"
#include "SUnit.h"
#include "ISceneManager.h"
#include "yonMath.h"

#include "ILogger.h"

namespace yon{
namespace scene{
namespace terrain{

	// Initialize a patch.
	void Patch::init(s32 heightX,s32 heightY,s32 worldX,s32 worldY,u8 *hMap)
	{
		// Clear all the relationships
		m_BaseLeft.rightNeighbor = m_BaseLeft.leftNeighbor = m_BaseRight.rightNeighbor = m_BaseRight.leftNeighbor =
			m_BaseLeft.leftChild = m_BaseLeft.rightChild = m_BaseRight.leftChild = m_BaseLeft.leftChild = NULL;

		// Attach the two m_Base triangles together
		m_BaseLeft.baseNeighbor = &m_BaseRight;
		m_BaseRight.baseNeighbor = &m_BaseLeft;

		// Store Patch offsets for the world and heightmap.
		m_WorldX = worldX;
		m_WorldY = worldY;

		// Store pointer to first byte of the height data for this patch.
		m_HeightMap = &hMap[heightY * MAP_SIZE + heightX];

		// Initialize flags
		m_VarianceDirty = 1;
		m_isVisible = false;
	}

	// Reset the patch.
	void Patch::reset()
	{
		// Assume patch is not visible.
		m_isVisible = false;

		// Reset the important relationships
		m_BaseLeft.leftChild = m_BaseLeft.rightChild = m_BaseRight.leftChild = m_BaseLeft.leftChild = NULL;

		// Attach the two m_Base triangles together
		m_BaseLeft.baseNeighbor = &m_BaseRight;
		m_BaseRight.baseNeighbor = &m_BaseLeft;

		// Clear the other relationships.
		m_BaseLeft.rightNeighbor = m_BaseLeft.leftNeighbor = m_BaseRight.rightNeighbor = m_BaseRight.leftNeighbor = NULL;
	}

	// Split a single Triangle and link it into the mesh.
	// Will correctly force-split diamonds.
	void Patch::split(TriTreeNode *tri)
	{
		// We are already split, no need to do it again.
		if (tri->leftChild)
			return;

		// If this triangle is not in a proper diamond, force split our base neighbor
		if ( tri->baseNeighbor && (tri->baseNeighbor->baseNeighbor != tri) )
			split(tri->baseNeighbor);

		// Create children and link into mesh
		tri->leftChild  = Landscape::allocateTri();
		tri->rightChild = Landscape::allocateTri();

		// If creation failed, just exit.
		if ( !tri->leftChild )
		{
			Logger->error(YON_LOG_FAILED_FORMAT,"Landscape::allocateTri");
			return;
		}

		// Fill in the information we can get from the parent (neighbor pointers)
		tri->leftChild->baseNeighbor  = tri->leftNeighbor;
		tri->leftChild->leftNeighbor  = tri->rightChild;

		tri->rightChild->baseNeighbor  = tri->rightNeighbor;
		tri->rightChild->rightNeighbor = tri->leftChild;

		// Link our Left Neighbor to the new children
		if (tri->leftNeighbor != NULL)
		{
			if (tri->leftNeighbor->baseNeighbor == tri)
				tri->leftNeighbor->baseNeighbor = tri->leftChild;
			else if (tri->leftNeighbor->leftNeighbor == tri)
				tri->leftNeighbor->leftNeighbor = tri->leftChild;
			else if (tri->leftNeighbor->rightNeighbor == tri)
				tri->leftNeighbor->rightNeighbor = tri->leftChild;
			else
				Logger->error(YON_LOG_FAILED_FORMAT,"Illegal Left Neighbor!");
		}

		// Link our Right Neighbor to the new children
		if (tri->rightNeighbor != NULL)
		{
			if (tri->rightNeighbor->baseNeighbor == tri)
				tri->rightNeighbor->baseNeighbor = tri->rightChild;
			else if (tri->rightNeighbor->rightNeighbor == tri)
				tri->rightNeighbor->rightNeighbor = tri->rightChild;
			else if (tri->rightNeighbor->leftNeighbor == tri)
				tri->rightNeighbor->leftNeighbor = tri->rightChild;
			else
				Logger->error(YON_LOG_FAILED_FORMAT,"Illegal Right Neighbor!");
		}

		// Link our Base Neighbor to the new children
		if (tri->baseNeighbor != NULL)
		{
			if ( tri->baseNeighbor->leftChild )
			{
				tri->baseNeighbor->leftChild->rightNeighbor = tri->rightChild;
				tri->baseNeighbor->rightChild->leftNeighbor = tri->leftChild;
				tri->leftChild->rightNeighbor = tri->baseNeighbor->rightChild;
				tri->rightChild->leftNeighbor = tri->baseNeighbor->leftChild;
			}
			else
				split( tri->baseNeighbor);  // Base Neighbor (in a diamond with us) was not split yet, so do that now.
		}
		else
		{
			// An edge triangle, trivial case.
			tri->leftChild->rightNeighbor = NULL;
			tri->rightChild->leftNeighbor = NULL;
		}
	}

	//TODO 替换为vector2d.crossProduct
	// Taken from "Programming Principles in Computer Graphics", L. Ammeraal (Wiley)
	inline s32 orientation(s32 pX,s32 pY,s32 qX,s32 qY,s32 rX,s32 rY)
	{
		s32 aX, aY, bX, bY;
		f32 d;

		aX = qX - pX;
		aY = qY - pY;

		bX = rX - pX;
		bY = rY - pY;

		d = (f32)aX * (f32)bY - (f32)aY * (f32)bX;
		return (d < 0) ? (-1) : (d > 0);
	}

	// Set patch's visibility flag.
	void Patch::setVisibility(s32 eyeX,s32 eyeY,s32 leftX,s32 leftY,s32 rightX,s32 rightY)
	{
		// Get patch's center point
		s32 patchCenterX = m_WorldX + PATCH_SIZE / 2;
		s32 patchCenterY = m_WorldY + PATCH_SIZE / 2;

		// Set visibility flag (orientation of both triangles must be counter clockwise)
		m_isVisible = (orientation(eyeX,eyeY,rightX,rightY,patchCenterX,patchCenterY ) < 0) &&
			(orientation(leftX,leftY,eyeX,eyeY,patchCenterX,patchCenterY ) < 0);
	}

	//完成LOD功能。在计算完到CAMERA的距离后，它调整当前节点的Variance值，以便于适应距离的变化。
	//它也可以让一个闭合的节点有一个比较大的Variance值。调整后的MESH将在近处使用比较多的三角形而在远处使用较少的三角形。
	//距离的计算使用了一个简单的平方根计算（他比较慢，我将用一个较快的方法来替换它）。
	// Tessellate a Patch.
	// Will continue to split until the variance metric is met.
	void Patch::recursTessellate(TriTreeNode *tri,s32 leftX,s32 leftY,s32 rightX,s32 rightY,s32 apexX,s32 apexY,s32 node)
	{
		f32 TriVariance;
		s32 centerX = (leftX + rightX)>>1; // Compute X coordinate of center of Hypotenuse
		s32 centerY = (leftY + rightY)>>1; // Compute Y coord...

		if(node<(1<<VARIANCE_DEPTH))
		{
			// Extremely slow distance metric (sqrt is used).
			// Replace this with a faster one!
			float distance = 1.0f + sqrtf( SQR((float)centerX - gViewPosition[0]) +SQR((float)centerY - gViewPosition[2]) );

			// Egads!  A division too?  What's this world coming to!
			// This should also be replaced with a faster operation.
			TriVariance = ((float)m_CurrentVariance[node] * MAP_SIZE * 2)/distance;	// Take both distance and variance into consideration
		}

		// IF we do not have variance info for this node, then we must have gotten here by splitting, so continue down to the lowest level.
		// OR if we are not below the variance tree, test for variance.
		if ( (node >= (1<<VARIANCE_DEPTH))||(TriVariance > gFrameVariance))	
		{
			// Split this triangle.
			split(tri);

			// If this triangle was split, try to split it's children as well.
			// Tessellate all the way down to one vertex per height field entry
			if (tri->leftChild &&((core::abs_(leftX-rightX) >= 3) || (core::abs_(leftY-rightY) >= 3)))	
			{
				recursTessellate( tri->leftChild,   apexX,  apexY, leftX, leftY, centerX, centerY,    node<<1  );
				recursTessellate( tri->rightChild, rightX, rightY, apexX, apexY, centerX, centerY, 1+(node<<1) );
			}
		}
	}

	// Create an approximate mesh.
	void Patch::tessellate()
	{
		// Split each of the base triangles
		m_CurrentVariance = m_VarianceLeft;
		recursTessellate (&m_BaseLeft,m_WorldX,m_WorldY+PATCH_SIZE,m_WorldX+PATCH_SIZE,m_WorldY,m_WorldX,m_WorldY,1);

		m_CurrentVariance = m_VarianceRight;
		recursTessellate(&m_BaseRight,m_WorldX+PATCH_SIZE,m_WorldY,m_WorldX,m_WorldY+PATCH_SIZE,m_WorldX+PATCH_SIZE,m_WorldY+PATCH_SIZE,1);
	}

	//用于获得当前三角形的所有坐标设置和我们保存在栈内的一部分扩展信息。三角的Variance值是和它的子三角一起合并计算的。
	//我选择通过传送每一个点的X和Y坐标而不是每点的高度值来减少在高度图数据数组的内存采样。
	// Computes Variance over the entire tree.  Does not examine node relationships.
	u8 Patch::recursComputeVariance(s32 leftX,s32 leftY,u8 leftZ,s32 rightX,s32 rightY,u8 rightZ,s32 apexX,s32 apexY,u8 apexZ,s32 node)
	{
		//        /|\
		//      /  |  \
		//    /    |    \
		//  /      |      \
		//  ~~~~~~~*~~~~~~~  <-- Compute the X and Y coordinates of '*'
		//
		s32 centerX = (leftX + rightX) >>1;		// Compute X coordinate of center of Hypotenuse
		s32 centerY = (leftY + rightY) >>1;		// Compute Y coord...
		u8 myVariance;

		// Get the height value at the middle of the Hypotenuse
		u8 centerZ  = m_HeightMap[(centerY * MAP_SIZE) + centerX];

		// Variance of this triangle is the actual height at it's hypotenuse midpoint minus the interpolated height.
		// Use values passed on the stack instead of re-accessing the Height Field.
		myVariance = core::abs_((s32)centerZ - (((s32)leftZ + (s32)rightZ)>>1));

		// Since we're after speed and not perfect representations,
		//    only calculate variance down to an 8x8 block
		if ((core::abs_(leftX - rightX) >= 8)||(core::abs_(leftY - rightY) >= 8) )
		{
			// Final Variance for this node is the max of it's own variance and that of it's children.
			myVariance = core::max_( myVariance, recursComputeVariance( apexX,   apexY,  apexZ, leftX, leftY, leftZ, centerX, centerY, centerZ,    node<<1 ) );
			myVariance = core::max_( myVariance, recursComputeVariance( rightX, rightY, rightZ, apexX, apexY, apexZ, centerX, centerY, centerZ, 1+(node<<1)) );
		}

		// Store the final variance for this node.  Note Variance is never zero.
		if (node < (1<<VARIANCE_DEPTH))
			m_CurrentVariance[node] = 1 + myVariance;

		return myVariance;
	}

	//这个函数非常的简单，但是你必须看一下在下面高级话题中的三角形排列优化技术。
	//简单的说来就是如果当前的三角形不是一个叶节点那么就把它重新并入到子节点中。
	//另外输出一个三角形使用了OPENGL，注意OPENGL渲染并没有被优化，这是为了使代码容易阅读。
	// Render the tree.  Simple no-fan method.
	void Patch::recursRender(TriTreeNode *tri, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY)
	{
		//All non-leaf nodes have both children, so just check for one
		if (tri->leftChild)
		{
			int centerX = (leftX + rightX)>>1;	//Compute X coordinate of center of Hypotenuse
			int centerY = (leftY + rightY)>>1;	//Compute Y coord...

			recursRender(tri->leftChild,  apexX,   apexY, leftX, leftY, centerX, centerY);
			recursRender(tri->rightChild, rightX, rightY, apexX, apexY, centerX, centerY);
		}
		//A leaf node!  Output a triangle to be rendered.
		else
		{
			//Actual number of rendered triangles...
			gNumTrisRendered++;

			f32 leftZ  = m_HeightMap[(leftY *MAP_SIZE)+leftX ];
			f32 rightZ = m_HeightMap[(rightY*MAP_SIZE)+rightX];
			f32 apexZ  = m_HeightMap[(apexY *MAP_SIZE)+apexX ];

			// Perform lighting calculations if requested.
			/*if (gDrawMode == DRAW_USE_LIGHTING)
			{
				float v[3][3];
				float out[3];

				// Create a vertex normal for this triangle.
				// NOTE: This is an extremely slow operation for illustration purposes only.
				//       You should use a texture map with the lighting pre-applied to the texture.
				v[0][0] = (GLfloat) leftX;
				v[0][1] = (GLfloat) leftZ;
				v[0][2] = (GLfloat) leftY;

				v[1][0] = (GLfloat) rightX;
				v[1][1] = (GLfloat) rightZ ;
				v[1][2] = (GLfloat) rightY;

				v[2][0] = (GLfloat) apexX;
				v[2][1] = (GLfloat) apexZ ;
				v[2][2] = (GLfloat) apexY;

				calcNormal( v, out );
				glNormal3fv( out );
			}*/

			// Perform polygon coloring based on a height sample
			float fColor = (60.0f + leftZ) / 256.0f;
			if ( fColor > 1.0f )  fColor = 1.0f;
			glColor3f( fColor, fColor, fColor );

			// Output the LEFT VERTEX for the triangle
			glVertex3f(		(GLfloat) leftX,
				(GLfloat) leftZ,
				(GLfloat) leftY );

			// Gaurad shading based on height samples instead of light normal
			if (gDrawMode == DRAW_USE_TEXTURE||gDrawMode == DRAW_USE_FILL_ONLY)
			{
				float fColor = (60.0f + rightZ) / 256.0f;
				if ( fColor > 1.0f )  fColor = 1.0f;
				glColor3f( fColor, fColor, fColor );
			}

			// Output the RIGHT VERTEX for the triangle
			glVertex3f((GLfloat) rightX,(GLfloat) rightZ,(GLfloat) rightY );

			// Gaurad shading based on height samples instead of light normal
			if (gDrawMode == DRAW_USE_TEXTURE||gDrawMode == DRAW_USE_FILL_ONLY )
			{
				float fColor = (60.0f + apexZ) / 256.0f;
				if ( fColor > 1.0f )  fColor = 1.0f;
				glColor3f( fColor, fColor, fColor );
			}

			// Output the APEX VERTEX for the triangle
			glVertex3f((GLfloat) apexX,(GLfloat) apexZ,(GLfloat) apexY );
		}
	}

	// Render the mesh.
	void Patch::render()
	{
		// Store old matrix
		glPushMatrix();

		// Translate the patch to the proper world coordinates
		glTranslatef( (GLfloat)m_WorldX, 0, (GLfloat)m_WorldY );
		glBegin(GL_TRIANGLES);

		recursRender(&m_BaseLeft,0,PATCH_SIZE,PATCH_SIZE,0,0,0);
		recursRender(&m_BaseRight,PATCH_SIZE,0,0,PATCH_SIZE,PATCH_SIZE,PATCH_SIZE);

		glEnd();
		// Restore the matrix
		glPopMatrix();
	}

	// Definition of the static member variables
	int         Landscape::m_NextTriNode;
	TriTreeNode Landscape::m_TriPool[POOL_SIZE];

	// Initialize all patches
	void Landscape::init(unsigned char *hMap)
	{
		Patch *patch;
		s32 X, Y;

		// Store the Height Field array
		m_HeightMap = hMap;

		// Initialize all terrain patches
		for ( Y=0; Y < NUM_PATCHES_PER_SIDE; Y++)
			for ( X=0; X < NUM_PATCHES_PER_SIDE; X++ )
			{
				patch = &(m_Patches[Y][X]);
				patch->init( X*PATCH_SIZE, Y*PATCH_SIZE, X*PATCH_SIZE, Y*PATCH_SIZE, hMap );
				patch->computeVariance();
			}
	}

	// Reset all patches, recompute variance if needed
	void Landscape::reset()
	{
		//
		// Perform simple visibility culling on entire patches.
		//   - Define a triangle set back from the camera by one patch size, following
		//     the angle of the frustum.
		//   - A patch is visible if it's center point is included in the angle: Left,Eye,Right
		//   - This visibility test is only accurate if the camera cannot look up or down significantly.
		//
		const float PI_DIV_180 = M_PI / 180.0f;
		const float FOV_DIV_2 = gFovX/2;

		int eyeX = (int)(gViewPosition[0] - PATCH_SIZE * sinf( gClipAngle * PI_DIV_180 ));
		int eyeY = (int)(gViewPosition[2] + PATCH_SIZE * cosf( gClipAngle * PI_DIV_180 ));

		int leftX  = (int)(eyeX + 100.0f * sinf( (gClipAngle-FOV_DIV_2) * PI_DIV_180 ));
		int leftY  = (int)(eyeY - 100.0f * cosf( (gClipAngle-FOV_DIV_2) * PI_DIV_180 ));

		int rightX = (int)(eyeX + 100.0f * sinf( (gClipAngle+FOV_DIV_2) * PI_DIV_180 ));
		int rightY = (int)(eyeY - 100.0f * cosf( (gClipAngle+FOV_DIV_2) * PI_DIV_180 ));

		int X, Y;
		Patch *patch;

		// Set the next free triangle pointer back to the beginning
		SetNextTriNode(0);

		// Reset rendered triangle count.
		gNumTrisRendered = 0;

		// Go through the patches performing resets, compute variances, and linking.
		for ( Y=0; Y < NUM_PATCHES_PER_SIDE; Y++ )
			for ( X=0; X < NUM_PATCHES_PER_SIDE; X++)
			{
				patch = &(m_Patches[Y][X]);

				// Reset the patch
				patch->Reset();
				patch->SetVisibility( eyeX, eyeY, leftX, leftY, rightX, rightY );

				// Check to see if this patch has been deformed since last frame.
				// If so, recompute the varience tree for it.
				if ( patch->isDirty() )
					patch->ComputeVariance();

				if ( patch->isVisibile() )
				{
					// Link all the patches together.
					if ( X > 0 )
						patch->GetBaseLeft()->LeftNeighbor = m_Patches[Y][X-1].GetBaseRight();
					else
						patch->GetBaseLeft()->LeftNeighbor = NULL;		// Link to bordering Landscape here..

					if ( X < (NUM_PATCHES_PER_SIDE-1) )
						patch->GetBaseRight()->LeftNeighbor = m_Patches[Y][X+1].GetBaseLeft();
					else
						patch->GetBaseRight()->LeftNeighbor = NULL;		// Link to bordering Landscape here..

					if ( Y > 0 )
						patch->GetBaseLeft()->RightNeighbor = m_Patches[Y-1][X].GetBaseRight();
					else
						patch->GetBaseLeft()->RightNeighbor = NULL;		// Link to bordering Landscape here..

					if ( Y < (NUM_PATCHES_PER_SIDE-1) )
						patch->GetBaseRight()->RightNeighbor = m_Patches[Y+1][X].GetBaseLeft();
					else
						patch->GetBaseRight()->RightNeighbor = NULL;	// Link to bordering Landscape here..
				}
			}

	}


	// Create an approximate mesh of the landscape.
	void Landscape::tessellate()
	{
		// Perform Tessellation
		int nCount;
		Patch *patch = &(m_Patches[0][0]);
		for (nCount=0; nCount < NUM_PATCHES_PER_SIDE*NUM_PATCHES_PER_SIDE; nCount++, patch++ )
		{
			if (patch->isVisibile())
				patch->tessellate( );
		}
	}

	// Allocate a TriTreeNode from the pool.
	TriTreeNode *Landscape::allocateTri()
	{
		TriTreeNode *pTri;

		// IF we've run out of TriTreeNodes, just return NULL (this is handled gracefully)
		if ( m_NextTriNode >= POOL_SIZE )
			return NULL;

		pTri = &(m_TriPool[m_NextTriNode++]);
		pTri->leftChild = pTri->rightChild = NULL;

		return pTri;
	}

	// Render each patch of the landscape & adjust the frame variance.
	void Landscape::Render()
	{
		int nCount;
		Patch *patch = &(m_Patches[0][0]);

		// Scale the terrain by the terrain scale specified at compile time.
		glScalef( 1.0f, MULT_SCALE, 1.0f );

		for (nCount=0; nCount < NUM_PATCHES_PER_SIDE*NUM_PATCHES_PER_SIDE; nCount++, patch++ )
		{
			if (patch->isVisibile())
				patch->Render();
		}

		// Check to see if we got close to the desired number of triangles.
		// Adjust the frame variance to a better value.
		if (GetNextTriNode() != gDesiredTris)
			gFrameVariance += ((float)GetNextTriNode() - (float)gDesiredTris) / (float)gDesiredTris;

		// Bounds checking.
		if (gFrameVariance < 0)
			gFrameVariance = 0;
	}

	CROAMTerrain::CROAMTerrain(IModel* parent,const core::vector3df& pos,
		const core::vector3df& rot,const core::vector3df& scale)
		:ITerrainModel(parent,pos,rot,scale),m_iSizePerSide(0),m_iImageSizePerSide(0)
	{
		m_pUnit=new Unit3D2T();
		//m_pUnit->setVertexHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC);
		//m_pUnit->setIndexHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_DYNAMIC);
		//m_pUnit->getMaterial()->setFrontFace(video::ENUM_FRONT_FACE_CW);
		m_pUnit->getMaterial()->setPolygonMode(video::ENUM_POLYGON_MODE_LINE);
		m_pUnit->setShap(&m_shap);
	}

	CROAMTerrain::~CROAMTerrain(){
		m_pUnit->drop();
	}


	void CROAMTerrain::loadHeightMap(video::IImage* image,ENUM_PATCH_SIZE patchSize)
	{
		if(image==NULL)
			return;
		YON_DEBUG_BREAK_IF(image->getDimension().w!=image->getDimension().h);

		m_iImageSizePerSide=image->getDimension().w;
		m_iSizePerSide=(core::nearestPowerOf2(m_iImageSizePerSide)>>1)+1;

		Logger->debug("image.w:%d,sizePerSide:%d\r\n",m_iImageSizePerSide,m_iSizePerSide);

		const u32 numVertices=m_iSizePerSide*m_iSizePerSide;
		m_shap.getVertexArray().set_used(numVertices);

		const f32 texcoordDelta = 1.0f/(f32)(m_iImageSizePerSide-1);
		const f32 texcoordDelta2 = 1.0f/(f32)(patchSize-1);
		s32 index=0;
		f32 fx=0.f;
		f32 fv=0.f;
		f32 fv2=0.f;
		for(s32 x = 0; x<m_iSizePerSide; ++x)
		{
			f32 fz=0.f;
			f32 fu=0.f;
			f32 fu2=0.f;
			for(s32 z = 0; z<m_iSizePerSide; ++z)
			{
				SVertex2TCoords& v=m_shap.getVertexArray()[index++];
				v.pos.x=fx*m_scale.x;
				v.pos.y=(f32)image->getValue(z,x)*m_scale.y;
				v.pos.z=fz*m_scale.z;

				v.texcoords0.x=fu;
				v.texcoords0.y=fv;

				v.texcoords1.x=fu2;
				v.texcoords1.y=fv2;

				//Logger->debug("vertex[%d]:%.2f,%.2f,%.2f\r\n",index-1,v.pos.x,v.pos.y,v.pos.z);

				++fz;
				fu+=texcoordDelta;
				fu2+=texcoordDelta2;
			}
			++fx;
			fv+=texcoordDelta;
			fv2+=texcoordDelta2;
		}
	}
}
}
}