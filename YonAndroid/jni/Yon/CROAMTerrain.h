#ifndef _YON_SCENE_TERRAIN_CROAMTERRAIN_H_
#define _YON_SCENE_TERRAIN_CROAMTERRAIN_H_

#include "ITerrainModel.h"
#include "IUnit.h"
#include "SDynamicShap.h"
#include "aabbox3d.h"

namespace yon{
namespace scene{
namespace terrain{

	//1 ROAM cannot eliminate sudden movement of vertices as tesselation level changes
	//2 ROAM cannot take advantage of some features that are very common on modern graphics accelerators such as programmable pipeline

	//3d coordinate:
	//             z index
	//             ↑ ↑
	//    right    ∣ ∣
	//  top┼bottom∣ ∣
	//    left     ∣ ∣
	//x←─────┘
	//
	//image coordinate:
	// z           
	// ↑
	// ∣
	// ∣
	// ∣
	// └─────→x

	//基本思想是:在对地形进行渲染时,根据视点位置和视线方向来计算视点距离地形表面的三角片元的距离,
	//再根据目标格网的空间粗糙程序来判断是否对地形表面的三角片元进行一系列基于三角型二叉分割的分解和合并,
	//最终生成逼近真实地形的无缝无重叠的简化三角化地形表面.

	//分裂操作和合并操作是ROAM算法的基本操作。
	//通过对从较低细节层次的地形网格进行分裂操作可以获得较高细节层次的地形网格；
	//反之，使用合并操作可以将较高层次分明的地形网格恢复到较低层次的地形网格。
	//这种自底向上逐步细化互为可逆的算法非常容易通过递归调用来实现。

	//分裂与合并
	//为了避免在分裂和合并过程中产生裂缝，ROAM算法定义了三角形3种邻接关系：Tb表示与等腰直角三角形T共享斜边的底邻居；
	//Tl表示与等腰直角三角形T共享左直角边的左邻居；Tr表示与等腰直角三角形T共享右直角边的右邻居。
	//Tb与T处在三角二叉树的同一层次形成的三角形对，称之为钻石型（Diamond）。
	//如果T被分裂，为了避免产生地形裂缝和阴影不连续现象，Tb也需要被分裂。在二叉树结构中当Tb比T低一个层次时，
	//分裂Tb会引起其他邻居被分裂，最终会导致一系列递归分裂。这样的递归分裂称之为强制分裂（forced split）。
	//因此只有当三角形对形成钻研型结构才能够被执行分裂操作，否则就会引起强制分裂操作。
	//T与Tb在二叉对中同处一个层次并且都被分裂过一次，称之为可合并钻石型（Mergeable Diamond）。
	//只有当三角形对形成可合并钻研型结构的时候才能被执行合并操作。

	//帧的一致性
	//帧的一致性是ROAM中的高级优化技术，对于这项技术来说，最后一帧建立的网格可以被再次使用。
	//这个特性也可以用来进行动态帧定时，允许你连续的改进当前帧的网格直到这帧结束。
	//在一个高速动作游戏中，这意味着你不必花费时间进行地形分块，相反可以先处理其他最重要的快速动作部件，而在帧时间静止时进行地形分块，而在结束时进行渲染。
	//如果一个玩家在进行交火时，地形将用一个低级细节来动态渲染以保存时间。用本文的空间来解释帧的一致性是远远不够的，但是对于他有一些小的标题步骤：
	//增加一个父节点指针到TriTreeNode中，建立一个不做Split()操作的Merge()函数，使用一个优先队列或其他优先结构来保存整个MESH中的叶节点。
	//在分块过程中，随着分割这一帧中非常粗糙的节点的操作，合并所有本帧中足够DETAIL的节点（或直到时间结束）。



