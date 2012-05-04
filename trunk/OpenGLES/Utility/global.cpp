#include "global.h"
/** 检查是否支持扩展 */
bool IsExtensionSupported(const char* string,const char* extensions)
{				    
	int idx;

	if (extensions == NULL)
		return false;	

	/** 得到最后一个字符 */
	const char* end = extensions + strlen(extensions);	

	/** 循环查找字符串string */
	while (extensions < end)
	{
		/** 找到空格前的一个字符串 */
		idx = strcspn(extensions, " ");

		/** 检查是否找到字符串string */
		if ( (strlen(string) == idx) && (strncmp(string, extensions, idx) == 0))
		{
			return true;
		}

		/** 当前串不是string,继续下一个字符串 */
		extensions += (idx + 1);
	}
	/** 没有找到,则返回false */
	return false;   
}

void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

//
/// \brief Generates geometry for a cube.  Allocates memory for the vertex data and stores 
///        the results in the arrays.  Generate index list for a TRIANGLES
/// \param scale The size of the cube, use 1.0 for a unit cube.
/// \param vertices If not NULL, will contain array of float3 positions
/// \param normals If not NULL, will contain array of float3 normals
/// \param texCoords If not NULL, will contain array of float2 texCoords
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         if it is not NULL ) as a GL_TRIANGLE_STRIP
//
int genCube( float scale, float **vertices, float **normals, float **texCoords, unsigned short **indices )
{
	int i;
	int numVertices = 24;
	int numIndices = 36;

	float cubeVerts[] =
	{
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f,  0.5f, 0.5f,
		0.5f,  0.5f, 0.5f, 
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
	};

	float cubeNormals[] =
	{
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};

	float cubeTex[] =
	{
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	};

	// Allocate memory for buffers
	if ( vertices != NULL )
	{
		*vertices = (float*)malloc(sizeof(float)*3*numVertices );
		memcpy( *vertices, cubeVerts, sizeof( cubeVerts ) );
		for ( i = 0; i < numVertices * 3; i++ )
		{
			(*vertices)[i] *= scale;
		}
	}

	if ( normals != NULL )
	{
		*normals = (float*)malloc ( sizeof(float) * 3 * numVertices );
		memcpy( *normals, cubeNormals, sizeof( cubeNormals ) );
	}

	if ( texCoords != NULL )
	{
		*texCoords = (float*)malloc ( sizeof(float) * 2 * numVertices );
		memcpy( *texCoords, cubeTex, sizeof( cubeTex ) ) ;
	}


	// Generate the indices
	if ( indices != NULL )
	{
		unsigned short cubeIndices[] =
		{
			0, 2, 1,
			0, 3, 2, 
			4, 5, 6,
			4, 6, 7,
			8, 9, 10,
			8, 10, 11, 
			12, 15, 14,
			12, 14, 13, 
			16, 17, 18,
			16, 18, 19, 
			20, 23, 22,
			20, 22, 21
		};

		*indices = (unsigned short*)malloc ( sizeof(unsigned short) * numIndices );
		memcpy( *indices, cubeIndices, sizeof( cubeIndices ) );
	}

	return numIndices;
}

#include "fish.cpp"
int genFish( float scale, float **vertices, float **normals, float **texCoords, unsigned short **indices )
{
	int i;
	int numVertices = NUM_FISH_OBJECT_VERTEX;
	int numIndices = NUM_FISH_OBJECT_INDEX;

	// Allocate memory for buffers
	if ( vertices != NULL )
	{
		*vertices = (float*)malloc(sizeof(float)*3*numVertices );
		memcpy( *vertices, fishVertices, sizeof( fishVertices ) );
		for ( i = 0; i < numVertices * 3; i++ )
		{
			(*vertices)[i] *= scale;
		}
	}

	if ( normals != NULL )
	{
		*normals = (float*)malloc ( sizeof(float) * 3 * numVertices );
		memcpy( *normals, fishNormals, sizeof( fishNormals ) );
	}

	if ( texCoords != NULL )
	{
		*texCoords = (float*)malloc ( sizeof(float) * 2 * numVertices );
		memcpy( *texCoords, fishTexCoords, sizeof( fishTexCoords ) ) ;
	}


	// Generate the indices
	if ( indices != NULL )
	{
		*indices = (unsigned short*)malloc ( sizeof(unsigned short) * numIndices );
		memcpy( *indices, fishIndices, sizeof( fishIndices ) );
	}

	return numIndices;

}

