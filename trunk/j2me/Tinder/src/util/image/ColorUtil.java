package util.image;

import javax.microedition.lcdui.Image;

public class ColorUtil {
	
	/**
	 * ��24λɫͼ(R8G8B8)תΪ16λɫͼ(R5G6B5)
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
	
	
	/**
	 * ����Image����ɫֵ(A8R8G8B8)����color��alphaֵ
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
