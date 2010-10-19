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
	 * ����������������ɫֵ(A8R8G8B8)����color��alphaֵ
	 */
	public static void setAlpha(int[] pixels,int color,byte alpha)
	{
		/*
		 * ÿһ��ͼ������ͨ��һ��4�ֽ�������չ�֡�
		 * ���λ�ֽ�����alphaͨ��������֮����ʵ��͸��/��͸�����ơ�
		 * 255������ȫ��͸����0�������ȫ͸����
		 * ������һ���ֽ���red��ɫͨ����255������ȫ�Ǻ�ɫ��
		 * �������ƽ����������ֽ���Ӧ��ʵ����ɫ����ɫͨ����
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
	 * ������/�ڻ�
	 * �ڻ�����0ʱ���������н�С��90�ȣ�����0ʱ���нǵ���90�ȣ�С����ʱ���нǴ���90����С��180�ȡ�
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
	 * ������ش���������ȥ����   
	 * �������0ʱ��������ǰ��˳��Ϊ��ʱ��˳����180��֮�ڣ�������0ʱ��������ƽ�У�Ҳ����ָ�нǵ���0�Ȼ�180�ȣ�С��0ʱ��������ǰ��˳��Ϊ˳ʱ��˳����180��֮�ڣ��� 
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
	 * ������ģ
	 * @param x
	 * @param y
	 * @return
	 */
	public static float modulus(int x,int y){
		return (float) Math.sqrt(x*x+y*y);
	}
	
	/**
	 * ��ת����,��ʱ��Ϊ������
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
