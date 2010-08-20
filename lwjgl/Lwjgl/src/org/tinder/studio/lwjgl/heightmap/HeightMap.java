package org.tinder.studio.lwjgl.heightmap;

import java.awt.Image;
import java.io.IOException;
import java.io.InputStream;

import javax.imageio.ImageIO;

import org.tinder.studio.lwjgl.util.GLImage;
import org.tinder.studio.lwjgl.util.Point3f;
import org.tinder.studio.lwjgl.util.Util;

/**
 * 
 * @author Micheal Hong 
 * @email babala_234@163.com
 * @version 2010-7-8 上午09:55:23
 *
 */
public class HeightMap {
	
	private int imageWidth;
	private int imageHeight;
	private short[][] heightWeights;
	private float[] mufactors;	//放大系数
	private int[] steps;		//步数
	
	public HeightMap(float[] mufactors,int[] steps,InputStream is) throws IOException
	{
		this.mufactors=mufactors;
		this.steps=steps;
		Image image=ImageIO.read(is);
		loadHeightWeight(image);
	}
	
	private void loadHeightWeight(Image image) throws IOException
	{
		int[] pixels=GLImage.getImagePixels(image);
		imageWidth=image.getWidth(null);
		imageHeight=image.getHeight(null);
		System.out.println("imageWidth:"+imageWidth+",imageHeight:"+imageHeight);
		heightWeights=new short[steps[1]][];
		/*偏量*/
		int xOffset = imageWidth/steps[0];
        int yOffset = imageHeight/steps[1];
        if(xOffset==0)
        	throw new IOException("step[0] is larger than image width:"+imageWidth);
        if(yOffset==0)
        	throw new IOException("step[1] is larger than image height:"+imageHeight);
        /*计算高度分量*/
        for(int y = 0; y < steps[1]; y++)
        {
        	heightWeights[y]=new short[steps[0]];
            for(int x = 0; x < steps[0]; x++)
            {
                heightWeights[y][x] = (short)(pixels[x * xOffset + y * yOffset * imageWidth] & 0x000000ff);
            }
        }
	}
	
	public Point3f[][] generateTriangleStrip()
	{
		Point3f[][] triangleStrip=new Point3f[steps[1]-1][];
		for(int i=0;i<triangleStrip.length;i++)
		{
			triangleStrip[i]=new Point3f[steps[0]*2];
			for(int j=0;j<steps[0];j++)
			{
				Point3f point=new Point3f();
				point.x=(j)*mufactors[0];
				point.y=(i)*mufactors[1];
				point.z=heightWeights[i][j]*mufactors[2];
				triangleStrip[i][j*2]=point;
//				System.out.print(point);
				
				point=new Point3f();
				point.x=(j)*mufactors[0];
				point.y=(i+1)*mufactors[1];
				point.z=heightWeights[i+1][j]*mufactors[2];
				triangleStrip[i][j*2+1]=point;
//				System.out.print(point);
			}
//			System.out.println();
		}
		return triangleStrip;
	}
	
	public Point3f[][][] generateTriangles()
	{
		Point3f[][][] triangles=new Point3f[steps[1]-1][][];
		for(int i=0;i<triangles.length;i++)
		{
			triangles[i]=new Point3f[steps[0]*2-2][];
			for(int j=0;j<steps[0]-1;j++)
			{
				Point3f[] points=new Point3f[3];
				for(int k=0;k<3;k++)
					points[k]=new Point3f();
				points[0].x=(j)*mufactors[0];
				points[0].y=(i)*mufactors[1];
				points[0].z=(heightWeights[i][j])*mufactors[2];
				points[1].x=(j+1)*mufactors[0];
				points[1].y=(i)*mufactors[1];
				points[1].z=(heightWeights[i][j+1])*mufactors[2];
				points[2].x=(j)*mufactors[0];
				points[2].y=(i+1)*mufactors[1];
				points[2].z=(heightWeights[i+1][j])*mufactors[2];
				triangles[i][j*2]=points;
				points=new Point3f[3];
				for(int k=0;k<3;k++)
					points[k]=new Point3f();
				points[0].x=(j+1)*mufactors[0];
				points[0].y=(i)*mufactors[1];
				points[0].z=(heightWeights[i][j+1])*mufactors[2];
				points[1].x=(j)*mufactors[0];
				points[1].y=(i+1)*mufactors[1];
				points[1].z=(heightWeights[i+1][j])*mufactors[2];
				points[2].x=(j+1)*mufactors[0];
				points[2].y=(i+1)*mufactors[1];
				points[2].z=(heightWeights[i+1][j+1])*mufactors[2];
				triangles[i][j*2+1]=points;
			}
		}
		return triangles;
	}
	
