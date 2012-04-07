#ifndef _YON_SCENE_ANIMATOR_IANIMATOR_H_
#define _YON_SCENE_ANIMATOR_IANIMATOR_H_

#include "position3d.h"

namespace yon{
namespace scene{
	class IModel;
namespace animator{

	enum ENUM_ANIMATOR_TYPE{
		ENUM_ANIMATOR_TYPE_UV = 0,
		ENUM_ANIMATOR_TYPE_SURROUND,
		ENUM_ANIMATOR_TYPE_COUNT
	};

	struct SAnimatorParam{

		struct SAnimatorUV{
			u32 unitIndex;
			u32 stage;
		};

		struct SAnimatorSurround{
			//TODO
			f32 x,y,z;
		};

		ENUM_ANIMATOR_TYPE type;
		union{
			struct SAnimatorUV animatorUV;
			struct SAnimatorSurround animatorSurround;
		};
	};

	class IAnimator : public virtual core::IReferencable{
	public:
		virtual void animateNode(IModel* model, u32 timeMs) =0;
		virtual ENUM_ANIMATOR_TYPE getType() const = 0;
	};
}
}
}
#endif