int genSphere(float radius,int hSteps,int vSteps,float **vertices, float **normals, float **texCoords, unsigned short **indices)
{
#ifndef M_PI
	#define M_PI 3.1415926535897932384626433832795f
#endif

	static const float DTOR=M_PI/180;
	static const float M_2PI=M_PI+M_PI;

	float dtheta=(float)360/hSteps;	//水平方向步增
	float dphi=(float)180/vSteps;	//垂直方向步增

	printf("dtheta=360/%d=%.2f,dphi=180/%d=%.2f\n",hSteps,dtheta,vSteps,dphi);

	int numVertices=vSteps*hSteps<<2;
	int numIndices=vSteps*hSteps*6;
	*vertices = (float*)malloc(sizeof(float)*3*numVertices );
	if(texCoords!=NULL)
		*texCoords=(float*)malloc(sizeof(float)*2*numVertices );
	if(normals!=NULL)
		*normals=(float*)malloc(sizeof(float)*3*numVertices );
	if(indices!=NULL)
		*indices=(unsigned short*)malloc(sizeof(unsigned short)*numIndices);


	float temp[3];

#define NORMALIZE_VERTEX_TO(S0,S1,S2,D) \
	{ \
		float l=S0*S0+S1*S1+S2*S2; \
		l=sqrt(l); \
		D[0]=S0/l; \
		D[1]=S1/l; \
		D[2]=S2/l; \
	}
	int index=0;
	int index2=0;
	float phi=0;
	float theta=0;
	for(int i=0;i<vSteps;i++,phi+=dphi)
	{
		for(int j=0;j<hSteps;j++,theta+=dtheta)
		{
			(*vertices)[index*3+2] = (float) (radius * sinf(phi*DTOR) * cosf(DTOR*theta));
			(*vertices)[index*3+0] = (float) (radius * sinf(phi*DTOR) * sinf(DTOR*theta));
			(*vertices)[index*3+1] = (float) (radius * cosf(phi*DTOR));
			NORMALIZE_VERTEX_TO((*vertices)[index*3+0],(*vertices)[index*3+1],(*vertices)[index*3+2],temp);
			if(texCoords!=NULL)
			{
				(*texCoords)[index*2+0]=(float)j/hSteps;
				(*texCoords)[index*2+1]=(float)asinf(temp[1])/M_PI+0.5f;
			}
			if(normals!=NULL)
			{
				(*normals)[index*3+0]=temp[0];
				(*normals)[index*3+1]=temp[1];
				(*normals)[index*3+2]=temp[2];
			}
			index++;
			(*vertices)[index*3+2] = (float) (radius * sinf((phi+dphi)*DTOR) * cosf(DTOR*theta));
			(*vertices)[index*3+0] = (float) (radius * sinf((phi+dphi)*DTOR) * sinf(DTOR*theta));
			(*vertices)[index*3+1] = (float) (radius * cosf((phi+dphi)*DTOR));
			NORMALIZE_VERTEX_TO((*vertices)[index*3+0],(*vertices)[index*3+1],(*vertices)[index*3+2],temp);
			if(texCoords!=NULL)
			{
				(*texCoords)[index*2+0]=(float)j/hSteps;
				(*texCoords)[index*2+1]=(float)asinf(temp[1])/M_PI+0.5f;
			}
			if(normals!=NULL)
			{
				(*normals)[index*3+0]=temp[0];
				(*normals)[index*3+1]=temp[1];
				(*normals)[index*3+2]=temp[2];
			}
			index++;
			(*vertices)[index*3+2]  = (float) (radius * sinf((phi+dphi)*DTOR) * cosf(DTOR*(theta+dtheta)));
			(*vertices)[index*3+0]  = (float) (radius * sinf((phi+dphi)*DTOR) * sinf(DTOR*(theta+dtheta)));
			(*vertices)[index*3+1]  = (float) (radius * cosf((phi+dphi)*DTOR));
			NORMALIZE_VERTEX_TO((*vertices)[index*3+0],(*vertices)[index*3+1],(*vertices)[index*3+2],temp);
			if(texCoords!=NULL)
			{
				(*texCoords)[index*2+0]=(float)(j+1)/hSteps;
				(*texCoords)[index*2+1]=(float)asinf(temp[1])/M_PI+0.5f;
			}
			if(normals!=NULL)
			{
				(*normals)[index*3+0]=temp[0];
				(*normals)[index*3+1]=temp[1];
				(*normals)[index*3+2]=temp[2];
			}
			index++;
			(*vertices)[index*3+2]  = (float) (radius * sinf(phi*DTOR) * cosf(DTOR*(theta+dtheta)));
			(*vertices)[index*3+0]  = (float) (radius * sinf(phi*DTOR) * sinf(DTOR*(theta+dtheta)));
			(*vertices)[index*3+1]  = (float) (radius * cosf(phi*DTOR));
			NORMALIZE_VERTEX_TO((*vertices)[index*3+0],(*vertices)[index*3+1],(*vertices)[index*3+2],temp);
			if(texCoords!=NULL)
			{
				(*texCoords)[index*2+0]=(float)(j+1)/hSteps;
				(*texCoords)[index*2+1]=(float)asinf(temp[1])/M_PI+0.5f;
			}
			if(normals!=NULL)
			{
				(*normals)[index*3+0]=temp[0];
				(*normals)[index*3+1]=temp[1];
				(*normals)[index*3+2]=temp[2];
			}
			index++;

			if(indices!=NULL)
			{
				(*indices)[index2++]=index-4;
				(*indices)[index2++]=index-3;
				(*indices)[index2++]=index-2;
				(*indices)[index2++]=index-4;
				(*indices)[index2++]=index-2;
				(*indices)[index2++]=index-1;
			}
		}
	}
#undef NORMALIZE_VERTEX_TO
	return numIndices;
}