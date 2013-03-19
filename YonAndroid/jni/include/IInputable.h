#ifndef _YON_GUI_IINPUTABLE_H_
#define _YON_GUI_IINPUTABLE_H_

namespace yon{
namespace gui{

	class IInputable{
	public:
		virtual void setEnable(bool on) = 0;
		virtual bool isEnable() const = 0;
	};
}
}
#endif