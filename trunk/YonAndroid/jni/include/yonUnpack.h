// include this file to switch back to default alignment
// file belongs to irrpack.h, see there for more info

#ifndef YON_PUSHPACK_IS_DEFINED
#	error yonPack.h must be included after yonUnpack.h
#endif

// Default alignment
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
#	pragma pack( pop, packing )
#elif defined (__DMC__)
#	pragma pack( pop )
#elif defined( __GNUC__ )
#   if (__GNUC__ >= 4 ) && (__GNUC_MINOR__ >= 7)
#	    pragma pack( pop, packing )
#   endif
#endif

#undef PACK_STRUCT

#undef YON_PUSHPACK_IS_DEFINED

