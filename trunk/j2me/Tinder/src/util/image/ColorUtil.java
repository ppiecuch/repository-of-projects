package util.image;

import javax.microedition.lcdui.Image;

public class ColorUtil {
	
	/**
	 * 将24位色图(R8G8B8)转为16位色图(R5G6B5)
	 */
	public static short[] R8G8B8ToR5G6B5(int[] pixels)
	{
		short[] embTex=new short[pixels.length];
		int a,b,c;
		for(int i=0;i<embTex.length;i++)
		{
			a=(((pixels[i]>>>16&0xFF)*0x1F/0xFF)<<11)&0xF800;
			b=((pixels[i]>>>8&0xFF)*0x3F/0xFF<<5)&0x7E0;
			c=((pixels[i]&0xFF)*0x1F/0xFF)&0x1F;
			embTex[i]=(short) (a|b|c);
		}
		return embTex;
	}
	
	/**
	 * 设置像素数组中颜色值(A8R8G8B8)等于color的alpha值
	 */
	public static void setAlpha(int[] pixels,int color,byte alpha)
	{
		/*
		 * 每一个图像像素通过一个4字节整数来展现。
		 * 最高位字节用作alpha通道；换言之用来实现透明/不透明控制。
		 * 255代表完全不透明；0则代表完全透明。
		 * 接下来一个字节是red红色通道；255代表完全是红色。
		 * 依次类推接下来两个字节相应的实现绿色和蓝色通道。
		 */
		for(int i=0;i<pixels.length;i++)
		{
			if(pixels[i]==color)
			{
				pixels[i]=pixels[i]&0x00FFFFFF|((int)0|alpha)<<24;
			}
		}
	}
	
	
	/**
	 * 设置Image中颜色值(A8R8G8B8)等于color的alpha值
	 * @param image
	 * @param color
	 * @param alpha
	 */
	public static Image setAlpha(Image image,int color,byte alpha)
	{
		int w = image.getWidth();
		int h = image.getHeight();
		int[] newData = new int[w * h];
		image.getRGB(newData, 0, w, 0, 0, w, h);
		setAlpha(newData, color, alpha);
		Image newImage=Image.createRGBImage(newData, w, h, true);
		newData=null;
		System.gc();
		return newImage;
	}

}
