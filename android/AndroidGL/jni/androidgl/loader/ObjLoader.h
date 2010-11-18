#ifndef _OBJ_LOADER_H_
#define _OBJ_LOADER_H_
#include "../math/point3f.h"
#include "../math/point2f.h"

/**
 * OBJÄ£ÐÍ¼ÓÔØÆ÷
 */
struct ObjModel
{
   int vertexCount, normalCount, texCoordCount, triangleCount;

   Point3f* vertexs;
   Point3f* normals;
   Point2f* texCoords;
   unsigned short* vertexIndice;
   unsigned short* normalIndice;
   unsigned short* texCoordIndice;
};

ObjModel* loadModel(char*, size_t);
size_t    ObjLoadFile(char*, char**);

#endif
