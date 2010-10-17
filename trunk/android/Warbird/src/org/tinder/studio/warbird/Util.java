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

}
