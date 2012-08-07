#ifndef _YON_SCENE_TERRAIN_CROAMTERRAIN_H_
#define _YON_SCENE_TERRAIN_CROAMTERRAIN_H_

#include "ITerrainModel.h"
#include "IUnit.h"
#include "SDynamicShap.h"
#include "aabbox3d.h"

namespace yon{
namespace scene{
namespace terrain{

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
#endif