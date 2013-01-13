#ifndef _YON_SCENE_EFFECT_IEFFECTSYSTEM_H_
#define _YON_SCENE_EFFECT_IEFFECTSYSTEM_H_

#include "IReferencable.h"

namespace yon{
namespace scene{
namespace effect{

	//TODO 特效编辑器可参考：http://code.google.com/p/cocos2d-windows-particle-editor/

	class IEffectSystem : public virtual core::IReferencable{
	public:

		/**
		* @brief Gets the number of active particles in this system
		*
		* The number of active particles in the system is the addition of the number of active particles in each group of the system.<br>
		* Note that the number of active particle of the system is updated after each update of the system.<br>
		* This means if the user changes manually the number of particles in a group and call this method before an update, the number returned will not be up to date.<br>
		* To compute and get the real number of active particles in the System, see computeParticleCount().
		*
		* @return the number of active particle in the system
		*/
		virtual u32 getParticleCount() const = 0;

		/**
		* @brief Computes the number of active particles in this System and returns it
		*
		* Unlike getParticleCount() which returns the last number of particles computed (after a call to update(float) or empty()),
		* this method recomputes the current number of active particles by parsing all the groups of this System.<br>
		* In that way, this method must not be used as an accessor but call once when necesseray between 2 updates.<br>
		* <br>
		* Note that this method updates the inner number of particles of this System, which means a call to getParticleCount() will
		* then return the computed number.
		*
		* @return the number of active particle in the system
		*/
		virtual u32 computeParticleCount() = 0;

		/**
		* @brief Gets the number of groups in the System
		* @return the number of groups in the System
		*/
		virtual u32 getGroupCount() const = 0;

	};
}
}
}
#endif