package org.tinder.studio.warbird;



import android.graphics.Bitmap;

public class Util {
	
	public static Bitmap alphaColor(Bitmap image,int color,byte alpha)
	{
		int w = image.getWidth();
		int h = image.getHeight();
		int[] pixels = new int[w*h];
		
		image.getPixels(pixels, 0, w, 0, 0, w, h);
		System.out.println(Integer.toHexString(pixels[0]));
		setAlpha(pixels, color, alpha);
		System.out.println(Integer.toHexString(pixels[0]));
		return Bitmap.createBitmap(pixels,w,h,image.getConfig());
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
	
	public static Bitmap generateSub(Bitmap bitmap,int[] bound){
		return Bitmap.createBitmap(bitmap,bound[0],bound[1],bound[2],bound[3]);
	}

}
