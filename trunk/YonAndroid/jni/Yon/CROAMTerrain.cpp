#include "CROAMTerrain.h"

#include "ILogger.h"

namespace yon{
namespace scene{
namespace terrain{

	// Reset the patch.
	//
	void Patch::reset()
	{
		// Assume patch is not visible.
		m_isVisible = 0;

		// Reset the important relationships
		m_BaseLeft.leftChild = m_BaseLeft.rightChild = m_BaseRight.leftChild = m_BaseLeft.leftChild = NULL;

		// Attach the two m_Base triangles together
		m_BaseLeft.baseNeighbor = &m_BaseRight;
		m_BaseRight.baseNeighbor = &m_BaseLeft;

		// Clear the other relationships.
		m_BaseLeft.rightNeighbor = m_BaseLeft.leftNeighbor = m_BaseRight.rightNeighbor = m_BaseRight.leftNeighbor = NULL;
	}

	// ---------------------------------------------------------------------
	// Split a single Triangle and link it into the mesh.
	// Will correctly force-split diamonds.
	//
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
				;// Illegal Left Neighbor!
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
				;// Illegal Right Neighbor!
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

	// Tessellate a Patch.
	// Will continue to split until the variance metric is met.
	//
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

		if ( (node >= (1<<VARIANCE_DEPTH)) ||	// IF we do not have variance info for this node, then we must have gotten here by splitting, so continue down to the lowest level.
			(TriVariance > gFrameVariance))	// OR if we are not below the variance tree, test for variance.
		{
			split(tri);														// Split this triangle.

			// If this triangle was split, try to split it's children as well.
			// Tessellate all the way down to one vertex per height field entry
			if (tri->leftChild &&((core::abs_(leftX-rightX) >= 3) || (core::abs_(leftY-rightY) >= 3)))	
			{
				recursTessellate( tri->leftChild,   apexX,  apexY, leftX, leftY, centerX, centerY,    node<<1  );
				recursTessellate( tri->rightChild, rightX, rightY, apexX, apexY, centerX, centerY, 1+(node<<1) );
			}
		}
	}
}
}
}