	//The ROAM algorithm, full name is Real-Time Optimally Adapting Mesh,developed by Mark Duchaineau
	//ROAM has been synonymous with terrain for the past few years, but it recently came under fire because it was widely considered
	//"too slow" for modern hardware.
	//The ROAM algorithm uses a unique called a binary triangle tree to store polygonal information. This tree starts off with a coarse
	//root triangle.
	//
	//Out method, dubbed Real-time Optimally Adapting Meshes(ROAM), uses two priority queues to drive split and merge operations that
	//maintain continuous triangulations built from preprocessed bintree triangles. We introduce two additional performance
	//optimizations: incremental triangle stripping and priority-computation deferral lists.ROAM execution time is proportionate to
	//the number of triangle changes per frame, which is typically a few percent of the output mesh size, hence ROAM performance is 
	//insensitive to resolution and extent of the input terrain. Dynamic terrain and simple vertex morphing are supported.
	//
	//ROAM consists of a preprocessing component and serveral runtim components. The preprocessing component produces nested, view-
	//dependent error bounds bottom-up for a triangle bintree. At runtime, four phases of computation are performed per frame:
	//1.recursive, incremental update to view-frustum culling
	//2.priority update only for output triangles that can potentially be split/merged in phase 3
	//3.triangulation update using greedy split and merge steps driven by two priority queues(for splits and merges,respectively)
	//4.as-needed updates for triangle strips affected by the culling changes from phase 1 and the splits/merges from phase 3
	//
	//Splitting and Merging, the ROAM paper suggests that instead of starting from scratch every frame, we can base the current frame
	//mesh off of the mesh from the previous frame and add/subtract detail where it is needed.
	//To accomplish this task, we need to split the triangle tree nodes into two priority queues: a split queue and a merge queue. These
	//queues will keep priorities for every triangle in the tessellated mesh, starting with the coarse tessellation, and then repeatedly
	//force split, or merge, the triangle with the highest priority. It is also important to maintain the requirement that a child node
	//never have a higher priority than its parent.
	//This is the basic and bare-bones explanation of priority queues because I don't want to spend too much time discussing them at
	//this moment.Just know that priority queues exist and know what basic pourpose they serve. 
	//
	//Seamus's changes
	//Seamus McNally made several highly notable changes to the ROAM algorithm:
	//1.No data stored for drawn triangles
	//2.A simpler error metric
	//3.No frame-to-frame coherence
	//
	//The base "unit" for the ROAM2.0 implementation is called a diamond. Each diamond in the tree consist of two right isosceles triangles
	//joined on a common base edge.
	class CROAMTerrain : public ITerrainModel{
	private:

		struct TriTreeNode {
			TriTreeNode *leftChild;
			TriTreeNode *rightChild;
			TriTreeNode *baseNeighbor; 
			TriTreeNode *leftNeighbor;   
			TriTreeNode *rightNeighbor;
		};

		struct SPatch
		{
			SPatch(s32 varianceDepth)
			{
				m_pVarianceLeft=new u8[1<<(varianceDepth)];
				m_pVarianceRight=new u8[1<<(varianceDepth)];
			}
			~SPatch()
			{
				delete[] m_pVarianceLeft;
				delete[] m_pVarianceRight;
			}
			u16 m_uOffset;				//Offset of the heightmap from 0
			u16 m_uIndex;
			u16 m_uIndexX;
			u16 m_uIndexZ;

			TriTreeNode m_baseLeft;		// Left base triangle tree node
			TriTreeNode m_baseRight;	// Right base triangle tree node

			u8 *m_pCurrentVariance;		// Which varience we are currently using. [Only valid during the Tessellate and ComputeVariance passes]
			bool m_varianceDirty;		// Does the Varience Tree need to be recalculated for this Patch?
			bool m_visible;				// Is this patch visible in the current frame?

			u8* m_pVarianceLeft;				// Left variance tree
			u8* m_pVarianceRight;				// Right variance tree

			void init(u16 x,u16 z,u16 index,u16 offset)		// Initialize a patch.
			{
				// Clear all the relationships
				m_baseLeft.rightNeighbor = m_baseLeft.leftNeighbor = m_baseRight.rightNeighbor = m_baseRight.leftNeighbor =
					m_baseLeft.leftChild = m_baseLeft.rightChild = m_baseRight.leftChild = m_baseLeft.leftChild = NULL;

				// Attach the two m_Base triangles together
				m_baseLeft.baseNeighbor = &m_baseRight;
				m_baseRight.baseNeighbor = &m_baseLeft;

				// Store Patch offsets for the patch
				m_uIndex=index;
				m_uIndexX=x;
				m_uIndexZ=z;
				m_uOffset=offset;

				// Initialize flags
				m_varianceDirty = true;
				m_visible = false;
			}
			void reset()						// Reset the patch.
			{
				// Assume patch is not visible.
				m_visible = false;

				// Reset the important relationships
				m_baseLeft.leftChild = m_baseLeft.rightChild = m_baseRight.leftChild = m_baseLeft.leftChild = NULL;

				// Attach the two m_Base triangles together
				m_baseLeft.baseNeighbor = &m_baseRight;
				m_baseRight.baseNeighbor = &m_baseLeft;

				// Clear the other relationships.
				m_baseLeft.rightNeighbor = m_baseLeft.leftNeighbor = m_baseRight.rightNeighbor = m_baseRight.leftNeighbor = NULL;
			}
		};

		

