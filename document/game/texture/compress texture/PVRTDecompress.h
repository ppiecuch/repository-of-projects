#ifndef _PVRTDECOMPRESS_H_
#define _PVRTDECOMPRESS_H_

/*!***********************************************************************
 @Function              PVRTDecompressPVRTC
 @Input                 pCompressedData The PVRTC texture data to decompress
 @Input                 Do2bitMode Signifies whether the data is PVRTC2 or PVRTC4
 @Input                 XDim X dimension of the texture
 @Input                 YDim Y dimension of the texture
 @Modified              pResultImage The decompressed texture data
 @Description   Decompresses PVRTC to RGBA 8888
*************************************************************************/
void PVRTDecompressPVRTC(const void *pCompressedData,
                                const int Do2bitMode,
                                const int XDim,
                                const int YDim,
                                unsigned char* pResultImage);

#endif