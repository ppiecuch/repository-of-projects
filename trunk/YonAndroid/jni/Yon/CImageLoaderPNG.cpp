#include "CImageLoaderPNG.h"

#include "lpng/png.h"

#include "CImage.h"
#include "CReadFile.h"

#include "yonUtil.h"

#include "ILogger.h"

namespace yon{
namespace video{

	static void png_cpexcept_error(png_structp png_ptr, png_const_charp msg)
	{
		Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("libpng occur error:%s",msg));
		//longjmp(png_ptr->longjmp_buffer, 1);
	}

	static void png_cpexcept_warn(png_structp png_ptr, png_const_charp msg)
	{
		Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("libpng occur warn:%s",msg));
	}

	bool CImageLoaderPng::checkFileExtension(const io::path& filename) const{
		return core::hasFileExtension(filename,"png");
	}

	bool CImageLoaderPng::checkFileHeader(io::IReadFile* file) const{
		if (!file)
			return false;

		png_byte buffer[8];
		// Read the first few bytes of the PNG file
		if (file->read(buffer, 8) != 8)
			return false;

		//��֤�Ƿ�PNG�ļ�
		return !png_sig_cmp(buffer, 0, 8);
	}

	IImage* CImageLoaderPng::loadImage(io::IReadFile* file) const{
		if (!file)
			return NULL;

		video::IImage* image = NULL;

		//libpng����������Ҫ�Ľṹ�壺png_struct��png_info
		//png_struct��ÿһ��libpng�����������õ��Ĳ���
		//png_info�ṩ��PNG�ļ���ص���Ϣ������ʹ��png_get_*()/png_set_*()
		png_structp png_ptr;
		png_infop info_ptr,end_info;
		unsigned int sig_read = 8;
		u32 width, height;
		s32 bit_depth, color_type;
		png_byte header[8];

		//��֤�Ƿ�PNG�ļ�
		if (file->read(header, sig_read) != sig_read)
			return NULL;
		if(png_sig_cmp(header,0,sig_read))
		{
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("%s check header failed!",file->getFileName().c_str()));
			return NULL;
		}

		//��������ʼ��png_struct
		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL, png_cpexcept_error, png_cpexcept_warn);
		if (png_ptr == NULL)
		{
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("%s fail to initialize png_struct!",file->getFileName().c_str()));
			return NULL;
		}

		//��������ʼ��png_info
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL)
		{
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("%s fail to initialize start png_info!",file->getFileName().c_str()));
			png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
			return NULL;
		}
		end_info = png_create_info_struct(png_ptr);
		if (!end_info) {
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("%s fail to initialize end png_info!",file->getFileName().c_str()));
			png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
			return NULL;
		}

		//��libpng������������������ʱ��������Ҫlongjmp back to our routine��
		//ͨ������setjmp��png_jmpbuf(png_ptr)ʵ��
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("fail to parse png file: %s",file->getFileName().c_str()));
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			return NULL;
		}

		//��ȡ�ļ�
		png_init_io(png_ptr, file->pointer());

		//֪ͨlibpng�����Ѿ���ȡ�˼����ֽ����ڼ����ļ������8����
		png_set_sig_bytes(png_ptr, sig_read);

		//png_read_info�ײ㴦��,���ڻ�ȡͼ�����ݵ����������Ϣ
		//libpng�ᴦ���������Ϣ�����������ͼ������(image data)
		png_read_info(png_ptr, info_ptr);

		//����API��ѯͼ����Ϣ
		{
			png_uint_32 w,h;
			png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type,NULL, NULL, NULL);
			width=w;
			height=h;
		}

		//�����������ɫ��תΪRGBɫ
		if (color_type == PNG_COLOR_TYPE_PALETTE)
			png_set_palette_to_rgb(png_ptr);

		//��λ��ɫΪ8λ��ɫ
		if (bit_depth < 8)
		{
			if (color_type==PNG_COLOR_TYPE_GRAY || color_type==PNG_COLOR_TYPE_GRAY_ALPHA){
				//ת����CMYKɫ
				png_set_expand_gray_1_2_4_to_8(png_ptr);
			}else{
				//����ѹ�������𣩣���ͨ���㷨����ɫֵѹ��������һ��������1��2��4��8����ɫ����ôԭ����4byte����ģ�����1byte���档
				png_set_packing(png_ptr);
			}
		}

		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)){
			//����ALPHAͨ��
			png_set_tRNS_to_alpha(png_ptr);
		}

		if (bit_depth == 16){
			//��ÿͨ��16λɫ�ĳ�8λɫ
			png_set_strip_16(png_ptr);
		}

		//�Ҷ�ɫΪRGBɫ
		if (color_type==PNG_COLOR_TYPE_GRAY || color_type==PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_gray_to_rgb(png_ptr);

#if 0
		if (color_type==PNG_COLOR_TYPE_RGB_ALPHA)
		{
			//TODO __BIG_ENDIAN__
			//ת��RGBAΪARGB(��BGRAתΪABGR) 
			//png_set_swap_alpha(png_ptr);
			//ת��RGBΪBGR(��RGBAתΪBGRA) 
			png_set_bgr(png_ptr);
		}
#endif

		//����һ��ת�����ٴε���API��ѯͼ����Ϣ
		{
			png_uint_32 w,h;
			png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type,NULL, NULL, NULL);
			width=w;
			height=h;
		}

		//����ͼ������������
		if (color_type==PNG_COLOR_TYPE_RGB_ALPHA)
			image = new CImage(ENUM_COLOR_FORMAT_R8G8B8A8, core::dimension2d<u32>(width, height));
		else
			image = new CImage(ENUM_COLOR_FORMAT_R8G8B8, core::dimension2d<u32>(width, height));
		if (!image)
		{
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("fail to create image from png file: %s",file->getFileName().c_str()));
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			return NULL;
		}
			
		//ͼ�����ݵĶ�ά����
		u8** rowPointers = new png_bytep[height];
		if (!rowPointers)
		{
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("fail to create row pointers for png file: %s",file->getFileName().c_str()));
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			image->drop();
			return NULL;
		}

		//��ֵÿ��ָ��
		u8* data = (u8*)image->lock();
		for (u32 i=0; i<height; ++i)
		{
			rowPointers[i]=data;
			data += image->getByteCountPerRow();
		}

		// for proper error handling
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			delete [] rowPointers;
			image->unlock();
			image->drop();
			return NULL;
		}

		//Read data using the library function that handles all transformations including interlacing
		png_read_image(png_ptr, rowPointers);

		//Read the end of the PNG file.  Will not read past the end of the
		//file, will verify the end is accurate, and will read any comments
		//or time information at the end of the file, if info is not NULL.
		png_read_end(png_ptr, NULL);
		delete [] rowPointers;
		image->unlock();
		// Clean up memory
		png_destroy_read_struct(&png_ptr,&info_ptr, 0); 

		return image;
	}

	IImageLoader* createImageLoaderPNG()
	{
		return new CImageLoaderPng();
	}
}//video
}//yon