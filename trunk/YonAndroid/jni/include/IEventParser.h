#ifndef _YON_EVENT_IEVENTPARSER_H_
#define _YON_EVENT_IEVENTPARSER_H_
namespace yon{
namespace event{

	//TODO 待完善
	//事件解析器，如MFC的LButtonDown等参数的解析、Android多点触控事件的解析

	enum ENUM_EVENT_PARSER{
		ENUM_EVENT_PARSER_MFC = 0,
		ENUM_EVENT_PARSER_COUNT
	};

	class IEventParser{
	};
}
}
#endif