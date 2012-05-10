#ifndef _YON_GUI_IGUIENVIRENMENT_H_
#define _YON_GUI_IGUIENVIRENMENT_H_

namespace yon{
namespace gui{

	class IGUIEnvirenment : public core::IReferencable{
	public:
		
		virtual void init() = 0;
		virtual void onResize(const core::dimension2du& size) = 0;
		virtual void render() = 0;
	};
}
}
#endif