	/**
	 * 根据x,y求出高度z
	 * 先根据xy判断在哪个矩形中,再判断在哪一个三角形中,再对其使用三角形插值
	 * @param x
	 * @param y
	 * @return
	 */
	public float getHeight(float x,float y)
	{
		int floorX=(int) Math.floor(x/mufactors[0]);
		int cellX=(int) Math.ceil(x/mufactors[0]);
		int floorY=(int) Math.floor(y/mufactors[1]);
		int cellY=(int) Math.ceil(y/mufactors[1]);
		if(floorX<0)floorX=0;
		if(floorY<0)floorY=0;
		if(floorX==cellX)cellX=floorX+1;
		if(floorY==cellY)cellY=floorY+1;
		if(cellX>steps[0]-1)cellX=steps[0]-1;
		if(cellY>steps[1]-1)cellY=steps[1]-1;
		
		Point3f lt=new Point3f(floorX,floorY,0);
		Point3f rt=new Point3f(cellX,floorY,0);
		Point3f lb=new Point3f(floorX,cellY,0);
		Point3f rb=new Point3f(cellX,cellY,0);
		Point3f p=new Point3f(x/mufactors[0],y/mufactors[1],0);
//		System.out.println(floorX+","+cellX+"|"+floorY+","+cellY);

		float Slt=Util.getArea(lt,rt,lb);
		float Sp1=Util.getArea(lt,rt,p);
		float Sp2=Util.getArea(lt,p,lb);
		float Sp3=Util.getArea(p,rt,lb);
//		System.out.println("S:"+Slt+","+Sp1+","+Sp2+","+Sp3+"Slt-Sp1-Sp2-Sp3:"+(Slt-Sp1-Sp2-Sp3));
		float diff=Math.abs(Slt-Sp1-Sp2-Sp3);
		if(diff<Util.TOLERATION&&diff>=0)
		{
			lt.z=heightWeights[(int) lt.y][(int) lt.x];
			rt.z=heightWeights[(int) rt.y][(int) rt.x];
			lb.z=heightWeights[(int) lb.y][(int) lb.x];
			rb.z=heightWeights[(int) rb.y][(int) rb.x];
			
			Util.linearInterpolate(lt,rt,lb,p);
//			System.out.println("lt:"+lt.z+","+rt.z+","+rb.z+"-->"+p.z);
		}
		else
		{
			lt.z=heightWeights[(int) lt.y][(int) lt.x];
			rt.z=heightWeights[(int) rt.y][(int) rt.x];
			lb.z=heightWeights[(int) lb.y][(int) lb.x];
			rb.z=heightWeights[(int) rb.y][(int) rb.x];
//			System.out.println("else");
			
			Util.linearInterpolate(rb,lb,rt,p);
//			System.out.println("rb:"+rb.z+","+rt.z+","+lb.z+"-->"+p.z);
		}
		return p.z*mufactors[2];
	}

	public short[][] getHeightWeights() {
		return heightWeights;
	}

	public int getImageWidth() {
		return imageWidth;
	}

	public int getImageHeight() {
		return imageHeight;
	}
	
	
	
	
}
