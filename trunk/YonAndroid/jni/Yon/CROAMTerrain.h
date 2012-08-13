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
	//             �� ��
	//    right    �O �O
	//  top��bottom�O �O
	//    left     �O �O
	//x��������������
	//
	//image coordinate:
	// z           
	// ��
	// �O
	// �O
	// �O
	// ��������������x

	//����˼����:�ڶԵ��ν�����Ⱦʱ,�����ӵ�λ�ú����߷����������ӵ������α��������ƬԪ�ľ���,
	//�ٸ���Ŀ������Ŀռ�ֲڳ������ж��Ƿ�Ե��α��������ƬԪ����һϵ�л��������Ͷ���ָ�ķֽ�ͺϲ�,
	//�������ɱƽ���ʵ���ε��޷����ص��ļ����ǻ����α���.

	//���Ѳ����ͺϲ�������ROAM�㷨�Ļ���������
	//ͨ���Դӽϵ�ϸ�ڲ�εĵ���������з��Ѳ������Ի�ýϸ�ϸ�ڲ�εĵ�������
	//��֮��ʹ�úϲ��������Խ��ϸ߲�η����ĵ�������ָ����ϵͲ�εĵ�������
	//�����Ե�������ϸ����Ϊ������㷨�ǳ�����ͨ���ݹ������ʵ�֡�

	//������ϲ�
	//Ϊ�˱����ڷ��Ѻͺϲ������в����ѷ죬ROAM�㷨������������3���ڽӹ�ϵ��Tb��ʾ�����ֱ��������T����б�ߵĵ��ھӣ�
	//Tl��ʾ�����ֱ��������T������ֱ�Ǳߵ����ھӣ�Tr��ʾ�����ֱ��������T������ֱ�Ǳߵ����ھӡ�
	//Tb��T�������Ƕ�������ͬһ����γɵ������ζԣ���֮Ϊ��ʯ�ͣ�Diamond����
	//���T�����ѣ�Ϊ�˱�����������ѷ����Ӱ����������TbҲ��Ҫ�����ѡ��ڶ������ṹ�е�Tb��T��һ�����ʱ��
	//����Tb�����������ھӱ����ѣ����ջᵼ��һϵ�еݹ���ѡ������ĵݹ���ѳ�֮Ϊǿ�Ʒ��ѣ�forced split����
	//���ֻ�е������ζ��γ������ͽṹ���ܹ���ִ�з��Ѳ���������ͻ�����ǿ�Ʒ��Ѳ�����
	//T��Tb�ڶ������ͬ��һ����β��Ҷ������ѹ�һ�Σ���֮Ϊ�ɺϲ���ʯ�ͣ�Mergeable Diamond����
	//ֻ�е������ζ��γɿɺϲ������ͽṹ��ʱ����ܱ�ִ�кϲ�������

	//֡��һ����
	//֡��һ������ROAM�еĸ߼��Ż������������������˵�����һ֡������������Ա��ٴ�ʹ�á�
	//�������Ҳ�����������ж�̬֡��ʱ�������������ĸĽ���ǰ֡������ֱ����֡������
	//��һ�����ٶ�����Ϸ�У�����ζ���㲻�ػ���ʱ����е��ηֿ飬�෴�����ȴ�����������Ҫ�Ŀ��ٶ�������������֡ʱ�侲ֹʱ���е��ηֿ飬���ڽ���ʱ������Ⱦ��
	//���һ������ڽ��н���ʱ�����ν���һ���ͼ�ϸ������̬��Ⱦ�Ա���ʱ�䡣�ñ��ĵĿռ�������֡��һ������ԶԶ�����ģ����Ƕ�������һЩС�ı��ⲽ�裺
	//����һ�����ڵ�ָ�뵽TriTreeNode�У�����һ������Split()������Merge()������ʹ��һ�����ȶ��л��������Ƚṹ����������MESH�е�Ҷ�ڵ㡣
	//�ڷֿ�����У����ŷָ���һ֡�зǳ��ֲڵĽڵ�Ĳ������ϲ����б�֡���㹻DETAIL�Ľڵ㣨��ֱ��ʱ���������



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
		//TODO �Ż�����Ӳ���patchIndex
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