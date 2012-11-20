#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned __int64 u64;

struct PVRTextureHeader2
{
	u32 dwHeaderSize;                  /*!< size of the structure */
	u32 dwHeight;                      /*!< height of surface to be created */
	u32 dwWidth;                       /*!< width of input surface */
	u32 dwMipMapCount;                 /*!< number of mip-map levels requested */
	u32 dwpfFlags;                     /*!< pixel format flags */
	u32 dwTextureDataSize;             /*!< Total size in bytes */
	u32 dwBitCount;                    /*!< number of bits per pixel  */
	u32 dwRBitMask;                    /*!< mask for red bit */
	u32 dwGBitMask;                    /*!< mask for green bits */
	u32 dwBBitMask;                    /*!< mask for blue bits */
	u32 dwAlphaBitMask;                /*!< mask for alpha channel */
	u32 dwPVR;                         /*!< magic number identifying pvr file */
	u32 dwNumSurfs;                    /*!< the number of surfaces present in the pvr */
};

struct PVRTextureHeader3
{
	u32 Version;			//Version 4 Unsigned 32bit Integer
	u32 Flags;				//Flags 4 Unsigned 32bit Integer
	u64 PixelFormat;		//Pixel Format 8 Unsigned 64bit Integer
	u32 ColourSpace;		//Colour Space 4 Unsigned 32bit Integer
	u32 ChannelType;		//Channel Type 4 Unsigned 32bit Integer
	u32 Height;				//Height 4 Unsigned 32bit Integer
	u32 Width;				//Width 4 Unsigned 32bit Integer
	u32 Depth;				//Depth 4 Unsigned 32bit Integer
	u32 NumSurfaces;		//Num. Surfaces 4 Unsigned 32bit Integer
	u32 NumFaces;			//Num. Faces 4 Unsigned 32bit Integer
	u32 MipmapCount;		//MIP-Map Count 4 Unsigned 32bit Integer
	u32 MetaDataSize;		//Meta Data Size 4 Unsigned 32bit Integer
};

struct S32{
	u32 a;
	u32 b;
	u32 c;
	u32 d;
};

struct S64{
	u64 a;
	u32 b;
	u32 c;
	u32 d;
};

struct S5A{
	u16 a;
	u8 b;
	u16 c;
};
struct S5B{
	u32 a;
	u8 b;
};

struct S16{
	u16 a;
	u8 b;
};

struct PVRTextureHeader
{
	union{
		PVRTextureHeader2 headerV2;
		PVRTextureHeader3 headerV3;
	};
};
int main(int argc, char* argv[])
{
	char buffer[64];
	printf("%d\r\n",sizeof(buffer));
	printf("%d\r\n",strlen(buffer));
	printf("%d\r\n",(int)('\0'));

	printf("-------------------\r\n");

	//复合数据类型，如union，struct，class的对齐方式为成员中对齐方式最大的成员的对齐方式。

	typedef long DWORD;
	printf("%d\r\n",sizeof(DWORD));
	printf("%d\r\n",sizeof(S5A)); //6，2+1+2=5，最长为2，按2的倍数对齐，结果为6
	printf("%d\r\n",sizeof(S5B)); //8，4+1=5，最长为4，按4的倍数对齐，结果为8
	printf("%d\r\n",sizeof(S16)); //4，以u16进行了对齐
	printf("%d\r\n",sizeof(S32));
	printf("%d\r\n",sizeof(S64)); //24，8+4+4+4=20，最长为8，按8的倍数对齐，结果为24
	printf("%d\r\n",sizeof(PVRTextureHeader3));
	printf("%d\r\n",sizeof(PVRTextureHeader));

	system("pause");
	return 0;
}