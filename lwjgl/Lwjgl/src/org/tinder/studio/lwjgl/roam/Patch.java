package org.tinder.studio.lwjgl.roam;

import org.tinder.studio.lwjgl.util.Util;

/**
 *
 * Patch负责管理地形的每个小块，其函数供Landscape调用
 * 一个Patch由左右两个等腰直角三角形构成，也就是沿patch对角线切分出来的两个三角形，每个三角形形成一个独立的二元三角树
 * @Author Micheal Hong
 * @Email babala_234@163.com
 * @Version 2010-7-21上午12:39:30
 *
 */
public class Patch {
	
	// Depth of variance tree: should be near SQRT(PATCH_SIZE) + 1
	private static final int VARIANCE_DEPTH=9;
	
	private int worldX,worldY;	// World coordinate offset of this patch.
	private int[] varianceLeft=new int[1<<(VARIANCE_DEPTH)];// Left variance tree
	private int[] varianceRight=new int[1<<(VARIANCE_DEPTH)];// Right variance tree
	
	private float[] currentVariance;// Which varience we are currently using. [Only valid during the Tessellate and ComputeVariance passes]
	private int varianceDirty;// Does the Varience Tree need to be recalculated for this Patch?
	private boolean isVisible;// Is this patch visible in the current frame?
	
	private TriTreeNode baseLeft;// Left base triangle tree node
	private TriTreeNode baseRight;// Right base triangle tree node
	
	// Beginning frame varience (should be high, it will adjust automatically)
	private float gFrameVariance = 50;
	
	private float[] gViewPosition={ 0.f, 5.f, 0.f };
	private int mapSize=0;
	private int gNumTrisRendered=0;
	
