#ifndef _YON_CORE_IRENDERABLE_H_
#define _YON_CORE_IRENDERABLE_H_

namespace yon{
	namespace video{
		class IVideoDriver;
	}
	namespace core{
		 
		class IRenderable{
		public:
			//TODO ¸ÄÎª virtual void render()
			virtual void render(video::IVideoDriver* driver){}
			virtual void render(){}
		};
	}
}
#endif