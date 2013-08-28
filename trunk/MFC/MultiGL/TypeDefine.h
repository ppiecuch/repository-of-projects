#ifndef _TypeDefine_H_
#define _TypeDefine_H_
namespace Run
{
typedef struct _Point
{
	float x;
	float y;
	float z;

}mlPoint;
typedef struct _Size 
{
	unsigned int width;
	unsigned int height;
}mlSize;
typedef struct _Color
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
}mlColor;
}
#endif