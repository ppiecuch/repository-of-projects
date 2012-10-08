#ifndef _YON_SCENE_ISKINNEDENTITY_H_
#define _YON_SCENE_ISKINNEDENTITY_H_

#include "yonArray.h"
#include "IBoneSceneNode.h"
#include "IAnimatedEntity.h"

namespace yon{
namespace scene{

	class ISkinnedEntity : public IAnimatedEntity{
	public:

		//! A vertex weight
		struct SWeight
		{
			//! Index of the unit
			u16 buffer_id; 

			//! Index of the vertex
			u32 vertex_id; //Store global ID here

			//! Weight Strength/Percentage (0-1)
			f32 strength;

		private:
			//! Internal members used by CSkinnedEntity
			friend class CSkinnedEntity;
			bool *Moved;
			core::vector3df StaticPos;
			//core::vector3df StaticNormal;
		};

		//! Animation keyframe which describes a new position
		struct SPositionKey
		{
			f32 frame;
			core::vector3df position;
		};

		//! Animation keyframe which describes a new scale
		struct SScaleKey
		{
			f32 frame;
			core::vector3df scale;
		};

		//! Animation keyframe which describes a new rotation
		struct SRotationKey
		{
			f32 frame;
			core::quaternion rotation;
		};

		//! Joints
		struct SJoint
		{
			SJoint() : UseAnimationFrom(0), LocalAnimatedMatrix_Animated(false), GlobalSkinningSpace(false),
				positionHint(-1),scaleHint(-1),rotationHint(-1),GlobalInversedMatrix(true),LocalMatrix(true)
			{
			}

			//! The name of this joint
			core::stringc Name;

			//! Local matrix of this joint
			core::matrix4 LocalMatrix;

			//! List of child joints
			core::array<SJoint*> Children;

			//! List of attached meshes
			core::array<u32> AttachedMeshes;

			//! Animation keys causing translation change
			core::array<SPositionKey> PositionKeys;

			//! Animation keys causing scale change
			core::array<SScaleKey> ScaleKeys;

			//! Animation keys causing rotation change
			core::array<SRotationKey> RotationKeys;

			//! Skin weights
			core::array<SWeight> Weights;

			//! Unnecessary for loaders, will be overwritten on finalize
			core::matrix4f GlobalMatrix;
			core::matrix4f GlobalAnimatedMatrix;
			core::matrix4f LocalAnimatedMatrix;
			core::vector3df Animatedposition;
			core::vector3df Animatedscale;
			core::quaternion Animatedrotation;

			core::matrix4 GlobalInversedMatrix; //the x format pre-calculates this

		private:
			//! Internal members used by CSkinnedEntity
			friend class CSkinnedEntity;

			SJoint *UseAnimationFrom;
			bool LocalAnimatedMatrix_Animated;

			bool GlobalSkinningSpace;

			s32 positionHint;
			s32 scaleHint;
			s32 rotationHint;
		};

		//Interface for the mesh loaders (finalize should lock these functions, and they should have some prefix like loader_

		//these functions will use the needed arrays, set values, etc to help the loaders

		//! Gets joint count.
		/** \return Amount of joints in the skeletal animated mesh. */
		virtual u32 getJointCount() const = 0;

		//! Animates this mesh's joints based on frame input
		virtual void animateMesh(f32 frame, f32 blend)=0;

		//! Preforms a software skin on this mesh based of joint positions
		virtual void skinEntity() = 0;

		//! exposed for loaders: to add units
		//virtual core::array<SSkinMeshBuffer*>& getMeshBuffers() = 0;
		virtual core::array<Unit3D*>& getUnits() = 0;

		//! exposed for loaders: joints list
		virtual core::array<SJoint*>& getAllJoints() = 0;

		//! exposed for loaders: joints list
		virtual const core::array<SJoint*>& getAllJoints() const = 0;

		//! loaders should call this after populating the mesh
		virtual void finalize() = 0;

		//! Adds a new unit to the entity, access it as last one
		//virtual SSkinMeshBuffer* addMeshBuffer() = 0;
		virtual Unit3D* addUnit() = 0;

		//! Adds a new joint to the mesh, access it as last one
		virtual SJoint* addJoint(SJoint *parent=0) = 0;

		//! Adds a new weight to the mesh, access it as last one
		virtual SWeight* addWeight(SJoint *joint) = 0;

		//! Adds a new position key to the mesh, access it as last one
		virtual SPositionKey* addPositionKey(SJoint *joint) = 0;
		//! Adds a new scale key to the mesh, access it as last one
		virtual SScaleKey* addScaleKey(SJoint *joint) = 0;
		//! Adds a new rotation key to the mesh, access it as last one
		virtual SRotationKey* addRotationKey(SJoint *joint) = 0;

		//! Check if the mesh is non-animated
		virtual bool isStatic()=0;

		//TODO
		//virtual bool attachSkeleton(ISkeleton* skeleton) = 0;
	};
}
}
#endif