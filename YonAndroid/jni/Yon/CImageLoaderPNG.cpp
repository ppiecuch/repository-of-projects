#include "CImageLoaderPNG.h"

#include "lpng/png.h"

#include "CImage.h"

#include "yonUtil.h"

#include "ILogger.h"

//for more detail, refer to:http://www.libpng.org/pub/png/libpng-manual.txt

namespace yon{
namespace video{

	static void png_cpexcept_error(png_structp png_ptr, png_const_charp msg)
	{
		Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("libpng occur error:%s",msg).c_str());
		//longjmp(png_ptr->longjmp_buffer, 1);
	}

	static void png_cpexcept_warn(png_structp png_ptr, png_const_charp msg)
	{
		Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("libpng occur warn:%s",msg).c_str());
	}

	bool CImageLoaderPng::checkFileExtension(const io::path& filename) const{
		return core::hasFileExtension(filename,"png");
	}

	bool CImageLoaderPng::checkFileHeader(io::IReadStream* file) const{
		if (!file)
			return false;

		png_byte buffer[8];
		// Read the first few bytes of the PNG file
		if (file->read(buffer, 8) != 8)
			return false;

		//验证是否PNG文件
		return !png_sig_cmp(buffer, 0, 8);
	}

	video::IImage* translateIntoColourImage(u32& width,u32& height,s32& bit_depth, s32& color_type,png_structp png_ptr,png_infop info_ptr)
	{
		video::IImage* image = NULL;

		//如果是索引颜色，转为RGB色
		if (color_type == PNG_COLOR_TYPE_PALETTE)
			png_set_palette_to_rgb(png_ptr);

		//低位颜色为8位颜色
		if (bit_depth < 8)
		{
			if (color_type==PNG_COLOR_TYPE_GRAY || color_type==PNG_COLOR_TYPE_GRAY_ALPHA){
				//转换成CMYK色
				png_set_expand_gray_1_2_4_to_8(png_ptr);
			}else{
				//象素压缩（无损），是通过算法将颜色值压缩。比如一个象素是1、2、4、8的颜色，那么原来用4byte保存的，则用1byte保存。
				png_set_packing(png_ptr);
			}
		}

		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)){
			//加入ALPHA通道
			png_set_tRNS_to_alpha(png_ptr);
		}

		if (bit_depth == 16){
			//将每通道16位色改成8位色
			png_set_strip_16(png_ptr);
		}

		png_set_interlace_handling(png_ptr);
		png_read_update_info(png_ptr, info_ptr);
		color_type = png_get_color_type(png_ptr, info_ptr);

		//灰度色为RGB色
		if (color_type==PNG_COLOR_TYPE_GRAY || color_type==PNG_COLOR_TYPE_GRAY_ALPHA)
		{
			png_set_gray_to_rgb(png_ptr);
		}

		png_set_interlace_handling(png_ptr);
		png_read_update_info(png_ptr, info_ptr);
		color_type = png_get_color_type(png_ptr, info_ptr);

		//创建图像对象并填充数据
		if (color_type==PNG_COLOR_TYPE_RGB_ALPHA)
			image = new CImage(ENUM_COLOR_FORMAT_R8G8B8A8, core::dimension2d<u32>(width, height));
		else
			image = new CImage(ENUM_COLOR_FORMAT_R8G8B8, core::dimension2d<u32>(width, height));

		return image;
	}

	video::IImage* translateIntoGrayscaleImage(u32& width,u32& height,s32& bit_depth, s32& color_type,png_structp png_ptr,png_infop info_ptr)
	{
		video::IImage* image = NULL;

		//如果是索引颜色，转为RGB色
		if (color_type == PNG_COLOR_TYPE_PALETTE)
			png_set_palette_to_rgb(png_ptr);

		//低位颜色为8位颜色
		if (bit_depth < 8)
		{
			if (color_type==PNG_COLOR_TYPE_GRAY || color_type==PNG_COLOR_TYPE_GRAY_ALPHA){
				//转换成CMYK色
				png_set_expand_gray_1_2_4_to_8(png_ptr);
			}else{
				//象素压缩（无损），是通过算法将颜色值压缩。比如一个象素是1、2、4、8的颜色，那么原来用4byte保存的，则用1byte保存。
				png_set_packing(png_ptr);
			}
		}

		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)){
			//加入ALPHA通道
			png_set_tRNS_to_alpha(png_ptr);
		}

		if (bit_depth == 16){
			//将每通道16位色改成8位色
			png_set_strip_16(png_ptr);
		}

		png_set_interlace_handling(png_ptr);
		png_read_update_info(png_ptr, info_ptr);
		//width = png_get_image_width(png_ptr, info_ptr);
		//height = png_get_image_height(png_ptr, info_ptr);
		//bit_depth = png_get_bit_depth(png_ptr, info_ptr);
		color_type = png_get_color_type(png_ptr, info_ptr);

		//RGB色转为灰度色
		if (color_type==PNG_COLOR_TYPE_RGB || color_type==PNG_COLOR_TYPE_RGB_ALPHA)
		{
			/*
			Conversely, you can convert an RGB or RGBA image to grayscale or grayscale with alpha. This is intended for conversion of images that really are gray (red == green == blue), so the function simply strips out the red and blue channels, leaving the green channel in the gray position.
			if (color_type == PNG_COLOR_TYPE_RGB ||
			color_type == PNG_COLOR_TYPE_RGB_ALPHA)
			png_set_rgb_to_gray(png_ptr, error_action,
			float red_weight, float green_weight);

			error_action = 1: silently do the conversion
			error_action = 2: issue a warning if the original
			image has any pixel where
			red != green or red != blue
			error_action = 3: issue an error and abort the
			conversion if the original
			image has any pixel where
			red != green or red != blue

			red_weight:       weight of red component
			(NULL -> default 54/256)
			green_weight:     weight of green component
			(NULL -> default 183/256)
			If you have set error_action = 1 or 2, you can later check whether the image really was gray, after processing the image rows, with the png_get_rgb_to_gray_status(png_ptr) function. It will return a png_byte that is zero if the image was gray or 1 if there were any non-gray pixels. bKGD and sBIT data will be silently converted to grayscale, using the green channel data, regardless of the error_action setting.
			With 0.0<=red_weight+green_weight<=1.0, the normalized graylevel is computed:
			int rw = red_weight * 256;
			int gw = green_weight * 256;
			int bw = 256 - (rw + gw);
			gray = (rw*red + gw*green + bw*blue)/256;
			The default values approximate those recommended in the Charles Poynton's Color FAQ, http://www.inforamp.net/~poynton/ Copyright (c) 1998-01-04 Charles Poynton poynton@inforamp.net
			Y = 0.212671 * R + 0.715160 * G + 0.072169 * B
			Libpng approximates this with
			Y = 0.211 * R + 0.715 * G + 0.074 * B
			which can be expressed with integers as
			Y = (54 * R + 183 * G + 19 * B)/256
			The calculation is done in a linear colorspace, if the image gamma is known.
			*/
			//png_set_rgb_to_gray(png_ptr,1,0.299f, 0.587f);
			png_set_rgb_to_gray(png_ptr,1,-1,-1);
		}

		png_set_interlace_handling(png_ptr);
		png_read_update_info(png_ptr, info_ptr);
		color_type = png_get_color_type(png_ptr, info_ptr);

		//创建图像对象并填充数据
		if (color_type==PNG_COLOR_TYPE_GRAY_ALPHA)
			image = new CImage(ENUM_COLOR_FORMAT_L8A8, core::dimension2d<u32>(width, height));
		else
			image = new CImage(ENUM_COLOR_FORMAT_L8, core::dimension2d<u32>(width, height));

		return image;
	}

	IImage* CImageLoaderPng::loadImage(io::IReadStream* file,bool translateIntoGray) const{
		if (!file)
			return NULL;

		video::IImage* image = NULL;

		//libpng中有两个重要的结构体：png_struct和png_info
		//png_struct是每一个libpng函数调用须用到的参数
		//png_info提供了PNG文件相关的信息，建议使用png_get_*()/png_set_*()
		png_structp png_ptr;
		png_infop info_ptr,end_info;
		unsigned int sig_read = 8;
		u32 width, height;
		s32 bit_depth, color_type;
		png_byte header[8];

		//验证是否PNG文件
		if (file->read(header, sig_read) != sig_read)
			return NULL;
		if(png_sig_cmp(header,0,sig_read))
		{
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("%s check header failed!",file->getPath().c_str()).c_str());
			return NULL;
		}

		//创建并初始化png_struct
		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL, png_cpexcept_error, png_cpexcept_warn);
		if (png_ptr == NULL)
		{
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("%s fail to initialize png_struct!",file->getPath().c_str()).c_str());
			return NULL;
		}

		//创建并初始化png_info
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL)
		{
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("%s fail to initialize start png_info!",file->getPath().c_str()).c_str());
			png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
			return NULL;
		}
		end_info = png_create_info_struct(png_ptr);
		if (!end_info) {
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("%s fail to initialize end png_info!",file->getPath().c_str()).c_str());
			png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
			return NULL;
		}

		//当libpng解析过程中遇到错误时，我们需要longjmp back to our routine，
		//通过调用setjmp和png_jmpbuf(png_ptr)实现
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("fail to parse png file: %s",file->getPath().c_str()).c_str());
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			return NULL;
		}

		//读取文件
		png_init_io(png_ptr, (FILE*)file->pointer());

		//通知libpng我们已经读取了几个字节用于检验文件（最多8个）
		png_set_sig_bytes(png_ptr, sig_read);

		//png_read_info底层处理,用于获取图像数据的所有相关信息
		//libpng会处理尽量多的信息，但不会包括图像数据(image data)
		png_read_info(png_ptr, info_ptr);

		//调用API查询图像信息
		{
			png_uint_32 w,h;
			png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type,NULL, NULL, NULL);
			width=w;
			height=h;
		}
