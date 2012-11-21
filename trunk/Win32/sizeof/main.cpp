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
	char   a;//Ԫ����1   
	int   b[5];//Ԫ����4   
	double   c;//Ԫ����8   
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

	//�����������ͣ���union��struct��class�Ķ��뷽ʽΪ��Ա�ж��뷽ʽ���ĳ�Ա�Ķ��뷽ʽ��

	printf("%d\r\n",sizeof(UU)); //24
	typedef long DWORD;
	printf("%d\r\n",sizeof(DWORD));
	printf("%d\r\n",sizeof(SSS)); //32
	printf("%d\r\n",sizeof(S5A)); //6��2+1+2=5���Ϊ2����2�ı������룬���Ϊ6������
	printf("%d\r\n",sizeof(S5B)); //8��4+1=5���Ϊ4����4�ı������룬���Ϊ8������
	printf("%d\r\n",sizeof(S16)); //4����u16�����˶��루����
	printf("%d\r\n",sizeof(S64)); //24��8+4+4+4=20���Ϊ8����8�ı������룬���Ϊ24������

	S32 s_32;//24������������Ӧ����16�Ŷԣ����������Ǵ��
	//��ȷ�������ǣ�1(7)81(3)4=24
	//���ϵĽṹ��Ӧ�����㣺
	//S5A:21(1)2=6
	//S5B:41(3)=8
	//S16:21(1)=4
	//S64:8444(4)=24;
	S64 s_64;
	S32A s_32a;
	SS ss;
	//S32A:1(7)811(6)=24
	//SS:82(2)41(7)=24
	//���ϣ������ǽ��м���()���������ֳ�Ϊ����λ��������
	//1��������һ�ṹ�����Ԫ��F��ռ���ֽ���ZF����ʼ���ÿһ��Ԫ��X������Ե�ַ���������Ԫ�ص�ַF��ΪXռ���ֽ���ZX�ı���
	//���F+ZF����ZX�ı���������F+ZF֮�󲹳���NXλʹX����ʼ��ַΪZX�ı���
	//2����1������ֱ�����һ��Ԫ��T��T֮�������ṹ�������ռ���ֽڵ�Ԫ��M��ռ�õ��ֽ�ZM�ٽ��в��䣬ʹ�����ṹ��ռ�õ��ܿռ�ΪZM�ı���

	//���#pragma pack
	//1�����ݳ�Ա������򣺽ṹ(struct)(������(union))�����ݳ�Ա����һ�����ݳ�Ա����offsetΪ0�ĵط���
	//�Ժ�ÿ�����ݳ�Ա�Ķ��밴��#pragma packָ������ֵ��������ݳ�Ա�������бȽ�С���Ǹ����С�
	//2���ṹ(������)�����������������ݳ�Ա��ɸ��Զ���֮�󣬽ṹ(������)����ҲҪ���ж��룬
	//���뽫����#pragma packָ������ֵ�ͽṹ(������)������ݳ�Ա�����У��Ƚ�С���Ǹ����С�
	//3�����1��2�ƶϣ���#pragma pack��nֵ���ڻ򳬹��������ݳ�Ա���ȵ�ʱ�����nֵ�Ĵ�С���������κ�Ч����

	//�������ϡ����ϡ�������Ҫ�ٴε����������ϡ�#pragma packָ������ֵ��������ݳ�Ա�������бȽ�С���Ǹ�����

	//λ��
	//�ο���http://www.cnblogs.com/kevinLee-xjtu/archive/2011/12/12/2299090.html
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