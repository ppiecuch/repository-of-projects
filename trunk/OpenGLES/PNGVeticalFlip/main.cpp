#include "lpng1510/png.h"

#include <iostream>
using namespace std;

typedef unsigned char u8;
typedef unsigned int u32;
typedef int s32;

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}


#include <stdio.h>
#include <wtypes.h>
#include <stdarg.h>
#include <tchar.h>

inline void TRACE(const char * pszFormat, ...)
{
	va_list pArgs;
	char szMessageBuffer[16380]={0};
	va_start( pArgs, pszFormat );
	vsnprintf_s( szMessageBuffer, 16380,16380-1,pszFormat, pArgs );
	va_end( pArgs );   
	OutputDebugStringA(szMessageBuffer);   
}

static void png_cpexcept_error(png_structp png_ptr, png_const_charp msg)
{
	printf("libpng occur error:%s",msg);
}

static void png_cpexcept_warn(png_structp png_ptr, png_const_charp msg)
{
	printf("libpng occur warn:%s",msg);
}

/*static void PNGAPI user_write_data_fcn(png_structp png_ptr, png_bytep data, png_size_t length)
{
	png_size_t check;

	//io::IWriteFile* file=(io::IWriteFile*)png_ptr->io_ptr;
	//check=(png_size_t) file->write((const void*)data,(u32)length);
	fwrite(data,1,length,png_ptr->io_ptr);

	if (check != length)
		png_error(png_ptr, "Write Error");
}
*/
int convert(const char *inname,const char *outname)
{
#if 0
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	FILE *fp;

	if ((fp = fopen(file_name, "rb")) == NULL)
		return (ERROR);

	png_structp png_ptr;
	png_infop info_ptr,end_info;
	unsigned int sig_read = 8;
	u32 width, height;
	s32 bit_depth, color_type;
	png_byte header[8];

	//��֤�Ƿ�PNG�ļ�
	if (fread(header, 1, sig_read, fp) != sig_read)
		return false;
	if(png_sig_cmp(header,0,sig_read))
	{
		fclose(fp);
		printf("%s check header failed!",file_name);
		return false;
	}

	//��������ʼ��png_struct
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL, png_cpexcept_error, png_cpexcept_warn);
	if (png_ptr == NULL)
	{
		fclose(fp);
		printf("%s fail to initialize png_struct!",file_name);
		return false;
	}

	//��������ʼ��png_info
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		printf("%s fail to initialize start png_info!",file_name);
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		return false;
	}
	end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		printf("%s fail to initialize end png_info!",file_name);
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fp);
		return false;
	}

	//��libpng������������������ʱ��������Ҫlongjmp back to our routine��
	//ͨ������setjmp��png_jmpbuf(png_ptr)ʵ��
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		printf("fail to parse png file: %s",file_name);
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return false;
	}

	//��ȡ�ļ�
	png_init_io(png_ptr, fp);

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

	/*
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

	png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);

	//�Ҷ�ɫΪRGBɫ
	if (color_type==PNG_COLOR_TYPE_GRAY || color_type==PNG_COLOR_TYPE_GRAY_ALPHA)
	{
		png_set_gray_to_rgb(png_ptr);
	}

	png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);

	*/

	u8* imageData=NULL;
	u32 byteCountPerRow=0;

	//����ͼ������������
	if (color_type==PNG_COLOR_TYPE_RGB_ALPHA)
		byteCountPerRow=width*4;
	else
		byteCountPerRow=width*3;

	imageData=new u8[byteCountPerRow*height];


	//ͼ�����ݵĶ�ά����
	u8** rowPointers = new png_bytep[height];
	if (!rowPointers)
	{
		printf("fail to create row pointers for png file: %s",file_name);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(fp);
		return false;
	}

	//��ֵÿ��ָ��
	u8* data = (u8*)imageData
	for (u32 i=0; i<height; ++i)
	{
		rowPointers[height-i-1]=data;
		//rowPointers[i]=data;
		data += byteCountPerRow
	}

	// for proper error handling
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		delete [] rowPointers;
		fclose(fp);
		return false;
	}

	//Read data using the library function that handles all transformations including interlacing
	png_read_image(png_ptr, rowPointers);


	//Read the end of the PNG file.  Will not read past the end of the
	//file, will verify the end is accurate, and will read any comments
	//or time information at the end of the file, if info is not NULL.
	png_read_end(png_ptr, NULL);
	delete [] rowPointers;
	// Clean up memory
	png_destroy_read_struct(&png_ptr,&info_ptr, &end_info); 
	fclose(fp);


	// Allocate the png write struct
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL, png_cpexcept_error, png_cpexcept_warn);
	if (!png_ptr)
	{
		printf("Internal PNG create write struct failure:%s\r\n",file_name);
		return false;
	}

	// Allocate the png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		printf("PNG create info struct failure:%s\r\n",file_name);
		return false;
	}

	// for proper error handling
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return false;
	}

	png_set_write_fn(png_ptr, fp, user_write_data_fcn, NULL);

	png_set_IHDR(png_ptr, info_ptr,width,height,bit_depth,color_type,PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	png_set_rows(png_ptr, info_ptr, rowPointers);

	if (image->getColorFormat()==ECF_A8R8G8B8 || image->getColorFormat()==ECF_A1R5G5B5)
		png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_BGR, NULL);
	else
	{
		png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
	}

	png_destroy_write_struct(&png_ptr, &info_ptr);
