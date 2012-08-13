#include "CROAMTerrain.h"
#include "CImage.h"
#include "SUnit.h"
#include "ISceneManager.h"
#include "yonMath.h"

#include "ILogger.h"

namespace yon{
namespace scene{
namespace terrain{

	//const s32 CROAMTerrain::POOL_SIZE=25000;
	s32  CROAMTerrain::m_NextTriNode;
	//TriTreeNode CROAMTerrain::m_TriPool[POOL_SIZE];

	// Beginning frame varience (should be high, it will adjust automatically)
	float gFrameVariance = 50;

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
	

	CROAMTerrain::CROAMTerrain(IModel* parent,const core::vector3df& pos,
		const core::vector3df& rot,const core::vector3df& scale)
		:ITerrainModel(parent,pos,rot,scale),m_iMapSize(0),m_iImageSizePerSide(0),
		m_iNumPatchPerSide(0),m_iPatchSize(0),m_aPatches(NULL),m_iVarianceDepth(9)
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
	void CROAMTerrain::recursTessellate(SPatch* patch,TriTreeNode *tri,s32 leftX,s32 leftY,s32 rightX,s32 rightY,s32 apexX,s32 apexY,s32 node)
	{
		f32 TriVariance;
		s32 centerX = (leftX + rightX)>>1; // Compute X coordinate of center of Hypotenuse
		s32 centerY = (leftY + rightY)>>1; // Compute Y coord...

		if(node<(1<<m_iVarianceDepth))
		{
			//TODO 优化

			// Extremely slow distance metric (sqrt is used).
			// Replace this with a faster one!
			f32 distance = 1.0f + core::squareroot(core::square(centerX - m_currentCameraPos.x)+core::square(m_currentCameraPos.y)+core::square(centerY - m_currentCameraPos.z));

			// Egads!  A division too?  What's this world coming to!
			// This should also be replaced with a faster operation.
			TriVariance = ((f32)patch->m_pCurrentVariance[node] * m_iMapSize * 2)/distance;	// Take both distance and variance into consideration
		}

		//TODO why?

		// IF we do not have variance info for this node, then we must have gotten here by splitting, so continue down to the lowest level.
		// OR if we are not below the variance tree, test for variance.
		if ( (node >= (1<<m_iVarianceDepth))||(TriVariance > gFrameVariance))
		{
			// Split this triangle.
			split(tri);

			//TODO why 3?

			// If this triangle was split, try to split it's children as well.
			// Tessellate all the way down to one vertex per height field entry
			if (tri->leftChild &&((core::abs_(leftX-rightX) >= 3) || (core::abs_(leftY-rightY) >= 3)))	
			{
				recursTessellate(patch, tri->leftChild,   apexX,  apexY, leftX, leftY, centerX, centerY,    node<<1  );
				recursTessellate(patch, tri->rightChild, rightX, rightY, apexX, apexY, centerX, centerY, 1+(node<<1) );
			}
		}
	}

	// Create an approximate mesh.
	void CROAMTerrain::tessellatePatch(SPatch* patch)
	{
		// Split each of the base triangles
		//∣
		//└--
		patch->m_pCurrentVariance = patch->m_pVarianceLeft;
		recursTessellate(patch,&patch->m_baseLeft,patch->m_uIndexX,patch->m_uIndexZ+m_iPatchSize,patch->m_uIndexX+m_iPatchSize,patch->m_uIndexZ,patch->m_uIndexX,patch->m_uIndexZ,1);

		//--┐
		//  ∣
		patch->m_pCurrentVariance = patch->m_pVarianceRight;
		recursTessellate(patch,&patch->m_baseRight,patch->m_uIndexX+m_iPatchSize,patch->m_uIndexZ,patch->m_uIndexX,patch->m_uIndexZ+m_iPatchSize,patch->m_uIndexX+m_iPatchSize,patch->m_uIndexZ+m_iPatchSize,1);
	}

	void CROAMTerrain::tessellate()
	{
		// Perform Tessellation
		s32 index=0;
		for(s32 x=0;x<m_iNumPatchPerSide;++x)
		{
			for(s32 z=0;z<m_iNumPatchPerSide;++z)
			{
				if(m_aPatches[x][z]->isVisibile())
					m_aPatches[x][z]->tessellate();
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
		// Since we're after speed and not perfect representations,
		//    only calculate variance down to an 8x8 block
		//if (core::abs_(leftX - rightX)+core::abs_(leftZ - rightZ) > 2 )
		if ((core::abs_(leftX - rightX) >= 2)||(core::abs_(leftZ - rightZ) >= 2) )
		{
			Logger->debug("node:%d,%d,%d,{%d,%d,%d}\r\n",node,node<<1,1+(node<<1),getIndex(patch,leftX,leftZ),getIndex(patch,rightX,rightZ),getIndex(patch,apexX,apexZ));
			// Final Variance for this node is the max of it's own variance and that of it's children.
			myVariance = core::max_(myVariance, recursComputeVariance(patch, apexX,   apexZ,  apexY, leftX, leftZ, leftY, centerX, centerZ, centerY,    node<<1 ));
			myVariance = core::max_(myVariance, recursComputeVariance(patch, rightX, rightZ, rightY, apexX, apexZ, apexY, centerX, centerZ, centerY, 1+(node<<1)));
		}

		//TODO 冗余了吧，如果只在node<(1<<m_iVarianceDepth)时才记录，那么应该将myVariance的计算部分也包进去才是最优
		// Store the final variance for this node.  Note Variance is never zero.
		if (node<(1<<m_iVarianceDepth))
		{
			patch->m_pCurrentVariance[node] = 1 + myVariance;

			if(patch->m_pCurrentVariance==patch->m_pVarianceLeft)
				Logger->debug("Patch[%d]:LeftVariance[%d]=%d{%d,%d,%d}\r\n",patch->m_uIndex,node,patch->m_pCurrentVariance[node],getIndex(patch,leftX,leftZ),getIndex(patch,rightX,rightZ),getIndex(patch,apexX,apexZ));
			else
				Logger->debug("Patch[%d]:RightVariance[%d]=%d{%d,%d,%d}\r\n",patch->m_uIndex,node,patch->m_pCurrentVariance[node],getIndex(patch,leftX,leftZ),getIndex(patch,rightX,rightZ),getIndex(patch,apexX,apexZ));
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
		//driver->setTransform(video::ENUM_TRANSFORM_WORLD, getAbsoluteTransformation());
		//driver->setTransform(video::ENUM_TRANSFORM_WORLD, core::IDENTITY_MATRIX);
		//driver->setMaterial(getMaterial(0));
		//driver->drawUnit(m_pUnit);
	}

	void CROAMTerrain::onRegisterForRender()
	{
		if(m_bVisible)
		{
			m_pSceneManager->registerForRender(this);

			ITerrainModel::onRegisterForRender();
		}
	}

}
}
}