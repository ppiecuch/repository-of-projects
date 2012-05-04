#ifdef BUMP

uniform sampler2DShadow		depthTexture;
uniform sampler2D			bumpTexture;
uniform float				dt;
uniform mat4				T;

varying vec3		N, V, L, V_M;
varying vec4		glVertex;
varying vec2		glTexCoord0;

void main(void)
{
	float height = dt * texture2D(bumpTexture, glTexCoord0).a;

	vec3 n = texture2D(bumpTexture, glTexCoord0 + height * V_M.xy).xyz;

	vec4 vertex = glVertex + vec4(height * V_M, 0.0);

	vec4 p = gl_ModelViewProjectionMatrix * vertex;
	vec4 q = T * gl_ModelViewMatrix * vertex;

	vec3 R = -normalize(reflect(L, n));
	
	vec4 ambient = gl_FrontLightProduct[0].ambient;
	vec4 diffuse = gl_FrontLightProduct[0].diffuse * max(dot(n, L), 0.0);
	vec4 specular = gl_FrontLightProduct[0].specular * pow(max(dot(R, V), 0.0), gl_FrontMaterial.shininess);
	
	vec3 coord = 0.5 * (q.xyz / q.w + 1.0);

	float shadow = shadow2D(depthTexture, coord).r;

	gl_FragColor = (ambient + (0.5 + 0.5 * shadow) * diffuse) + specular * shadow;
	gl_FragDepth = 0.5 * (p.z / p.w + 1.0);
}

#else

uniform sampler2DShadow		depthTexture;

varying vec3		N, V, L;
varying vec4		q;

void main(void)
{
	vec3 R = -normalize(reflect(L, N));
	
	vec4 ambient = gl_FrontLightProduct[0].ambient;
	vec4 diffuse = gl_FrontLightProduct[0].diffuse * max(dot(N, L), 0.0);
	vec4 specular = gl_FrontLightProduct[0].specular * pow(max(dot(R, V), 0.0), gl_FrontMaterial.shininess);
	
	vec3 coord = 0.5 * (q.xyz / q.w + 1.0);

	float shadow = shadow2D(depthTexture, coord).r;
	gl_FragColor = (ambient + (0.5 + 0.5 * shadow) * diffuse) + specular * shadow;
}

#endif