#endif

	static png_FILE_p fpin;
	static png_FILE_p fpout;
	//�����ļ���
	//char *inname = "/home/mingming/graph/1.png";
	//char *outname = "/home/mingming/graph/1_like.png";
	//����
	png_structp read_ptr;
	png_infop read_info_ptr, end_info_ptr;
	//д
	png_structp write_ptr;
	png_infop write_info_ptr,write_end_info_ptr;
	//
	png_bytep row_buf;
	png_uint_32 y;
	int num_pass, pass;
	png_uint_32 width, height;//��ȣ��߶�
	int bit_depth, color_type;//λ���ɫ����
	int interlace_type, compression_type, filter_type;//ɨ�跽ʽ��ѹ����ʽ���˲���ʽ
	//��
	row_buf = NULL;
	//�򿪶��ļ�
	if ((fpin = fopen(inname, "rb")) == NULL)
	{
		fprintf(stderr,"Could not find input file %s\n", inname);
		return (1);
	}
	//��д�ļ�
	if ((fpout = fopen(outname, "wb")) == NULL)
	{
		printf("Could not open output file %s\n", outname);
		fclose(fpin);
		return (1);
	}
	//�������ﲻ����δ֪�Ŀ�unknown chunk
	//��ʼ��1
	read_ptr =
		png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	write_ptr =
		png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	read_info_ptr = png_create_info_struct(read_ptr);
	end_info_ptr = png_create_info_struct(read_ptr);
	write_info_ptr = png_create_info_struct(write_ptr);
	write_end_info_ptr = png_create_info_struct(write_ptr);
	//��ʼ��2
	png_init_io(read_ptr, fpin);
	png_init_io(write_ptr, fpout);
	//���ļ���high level(�߲㣩��low level���֣�����ѡ��ӵײ������Ϣ�ж�ȡ��
	//�������Ƕ�ȡ���п�ѡ��
	png_read_info(read_ptr, read_info_ptr);    
	//��1��IHDR
	//��ȡͼ����(width)���߶�(height)��λ��(bit_depth)����ɫ����(color_type)��ѹ������(compression_type)
	//�˲�������(filter_type),����ɨ�跽ʽ(interlace_type)
	if (png_get_IHDR(read_ptr, read_info_ptr, &width, &height, &bit_depth,
		&color_type, &interlace_type, &compression_type, &filter_type))
	{
		//���ǲ���Ĭ��ɨ�跽ʽ
		png_set_IHDR(write_ptr, write_info_ptr, width, height, bit_depth,
			color_type, PNG_INTERLACE_NONE, compression_type, filter_type);
	}
	//��2��cHRM
	//��ȡ��ɫ����Ϣ  ��/��/��/�� ���x,y���꣬����������Σ������ø�����
	png_fixed_point white_x, white_y, red_x, red_y, green_x, green_y, blue_x,blue_y;

	if (png_get_cHRM_fixed(read_ptr, read_info_ptr, &white_x, &white_y,
		&red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y))
	{
		png_set_cHRM_fixed(write_ptr, write_info_ptr, white_x, white_y, red_x,
			red_y, green_x, green_y, blue_x, blue_y);
	}
	//��3��gAMA
	png_fixed_point gamma;

	if (png_get_gAMA_fixed(read_ptr, read_info_ptr, &gamma))
		png_set_gAMA_fixed(write_ptr, write_info_ptr, gamma);
	//��4��iCCP
	png_charp name;
	png_bytep profile;
	png_uint_32 proflen;

	if (png_get_iCCP(read_ptr, read_info_ptr, &name, &compression_type,
		&profile, &proflen))
	{
		png_set_iCCP(write_ptr, write_info_ptr, name, compression_type,
			profile, proflen);
	}
	//(5)sRGB
	int intent;
	if (png_get_sRGB(read_ptr, read_info_ptr, &intent))
		png_set_sRGB(write_ptr, write_info_ptr, intent);
	//(7)PLTE
	png_colorp palette;
	int num_palette;

	if (png_get_PLTE(read_ptr, read_info_ptr, &palette, &num_palette))
		png_set_PLTE(write_ptr, write_info_ptr, palette, num_palette);
	//(8)bKGD
	png_color_16p background;

	if (png_get_bKGD(read_ptr, read_info_ptr, &background))
	{
		png_set_bKGD(write_ptr, write_info_ptr, background);
	}
	//(9)hist

	png_uint_16p hist;

	if (png_get_hIST(read_ptr, read_info_ptr, &hist))
		png_set_hIST(write_ptr, write_info_ptr, hist);
	//(10)oFFs
	png_int_32 offset_x, offset_y;
	int unit_type;

	if (png_get_oFFs(read_ptr, read_info_ptr, &offset_x, &offset_y,
		&unit_type))
	{
		png_set_oFFs(write_ptr, write_info_ptr, offset_x, offset_y, unit_type);
	}
	//(11)pCAL
	png_charp purpose, units;
	png_charpp params;
	png_int_32 X0, X1;
	int type, nparams;

	if (png_get_pCAL(read_ptr, read_info_ptr, &purpose, &X0, &X1, &type,
		&nparams, &units, &params))
	{
		png_set_pCAL(write_ptr, write_info_ptr, purpose, X0, X1, type,
			nparams, units, params);
	}
	//(12)pHYs

	png_uint_32 res_x, res_y;

	if (png_get_pHYs(read_ptr, read_info_ptr, &res_x, &res_y, &unit_type))
		png_set_pHYs(write_ptr, write_info_ptr, res_x, res_y, unit_type);
	//(13)sBIT
	png_color_8p sig_bit;

	if (png_get_sBIT(read_ptr, read_info_ptr, &sig_bit))
		png_set_sBIT(write_ptr, write_info_ptr, sig_bit);
	//��14��sCAL
	int unit;
	png_charp scal_width, scal_height;

	if (png_get_sCAL_s(read_ptr, read_info_ptr, &unit, &scal_width,
		&scal_height))
	{
		png_set_sCAL_s(write_ptr, write_info_ptr, unit, scal_width,
			scal_height);
	}
	//(15)iTXt
	png_textp text_ptr;
	int num_text;

	if (png_get_text(read_ptr, read_info_ptr, &text_ptr, &num_text) > 0)
	{
		png_set_text(write_ptr, write_info_ptr, text_ptr, num_text);
	}
	//(16)tIME,�������ǲ�֧��RFC1123
	png_timep mod_time;

	if (png_get_tIME(read_ptr, read_info_ptr, &mod_time))
	{
		png_set_tIME(write_ptr, write_info_ptr, mod_time);
	}
	//(17)tRNS
	png_bytep trans_alpha;
	int num_trans;
	png_color_16p trans_color;

	if (png_get_tRNS(read_ptr, read_info_ptr, &trans_alpha, &num_trans,
		&trans_color))
	{
		int sample_max = (1 << bit_depth);
		/* libpng doesn't reject a tRNS chunk with out-of-range samples */
		if (!((color_type == PNG_COLOR_TYPE_GRAY &&
			(int)trans_color->gray > sample_max) ||
			(color_type == PNG_COLOR_TYPE_RGB &&
			((int)trans_color->red > sample_max ||
			(int)trans_color->green > sample_max ||
			(int)trans_color->blue > sample_max))))
			png_set_tRNS(write_ptr, write_info_ptr, trans_alpha, num_trans,
			trans_color);
	}

	//д���µ�png�ļ���
	png_write_info(write_ptr, write_info_ptr);
	//��������ͼ������
	/*num_pass = 1;
	for (pass = 0; pass < num_pass; pass++)
	{
		for (y = 0; y < height; y++)
		{
			//�����ڴ�
			row_buf = (png_bytep)png_malloc(read_ptr,png_get_rowbytes(read_ptr, read_info_ptr));
			TRACE("%d\r\n",png_get_rowbytes(read_ptr, read_info_ptr));
			png_read_rows(read_ptr, (png_bytepp)&row_buf, NULL, 1);
			png_write_rows(write_ptr, (png_bytepp)&row_buf, 1);
			png_free(read_ptr, row_buf);
			row_buf = NULL;
		}
	}
	png_read_end(read_ptr, end_info_ptr);
	*/
	png_uint_32 row;
	num_pass = png_set_interlace_handling(read_ptr);
	png_read_update_info(read_ptr, read_info_ptr);
	u8** row_pointers = new png_bytep[height];
	u8** flip_pointers = new png_bytep[height];
	for (row = 0; row < height; row++)
		row_pointers[row] = NULL;

	for (row = 0; row < height; row++)
		row_pointers[row]=flip_pointers[height-row-1] = new u8[png_get_rowbytes(read_ptr,read_info_ptr)];
	png_read_image(read_ptr, row_pointers);
	png_read_end(read_ptr, end_info_ptr);

	png_write_image(write_ptr,flip_pointers);


	for (row = 0; row < height; row++)
		delete row_pointers[row];
	delete[] row_pointers;
	delete[] flip_pointers;

	//
	//tTXt
	if (png_get_text(read_ptr, end_info_ptr, &text_ptr, &num_text) > 0)
	{
		png_set_text(write_ptr, write_end_info_ptr, text_ptr, num_text);
	}
	//tIME
	if (png_get_tIME(read_ptr, end_info_ptr, &mod_time))
	{
		png_set_tIME(write_ptr, write_end_info_ptr, mod_time);
	}
	//
	png_write_end(write_ptr, write_end_info_ptr);
	//����
	png_free(read_ptr, row_buf);
	row_buf = NULL;
	png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
	png_destroy_info_struct(write_ptr, &write_end_info_ptr);
	png_destroy_write_struct(&write_ptr, &write_info_ptr);
	//
	fclose(fpin);
	fclose(fpout);
	/*
	//���ԣ��Ƚ�����PNG�ļ��Ƿ���ͬ
	if ((fpin = fopen(inname, "rb")) == NULL)
	{
		printf("Could not find file %s\n", inname);
		return (1);
	}

	if ((fpout = fopen(outname, "rb")) == NULL)
	{
		printf("Could not find file %s\n", outname);
		fclose(fpin);
		return (1);
	}
	char inbuf[256], outbuf[256];
	for (;;)
	{
		png_size_t num_in, num_out;

		num_in = fread(inbuf, 1, 1, fpin);
		num_out = fread(outbuf, 1, 1, fpout);

		if (num_in != num_out)
		{
			printf("\nFiles %s and %s ��С��ͬ\n",
				inname, outname);

			fclose(fpin);
			fclose(fpout);
			return (0);
		}

		if (!num_in)
			break;

		if (memcmp(inbuf, outbuf, num_in))
		{
			printf("\nFiles %s and %s ���ݲ�ͬ\n", inname, outname);
			fclose(fpin);
			fclose(fpout);
			return (0);
		}
	}

	fclose(fpin);
	fclose(fpout);*/

	return (0);

}


int main(int argc, char* argv[])
{
	EnableMemLeakCheck();
	if(argc<5)
	{
		cout<<"No param! Please execute command as:"<<endl;
		cout<<"PNGVeticalFlip -i [inputfile] -o [outputfile]"<<endl;
		return 0;
	}
	if(strcmp(argv[1],"-i")!=0)
	{
		cout<<"No input!"<<endl;
		return 0;
	}
	if(strcmp(argv[3],"-o")!=0)
	{
		cout<<"No output:"<<argv[3]<<"!"<<endl;
		return 0;
	}
	const char* input=argv[2];
	const char* output=argv[4];
	FILE* file;
	errno_t ir=fopen_s(&file,input,"rb+");
	if(ir!=0)
	{
		cout<<"No input file,error:"<<ir<<"!"<<endl;
		return 0;
	}
	fclose(file);
	errno_t or=fopen_s(&file,output,"rb+");
	if(or==0)
	{
		cout<<"Already exist output file:"<<output<<"!"<<endl;
		fclose(file);
		return 0;
	}

	if(convert(input,output))
		cout<<"Convert "<<input<<"failed!"<<endl;

	//system("pause");

	return 0;
}