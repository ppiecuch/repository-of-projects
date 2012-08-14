#include "CROAMTerrain.h"
#include "CImage.h"
#include "SUnit.h"
#include "ISceneManager.h"
#include "yonMath.h"

#include "ILogger.h"

namespace yon{
namespace scene{
namespace terrain{

	const s32 CROAMTerrain::POOL_SIZE=25000;
	s32  CROAMTerrain::m_NextTriNode=0;
	CROAMTerrain::TriTreeNode CROAMTerrain::m_TriPool[POOL_SIZE];

	// Beginning frame varience (should be high, it will adjust automatically)
	f32 gFrameVariance = 50.f;

	//TODO 不使用期望值，这会导致闪烁的问题
	// Desired number of Binary Triangle tessellations per frame.
	// This is not the desired number of triangles rendered!
	// There are usually twice as many Binary Triangle structures as there are rendered triangles.
	int gDesiredTris = 6000;


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

	f32 CROAMTerrain::calculateL1Norm(const core::vector3df& a,const core::vector3df& b){
		return core::abs_(a.x-b.x)+core::abs_(a.y-b.y)+core::abs_(a.z-b.z);
	}
	

	CROAMTerrain::CROAMTerrain(IModel* parent,const core::vector3df& pos,
		const core::vector3df& rot,const core::vector3df& scale)
		:ITerrainModel(parent,pos,rot,scale),m_iMapSize(0),m_iImageSizePerSide(0),
		m_iNumPatchPerSide(0),m_iPatchSize(0),m_aPatches(NULL),m_iVarianceDepth(9)
		//,m_triTreeNodePool(TriTreeNodePool(10))
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
		if(m_aPatches!=NULL)
		{
			for(s32 x=0;x<m_iNumPatchPerSide;++x)
			{
				for(s32 z=0;z<m_iNumPatchPerSide;++z)
					delete m_aPatches[x][z];
				delete[] m_aPatches[x];
			}
			delete[] m_aPatches;
			m_aPatches=NULL;
		}
	}

