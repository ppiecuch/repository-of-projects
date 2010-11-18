#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "ObjLoader.h"

ObjModel* loadModel(char* memory, size_t size)
{
   char *p = NULL, *e = NULL;
   ObjModel* model=new ObjModel();

	p = memory;
	e = memory + size;

	while (p != e)
	{
           if (memcmp(p, "vn", 2) == 0) model->normalCount++;
      else if (memcmp(p, "vt", 2) == 0) model->texCoordCount++;
      else if (memcmp(p, "v",  1) == 0) model->vertexCount++;
      else if (memcmp(p, "f",  1) == 0) model->triangleCount++;

      while (*p++ != (char) 0x0A);
   }

	model->vertexs=new Point3f[model->vertexCount];
	model->normals=new Point3f[model->normalCount];
	model->texCoords=new Point2f[model->texCoordCount];
	model->vertexIndice=new unsigned short[model->triangleCount*3];
	model->normalIndice=new unsigned short[model->triangleCount*3];
	model->texCoordIndice=new unsigned short[model->triangleCount*3];
	p = memory;

   int v = 0, n=0, t=0,  ni = 0, ti = 0, vi = 0;

	while (p != e)
	{
		if(memcmp(p,"mtllib",6)==0)
		{
			model->mtlFilePath=new char[256];
			sscanf(p, "mtllib %s", model->mtlFilePath);
		}
		else if (memcmp(p, "vn", 2) == 0)
		{
		 sscanf(p, "vn %f %f %f", &model->normals[n].x,
								  &model->normals[n].y,
								  &model->normals[n].z);
		 n++;
		}
		else if (memcmp(p, "vt", 2) == 0)
		{
		 sscanf(p, "vt %f %f", &model->texCoords[t].x,
							   &model->texCoords[t].y);
		 t++;
		}
		else if (memcmp(p, "v", 1) == 0) /* or *p == 'v' */
		{
		 sscanf(p, "v %f %f %f", &model->vertexs[v].x,
								 &model->vertexs[v].y,
								 &model->vertexs[v].z);
		 v++;
		}
		else if (memcmp(p, "f", 1) == 0) /* or *p == 'f' */
		{
		 sscanf(p, "f %d/%d/%d %d/%d/%d %d/%d/%d", &model->vertexIndice[vi++],
												   &model->texCoordIndice[ti++],
												   &model->normalIndice[ni++],
												   &model->vertexIndice[vi++],
												   &model->texCoordIndice[ti++],
												   &model->normalIndice[ni++],
												   &model->vertexIndice[vi++],
												   &model->texCoordIndice[ti++],
												   &model->normalIndice[ni++]);
		}

		while (*p++ != (char) 0x0A);
   }

   return model;
}

size_t ObjLoadFile(char* szFileName, char** memory)
{
	size_t bytes = 0;
	FILE* file = fopen(szFileName, "rt");

	if (file != NULL)
   {
	   fseek(file, 0, SEEK_END);
	   size_t end = ftell(file);
	   fseek(file, 0, SEEK_SET);

	   *memory = (char*) malloc(end);
	   bytes = fread(*memory, sizeof(char), end, file);

	   fclose(file);
   }

	return bytes;
}

