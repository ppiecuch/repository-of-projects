#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(show)
//#pragma pack(4)

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
	u8 a;
	u64 b;
	u8 c;
	u32 d;
};

struct S32A{
	u8 a;
	u64 b;
	u8 c;
	u8 d;
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

struct SS{
	u64 a;
	u16 b;
	u32 c;
	u8 d;
};
struct SSS{
	SS s;
	u8 a;
};

union UU{   
	char   a;//元长度1   
	int   b[5];//元长度4   
	double   c;//元长度8   
	int   d[3];   
};

struct PVRTextureHeader
{
	union{
		PVRTextureHeader2 headerV2;
		PVRTextureHeader3 headerV3;
	};
};
typedef unsigned int u32;
int main(int argc, char* argv[])
{
	char buffer[64];
	printf("%d\r\n",sizeof(buffer));
	printf("%d\r\n",strlen(buffer));
	printf("%d\r\n",(int)('\0'));

	printf("-------------------\r\n");

	//复合数据类型，如union，struct，class的对齐方式为成员中对齐方式最大的成员的对齐方式。

	printf("%d\r\n",sizeof(UU)); //24
	typedef long DWORD;
	printf("%d\r\n",sizeof(DWORD));
	printf("%d\r\n",sizeof(SSS)); //32
	printf("%d\r\n",sizeof(S5A)); //6，2+1+2=5，最长为2，按2的倍数对齐，结果为6（有误）
	printf("%d\r\n",sizeof(S5B)); //8，4+1=5，最长为4，按4的倍数对齐，结果为8（有误）
	printf("%d\r\n",sizeof(S16)); //4，以u16进行了对齐（有误）
	printf("%d\r\n",sizeof(S64)); //24，8+4+4+4=20，最长为8，按8的倍数对齐，结果为24（有误）

	S32 s_32;//24按上述的推算应该是16才对，所以上述是错的
	//正确的推算是：1(7)81(3)4=24
	//以上的结构对应的推算：
	//S5A:21(1)2=6
	//S5B:41(3)=8
	//S16:21(1)=4
	//S64:8444(4)=24;
	S64 s_64;
	S32A s_32a;
	SS ss;
	//S32A:1(7)811(6)=24
	//SS:82(2)41(7)=24
	//综上：（我们将中间用()包含的数字称为补充位）（有误）
	//1、对于任一结构体从首元素F（占用字节数ZF）开始后的每一个元素X，其相对地址（相对于首元素地址F）为X占用字节数ZX的倍数
	//如果F+ZF不是ZX的倍数，则在F+ZF之后补充上NX位使X的起始地址为ZX的倍数
	//2、按1规则处理直到最后一个元素T，T之后按整个结构体中最大占用字节的元素M所占用的字节ZM再进行补充，使整个结构体占用的总空间为ZM的倍数

	//结合#pragma pack
	//1、数据成员对齐规则：结构(struct)(或联合(union))的数据成员，第一个数据成员放在offset为0的地方，
	//以后每个数据成员的对齐按照#pragma pack指定的数值和这个数据成员自身长度中比较小的那个进行。
	//2、结构(或联合)的整体对齐规则：在数据成员完成各自对齐之后，结构(或联合)本身也要进行对齐，
	//对齐将按照#pragma pack指定的数值和结构(或联合)最大数据成员长度中，比较小的那个进行。
	//3、结合1、2推断：当#pragma pack的n值等于或超过所有数据成员长度的时候，这个n值的大小将不产生任何效果。

	//所以以上“综上”结论再要再次调整：（加上“#pragma pack指定的数值和这个数据成员自身长度中比较小的那个”）

	//位域
	//参考：http://www.cnblogs.com/kevinLee-xjtu/archive/2011/12/12/2299090.html
	printf("S32:%d,%0X8,%0X8,%u,%u,%u,%u\r\n",sizeof(S32),&s_32,&s_32.a,(u32)(void*)&s_32.a-(u32)(void*)&s_32,(u32)(void*)&s_32.b-(u32)(void*)&s_32,(u32)(void*)&s_32.c-(u32)(void*)&s_32,(u32)(void*)&s_32.d-(u32)(void*)&s_32); 
	printf("S32A:%d,%0X8,%0X8,%u,%u,%u,%u\r\n",sizeof(S32A),&s_32a,&s_32a.a,(u32)(void*)&s_32a.a-(u32)(void*)&s_32a,(u32)(void*)&s_32a.b-(u32)(void*)&s_32a,(u32)(void*)&s_32a.c-(u32)(void*)&s_32a,(u32)(void*)&s_32a.d-(u32)(void*)&s_32a); 
	printf("S64:%d,%u,%u,%u,%u\r\n",sizeof(S64),(u32)(void*)&s_64.a-(u32)(void*)&s_64,(u32)(void*)&s_64.b-(u32)(void*)&s_64,(u32)(void*)&s_64.c-(u32)(void*)&s_64,(u32)(void*)&s_64.d-(u32)(void*)&s_64); 
	printf("SS:%d,%0X8,%0X8,%u,%u,%u,%u\r\n",sizeof(SS),&ss,&ss.a,(u32)(void*)&ss.a-(u32)(void*)&ss,(u32)(void*)&ss.b-(u32)(void*)&ss,(u32)(void*)&ss.c-(u32)(void*)&ss,(u32)(void*)&ss.d-(u32)(void*)&ss); 
	printf("%d\r\n",sizeof(PVRTextureHeader3));
	printf("%d\r\n",sizeof(PVRTextureHeader3));
	printf("%d\r\n",sizeof(PVRTextureHeader));

	system("pause");
	return 0;
}