package org.tinder.studio.warbird;



import java.util.Date;
import java.util.Random;

import android.graphics.Bitmap;
import android.graphics.Point;

public class Util {
	
	private static Random random=new Random(new Date().getTime());
	
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
	
	/**
	 * 数量积/内积
	 * 内积大于0时，两向量夹角小于90度；等于0时，夹角等于90度；小于零时，夹角大于90度且小于180度。
	 * @param x1
	 * @param y1
	 * @param x2
	 * @param y2
	 * @return
	 */
	public static int dot(int x1,int y1,int x2,int y2)
	{
		return x1*x2+y1*y2;
	}
	public static double dot(double x1,double y1,double x2,double y2)
	{
		return x1*x2+y1*y2;
	}
	
	/**
	 * 外积，回传纯量（除去方向）   
	 * 外积大于0时，两向量前后顺序为逆时针顺序（在180度之内）；等于0时，两向量平行，也就是指夹角等于0度或180度；小于0时，两向量前后顺序为顺时针顺序（在180度之内）。 
	 * @param x1
	 * @param y1
	 * @param x2
	 * @param y2
	 * @return
	 */
	public static int cross(int x1,int y1,int x2,int y2){
		return x1*y2-y1*x2;
	}
	public static double cross(double x1,double y1,double x2,double y2){
		return x1*y2-y1*x2;
	}
	
	/**
	 * 向量的模
	 * @param x
	 * @param y
	 * @return
	 */
	public static float modulus(int x,int y){
		return (float) Math.sqrt(x*x+y*y);
	}
	
	/**
	 * 旋转向量,逆时针为正方向
	 * @param x
	 * @param y
	 * @param angle
	 * @param result
	 */
	public static void rotate(int x,int y,double angle,Point2D result)
	{
		double sin=Math.sin(angle);
		double cos=Math.cos(angle);
		result.x = x*cos - y*sin;
		result.y = x*sin + y*cos;
	}
	public static void rotate(double x,double y,double angle,Point2D result)
	{
		double sin=Math.sin(angle);
		double cos=Math.cos(angle);
		result.x = x*cos - y*sin;
		result.y = x*sin + y*cos;
	}
	
	public static int random(int start,int end){
		return random.nextInt(end-start)+start;
	}

}