	/**
	 * 对当前节点进行递归分解
	 * 当分割一个节点时存在三种可能：
	 * 1、节点是钻石的一部分---分割它和它的下邻节点。
	 * 2、节点是网格的边---只分割这个节点。
	 * 3、节点不是钻石的一部分---强制分割下邻节点。
	 * Split a single Triangle and link it into the mesh.
	 * Will correctly force-split diamonds.
	 * @param node
	 */
	public void split(TriTreeNode node)
	{
		//如果已经切割，无需再处理
		if(node.leftChild!=null)
			return;
		
		//如果节点不是钻石的一部分---强制分割下邻节点
		//If this triangle is not in a proper diamond, force split our base neighbor
		if(node.baseNeighbor!=null&&node.baseNeighbor.baseNeighbor!=node)
			split(node.baseNeighbor);
		// Create children and link into mesh
		node.leftChild=Landscape.allocateTri();
		node.rightChild=Landscape.allocateTri();
		
		// If creation failed, just exit.
		if(node.leftChild==null)
			return;
		
		// Fill in the information we can get from the parent (neighbor pointers)
		node.leftChild.baseNeighbor=node.leftNeighbor;
		node.leftChild.leftNeighbor=node.rightChild;
		
		node.rightChild.baseNeighbor=node.rightNeighbor;
		node.rightChild.rightNeighbor=node.leftChild;
		
		
		// Link our Left Neighbor to the new children
		if(node.leftNeighbor!=null)
		{
			if(node.leftNeighbor.baseNeighbor==node)
				node.leftNeighbor.baseNeighbor=node.leftChild;
			else if(node.leftNeighbor.leftNeighbor==node)
				node.leftNeighbor.leftNeighbor=node.leftChild;
			else if(node.leftNeighbor.rightNeighbor==node)
				node.leftNeighbor.rightNeighbor=node.leftChild;
			else
				;// Illegal Left Neighbor!
		}
		// Link our Right Neighbor to the new children
		if(node.rightNeighbor!=null)
		{
			if(node.rightNeighbor.baseNeighbor==node)
				node.rightNeighbor.baseNeighbor=node.rightChild;
			else if(node.rightNeighbor.rightNeighbor==node)
				node.rightNeighbor.rightNeighbor=node.rightChild;
			else if(node.rightNeighbor.leftNeighbor==node)
				node.rightNeighbor.leftNeighbor=node.rightChild;
			else
				;// Illegal Right Neighbor!
		}
		
		//如果节点是钻石的一部分---分割它和它的下邻节点。
		// Link our Base Neighbor to the new children
		if(node.baseNeighbor!=null)
		{
			if(node.baseNeighbor.leftChild!=null)
			{
				node.baseNeighbor.leftChild.rightNeighbor=node.rightChild;
				node.baseNeighbor.rightChild.leftNeighbor=node.leftChild;
				node.leftChild.rightNeighbor=node.baseNeighbor.rightChild;
				node.rightChild.leftNeighbor=node.baseNeighbor.leftChild;
			}
			else
				split(node.baseNeighbor); // Base Neighbor (in a diamond with us) was not split yet, so do that now.
		}
		else
		{
			// An edge triangle, trivial case.
			node.leftChild.rightNeighbor=null;
			node.rightChild.leftNeighbor=null;
		}
		
		/*// We are already split, no need to do it again.
	if (tri->LeftChild)
		return;

	// If this triangle is not in a proper diamond, force split our base neighbor
	if ( tri->BaseNeighbor && (tri->BaseNeighbor->BaseNeighbor != tri) )
		Split(tri->BaseNeighbor);

	// Create children and link into mesh
	tri->LeftChild  = Landscape::AllocateTri();
	tri->RightChild = Landscape::AllocateTri();

	// If creation failed, just exit.
	if ( !tri->LeftChild )
		return;

	// Fill in the information we can get from the parent (neighbor pointers)
	tri->LeftChild->BaseNeighbor  = tri->LeftNeighbor;
	tri->LeftChild->LeftNeighbor  = tri->RightChild;

	tri->RightChild->BaseNeighbor  = tri->RightNeighbor;
	tri->RightChild->RightNeighbor = tri->LeftChild;

	// Link our Left Neighbor to the new children
	if (tri->LeftNeighbor != NULL)
	{
		if (tri->LeftNeighbor->BaseNeighbor == tri)
			tri->LeftNeighbor->BaseNeighbor = tri->LeftChild;
		else if (tri->LeftNeighbor->LeftNeighbor == tri)
			tri->LeftNeighbor->LeftNeighbor = tri->LeftChild;
		else if (tri->LeftNeighbor->RightNeighbor == tri)
			tri->LeftNeighbor->RightNeighbor = tri->LeftChild;
		else
			;// Illegal Left Neighbor!
	}

	// Link our Right Neighbor to the new children
	if (tri->RightNeighbor != NULL)
	{
		if (tri->RightNeighbor->BaseNeighbor == tri)
			tri->RightNeighbor->BaseNeighbor = tri->RightChild;
		else if (tri->RightNeighbor->RightNeighbor == tri)
			tri->RightNeighbor->RightNeighbor = tri->RightChild;
		else if (tri->RightNeighbor->LeftNeighbor == tri)
			tri->RightNeighbor->LeftNeighbor = tri->RightChild;
		else
			;// Illegal Right Neighbor!
	}

	// Link our Base Neighbor to the new children
	if (tri->BaseNeighbor != NULL)
	{
		if ( tri->BaseNeighbor->LeftChild )
		{
			tri->BaseNeighbor->LeftChild->RightNeighbor = tri->RightChild;
			tri->BaseNeighbor->RightChild->LeftNeighbor = tri->LeftChild;
			tri->LeftChild->RightNeighbor = tri->BaseNeighbor->RightChild;
			tri->RightChild->LeftNeighbor = tri->BaseNeighbor->LeftChild;
		}
		else
			Split( tri->BaseNeighbor);  // Base Neighbor (in a diamond with us) was not split yet, so do that now.
	}
	else
	{
		// An edge triangle, trivial case.
		tri->LeftChild->RightNeighbor = NULL;
		tri->RightChild->LeftNeighbor = NULL;
	}*/
	}
	
