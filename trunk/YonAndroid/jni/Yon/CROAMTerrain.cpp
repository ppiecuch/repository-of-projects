#include "CROAMTerrain.h"

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

	// Render each patch of the landscape & adjust the frame variance.
	//
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
}
}
}