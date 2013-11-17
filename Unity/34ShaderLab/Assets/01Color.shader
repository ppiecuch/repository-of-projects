//refer to:Unity shader编写教程（一）
//http://game.ceeger.com/forum/read.php?tid=11975&fid=2
//refer to:Unity Shader编写教程（二）
//http://game.ceeger.com/forum/read.php?tid=11982&fid=2
//refer to:Unity Shader编写教程（三）
//http://game.ceeger.com/forum/read.php?tid=11987&fid=2

//Unity的Shader代码可以分为两块：Properties（变量块）、SubShader（代码块） 
//在变量块中定义的变量就相当于Unity脚本中的一个Public变量，可以直接在编辑器中看到。
//而代码块则是设置渲染的参数，直接给显卡用的，而程序肯定不能只支持一张显卡，所以这个subshader不止一个的，

//refer to:http://docs.unity3d.com/Documentation/Manual/Shaders.html
//Shaders in Unity can be written in one of three different ways:

//Surface Shaders
//Surface Shaders are your best option if your shader needs to be affected by lights and shadows. 
//Surface shaders make it easy to write complex shaders in a compact way - it's a higher level of abstraction for interaction with Unity's lighting pipeline. 
//Most surface shaders automatically support both forward and deferred lighting. 
//You write surface shaders in a couple of lines of Cg/HLSL and a lot more code gets auto-generated from that.

//Do not use surface shaders if your shader is not doing anything with lights. 
//For Image Effects or many special-effect shaders, surface shaders are a suboptimal option, since they will do a bunch of lighting calculations for no good reason!

//Vertex and Fragment Shaders
//Vertex and Fragment Shaders will be required, if your shader doesn't need to interact with lighting, 
//or if you need some very exotic effects that the surface shaders can't handle. 
//Shader programs written this way are the most flexible way to create the effect you need (even surface shaders are automatically converted
// to a bunch of vertex and fragment shaders), but that comes at a price: you have to write more code and it's harder to make it interact with lighting. 
//These shaders are written in Cg/HLSL as well.

//Fixed Function Shaders
//Fixed Function Shaders need to be written for old hardware that doesn't support programmable shaders. 
//You will probably want to write fixed function shaders as an n-th fallback to your fancy fragment or surface shaders, 
//to make sure your game still renders something sensible when run on old hardware or simpler mobile platforms. 
//Fixed function shaders are entirely written in a language called ShaderLab, which is similar to Microsoft's .FX files or NVIDIA's CgFX.
	
Shader "Tutorial/01Color" {//shader名字
	
	Properties {//变量块
		_MainColor ("Main_Color(RGBA)", Color) = (1,0.5,0.5,1) //定义一个名字为MainColor的变量,类型为Color(RGBA格式),同时给一个默认值
		// other properties like colors or vectors go here as well
	}
	SubShader {//代码块
		// here goes the 'meat' of your
        //  - surface shader or
        //  - vertex and fragment shader or
        //  - fixed function shader
		pass
		{
			Color[_MainColor]//使用上面定义的变量,将对象颜色设置为变量的值
		}
	} 
	FallBack "Diffuse"//如果当前显卡不支持上面的SubShader,则使用这个
}
