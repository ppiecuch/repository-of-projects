#ifndef _YON_SCENE_EFFECT_IPARTICLEAFFECTOR_H_
#define _YON_SCENE_EFFECT_IPARTICLEAFFECTOR_H_

namespace yon{
namespace scene{
namespace effect{

	//! Types of built in particle affectors
	enum ENUM_AFFECTOR_TYPE
	{
		ENUM_AFFECTOR_TYPE_NONE = 0,
		ENUM_AFFECTOR_TYPE_ATTRACT,
		ENUM_AFFECTOR_TYPE_FADE_OUT,
		ENUM_AFFECTOR_TYPE_GRAVITY,
		ENUM_AFFECTOR_TYPE_ROTATE,
		ENUM_AFFECTOR_TYPE_SCALE,
		ENUM_AFFECTOR_TYPE_COUNT
	};

	//! Names for built in particle affectors
	const c8* const AffectorTypeNames[] =
	{
		"None",
		"Attract",
		"FadeOut",
		"Gravity",
		"Rotate",
		"Scale",
		0
	};

	class IParticleAffector : public virtual core::IReferencable{
	protected:
		bool m_bEnabled;
	public:
		IParticleAffector() : m_bEnabled(true) {}
	};
}
}
}
#endif