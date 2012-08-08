#ifndef _YON_SCENE_TERRAIN_CROAMTERRAIN_H_
#define _YON_SCENE_TERRAIN_CROAMTERRAIN_H_

#include "ITerrainModel.h"
#include "IUnit.h"
#include "SDynamicShap.h"
#include "aabbox3d.h"

namespace yon{
namespace scene{
namespace terrain{

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

	// Depth of variance tree: should be near SQRT(PATCH_SIZE) + 1
#define VARIANCE_DEPTH (9)

	class CROAMTerrain : public ITerrainModel{
	private:

		struct TriTreeNode {
			TriTreeNode *leftChild;
			TriTreeNode *rightChild;
			TriTreeNode *baseNeighbor; 
			TriTreeNode *leftNeighbor;   
			TriTreeNode *rightNeighbor;
		};

		class Patch
		{
		protected:
			u8 *m_HeightMap;								// Pointer to height map to use
			s32 m_WorldX, m_WorldY;							// World coordinate offset of this patch.

			u8 m_VarianceLeft[ 1<<(VARIANCE_DEPTH)];		// Left variance tree
			u8 m_VarianceRight[1<<(VARIANCE_DEPTH)];		// Right variance tree

			u8 *m_CurrentVariance;							// Which varience we are currently using. [Only valid during the Tessellate and ComputeVariance passes]
			u8 m_VarianceDirty;								// Does the Varience Tree need to be recalculated for this Patch?
			bool m_isVisible;								// Is this patch visible in the current frame?

			TriTreeNode m_BaseLeft;							// Left base triangle tree node
			TriTreeNode m_BaseRight;						// Right base triangle tree node

		public:
			// Some encapsulation functions & extras
			TriTreeNode *getBaseLeft(){ return &m_BaseLeft; }
			TriTreeNode *getBaseRight(){ return &m_BaseRight; }
			u8 isDirty(){ return m_VarianceDirty; }
			bool isVisibile(){ return m_isVisible; }
			void setVisibility(s32 eyeX, s32 eyeY, s32 leftX, s32 leftY, s32 rightX, s32 rightY);

			// The static half of the Patch Class
			virtual void init(s32 heightX, s32 heightY, s32 worldX, s32 worldY, u8 *hMap);
			virtual void reset();
			virtual void tessellate();
			virtual void render();
			virtual void computeVariance();

			// The recursive half of the Patch Class
			virtual void split(TriTreeNode *tri);
			virtual void recursTessellate( TriTreeNode *tri, s32 leftX, s32 leftY, s32 rightX, s32 rightY, s32 apexX, s32 apexY, s32 node);
			virtual void recursRender(TriTreeNode *tri, s32 leftX, s32 leftY, s32 rightX, s32 rightY, s32 apexX, s32 apexY);
			virtual u8 recursComputeVariance(s32 leftX,s32 leftY,u8 leftZ, s32 rightX, s32 rightY, u8 rightZ, s32 apexX,  s32 apexY, u8 apexZ, s32 node);
		};


		class Landscape {
		public:
			// Initialize the whole process
			void init(u8 *hMap);
			// Reset for a new frame
			void reset();
			// Create mesh approximation
			void tessellate();
			// Render current mesh static
			void render();
			// Allocate a new node for the mesh
			TriTreeNode *allocateTri();
			
		protected:
			// Index to the next free TriTreeNode
			static int m_NextTriNode;
			// Pool of nodes for tessellation
			static TriTreeNode m_TriPool[];
			// Array of patches to be rendered
			Patch m_aPatches[][];
			// Pointer to Height Field data
			u8 *m_HeightMap;
			
		};

		f32* m_fpLevelMDSize;				//max midpoint displacement per level
		s32 m_iMaxLevel;

		//the function needs to take four arguments, three of which are vertex information and the fourth of which is the current level that is
		//being rendered.(We need the level information so that we can dig into the midpoint displacement table we created earlier.) At the start of
		// the function, we can store the maximum midpoint displacement value for the current level int a local variable, fMD, to make things easier.
		//Then we can calculate the new vertex that was formed when we split the parent triangle.
		//We must meet two requirements to subdivide the node. First, we need to find out if subdividing to another node requires going over the maximum
		//detail level. If it does, then we can't subdivide; we just have to render the current triangle. Second, we need to see if the viewer is close
		//enough to bother with a subdivision. If both of these requirements are met, we can recurse down to the current triangle's two children.
		void renderChild();
	public:
		CROAMTerrain(IModel* parent,const core::vector3df& pos,
			const core::vector3df& rot,const core::vector3df& scale);
		~CROAMTerrain();
	};
}
}
}

#undef VARIANCE_DEPTH

#endif