#if 1
	if(translateIntoGray)
		image=translateIntoGrayscaleImage(width,height,bit_depth,color_type,png_ptr,info_ptr);
	else
		image=translateIntoColourImage(width,height,bit_depth,color_type,png_ptr,info_ptr);

	if (!image)
	{
		Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("fail to create image from png file: %s",file->getPath().c_str()).c_str());
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return NULL;
	}

	//图像数据的二维数组
	u8** rowPointers = new png_bytep[height];
	if (!rowPointers)
	{
		Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("fail to create row pointers for png file: %s",file->getPath().c_str()).c_str());
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		image->drop();
		return NULL;
	}

	//赋值每行指针
	u8* data = (u8*)image->lock();
	for (u32 i=0; i<height; ++i)
	{
		rowPointers[height-i-1]=data;
		//rowPointers[i]=data;
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
	png_destroy_read_struct(&png_ptr,&info_ptr, &end_info); 
#elif 0
		//如果是索引颜色，转为RGB色
		if (color_type == PNG_COLOR_TYPE_PALETTE)
			png_set_palette_to_rgb(png_ptr);

		//低位颜色为8位颜色
		if (bit_depth < 8)
		{
			if (color_type==PNG_COLOR_TYPE_GRAY || color_type==PNG_COLOR_TYPE_GRAY_ALPHA){
				//转换成CMYK色
				png_set_expand_gray_1_2_4_to_8(png_ptr);
			}else{
				//象素压缩（无损），是通过算法将颜色值压缩。比如一个象素是1、2、4、8的颜色，那么原来用4byte保存的，则用1byte保存。
				png_set_packing(png_ptr);
			}
		}

		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)){
			//加入ALPHA通道
			png_set_tRNS_to_alpha(png_ptr);
			//修正从PALETTE+RGBA->RGBA时color_type仍为3的问题
			color_type=PNG_COLOR_TYPE_RGB_ALPHA;
		}

		if (bit_depth == 16){
			//将每通道16位色改成8位色
			png_set_strip_16(png_ptr);
		}

		//灰度色为RGB色
		if (color_type==PNG_COLOR_TYPE_GRAY || color_type==PNG_COLOR_TYPE_GRAY_ALPHA)
		{
			png_set_gray_to_rgb(png_ptr);
			//修正从GRAYALPHA->RGBA时color_type仍为4的问题
			if(color_type==PNG_COLOR_TYPE_GRAY)
				color_type=PNG_COLOR_TYPE_RGB;
			else
				color_type=PNG_COLOR_TYPE_RGB_ALPHA;
		}

