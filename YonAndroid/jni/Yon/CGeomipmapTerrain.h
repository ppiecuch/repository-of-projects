#ifndef _YON_SCENE_TERRAIN_CGEOMIPMAPTERRAIN_H_
#define _YON_SCENE_TERRAIN_CGEOMIPMAPTERRAIN_H_

#include "ITerrain.h"

namespace yon{
namespace scene{
namespace terrain{

	class CGeomipmapTerrain : public ITerrain{
	private:
		struct SPatch{
			f32 m_fDistance;
			s16 m_iLOD;
		};
	};
}
}
}
#endif