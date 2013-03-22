#ifndef _YON_VIDEO_SRENDERUNIT_H_
#define _YON_VIDEO_SRENDERUNIT_H_

#include "objectpool.h"
#include "yonArray.h"
#include "SVertex.h"

namespace yon{
namespace video{

	class ITexture;

	template<class V>
	struct SRenderUnit : public core::IRecyclable{
	public:
		video::ITexture* Texture;
		core::array<V> Vertices;
		u16 IndexCount;

		virtual void reset(){
			Texture=NULL;
			Vertices.set_used(0);
			IndexCount=0;
		}

		scene::ENUM_VERTEX_TYPE getType() const{
			return V::getType();
		}
	};

	template<class V>
	struct SRenderEntry{
		video::SRenderUnit<V>* Unit;

		inline bool operator < (const SRenderEntry<V>& other) const{
			return Unit->Texture < other.Unit->Texture;
		}

		scene::ENUM_VERTEX_TYPE getType() const{
			return V::getType();
		}
	};

	typedef SRenderUnit<scene::SVertex> SRenderUnit3D;
	typedef SRenderUnit<scene::S2DVertex> SRenderUnit2D;

	typedef SRenderEntry<scene::SVertex> SRenderEntry3D;
	typedef SRenderEntry<scene::S2DVertex> SRenderEntry2D;

}
}
#endif