#ifndef _YON_SCENE_IRENDERABLE_H_
#define _YON_SCENE_IRENDERABLE_H_

namespace yon{
	namespace video{
		class IVideoDriver;
	}
	namespace scene{
		 
		class IRenderable{
		public:
			virtual void render(video::IVideoDriver* driver) = 0;
		};
	}
}
#endif