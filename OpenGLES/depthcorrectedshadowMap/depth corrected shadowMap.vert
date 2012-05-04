#ifdef BUMP

varying vec3		N, V, L, V_M;
varying vec4		glVertex;
varying vec2		glTexCoord0;

mat3 glNormalMatrixInverse(void)
{
	return mat3(	gl_ModelViewMatrix[0][0], gl_ModelViewMatrix[1][0], gl_ModelViewMatrix[2][0], 
					gl_ModelViewMatrix[0][1], gl_ModelViewMatrix[1][1], gl_ModelViewMatrix[2][1], 
					gl_ModelViewMatrix[0][2], gl_ModelViewMatrix[1][2], gl_ModelViewMatrix[2][2]	);
}

void main(void)
{
	vec4 p = gl_ModelViewMatrix * gl_Vertex;
	
	V = normalize(-p.xyz);
	L = normalize(gl_LightSource[0].position.xyz - p.xyz);
	N = normalize(gl_NormalMatrix * gl_Normal);

	glTexCoord0 = gl_MultiTexCoord0.st;
	glVertex = gl_Vertex;

	V_M = normalize(-glNormalMatrixInverse() * gl_ModelViewMatrix[3].xyz - glVertex.xyz);

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

#else

varying vec3		N, V, L;
varying vec4		q;

uniform mat4		T;

mat3 glNormalMatrixInverse(void)
{
	return mat3(	gl_ModelViewMatrix[0][0], gl_ModelViewMatrix[1][0], gl_ModelViewMatrix[2][0], 
					gl_ModelViewMatrix[0][1], gl_ModelViewMatrix[1][1], gl_ModelViewMatrix[2][1], 
					gl_ModelViewMatrix[0][2], gl_ModelViewMatrix[1][2], gl_ModelViewMatrix[2][2]	);
}

void main(void)
{
	vec4 p = gl_ModelViewMatrix * gl_Vertex;
	
	V = normalize(-p.xyz);
	L = normalize(gl_LightSource[0].position.xyz - p.xyz);
	N = normalize(gl_NormalMatrix * gl_Normal);

	q = T * p;

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

#endif
