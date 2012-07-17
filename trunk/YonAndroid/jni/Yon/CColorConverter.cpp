
#include "CColorConverter.h"

namespace yon{
namespace video{

	void CColorConverter::convert_A8B8G8R8toR5G5B5A1(const void* sP, s32 sN, void* dP)
	{
		u32* sB = (u32*)sP;
		u16* dB = (u16*)dP;

		for (s32 x = 0; x < sN; ++x)
			*dB++ = A8B8G8R8toR5G5B5A1(*sB++);
	}

	void CColorConverter::convert_A8B8G8R8toR4G4B4A4(const void* sP, s32 sN, void* dP)
	{
		u32* sB = (u32*)sP;
		u16* dB = (u16*)dP;

		for (s32 x = 0; x < sN; ++x)
			*dB++ = A8B8G8R8toR4G4B4A4(*sB++);
	}

	void CColorConverter::convert_R8G8B8toR5G6B5(const void* sP, s32 sN, void* dP)
	{
		u8 * sB = (u8 *)sP;
		u16* dB = (u16*)dP;

		for (s32 x = 0; x < sN; ++x)
		{
			s32 r = sB[0] >> 3;
			s32 g = sB[1] >> 2;
			s32 b = sB[2] >> 3;

			dB[0] = (r << 11) | (g << 5) | (b);

			sB += 3;
			dB += 1;
		}
	}
}
}