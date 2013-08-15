#ifndef _MUI_ISERIALIZABLE_H_
#define _MUI_ISERIALIZABLE_H_

namespace mui{

	class ISerializable{
	public:
		ISerializable() {}
		virtual ~ISerializable() {}

		//virtual void serialization(xml::ElementPtr _node, Version _version) { }
		//virtual void deserialization(xml::ElementPtr _node, Version _version) { }
		virtual void serialization(xml::ElementPtr node) {}
		virtual void deserialization(xml::ElementPtr node) {}
	};

}
#endif