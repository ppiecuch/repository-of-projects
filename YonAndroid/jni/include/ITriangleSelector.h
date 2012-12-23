#ifndef _YON_SCENE_ITRIANGLESELECTOR_H_
#define _YON_SCENE_ITRIANGLESELECTOR_H_

#include "IReferencable.h"
#include "triangle3d.h"
#include "aabbox3d.h"
#include "matrix4.h"
#include "line3d.h"

namespace yon{
namespace scene{
	
	class ISceneNode;

	//! Interface to return triangles with specific properties.
	/** Every ISceneNode may have a triangle selector, available with
	ISceneNode::getTriangleScelector() or ISceneManager::createTriangleSelector.
	This is used for doing collision detection: For example if you know, that a
	collision may have happened in the area between (1,1,1) and (10,10,10), you
	can get all triangles of the scene node in this area with the
	ITriangleSelector easily and check every triangle if it collided. */
	class ITriangleSelector : public virtual core::IReferencable{
	public:
		virtual ~ITriangleSelector() {}
		//! Returns amount of all available triangles in this selector
		virtual s32 getTriangleCount() const = 0;

		//! Gets the triangles for one associated node which have or may have contact with a 3d line.
		/**
		This returns all triangles for one scene node associated with this
		selector.  If there is more than one scene node associated (e.g. for
		an IMetaTriangleSelector) this this function may be called multiple
		times to retrieve all triangles.

		Please note that unoptimized triangle selectors also may return
		triangles which are not in contact at all with the 3d line.
		\param triangles: Array where the resulting triangles will be written
		to.
		\param arraySize: Size of the target array.
		\param outTriangleCount: Amount of triangles which have been written
		into the array.
		\param line: Only triangles which may be in contact with this 3d line
		will be written into the array.
		\param transform: Pointer to matrix for transforming the triangles
		before they are returned. Useful for example to scale all triangles
		down into an ellipsoid space. If this pointer is null, no
		transformation will be done. */
		virtual void getTriangles(core::triangle3df* triangles, s32 arraySize,
			s32& outTriangleCount, const core::line3df& line,
			const core::matrix4* transform=NULL) const = 0;
	};
}
}
#endif