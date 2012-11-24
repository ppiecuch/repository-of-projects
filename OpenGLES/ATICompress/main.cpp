#include "ATI_Compress.h"

/************************************************************************/
/* ATICompress                                                          */
/************************************************************************/
/*
Introduction
ATI_Compress is AMD's library for texture compression. It is used by The Compressonator in its ATI 3Dc compression plug-in. 
It is also available to third party developers who wish to incorporate it within their own applications. 
It can compress to a wide range of compression formats including ATI2N, DXT & swizzled DXT formats. 
It also supports conversion of textures between 8bit fixed, 16bit fixed & 32bit float formats.

Version 1.81 (6/29/2011)
VisualStudio 2008 and 2010 are now the only supported builds.
Fixed issues with size calculation for compressing to R8 and RG8 formats.
Add support for compiling into applications using SAFESEH option.
v1.7
New ATI_TC_Speed compression option with new fast & super-fast compression modes. 
New ATI_TC_FORMAT_RGB_888 texture format. 
Support for ATI_TC & ETC compression formats. 
64-bit libraries. 
Help file documentation. 
New ATI_Compress_Test & ATI_Compress_Benchmark sample applications. 
v1.6
Support for ATI_TC & ETC compression formats. 
v1.5
Support for DirectX10 BCn compression formats. 
Support for single channel (i.e. R) & double channel (i.e. RG) uncompressed texture formats. 
v1.4
Support for ATI_TC_FORMAT_ARGB_32 texture format. 
New bDisableMultiThreading compression option for forcing compression to be single-threaded. 
v1.3
Support for ATI_TC_FORMAT_ARGB_16F texture format. 
Enforced __cdecl calling standard. 
v1.2
New bUseChannelWeighting compression option to control whether channel weightings are taken into account. 
Support for compressing to DXT1 with alpha using the new bDXT1UseAlpha & nAlphaThreshold compression options. 
New ATI_TC_FORMAT_ARGB_2101010 texture format. 
v1.1
Support for ATI1N compression format. 
New ATI_TC_FORMAT_ARGB_16 texture format. 
v1.0
Initial release. 
Everything is new. 
*/

int main(int argc, char* argv[])
{

	//Initialise the source texture. 
	ATI_TC_Texture srcTexture;
	srcTexture.dwSize = sizeof(srcTexture);
	srcTexture.dwWidth = dwWidth;
	srcTexture.dwHeight = dwHeight;
	srcTexture.dwPitch = 0;
	srcTexture.format = ATI_TC_FORMAT_ARGB_8888;

	//Call ATI_TC_CalculateBufferSize to calculate the size of the source texture buffer. 
	srcTexture.dwDataSize = ATI_TC_CalculateBufferSize(&srcTexture);
	srcTexture.pData = pSrc;

	//Initialise the destination texture. 
	ATI_TC_Texture destTexture;
	destTexture.dwSize = sizeof(destTexture);
	destTexture.dwWidth = dwWidth;
	destTexture.dwHeight = dwHeight;
	destTexture.dwPitch = 0;
	destTexture.format = ATI_TC_FORMAT_;

	//Call ATI_TC_CalculateBufferSize to calculate the size of the destination texture buffer. 
	destTexture.dwDataSize = ATI_TC_CalculateBufferSize(&destTexture);
	destTexture.pData = (ATI_TC_BYTE*) malloc(destTexture.dwDataSize);

	//Initialise the compression options. 
	ATI_TC_CompressOptions options;
	memset(&options, 0, sizeof(options));
	options.dwSize = sizeof(options);
	options.nCompressionSpeed = ATI_TC_Speed_SuperFast;

	//Call ATI_TC_ConvertTexture 
	ATI_TC_ConvertTexture(&srcTexture, &destTexture, &options, NULL, NULL, NULL);

	//And copy the results. 
	*pDest = destTexture.pData;


	return 0;
}