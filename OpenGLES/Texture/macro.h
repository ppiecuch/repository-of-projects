//#define GLES2
#define GL

#ifdef TEXTURE_EXPORTS
#define TEXTURE_API __declspec(dllexport)
#else
#define TEXTURE_API __declspec(dllimport)
#endif