#include <stdio.h>
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

	printf("%d\r\n",sizeof(S16)); //4����u16�����˶���
	printf("%d\r\n",sizeof(S32));
	printf("%d\r\n",sizeof(S64)); //24����u64�����˶��� 
	printf("%d\r\n",sizeof(PVRTextureHeader3));
	printf("%d\r\n",sizeof(PVRTextureHeader));

	getchar();
	return 0;
}