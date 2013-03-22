#ifndef _YON_IO_ISERIALIZABLE_H_
#define _YON_IO_ISERIALIZABLE_H_

namespace yon{
namespace io{

	class IReadStream;
	class IWriteStream;

	class ISerializable{
	public:
		virtual bool serialize(io::IWriteStream* ws){return false;}
		virtual bool deserialize(io::IReadStream* rs){return false;}
	};
}
}
#endif