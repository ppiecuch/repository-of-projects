#ifndef _YON_VIDEO_IGPUPROGRAMMINGSERVICES_H_
#define _YON_VIDEO_IGPUPROGRAMMINGSERVICES_H_

namespace yon{
	namespace io{
		class IReadStream;
	}
namespace video{

	class IVideoDriver;

	class IGPUProgrammingServices{
	public:
		virtual ~IGPUProgrammingServices() {}

		//virtual s32 addShaderMaterialFromFiles(io::IReadStream* vertexShaderProgram,io::IReadStream* pixelShaderProgram,IShaderConstantSetCallBack* callback = 0,ENUM_MATERIAL_TYPE materialType = video::ENUM_MATERIAL_TYPE_SOLID,s32 userData = 0) = 0;
		//virtual s32 addShaderMaterialFromFiles(io::IReadStream* vertexShaderProgram,io::IReadStream* pixelShaderProgram) = 0;
	};
}
}
#endif