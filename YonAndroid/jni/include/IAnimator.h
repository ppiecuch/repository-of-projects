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
		ENUM_ANIMATOR_TYPE_ALPHA,
		ENUM_ANIMATOR_TYPE_FPSCAMERA,
		ENUM_ANIMATOR_TYPE_COUNT
	};

	struct SAnimatorParam{

		struct SAnimatorUV{
			//SAnimatorUV():unitIndex(0),stage(0){}
			u32 unitIndex;
			u32 stage;
			//core::vector3df translate;
			struct{
				f32 u;
				f32 v;
				f32 w;
			}translate;
		};

		struct SAnimatorSurround{
			//TODO
			f32 x,y,z;
		};

		struct SAnimatorAlpha{
			u32 unitIndex;
			u8 minValue;
			u8 maxValue;
			u8 increment;
		};

		ENUM_ANIMATOR_TYPE type;
		union{
			struct SAnimatorUV animatorUV;
			struct SAnimatorSurround animatorSurround;
			struct SAnimatorAlpha animatorAlpha;
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