#ifndef _YON_CORE_PLANE3D_H_
#define _YON_CORE_PLANE3D_H_

#include "yonTypes.h"
#include "vector3d.h"

namespace yon{
namespace core{

	template<class T>
	class plane3d{
	public:
		plane3d():normal(0,1,0){recalculateD(core::UP_VECTOR3DF);}
		plane3d(const core::vector3d<T>& normal,const core::vector3d<T> point):normal(normal){recalculateD(point);}

		//Ax+By+Cz+D=0
		void recalculateD(const vector3d<T>& point)
		{
			d = - point.dotProduct(normal);
		}

		core::vector3d<T> normal;
		T d;
	};
	typedef plane3d<f32> plane3df;
}
}
#endif