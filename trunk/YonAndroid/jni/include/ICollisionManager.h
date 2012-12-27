#ifndef _YON_SCENE_ICOLLISIONMANAGER_H_
#define _YON_SCENE_ICOLLISIONMANAGER_H_

#include "IReferencable.h"
#include "triangle3d.h"
#include "position2d.h"
#include "line3d.h"

namespace yon{
namespace scene{

	class ISceneNode;
	class ITriangleSelector;

	namespace camera{
		class ICamera;
	}

	//! The Scene Collision Manager provides methods for performing collision tests and picking on scene nodes.
	class ICollisionManager : public virtual core::IReferencable{

		//! Finds the nearest collision point of a line and lots of triangles, if there is one.
		/** \param ray: Line with which collisions are tested.
		\param selector: TriangleSelector containing the triangles. It
		can be created for example using
		ISceneManager::createTriangleSelector() or
		ISceneManager::createTriangleOctreeSelector().
		\param outCollisionPoint: If a collision is detected, this will
		contain the position of the nearest collision to the line-start.
		\param outTriangle: If a collision is detected, this will
		contain the triangle with which the ray collided.
		\param outNode: If a collision is detected, this will contain
		the scene node associated with the triangle that was hit.
		\return True if a collision was detected and false if not. */
		virtual bool getCollisionPoint(const core::line3d<f32>& ray,
			ITriangleSelector* selector, core::vector3df& outCollisionPoint,
			core::triangle3df& outTriangle, const ISceneNode*& outNode) =0;


		//! Perform a ray/box and ray/triangle collision check on a heirarchy of scene nodes.
		/** This checks all scene nodes under the specified one, first by ray/bounding
		box, and then by accurate ray/triangle collision, finding the nearest collision,
		and the scene node containg it.  It returns the node hit, and (via output
		parameters) the position of the collision, and the triangle that was hit.

		All scene nodes in the hierarchy tree under the specified node are checked. Only
		nodes that are visible, with an ID that matches at least one bit in the supplied
		bitmask, and which have a triangle selector are considered as candidates for being hit.
		You do not have to build a meta triangle selector; the individual triangle selectors
		of each candidate scene node are used automatically.

		\param ray: Line with which collisions are tested.
		\param outCollisionPoint: If a collision is detected, this will contain the
		position of the nearest collision.
		\param outTriangle: If a collision is detected, this will contain the triangle
		with which the ray collided.
		\param idBitMask: Only scene nodes with an id which matches at least one of the
		bits contained in this mask will be tested. However, if this parameter is 0, then
		all nodes are checked.
		\param collisionRootNode: the scene node at which to begin checking. Only this
		node and its children will be checked. If you want to check the entire scene,
		pass 0, and the root scene node will be used (this is the default).
		\param noDebugObjects: when true, debug objects are not considered viable targets.
		Debug objects are scene nodes with IsDebugObject() = true.
		\return Returns the scene node containing the hit triangle nearest to ray.start.
		If no collision is detected, then 0 is returned. */
		/*virtual ISceneNode* getSceneNodeAndCollisionPointFromRay(
			core::line3df ray,
			core::vector3df & outCollisionPoint,
			core::triangle3df & outTriangle,
			s32 idBitMask = 0,
			ISceneNode * collisionRootNode = 0,
			bool noDebugObjects = false) = 0;*/

	};
}
}
#endif