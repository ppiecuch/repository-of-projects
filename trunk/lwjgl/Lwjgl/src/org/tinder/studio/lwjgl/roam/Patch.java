package org.tinder.studio.lwjgl.roam;
/**
 *
 * @Author Micheal Hong
 * @Email babala_234@163.com
 * @Version 2010-7-21上午12:39:30
 *
 */
public class Patch {
	
	// Depth of variance tree: should be near SQRT(PATCH_SIZE) + 1
	private static final int VARIANCE_DEPTH=9;
	
	/**
	 * Split a single Triangle and link it into the mesh.
	 * Will correctly force-split diamonds.
	 * @param node
	 */
	public void split(TriTriangleNode node)
	{
		//如果已经切割，无需再处理
		if(node.leftChild!=null)
			return;
		//TODO 不明白
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
		
		//TODO 不明白
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
		//TODO 不明白
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
	 * Tessellate a Patch.
	 * Will continue to split until the variance metric is met.
	 */
	public void recursTessellate(TriTriangleNode tri,int leftX,int leftY,int rightX,int rightY,int apexX,int apexY,int node )
	{
		float triVariance;
		int centerX = (leftX + rightX)>>1; // Compute X coordinate of center of Hypotenuse
		int centerY = (leftY + rightY)>>1; // Compute Y coord...
		
		if (node<(1<<VARIANCE_DEPTH))
		{
			// Extremely slow distance metric (sqrt is used).
			// Replace this with a faster one!
//			float distance = 1.0f + sqrtf( SQR((float)centerX - gViewPosition[0]) +
//					   SQR((float)centerY - gViewPosition[2]) );
			// Egads!  A division too?  What's this world coming to!
			// This should also be replaced with a faster operation.
//			triVariance = ((float)m_CurrentVariance[node] * MAP_SIZE * 2)/distance;	// Take both distance and variance into consideration
		}
		
		if((node>=(1<<VARIANCE_DEPTH))||	// IF we do not have variance info for this node, then we must have gotten here by splitting, so continue down to the lowest level.
				(triVariance > gFrameVariance))	// OR if we are not below the variance tree, test for variance.
		{
			split(tri);
			if(tri.leftChild &&		// If this triangle was split, try to split it's children as well.
					((abs(leftX - rightX) >= 3) || (abs(leftY - rightY) >= 3)))	// Tessellate all the way down to one vertex per height field entry
				{
					RecursTessellate( tri.leftChild,   apexX,  apexY, leftX, leftY, centerX, centerY,    node<<1  );
					RecursTessellate( tri.rightChild, rightX, rightY, apexX, apexY, centerX, centerY, 1+(node<<1) );
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

}
