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
	png_infop info_ptr;
	unsigned int sig_read = 0;
	png_uint_32 width, height;
	int bit_depth, color_type;

	//打开文件
	//确认文件是以二进制形式打开的
	if((fp = fopen(file, "rb")) == NULL)
	{
		printf("Fail to open file %s\n",file);
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

	//当libpng解析过程中遇到错误时，我们需要longjmp back to our routine，
	//通过调用setjmp和png_jmpbuf(png_ptr)实现
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
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

	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
	fclose(fp);

	return true;
}