	void CROAMTerrain::reset()
	{
		/*
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
		//SetNextTriNode(0);
		m_NextTriNode=0;

		// Reset rendered triangle count.
		gNumTrisRendered = 0;

		// Go through the patches performing resets, compute variances, and linking.
		for ( Y=0; Y < m_iNumPatchPerSide; Y++ )
		{
			for ( X=0; X < m_iNumPatchPerSide; X++)
			{
				patch = &(m_aPatches[Y][X]);

				// Reset the patch
				patch->Reset();
				patch->SetVisibility( eyeX, eyeY, leftX, leftY, rightX, rightY );

				// Check to see if this patch has been deformed since last frame.
				// If so, recompute the varience tree for it.
				if ( patch->isDirty() )
					patch->computeVariance();

				if ( patch->isVisibile() )
				{
					// Link all the patches together.
					if ( X > 0 )
						patch->m_baseLeft->leftNeighbor = m_aPatches[Y][X-1].m_baseRight;
					else
						patch->m_baseLeft->leftNeighbor = NULL;		// Link to bordering Landscape here..

					if ( X < (m_iNumPatchPerSide-1) )
						patch->m_BaseRight->leftNeighbor = m_aPatches[Y][X+1].m_baseLeft;
					else
						patch->m_BaseRight->leftNeighbor = NULL;		// Link to bordering Landscape here..

					if ( Y > 0 )
						patch->m_baseLeft->rightNeighbor = m_aPatches[Y-1][X].m_baseRight;
					else
						patch->m_baseLeft->rightNeighbor = NULL;		// Link to bordering Landscape here..

					if ( Y < (m_iNumPatchPerSide-1) )
						patch->m_BaseRight->rightNeighbor = m_aPatches[Y+1][X].m_baseLeft;
					else
						patch->m_BaseRight->rightNeighbor = NULL;	// Link to bordering Landscape here..
				}
			}
		}
		*/
		//TODO culling
		m_NextTriNode=0;
		m_shap.getIndexArray().reallocate(0);

		camera::ICamera* camera=m_pSceneManager->getLogisticCamera();
		if(camera==NULL)
			camera=m_pSceneManager->getViewingCamera();
		m_currentCameraPos = camera->getAbsolutePosition();
		m_currentCameraPos/=m_scale;

		//Logger->debug("camPos:%.2f,%.2f,%.2f\r\n",m_currentCameraPos.x,m_currentCameraPos.y,m_currentCameraPos.z);

		SPatch *patch;
		for(s32 x=0;x<m_iNumPatchPerSide;++x)
		{
			for(s32 z=0;z<m_iNumPatchPerSide;++z)
			{
				patch=m_aPatches[x][z];
				patch->reset();
				//patch->SetVisibility( eyeX, eyeY, leftX, leftY, rightX, rightY );
				patch->setVisible(true);
				// Check to see if this patch has been deformed since last frame.
				// If so, recompute the varience tree for it.
				if(patch->isDirty())
					computeVariance(patch);

				if(patch->isVisible())
				{
					// Link all the patches together.
					if ( x>0 )
						patch->m_baseLeft.leftNeighbor = &m_aPatches[x-1][z]->m_baseRight;
					else
						patch->m_baseLeft.leftNeighbor = NULL;		// Link to bordering Landscape here..

					if ( x<(m_iNumPatchPerSide-1) )
						patch->m_baseRight.leftNeighbor = &m_aPatches[x+1][z]->m_baseLeft;
					else
						patch->m_baseRight.leftNeighbor = NULL;		// Link to bordering Landscape here..

					if ( z>0 )
						patch->m_baseLeft.rightNeighbor = &m_aPatches[x][z-1]->m_baseRight;
					else
						patch->m_baseLeft.rightNeighbor = NULL;		// Link to bordering Landscape here..

					if ( z<(m_iNumPatchPerSide-1) )
						patch->m_baseRight.rightNeighbor = &m_aPatches[x][z+1]->m_baseLeft;
					else
						patch->m_baseRight.rightNeighbor = NULL;	// Link to bordering Landscape here..
				}
			}
		}
	}

