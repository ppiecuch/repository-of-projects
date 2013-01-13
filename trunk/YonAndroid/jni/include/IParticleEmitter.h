#ifndef _YON_SCENE_EFFECT_IPARTICLEEMITTER_H_
#define _YON_SCENE_EFFECT_IPARTICLEEMITTER_H_

#include "IReferencable.h"

namespace yon{
namespace scene{
namespace effect{

	//! Types of built in particle emitters
	enum ENUM_EMITTER_TYPE
	{
		ENUM_EMITTER_TYPE_POINT = 0,
		ENUM_EMITTER_TYPE_ANIMATED_MESH,
		ENUM_EMITTER_TYPE_BOX,
		ENUM_EMITTER_TYPE_CYLINDER,
		ENUM_EMITTER_TYPE_MESH,
		ENUM_EMITTER_TYPE_RING,
		ENUM_EMITTER_TYPE_SPHERE,
		ENUM_EMITTER_TYPE_COUNT
	};

	//! Names for built in particle emitters
	const c8* const EmitterTypeNames[] =
	{
		"Point",
		"AnimatedMesh",
		"Box",
		"Cylinder",
		"Mesh",
		"Ring",
		"Sphere",
		0
	};

	class IParticleEmitter : public virtual core::IReferencable{
	public:
		//! Set direction the emitter emits particles
		virtual void setDirection( const core::vector3df& newDirection ) = 0;

		//! Set minimum number of particles the emitter emits per second
		virtual void setMinParticlesPerSecond( u32 minPPS ) = 0;

		//! Set maximum number of particles the emitter emits per second
		virtual void setMaxParticlesPerSecond( u32 maxPPS ) = 0;

		//! Set minimum starting color for particles
		virtual void setMinStartColor( const video::SColor& color ) = 0;

		//! Set maximum starting color for particles
		virtual void setMaxStartColor( const video::SColor& color ) = 0;

		//! Set the maximum starting size for particles
		virtual void setMaxStartSize( const core::dimension2df& size ) = 0;

		//! Set the minimum starting size for particles
		virtual void setMinStartSize( const core::dimension2df& size ) = 0;

		//! Get direction the emitter emits particles
		virtual const core::vector3df& getDirection() const = 0;

		//! Get the minimum number of particles the emitter emits per second
		virtual u32 getMinParticlesPerSecond() const = 0;

		//! Get the maximum number of particles the emitter emits per second
		virtual u32 getMaxParticlesPerSecond() const = 0;

		//! Get the minimum starting color for particles
		virtual const video::SColor& getMinStartColor() const = 0;

		//! Get the maximum starting color for particles
		virtual const video::SColor& getMaxStartColor() const = 0;

		//! Get the maximum starting size for particles
		virtual const core::dimension2df& getMaxStartSize() const = 0;

		//! Get the minimum starting size for particles
		virtual const core::dimension2df& getMinStartSize() const = 0;

		//! Get emitter type
		virtual ENUM_EMITTER_TYPE getType() const { return ENUM_EMITTER_TYPE_POINT; }
	};
}
}
}
#endif