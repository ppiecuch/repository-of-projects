//refer to:ShaderLab syntax: Properties
//http://docs.unity3d.com/Documentation/Components/SL-Properties.html

//Shaders can define a list of parameters to be set by artists in Unity's material inspector. The Properties block in the shader file defines them.

//Syntax
//Properties { Property [Property ...] }
//Defines the property block. Inside braces multiple properties are defined as follows.
//name ("display name", Range (min, max)) = number
//Defines a float property, represented as a slider from min to max in the inspector.
//name ("display name", Color) = (number,number,number,number)
//Defines a color property.
//name ("display name", 2D) = "name" { options }
//Defines a 2D texture property.
//name ("display name", Rect) = "name" { options }
//Defines a rectangle (non power of 2) texture property.
//name ("display name", Cube) = "name" { options }
//Defines a cubemap texture property.
//name ("display name", Float) = number
//Defines a float property.
//name ("display name", Vector) = (number,number,number,number)
//Defines a four component vector property.

//Each property inside the shader is referenced by name (in Unity, it's common to start shader property names with underscore). 
//The property will show up in material inspector as display name. For each property a default value is given after equals sign:

//For Range and Float properties it's just a single number.
//For Color and Vector properties it's four numbers in parentheses.
//For texture (2D, Rect, Cube) the default value is either an empty string, or one of built-in default textures: "white", "black", "gray" or "bump".
//Later on in the shader, property values are accessed using property name in square brackets: [name].

//Example
//Properties {
//    // properties for water shader
//    _WaveScale ("Wave scale", Range (0.02,0.15)) = 0.07 // sliders
//    _ReflDistort ("Reflection distort", Range (0,1.5)) = 0.5
//    _RefrDistort ("Refraction distort", Range (0,1.5)) = 0.4
//    _RefrColor ("Refraction color", Color)  = (.34, .85, .92, 1) // color
//    _ReflectionTex ("Environment Reflection", 2D) = "" {} // textures
//    _RefractionTex ("Environment Refraction", 2D) = "" {}
//    _Fresnel ("Fresnel (A) ", 2D) = "" {}
//    _BumpMap ("Bumpmap (RGB) ", 2D) = "" {}
//}

//Texture property options
//The options inside curly braces of the texture property are optional. The available options are:

//TexGen texgenmode: Automatic texture coordinate generation mode for this texture. 
//Can be one of ObjectLinear, EyeLinear, SphereMap, CubeReflect, CubeNormal; these correspond directly to OpenGL texgen modes. 
//Note that TexGen is ignored if custom vertex programs are used.

//不懂
//LightmapMode If given, this texture will be affected by per-renderer lightmap parameters. 
//That is, the texture to use can be not in the material, but taken from the settings of the Renderer instead, see Renderer scripting documentation. 

//refer to: http://docs.unity3d.com/Documentation/Components/SL-SurfaceShaderExamples.html
Shader "Tutorial/03Properties" {
	Properties {
		_MainTex ("Texture", 2D) = "white" {}
	}
	SubShader {
		Tags { "RenderType" = "Opaque" }
		CGPROGRAM
		#pragma surface surf Lambert
		struct Input {
			float2 uv_MainTex;
		};
		sampler2D _MainTex;
		void surf (Input IN, inout SurfaceOutput o) {
			o.Albedo = tex2D (_MainTex, IN.uv_MainTex).rgb;
		}
		ENDCG
	} 
	FallBack "Diffuse"
}