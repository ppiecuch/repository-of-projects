#ifndef _YON_GUI_IINPUTABLE_H_
#define _YON_GUI_IINPUTABLE_H_

#include "yonUString.h"

namespace yon{
namespace gui{

	class IInputable{
	public:
		virtual void setEnable(bool on) = 0;
		virtual bool isEnable() const = 0;

		virtual const core::ustring& getLabel() const = 0;
	};
}
}
#endif