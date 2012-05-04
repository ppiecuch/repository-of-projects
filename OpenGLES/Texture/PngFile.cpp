#include "PngFile.h"

CPngFile::CPngFile(void)
{
}

CPngFile::~CPngFile(void)
{
}
void CPngFile::user_error_fn(png_structp png_ptr, png_const_charp error_message)
{
	printf("libpng error:%s\n",error_message);
}

void CPngFile::user_warning_fn(png_structp png_ptr, png_const_charp warning_message)
{
	printf("libpng warn:%s\n",warning_message);
}
bool CPngFile::load(const char* file,png_bytepp dataBuf,unsigned int *imgWidth, unsigned int *imgHeight, int *components)
{
	FILE *fp;
	//libpng中有两个重要的结构体：png_struct和png_info
	//png_struct是每一个libpng函数调用须用到的参数
	//png_info提供了PNG文件相关的信息，建议使用png_get_*()/png_set_*()
	png_structp png_ptr;
	png_infop info_ptr,end_info;
	unsigned int sig_read = 8;
	png_uint_32 width, height;
	int bit_depth, color_type;
	png_byte header[8];

	//打开文件
	//确认文件是以二进制形式打开的
	if((fp = fopen(file, "rb")) == NULL)
	{
		printf("Fail to open file %s\n",file);
		return false;
	}

	//验证是否PNG文件
	fread(header,1,sig_read,fp);
	if(png_sig_cmp(header,0,sig_read))
	{
		printf("not png %s\n",file);
		return false;
	}

	//创建并初始化png_struct
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL, user_error_fn, user_warning_fn);
	if (png_ptr == NULL)
	{
		printf("Fail to initialize png_struct when read file %s\n",file);
		fclose(fp);
		return false;
	}

	//创建并初始化png_info
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		printf("Fail to initialize png_info when read file %s\n",file);
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		return false;
	}
	end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fp);
		return NULL;
	}

	//当libpng解析过程中遇到错误时，我们需要longjmp back to our routine，
	//通过调用setjmp和png_jmpbuf(png_ptr)实现
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return false;
	}

	//读取文件
	png_init_io(png_ptr, fp);

	//通知libpng我们已经读取了几个字节用于检验文件（最多8个）
	png_set_sig_bytes(png_ptr, sig_read);

	//采用low-level的解析方式
	//png_read_info用于获取图像数据的所有相关信息
	//libpng会处理尽量多的信息，但不会包括图像数据(image data)
	png_read_info(png_ptr, info_ptr);

	//调用API查询图像信息
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,NULL, NULL, NULL);
	(*imgWidth) = width;
	(*imgHeight) = height;
#if 1
	if(color_type == PNG_COLOR_TYPE_GRAY)
	{
		(*components) = 1;
	}
	else if(color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	{
		(*components) = 2;
	}
	else if(color_type == PNG_COLOR_TYPE_RGB)
	{
		(*components) = 3;
	}
	else if(color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	{
		(*components) = 4;
	}

	//转换格式
	if (color_type == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_palette_to_rgb(png_ptr);
		(*components) = 3;
	}
	else if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
	{
		png_set_expand_gray_1_2_4_to_8(png_ptr);
	}
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(png_ptr);
		(*components) = 4;
	}
	if (bit_depth == 16)
	{
		png_set_strip_16(png_ptr);
	}
	else if (bit_depth < 8)
	{
		png_set_packing(png_ptr);
	}


	//分配内存,需在使用后自行删除
	png_uint_32 rowbytes = width * (*components);
	(*dataBuf) = new png_byte[rowbytes*height];

	//读取数据
	for(png_uint_32 y=0; y < height; y++)
	{
		png_bytep row = (*dataBuf) +  rowbytes * (height - y - 1);
		png_read_rows(png_ptr, &row, (png_bytepp)NULL, 1);
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	fclose(fp);
#else

	switch(color_type)
	{
	case PNG_COLOR_TYPE_GRAY:
		png_set_gray_to_rgb(png_ptr);
		(*components) = 3;
		break;
	case PNG_COLOR_TYPE_RGB:
		(*components) = 3;
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		(*components) = 4;
		break;
	case PNG_COLOR_TYPE_PALETTE:
		png_set_palette_to_rgb (png_ptr);
		(*components) = 3;
		break;

	}
	if (png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha (png_ptr);
	}
	if (bit_depth == 16)
	{
		png_set_strip_16 (png_ptr);
	}
	else if (bit_depth < 8)
	{
		png_set_packing (png_ptr);
	}

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// Allocate the image_data as a big block, to be given to opengl
	(*dataBuf) = new png_byte[rowbytes * height];
	if (!dataBuf) {
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return NULL;
	}

	//row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep *row_pointers = new png_bytep[height];
	if (!row_pointers) {
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return NULL;
	}
	// set the individual row_pointers to point at the correct offsets of image_data
	for (int i = 0; i < (unsigned int)height; ++i)
		row_pointers[height - 1 - i] = *dataBuf + i * rowbytes;

	//read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	//clean up memory and close stuff
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete[] row_pointers;
	fclose(fp);
#endif
	return true;
}