	// Split a single Triangle and link it into the mesh.
	// Will correctly force-split diamonds.
	void CROAMTerrain::split(TriTreeNode *tri)
	{
		// We are already split, no need to do it again.
		if (tri->leftChild)
			return;

		// If this triangle is not in a proper diamond, force split our base neighbor
		if ( tri->baseNeighbor && (tri->baseNeighbor->baseNeighbor != tri) )
			split(tri->baseNeighbor);

		// Create children and link into mesh
		tri->leftChild  = allocateTri();
		tri->rightChild = allocateTri();
		//tri->leftChild  = m_triTreeNodePool.get();
		//tri->rightChild = m_triTreeNodePool.get();

		// If creation failed, just exit.
		if ( !tri->leftChild )
		{
			Logger->error(YON_LOG_FAILED_FORMAT,"allocateTri");
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

	//完成LOD功能。在计算完到CAMERA的距离后，它调整当前节点的Variance值，以便于适应距离的变化。
	//它也可以让一个闭合的节点有一个比较大的Variance值。调整后的MESH将在近处使用比较多的三角形而在远处使用较少的三角形。
	//距离的计算使用了一个简单的平方根计算（他比较慢，我将用一个较快的方法来替换它）。
	// Tessellate a Patch.
	// Will continue to split until the variance metric is met.
	void CROAMTerrain::recursTessellate(SPatch* patch,TriTreeNode *tri,s32 leftX,s32 leftZ,s32 rightX,s32 rightZ,s32 apexX,s32 apexZ,s32 node)
	{
		f32 TriVariance;
		s32 centerX = (leftX + rightX)>>1; // Compute X coordinate of center of Hypotenuse
		s32 centerZ = (leftZ + rightZ)>>1; // Compute Y coord...

		//core::position3df& pos=m_shap.getVertexArray()[centerX*m_iPatchSize+centerY+patch->m_uOffset].pos;

		if(node<(1<<m_iVarianceDepth))
		{
			//TODO 优化

			// Extremely slow distance metric (sqrt is used).
			// Replace this with a faster one!
			//f32 distance = 1.0f + core::squareroot(core::square((f32)centerX - m_currentCameraPos.x)+core::square(m_currentCameraPos.y)+core::square((f32)centerY - m_currentCameraPos.z));
			f32 distance = 1.0f + core::squareroot(core::square(centerX - m_currentCameraPos.x)+core::square(centerZ - m_currentCameraPos.z));
			//f32 distance=calculateL1Norm(pos,m_currentCameraPos);

			// Egads!  A division too?  What's this world coming to!
			// This should also be replaced with a faster operation.
			TriVariance = ((f32)patch->m_pCurrentVariance[node] * m_iMapSize * 2)/distance;	// Take both distance and variance into consideration

			//if(patch->m_uIndex==0)
			//	Logger->debug("center:%d,%d,distance:%.2f,camPos:%.2f,%.2f,%.2f,TriVariance:%.2f\r\n",centerX,centerZ,distance,m_currentCameraPos.x,m_currentCameraPos.y,m_currentCameraPos.z,TriVariance);
		}

		//why?
		//node >= (1<<m_iVarianceDepth)这条件应该可以不用，测试不存在差别

		// IF we do not have variance info for this node, then we must have gotten here by splitting, so continue down to the lowest level.
		// OR if we are not below the variance tree, test for variance.
		//if ( (node >= (1<<m_iVarianceDepth))||(TriVariance > gFrameVariance))
		if (TriVariance > gFrameVariance)
		{
			// Split this triangle.
			split(tri);

			//why 3?
			//answer:like recursComputeVariance function, but here we need tessellate the children so we have to increase 2 to 3.

			// If this triangle was split, try to split it's children as well.
			// Tessellate all the way down to one vertex per height field entry
			if (tri->leftChild &&((core::abs_(leftX-rightX) >= 3) || (core::abs_(leftZ-rightZ) >= 3)))	
			{
				recursTessellate(patch, tri->leftChild,   apexX,  apexZ, leftX, leftZ, centerX, centerZ,    node<<1  );
				recursTessellate(patch, tri->rightChild, rightX, rightZ, apexX, apexZ, centerX, centerZ, 1+(node<<1) );
			}
		}
	}

	// Create an approximate mesh.
	void CROAMTerrain::tessellatePatch(SPatch* patch)
	{
		s32 x=patch->m_uIndexX*m_iPatchSize;
		s32 z=patch->m_uIndexZ*m_iPatchSize;
		// Split each of the base triangles
		//∣
		//└--
		patch->m_pCurrentVariance = patch->m_pVarianceLeft;
		recursTessellate(patch,&patch->m_baseLeft,x,z+m_iPatchSize,x+m_iPatchSize,z,x,z,1);

		//--┐
		//  ∣
		patch->m_pCurrentVariance = patch->m_pVarianceRight;
		recursTessellate(patch,&patch->m_baseRight,x+m_iPatchSize,z,x,z+m_iPatchSize,x+m_iPatchSize,z+m_iPatchSize,1);
	}

	void CROAMTerrain::tessellate()
	{
		// Perform Tessellation
		s32 index=0;
		for(s32 x=0;x<m_iNumPatchPerSide;++x)
		{
			for(s32 z=0;z<m_iNumPatchPerSide;++z)
			{
				if(m_aPatches[x][z]->isVisible())
					tessellatePatch(m_aPatches[x][z]);
			}
		}
	}

	//u8 CROAMTerrain::recursComputeVariance(s32 leftIndex,u8 leftHeight, s32 rightIndex,u8 rightHeight,s32 apexIndex,u8 apexHeight, s32 nodeIndex)
	u8 CROAMTerrain::recursComputeVariance(SPatch* patch,s32 leftX,s32 leftZ,u8 leftY,s32 rightX,s32 rightZ,u8 rightY,s32 apexX,s32 apexZ,u8 apexY,s32 node)
	{
		/*
		s32 centerIndex=(leftIndex+rightIndex)>>1;
		u8 maxVariance;

		// Get the height value at the middle of the Hypotenuse
		u8 centerHeight=m_pHeightMap[centerIndex];

		// Variance of this triangle is the actual height at it's hypotenuse midpoint minus the interpolated height.
		// Use values passed on the stack instead of re-accessing the Height Field.
		maxVariance = (u8)core::abs_((s32)centerHeight - (((s32)leftHeight + (s32)rightHeight)>>1));

		// Since we're after speed and not perfect representations, only calculate variance down to an 8x8 block
		if ((core::abs_(leftX - rightX) >= 8)||(core::abs_(leftY - rightY) >= 8) )
		{
			// Final Variance for this node is the max of it's own variance and that of it's children.
			maxVariance = core::max_( maxVariance, recursComputeVariance(apexIndex,apexHeight,leftIndex,leftHeight,centerIndex,centerHeight,nodeIndex<<1));
			maxVariance = core::max_( maxVariance, recursComputeVariance(rightIndex,rightHeight,apexIndex,apexHeight,centerIndex,centerHeight,1+(nodeIndex<<1)));
		}

		// Store the final variance for this node.  Note Variance is never zero.
		if (nodeIndex<(1<<VARIANCE_DEPTH))
			m_pCurrentVariance[nodeIndex] = 1 + maxVariance;

		return maxVariance;
		*/

		//        /|\
		//      /  |  \
		//    /    |    \
		//  /      |      \
		//  ~~~~~~~*~~~~~~~  <-- Compute the X and Y coordinates of '*'
		//
		s32 centerX = (leftX + rightX) >>1;		// Compute X coordinate of center of Hypotenuse
		s32 centerZ = (leftZ + rightZ) >>1;		// Compute Y coord...
		u8 myVariance;

		// Get the height value at the middle of the Hypotenuse
		u8 centerY  = m_heightMap[(centerX*m_iMapSize)+centerZ+patch->m_uOffset];

		// Variance of this triangle is the actual height at it's hypotenuse midpoint minus the interpolated height.
		// Use values passed on the stack instead of re-accessing the Height Field.
		myVariance = core::abs_((s32)centerY - (((s32)leftY + (s32)rightY)>>1));

		//不能使用Mahattan距离，因为LODmax与LOD(max-1)两者的Mahattan距离都是2
		//注意这里顶点的递归关系
		// Since we're after speed and not perfect representations,
		//    only calculate variance down to an 8x8 block
		//if (core::abs_(leftX - rightX)+core::abs_(leftZ - rightZ) > 2 )
		if ((core::abs_(leftX - rightX) >= 2)||(core::abs_(leftZ - rightZ) >= 2) )
		{
			//Logger->debug("node:%d,%d,%d,{%d,%d,%d}\r\n",node,node<<1,1+(node<<1),getIndex(patch,leftX,leftZ),getIndex(patch,rightX,rightZ),getIndex(patch,apexX,apexZ));
			// Final Variance for this node is the max of it's own variance and that of it's children.
			myVariance = core::max_(myVariance, recursComputeVariance(patch, apexX,   apexZ,  apexY, leftX, leftZ, leftY, centerX, centerZ, centerY,    node<<1 ));
			myVariance = core::max_(myVariance, recursComputeVariance(patch, rightX, rightZ, rightY, apexX, apexZ, apexY, centerX, centerZ, centerY, 1+(node<<1)));
		}

		//TODO 冗余了吧，如果只在node<(1<<m_iVarianceDepth)时才记录，那么应该将myVariance的计算部分也包进去才是最优
		// Store the final variance for this node.  Note Variance is never zero.
		if (node<(1<<m_iVarianceDepth))
		{
			patch->m_pCurrentVariance[node] = 1 + myVariance;
			//temp
			patch->m_pCurrentVariance[node] = core::max_(patch->m_pCurrentVariance[node],(u8)5);
			//patch->m_pCurrentVariance[node] = 5;

			//if(patch->m_pCurrentVariance==patch->m_pVarianceLeft)
			//	Logger->debug("Patch[%d]:LeftVariance[%d]=%d{%d,%d,%d}\r\n",patch->m_uIndex,node,patch->m_pCurrentVariance[node],getIndex(patch,leftX,leftZ),getIndex(patch,rightX,rightZ),getIndex(patch,apexX,apexZ));
			//else
			//	Logger->debug("Patch[%d]:RightVariance[%d]=%d{%d,%d,%d}\r\n",patch->m_uIndex,node,patch->m_pCurrentVariance[node],getIndex(patch,leftX,leftZ),getIndex(patch,rightX,rightZ),getIndex(patch,apexX,apexZ));
		}

		return myVariance;
	}

	void CROAMTerrain::computeVariance(SPatch* patch)
	{
		// Compute variance on each of the base triangles...
		//∣
		//└--
		//why node index start from 1,answer: for detail refer to "Tree Traversal Function"
		patch->m_pCurrentVariance = patch->m_pVarianceLeft;
		recursComputeVariance(patch,0,m_iPatchSize,m_heightMap[m_iPatchSize*m_iMapSize+patch->m_uOffset],m_iPatchSize,0,m_heightMap[m_iPatchSize+patch->m_uOffset],0,0,m_heightMap[0+patch->m_uOffset],1);

		//--┐
		//  ∣
		patch->m_pCurrentVariance = patch->m_pVarianceRight;
		recursComputeVariance(patch,m_iPatchSize,0,m_heightMap[m_iPatchSize+patch->m_uOffset],0,m_iPatchSize,m_heightMap[m_iPatchSize*m_iMapSize+patch->m_uOffset],m_iPatchSize, m_iPatchSize,m_heightMap[(m_iPatchSize * m_iMapSize)+m_iPatchSize+patch->m_uOffset],1);

		// Clear the dirty flag for this patch
		patch->m_varianceDirty = false;
	}

	//这个函数非常的简单，但是你必须看一下在下面高级话题中的三角形排列优化技术。
	//简单的说来就是如果当前的三角形不是一个叶节点那么就把它重新并入到子节点中。
	//另外输出一个三角形使用了OPENGL，注意OPENGL渲染并没有被优化，这是为了使代码容易阅读。
	// Render the tree.  Simple no-fan method.
	void CROAMTerrain::recursRender(SPatch* patch,TriTreeNode *tri, s32 leftX, s32 leftZ, s32 rightX, s32 rightZ, s32 apexX, s32 apexZ)
	{
		/*
		if ( tri->LeftChild )					// All non-leaf nodes have both children, so just check for one
		{
			int centerX = (leftX + rightX)>>1;	// Compute X coordinate of center of Hypotenuse
			int centerY = (leftY + rightY)>>1;	// Compute Y coord...

			RecursRender( tri->LeftChild,  apexX,   apexY, leftX, leftY, centerX, centerY );
			RecursRender( tri->RightChild, rightX, rightY, apexX, apexY, centerX, centerY );
		}
		else									// A leaf node!  Output a triangle to be rendered.
		{
			// Actual number of rendered triangles...
			gNumTrisRendered++;

			GLfloat leftZ  = m_HeightMap[(leftY *MAP_SIZE)+leftX ];
			GLfloat rightZ = m_HeightMap[(rightY*MAP_SIZE)+rightX];
			GLfloat apexZ  = m_HeightMap[(apexY *MAP_SIZE)+apexX ];

			// Perform lighting calculations if requested.
			if (gDrawMode == DRAW_USE_LIGHTING)
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
			}

			// Perform polygon coloring based on a height sample
			float fColor = (60.0f + leftZ) / 256.0f;
			if ( fColor > 1.0f )  fColor = 1.0f;
			glColor3f( fColor, fColor, fColor );

			// Output the LEFT VERTEX for the triangle
			glVertex3f(		(GLfloat) leftX,
				(GLfloat) leftZ,
				(GLfloat) leftY );

			if ( gDrawMode == DRAW_USE_TEXTURE ||	// Gaurad shading based on height samples instead of light normal
				gDrawMode == DRAW_USE_FILL_ONLY )
			{
				float fColor = (60.0f + rightZ) / 256.0f;
				if ( fColor > 1.0f )  fColor = 1.0f;
				glColor3f( fColor, fColor, fColor );
			}

			// Output the RIGHT VERTEX for the triangle
			glVertex3f(		(GLfloat) rightX,
				(GLfloat) rightZ,
				(GLfloat) rightY );


			if ( gDrawMode == DRAW_USE_TEXTURE ||	// Gaurad shading based on height samples instead of light normal
				gDrawMode == DRAW_USE_FILL_ONLY )
			{
				float fColor = (60.0f + apexZ) / 256.0f;
				if ( fColor > 1.0f )  fColor = 1.0f;
				glColor3f( fColor, fColor, fColor );
			}

			// Output the APEX VERTEX for the triangle
			glVertex3f(		(GLfloat) apexX,
				(GLfloat) apexZ,
				(GLfloat) apexY );
		}
		*/
		if ( tri->leftChild )					// All non-leaf nodes have both children, so just check for one
		{
			int centerX = (leftX + rightX)>>1;	// Compute X coordinate of center of Hypotenuse
			int centerZ = (leftZ + rightZ)>>1;	// Compute Y coord...

			recursRender(patch, tri->leftChild,  apexX,   apexZ, leftX, leftZ, centerX, centerZ );
			recursRender(patch, tri->rightChild, rightX, rightZ, apexX, apexZ, centerX, centerZ );
		}
		else									// A leaf node!  Output a triangle to be rendered.
		{
			u16 left=(u16)(leftX*m_iMapSize+leftZ+patch->m_uOffset);
			u16 right=(u16)(rightX*m_iMapSize+rightZ+patch->m_uOffset);
			u16 apex=(u16)(apexX*m_iMapSize+apexZ+patch->m_uOffset);

			//Logger->debug("left,right,apex:%d,%d,%d\r\n",left,right,apex);

			core::array<u16>& indices=m_shap.getIndexArray();
			if(m_pUnit->getMaterial()->getPolygonMode()==video::ENUM_POLYGON_MODE_LINE)
			{
				indices.push_back(left);
				indices.push_back(right);
				indices.push_back(left);
				indices.push_back(apex);
				indices.push_back(right);
				indices.push_back(apex);
			}
			else
			{
				indices.push_back(left);
				indices.push_back(right);
				indices.push_back(apex);
			}
		}
	}

	void CROAMTerrain::renderPatch(SPatch* patch)
	{
		/*
		// Store old matrix
		glPushMatrix();

		// Translate the patch to the proper world coordinates
		glTranslatef( (GLfloat)m_WorldX, 0, (GLfloat)m_WorldY );
		glBegin(GL_TRIANGLES);

		recursRender(&m_BaseLeft,0,m_iPatchSize,m_iPatchSize,0,0,0);
		recursRender(&m_BaseRight,m_iPatchSize,0,0,m_iPatchSize,m_iPatchSize,m_iPatchSize);

		glEnd();
		// Restore the matrix
		glPopMatrix();
		*/
		recursRender(patch,&patch->m_baseLeft,0,m_iPatchSize,m_iPatchSize,0,0,0);
		recursRender(patch,&patch->m_baseRight,m_iPatchSize,0,0,m_iPatchSize,m_iPatchSize,m_iPatchSize);

		// Check to see if we got close to the desired number of triangles.
		// Adjust the frame variance to a better value.
		//if ( m_NextTriNode != gDesiredTris )
		//	gFrameVariance += ((float)m_NextTriNode - (float)gDesiredTris) / (float)gDesiredTris;

		// Bounds checking.
		//if ( gFrameVariance < 0 )
		//	gFrameVariance = 0;

		//Logger->debug("m_NextTriNode:%d,gFrameVariance:%.2f\r\n",m_NextTriNode,gFrameVariance);

	}

	void CROAMTerrain::preRender()
	{
		for(s32 x=0;x<m_iNumPatchPerSide;++x)
		{
			for(s32 z=0;z<m_iNumPatchPerSide;++z)
			{
				if(m_aPatches[x][z]->isVisible())
					renderPatch(m_aPatches[x][z]);
			}
		}

		//Logger->debug("indices:%d\r\n",m_shap.getIndexArray().size());
	}


	void CROAMTerrain::loadHeightMap(video::IImage* image,ENUM_PATCH_SIZE patchSize)
	{
		if(image==NULL)
			return;
		YON_DEBUG_BREAK_IF(image->getDimension().w!=image->getDimension().h);

		m_iImageSizePerSide=image->getDimension().w;
		m_iMapSize=(core::nearestPowerOf2(m_iImageSizePerSide)>>1)+1;
		m_iPatchSize=patchSize-1;
		m_iNumPatchPerSide=m_iMapSize/m_iPatchSize;

		Logger->debug("image.w:%d,mapSize:%d,patchSize:%d,patchNumPerSide:%d\r\n",m_iImageSizePerSide,m_iMapSize,m_iPatchSize,m_iNumPatchPerSide);

		const u32 numVertices=m_iMapSize*m_iMapSize;
		m_shap.getVertexArray().set_used(numVertices);
		m_heightMap.set_used(numVertices);

		video::SColor red(0xFF800080);
		video::SColor green(0xFF808000);
		video::SColor blue(0xFF008080);

		const f32 texcoordDelta = 1.0f/(f32)(m_iImageSizePerSide-1);
		const f32 texcoordDelta2 = 1.0f/(f32)(m_iPatchSize);
		s32 index=0;
		f32 fx=0.f;
		f32 fv=0.f;
		f32 fv2=0.f;
		for(s32 x = 0; x<m_iMapSize; ++x)
		{
			f32 fz=0.f;
			f32 fu=0.f;
			f32 fu2=0.f;
			for(s32 z = 0; z<m_iMapSize; ++z)
			{
				u8 height=image->getValue(z,x);
				m_heightMap[index]=height;

				SVertex2TCoords& v=m_shap.getVertexArray()[index++];
				v.pos.x=fx*m_scale.x;
				v.pos.y=(f32)height*m_scale.y;
				v.pos.z=fz*m_scale.z;

				if(((x/m_iPatchSize)+(z/m_iPatchSize))%3==0)
					v.color=red;
				else if(((x/m_iPatchSize)+(z/m_iPatchSize))%3==1)
					v.color=green;
				else
					v.color=blue;

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

		m_aPatches=(SPatch***)new SPatch**[m_iNumPatchPerSide];
		index=0;
		for(s32 x=0;x<m_iNumPatchPerSide;++x)
		{
			m_aPatches[x]=(SPatch**)new SPatch*[m_iNumPatchPerSide];
			for(s32 z=0;z<m_iNumPatchPerSide;++z)
			{
				m_aPatches[x][z]=new SPatch(m_iVarianceDepth);
				//m_aPatches[x][z]->init(x*m_iPatchSize,z*m_iPatchSize,x*m_iPatchSize,z*m_iPatchSize, hMap );
				m_aPatches[x][z]->init(x,z,index++,x*m_iPatchSize*m_iMapSize+z*m_iPatchSize);
				computeVariance(m_aPatches[x][z]);
			}
		}
	}

	void CROAMTerrain::render(video::IVideoDriver* driver)
	{
		/*
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
		*/
		//driver->setTransform(video::ENUM_TRANSFORM_WORLD, getAbsoluteTransformation());

		driver->setTransform(video::ENUM_TRANSFORM_WORLD, core::IDENTITY_MATRIX);
		driver->setMaterial(getMaterial(0));
		driver->drawUnit(m_pUnit);
	}

	void CROAMTerrain::onRegisterForRender()
	{
		if(m_bVisible)
		{
			m_pSceneManager->registerForRender(this);

			reset();
			tessellate();
			preRender();

			ITerrainModel::onRegisterForRender();
		}
	}

}
}
}