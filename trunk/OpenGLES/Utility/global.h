#include "macro.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool UTILITY_API IsExtensionSupported(const char* string,const char* extensions);

#include <crtdbg.h>
#include <typeinfo>
void UTILITY_API EnableMemLeakCheck();


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
int UTILITY_API genCube( float scale, float **vertices, float **normals, float **texCoords, unsigned short **indices );

int UTILITY_API genFish( float scale, float **vertices, float **normals, float **texCoords, unsigned short **indices );

#include <math.h>
//glEnableClientState(GL_VERTEX_ARRAY);
//glVertexPointer(3,GL_FLOAT,0,vertices);
//glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT,indices);
//glDisableClientState(GL_VERTEX_ARRAY);
int UTILITY_API genSphere(float radius,int hSteps,int vSteps,float **vertices, float **normals, float **texCoords, unsigned short **indices);