	/**
	 * 分割一个小块，直到满足一定细节层次
	 * Tessellate a Patch.
	 * Will continue to split until the variance metric is met.
	 */
	public void recursTessellate(TriTreeNode tri,int leftX,int leftY,int rightX,int rightY,int apexX,int apexY,int node )
	{
		float triVariance=0;
		int centerX = (leftX + rightX)>>1; // Compute X coordinate of center of Hypotenuse
		int centerY = (leftY + rightY)>>1; // Compute Y coord...
		
		if (node<(1<<VARIANCE_DEPTH))
		{
			// Extremely slow distance metric (sqrt is used).
			// Replace this with a faster one!
			float distance = (float) (1.0f + Math.sqrt(Util.square((float)centerX - gViewPosition[0])+Util.square((float)centerY - gViewPosition[2]) ));
			// Egads!  A division too?  What's this world coming to!
			// This should also be replaced with a faster operation.
			triVariance = ((float)currentVariance[node]*mapSize*2)/distance;	// Take both distance and variance into consideration
		}
		
		if((node>=(1<<VARIANCE_DEPTH))||	// IF we do not have variance info for this node, then we must have gotten here by splitting, so continue down to the lowest level.
				(triVariance > gFrameVariance))	// OR if we are not below the variance tree, test for variance.
		{
			split(tri);
			if(tri.leftChild!=null	// If this triangle was split, try to split it's children as well.
					&&((Math.abs(leftX - rightX)>=3)||(Math.abs(leftY-rightY)>= 3)))	// Tessellate all the way down to one vertex per height field entry
			{
				recursTessellate( tri.leftChild,   apexX,  apexY, leftX, leftY, centerX, centerY,    node<<1  );	//TODO 不明白这个node
				recursTessellate( tri.rightChild, rightX, rightY, apexX, apexY, centerX, centerY, 1+(node<<1) );
			}
		}
		/*
		 * float TriVariance;
	int centerX = (leftX + rightX)>>1; // Compute X coordinate of center of Hypotenuse
	int centerY = (leftY + rightY)>>1; // Compute Y coord...

	if ( node < (1<<VARIANCE_DEPTH) )
	{
		// Extremely slow distance metric (sqrt is used).
		// Replace this with a faster one!
		float distance = 1.0f + sqrtf( SQR((float)centerX - gViewPosition[0]) +
									   SQR((float)centerY - gViewPosition[2]) );
		
		// Egads!  A division too?  What's this world coming to!
		// This should also be replaced with a faster operation.
		TriVariance = ((float)m_CurrentVariance[node] * MAP_SIZE * 2)/distance;	// Take both distance and variance into consideration
	}

	if ( (node >= (1<<VARIANCE_DEPTH)) ||	// IF we do not have variance info for this node, then we must have gotten here by splitting, so continue down to the lowest level.
		 (TriVariance > gFrameVariance))	// OR if we are not below the variance tree, test for variance.
	{
		Split(tri);														// Split this triangle.
		
		if (tri->LeftChild &&											// If this triangle was split, try to split it's children as well.
			((abs(leftX - rightX) >= 3) || (abs(leftY - rightY) >= 3)))	// Tessellate all the way down to one vertex per height field entry
		{
			RecursTessellate( tri->LeftChild,   apexX,  apexY, leftX, leftY, centerX, centerY,    node<<1  );
			RecursTessellate( tri->RightChild, rightX, rightY, apexX, apexY, centerX, centerY, 1+(node<<1) );
		}
	}
		 */
	}
	
	/**
	 * 	渲染每个叶子节点
	 *  Render the tree.  Simple no-fan method.
	 * @param tri
	 * @param leftX
	 * @param leftY
	 * @param rightX
	 * @param rightY
	 * @param apexX
	 * @param apexY
	 */
	public void recursRender( TriTreeNode tri, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY )
	{
		//非叶子节点都有两个孩子，所以只判断一个就可以了
		if(tri.leftChild!=null)// All non-leaf nodes have both children, so just check for one
		{
			int centerX = (leftX + rightX)>>1;	// Compute X coordinate of center of Hypotenuse
			int centerY = (leftY + rightY)>>1;	// Compute Y coord...
			recursRender( tri.leftChild,  apexX,   apexY, leftX, leftY, centerX, centerY );
			recursRender( tri.rightChild, rightX, rightY, apexX, apexY, centerX, centerY );
		}
		else // A leaf node!  Output a triangle to be rendered.
		{
			// Actual number of rendered triangles...
			gNumTrisRendered++;
			
//			GLfloat leftZ  = m_HeightMap[(leftY *MAP_SIZE)+leftX ];
//			GLfloat rightZ = m_HeightMap[(rightY*MAP_SIZE)+rightX];
//			GLfloat apexZ  = m_HeightMap[(apexY *MAP_SIZE)+apexX ];
			
			// Perform lighting calculations if requested.
		}
		/*
		 * // ---------------------------------------------------------------------
// Render the tree.  Simple no-fan method.
//
void Patch::RecursRender( TriTreeNode *tri, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY )
{
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
}
		 */
	}
	
