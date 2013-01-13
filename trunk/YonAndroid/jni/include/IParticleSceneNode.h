#ifndef _YON_SCENE_EFFECT_IPARTICLESCENENODE_H_
#define _YON_SCENE_EFFECT_IPARTICLESCENENODE_H_

#include "ISceneNode.h"

namespace yon{
namespace scene{
namespace effect{

	//! A particle scene node for creating snow, fire, exlosions, smoke...
	/** A scene node controlling a particle System. The behavior of the particles
	can be controlled by setting the right particle emitters and affectors.
	You can for example easily create a campfire by doing this:

	\code
	scene::IParticleSystemSceneNode* p = scenemgr->addParticleSystemSceneNode();
	p->setParticleSize(core::dimension2d<f32>(20.0f, 10.0f));
	scene::IParticleEmitter* em = p->createBoxEmitter(
		core::aabbox3d<f32>(-5,0,-5,5,1,5),
		core::vector3df(0.0f,0.03f,0.0f),
		40,80, video::SColor(0,255,255,255),video::SColor(0,255,255,255), 1100,2000);
	p->setEmitter(em);
	em->drop();
	scene::IParticleAffector* paf = p->createFadeOutParticleAffector();
	p->addAffector(paf);
	paf->drop();
	\endcode

	*/

	class IParticleEmitter;
	class IParticleAffector;

	class IParticleSceneNode : public ISceneNode{
	public:
		IParticleSceneNode(ISceneNode* parent,
			const core::vector3df& pos = core::ORIGIN_VECTOR3DF,
			const core::vector3df& rotation = core::ORIGIN_VECTOR3DF,
			const core::vector3df& scale = core::IDENTITY_VECTOR3DF)
			: ISceneNode(parent, pos, rotation, scale) {}

		//! Sets the size of all particles.
		virtual void setParticleSize(f32 width=5.0f,f32 height=5.0f) = 0;

		//! Sets if the particles should be global.
		/** If they are, the particles are affected by the movement of the
		particle system scene node too, otherwise they completely ignore it.
		Default is true. */
		//virtual void setParticlesAreGlobal(bool global=true) = 0;

		//! Gets the particle emitter, which creates the particles.
		/** \return The particle emitter. Can be 0 if none is set. */
		virtual IParticleEmitter* getEmitter() =0;

		//! Sets the particle emitter, which creates the particles.
		/** A particle emitter can be created using one of the createEmitter
		methods. For example to create and use a simple PointEmitter, call
		IParticleEmitter* p = createPointEmitter(); 
		setEmitter(p); 
		p->drop();
		\param emitter: Sets the particle emitter. You can set this to 0 for
		removing the current emitter and stopping the particle system emitting
		new particles. */
		virtual void setEmitter(IParticleEmitter* emitter) = 0;

		//! Adds new particle effector to the particle system.
		/** A particle affector modifies the particles. For example, the FadeOut
		affector lets all particles fade out after some time. It is created and
		used in this way:
		\code
		IParticleAffector* p = createFadeOutParticleAffector();
		addAffector(p);
		p->drop();
		\endcode
		Please note that an affector is not necessary for the particle system to work.
		\param affector: New affector. */
		virtual void addAffector(IParticleAffector* affector) = 0;

		//! Removes all particle affectors in the particle system.
		virtual void removeAllAffectors() = 0;
	};
}
}
}
#endif