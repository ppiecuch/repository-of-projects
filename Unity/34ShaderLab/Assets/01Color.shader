//refer to:Unity shader编写教程（一）
//http://game.ceeger.com/forum/read.php?tid=11975&fid=2

//Unity的Shader代码可以分为两块：Properties（变量块）、SubShader（代码块） 
//在变量块中定义的变量就相当于Unity脚本中的一个Public变量，可以直接在编辑器中看到。
//而代码块则是设置渲染的参数，直接给显卡用的，而程序肯定不能只支持一张显卡，所以这个subshader不止一个的，

//refer to:http://docs.unity3d.com/Documentation/Manual/Shaders.html
//Shaders in Unity can be written in one of three different ways:
	
Shader "Tutorial/01Color" {//shader名字
	
	Properties {//变量块
		_MainColor ("Main_Color(RGBA)", Color) = (1,0.5,0.5,1) //定义一个名字为MainColor的变量,类型为Color(RGBA格式),同时给一个默认值
	}
	SubShader {//代码块
		pass
		{
			Color[_MainColor]//使用上面定义的变量,将对象颜色设置为变量的值
		}
	} 
	FallBack "Diffuse"//如果当前显卡不支持上面的SubShader,则使用这个
}