	/**
	 * 调用recursComputeVariance方法完成各个节点的变差计算
	 * Compute the variance tree for each of the Binary Triangles in this patch.
	 */
	public void computeVariance()
	{
		/*
		 * // ---------------------------------------------------------------------
// Compute the variance tree for each of the Binary Triangles in this patch.
//
void Patch::ComputeVariance()
{
	// Compute variance on each of the base triangles...

	m_CurrentVariance = m_VarianceLeft;
	RecursComputeVariance(	0,          PATCH_SIZE, m_HeightMap[PATCH_SIZE * MAP_SIZE],
							PATCH_SIZE, 0,          m_HeightMap[PATCH_SIZE],
							0,          0,          m_HeightMap[0],
							1);

	m_CurrentVariance = m_VarianceRight;
	RecursComputeVariance(	PATCH_SIZE, 0,          m_HeightMap[ PATCH_SIZE],
							0,          PATCH_SIZE, m_HeightMap[ PATCH_SIZE * MAP_SIZE],
							PATCH_SIZE, PATCH_SIZE, m_HeightMap[(PATCH_SIZE * MAP_SIZE) + PATCH_SIZE],
							1);

	// Clear the dirty flag for this patch
	m_VarianceDirty = 0;
}
		 */
	}
	
	/**
	 *  计算所有节点的变差
	 *  Computes Variance over the entire tree.  Does not examine node relationships.
	 * @param leftX
	 * @param leftY
	 * @param leftZ
	 * @param rightX
	 * @param rightY
	 * @param rightZ
	 * @param apexX
	 * @param apexY
	 * @param apexZ
	 * @param node
	 */
	public void recursComputeVariance(int leftX,int leftY,int leftZ,
				int rightX,int rightY,int rightZ,
				int apexX,int apexY,int apexZ,int node)
	{
		/*
		 * // ---------------------------------------------------------------------
// Computes Variance over the entire tree.  Does not examine node relationships.
//
unsigned char Patch::RecursComputeVariance( int leftX,  int leftY,  unsigned char leftZ,
										    int rightX, int rightY, unsigned char rightZ,
											int apexX,  int apexY,  unsigned char apexZ,
											int node)
{
	//        /|\
	//      /  |  \
	//    /    |    \
	//  /      |      \
	//  ~~~~~~~*~~~~~~~  <-- Compute the X and Y coordinates of '*'
	//
	int centerX = (leftX + rightX) >>1;		// Compute X coordinate of center of Hypotenuse
	int centerY = (leftY + rightY) >>1;		// Compute Y coord...
	unsigned char myVariance;

	// Get the height value at the middle of the Hypotenuse
	unsigned char centerZ  = m_HeightMap[(centerY * MAP_SIZE) + centerX];

	// Variance of this triangle is the actual height at it's hypotenuse midpoint minus the interpolated height.
	// Use values passed on the stack instead of re-accessing the Height Field.
	myVariance = abs((int)centerZ - (((int)leftZ + (int)rightZ)>>1));

	// Since we're after speed and not perfect representations,
	//    only calculate variance down to an 8x8 block
	if ( (abs(leftX - rightX) >= 8) ||
		 (abs(leftY - rightY) >= 8) )
	{
		// Final Variance for this node is the max of it's own variance and that of it's children.
		myVariance = MAX( myVariance, RecursComputeVariance( apexX,   apexY,  apexZ, leftX, leftY, leftZ, centerX, centerY, centerZ,    node<<1 ) );
		myVariance = MAX( myVariance, RecursComputeVariance( rightX, rightY, rightZ, apexX, apexY, apexZ, centerX, centerY, centerZ, 1+(node<<1)) );
	}

	// Store the final variance for this node.  Note Variance is never zero.
	if (node < (1<<VARIANCE_DEPTH))
		m_CurrentVariance[node] = 1 + myVariance;

	return myVariance;
}
		 */
	}
	
	/**
	 * 根据嵌套包围盒算法来判断地形的可见性，从而实现裁剪
	 * Set patch's visibility flag.
	 * @param eyeX
	 * @param eyeY
	 * @param leftX
	 * @param leftY
	 * @param rightX
	 * @param rightY
	 */
	public void setVisibility(int eyeX,int eyeY,int leftX,int leftY,int rightX,int rightY )
	{
		/*
		 * // ---------------------------------------------------------------------
// Set patch's visibility flag.
//
void Patch::SetVisibility( int eyeX, int eyeY, int leftX, int leftY, int rightX, int rightY )
{
	// Get patch's center point
	int patchCenterX = m_WorldX + PATCH_SIZE / 2;
	int patchCenterY = m_WorldY + PATCH_SIZE / 2;
	
	// Set visibility flag (orientation of both triangles must be counter clockwise)
	m_isVisible = (orientation( eyeX,  eyeY,  rightX, rightY, patchCenterX, patchCenterY ) < 0) &&
				  (orientation( leftX, leftY, eyeX,   eyeY,   patchCenterX, patchCenterY ) < 0);
}

		 */
	}

}