		//f32* m_fpLevelMDSize;				//max midpoint displacement per level
		//s32 m_iMaxLevel;

		//the function needs to take four arguments, three of which are vertex information and the fourth of which is the current level that is
		//being rendered.(We need the level information so that we can dig into the midpoint displacement table we created earlier.) At the start of
		// the function, we can store the maximum midpoint displacement value for the current level int a local variable, fMD, to make things easier.
		//Then we can calculate the new vertex that was formed when we split the parent triangle.
		//We must meet two requirements to subdivide the node. First, we need to find out if subdividing to another node requires going over the maximum
		//detail level. If it does, then we can't subdivide; we just have to render the current triangle. Second, we need to see if the viewer is close
		//enough to bother with a subdivision. If both of these requirements are met, we can recurse down to the current triangle's two children.
		//void renderChild();

		//TODO use objectpool
		// How many TriTreeNodes should be allocated?
		const static s32 POOL_SIZE=25000;

		// Depth of variance tree: should be near SQRT(PATCH_SIZE) + 1
		const s32 m_iVarianceDepth;

		// Index to the next free TriTreeNode
		static int m_NextTriNode;
		// Pool of nodes for tessellation
		static TriTreeNode m_TriPool[POOL_SIZE];
		// Array of patches to be rendered
		SPatch*** m_aPatches;
		// Pointer to Height Field data
		//u8 *m_HeightMap;

		s32 m_iImageSizePerSide;
		s32 m_iMapSize;
		s32 m_iNumPatchPerSide;
		s32 m_iPatchSize;

		IUnit* m_pUnit;
		SDynamicShap3D2T m_shap;
		core::array<u8> m_heightMap;
		core::position3df m_currentCameraPos;

		friend struct SPatch;

		u16 getIndex(SPatch* patch,s32 x,s32 z)
		{
			return x*m_iMapSize+z+patch->m_uOffset;
		}

		// Allocate a new node for the mesh
		TriTreeNode *allocateTri()
		{
			TriTreeNode *pTri;

			// IF we've run out of TriTreeNodes, just return NULL (this is handled gracefully)
			if ( m_NextTriNode >= POOL_SIZE )
				return NULL;

			pTri = &(m_TriPool[m_NextTriNode++]);
			pTri->leftChild = pTri->rightChild = NULL;

			return pTri;
		}

		// Reset all patches, recompute variance if needed
		void reset();

		void split(TriTreeNode *tri);
		void recursTessellate(SPatch* patch,TriTreeNode *tri, s32 leftX, s32 leftY, s32 rightX, s32 rightY, s32 apexX, s32 apexY, s32 node);
		void tessellatePatch(SPatch* patch);
		void tessellate();
		//TODO 优化，添加参数patchIndex
		// Computes Variance over the entire tree, and return the max variance among them.  Does not examine node relationships.
		//u8 recursComputeVariance(SPatch* patch,s32 leftIndex,u8 leftHeight, s32 rightIndex,u8 rightHeight,s32 apexIndex,u8 apexHeight, s32 nodeIndex);
		u8 recursComputeVariance(SPatch* patch,s32 leftX,s32 leftZ,u8 leftY,s32 rightX,s32 rightZ,u8 rightY,s32 apexX,s32 apexZ,u8 apexY,s32 node);
		// Compute the variance tree for each of the Binary Triangles in this patch.
		void computeVariance(SPatch* patch);

	public:
		CROAMTerrain(IModel* parent,const core::vector3df& pos,
			const core::vector3df& rot,const core::vector3df& scale);
		~CROAMTerrain();

		virtual f32 getHeight(f32 x,f32 z) const{
			//TODO
			return 0;
		}

		virtual void loadHeightMap(video::IImage* image,ENUM_PATCH_SIZE patchSize);

		virtual void render(video::IVideoDriver* driver);

		virtual void onRegisterForRender();

		virtual u32 getMaterialCount() const{
			return 1;
		}

		virtual video::IMaterial* getMaterial(u32 num) const{
			if(num>=getMaterialCount())
				return NULL;
			return m_pUnit->getMaterial();
		}
	};
}
}
}

#endif