#if 0
		if (color_type==PNG_COLOR_TYPE_RGB_ALPHA)
		{
			//TODO __BIG_ENDIAN__
			//转换RGBA为ARGB(或BGRA转为ABGR) 
			//png_set_swap_alpha(png_ptr);
			//转换RGB为BGR(或RGBA转为BGRA) 
			png_set_bgr(png_ptr);
		}
#endif

		//经过一番转换后再次调用API查询图像信息
		/*{
		png_uint_32 w,h;
		png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type,NULL, NULL, NULL);
		width=w;
		height=h;
		}*/

		//创建图像对象并填充数据
		if (color_type==PNG_COLOR_TYPE_RGB_ALPHA)
			image = new CImage(ENUM_COLOR_FORMAT_R8G8B8A8, core::dimension2d<u32>(width, height));
		else
			image = new CImage(ENUM_COLOR_FORMAT_R8G8B8, core::dimension2d<u32>(width, height));
		if (!image)
		{
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("fail to create image from png file: %s",file->getPath().c_str()).c_str());
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			return NULL;
		}

		//图像数据的二维数组
		u8** rowPointers = new png_bytep[height];
		if (!rowPointers)
		{
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("fail to create row pointers for png file: %s",file->getPath().c_str()).c_str());
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			image->drop();
			return NULL;
		}

		//赋值每行指针
		u8* data = (u8*)image->lock();
		for (u32 i=0; i<height; ++i)
		{
			rowPointers[height-i-1]=data;
			//rowPointers[i]=data;
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
		png_destroy_read_struct(&png_ptr,&info_ptr, &end_info); 
#else
		u32 component=0;
		if(color_type == PNG_COLOR_TYPE_GRAY)
		{
			component = 1;
		}
		else if(color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		{
			component = 2;
		}
		else if(color_type == PNG_COLOR_TYPE_RGB)
		{
			component = 3;
		}
		else if(color_type == PNG_COLOR_TYPE_RGB_ALPHA)
		{
			component = 4;
		}

		//转换格式
		if (color_type == PNG_COLOR_TYPE_PALETTE)
		{
			png_set_palette_to_rgb(png_ptr);
			component = 3;
		}
		else if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		{
			png_set_expand_gray_1_2_4_to_8(png_ptr);
		}
		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		{
			png_set_tRNS_to_alpha(png_ptr);
			component = 4;
			//修正从PALETTE+RGBA->RGBA时color_type仍为3的问题
			color_type=PNG_COLOR_TYPE_RGB_ALPHA;
		}
		if (bit_depth == 16)
		{
			png_set_strip_16(png_ptr);
		}
		else if (bit_depth < 8)
		{
			png_set_packing(png_ptr);
		}

		//经过一番转换后再次调用API查询图像信息
		/*{
		png_uint_32 w,h;
		png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type,NULL, NULL, NULL);
		width=w;
		height=h;
		}*/

		//创建图像对象并填充数据
		if (color_type==PNG_COLOR_TYPE_RGB_ALPHA)
			image = new CImage(ENUM_COLOR_FORMAT_R8G8B8A8, core::dimension2d<u32>(width, height));
		else
			image = new CImage(ENUM_COLOR_FORMAT_R8G8B8, core::dimension2d<u32>(width, height));
		if (!image)
		{
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("fail to create image from png file: %s",file->getPath().c_str()).c_str());
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			return NULL;
		}



		//分配内存,需在使用后自行删除
		/*png_uint_32 rowbytes = width * component;
		(*dataBuf) = new png_byte[rowbytes*height];

		//读取数据
		for(png_uint_32 y=0; y < height; y++)
		{
		png_bytep row = (*dataBuf) +  rowbytes * (height - y - 1);
		png_read_rows(png_ptr, &row, (png_bytepp)NULL, 1);
		}

		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);*/
		//图像数据的二维数组
		u8** rowPointers = new png_bytep[height];
		if (!rowPointers)
		{
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("fail to create row pointers for png file: %s",file->getPath().c_str()).c_str());
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			image->drop();
			return NULL;
		}

		//赋值每行指针
		u8* data = (u8*)image->lock();
		//png_uint_32 rowbytes = width * component;
		for (u32 i=0; i<height; ++i)
		{
			rowPointers[height-i-1]=data;
			data += image->getByteCountPerRow();
			//data += rowbytes;
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
		/*unsigned char* mDataBuf;
		png_bytepp dataBuf=&mDataBuf;
		(*dataBuf) = new png_byte[image->getByteCountPerRow()*height];
		for(png_uint_32 y=0; y < height; y++)
		{
		png_bytep row = (*dataBuf) +  image->getByteCountPerRow() * (height - y - 1);
		png_read_rows(png_ptr, &row, (png_bytepp)NULL, 1);
		}*/


		//Read the end of the PNG file.  Will not read past the end of the
		//file, will verify the end is accurate, and will read any comments
		//or time information at the end of the file, if info is not NULL.
		png_read_end(png_ptr, NULL);
		delete [] rowPointers;
		image->unlock();
		// Clean up memory
		png_destroy_read_struct(&png_ptr,&info_ptr, &end_info); 
#endif
		return image;
	}

	IImageLoader* createImageLoaderPNG()
	{
		return new CImageLoaderPng();
	}
}//video
}//yon