package org.tinder.studio.lwjgl.heightmap;

import java.awt.Image;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import javax.imageio.ImageIO;

import org.tinder.studio.lwjgl.util.GLImage;
import org.tinder.studio.lwjgl.util.Point3f;
import org.tinder.studio.lwjgl.util.Util;

/**
 * 高度图
 * @Author Micheal Hong
 * @Email babala_234@163.com
 * @Version 2010-7-1下午10:08:17
 *
 */
public class HeightMap {
	
	private int width;
	private int height;
	private int imageWidth;
	private int imageHeight;
	private short[][] heightWeights;
	
	public HeightMap(int width,int height,File file) throws IOException
	{
		this(width,height,new FileInputStream(file));
	}
	
	public HeightMap(int width,int height,InputStream is) throws IOException
	{
		this.width=width;
		this.height=height;
		Image image=ImageIO.read(is);
		loadHeightWeight(image);
//		generaVertex();
	}
	
	public Point3f[][] generateTriangleStrip()
	{
		Point3f[][] triangleStrip=new Point3f[height-1][];
		for(int i=0;i<triangleStrip.length;i++)
		{
			triangleStrip[i]=new Point3f[width*2];
			for(int j=0;j<width;j++)
			{
				Point3f point=new Point3f();
				point.x=j-width/2;
				point.y=i-height/2;
				point.z=heightWeights[i][j];
				triangleStrip[i][j*2]=point;
				
				point=new Point3f();
				point.x=j-width/2;
				point.y=i+1-height/2;
				point.z=heightWeights[i+1][j];
				triangleStrip[i][j*2+1]=point;
			}
		}
		return triangleStrip;
	}
	
	public Point3f[][][] generateTriangles()
	{
		Point3f[][][] triangles=new Point3f[height-1][][];
		for(int i=0;i<triangles.length;i++)
		{
			triangles[i]=new Point3f[width*2-2][];
			for(int j=0;j<width-1;j++)
			{
				Point3f[] points=new Point3f[3];
				for(int k=0;k<3;k++)
					points[k]=new Point3f();
				points[0].x=j-width/2;
				points[0].y=i-height/2;
				points[0].z=heightWeights[i][j];
				points[1].x=j+1-width/2;
				points[1].y=i-height/2;
				points[1].z=heightWeights[i][j+1];
				points[2].x=j-width/2;
				points[2].y=i+1-height/2;
				points[2].z=heightWeights[i+1][j];
				triangles[i][j*2]=points;
				points=new Point3f[3];
				for(int k=0;k<3;k++)
					points[k]=new Point3f();
				points[0].x=j+1-width/2;
				points[0].y=i-height/2;
				points[0].z=heightWeights[i][j+1];
				points[1].x=j-width/2;
				points[1].y=i+1-height/2;
				points[1].z=heightWeights[i+1][j];
				points[2].x=j+1-width/2;
				points[2].y=i+1-height/2;
				points[2].z=heightWeights[i+1][j+1];
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
		int floorX=(int) Math.floor(x*imageWidth/width);
		int cellX=(int) Math.ceil(x*imageWidth/width);
		int floorY=(int) Math.floor(y*imageHeight/height);
		int cellY=(int) Math.ceil(y*imageHeight/height);
		if(floorX==cellX)cellX=floorX+1;
		if(floorY==cellY)cellY=floorY+1;
		
		Point3f lt=new Point3f(floorX,floorY,0);
		Point3f rt=new Point3f(cellX,floorY,0);
		Point3f lb=new Point3f(floorX,cellY,0);
		Point3f rb=new Point3f(cellX,cellY,0);
		Point3f p=new Point3f(x,y,0);
		System.out.println(floorX+","+floorY+","+cellX+","+cellY);

		float Slt=Util.getArea(lt,rt,lb);
		float Sp1=Util.getArea(lt,rt,p);
		float Sp2=Util.getArea(lt,p,lb);
		float Sp3=Util.getArea(p,rt,lb);
		System.out.println("S:"+Slt+","+Sp1+","+Sp2+","+Sp3);
		
		if(Slt-Sp1-Sp2-Sp3<Util.TOLERATION)
		{
			lt.z=heightWeights[(int) lt.y+height/2][(int) lt.x+width/2];
			rt.z=heightWeights[(int) rt.y+height/2][(int) rt.x+width/2];
			lb.z=heightWeights[(int) lb.y+height/2][(int) lb.x+width/2];
			rt.z=heightWeights[(int) rb.y+height/2][(int) rb.x+width/2];
			Util.linearInterpolate(lt,rt,lb,p);
		}
		else
		{
			lt.z=heightWeights[(int) lt.y+height/2][(int) lt.x+width/2];
			rt.z=heightWeights[(int) rt.y+height/2][(int) rt.x+width/2];
			lb.z=heightWeights[(int) lb.y+height/2][(int) lb.x+width/2];
			rt.z=heightWeights[(int) rb.y+height/2][(int) rb.x+width/2];
			Util.linearInterpolate(rb,rt,lb,p);
		}
		return p.z;
	}
	
//	private void generaVertex()
//	{
//		this.vertexs=new int[height-1][][][];
//		for(int i=0;i<this.vertexs.length;i++)
//		{
//			this.vertexs[i]=new int[width-1][][];
//			for(int j=0;j<this.vertexs[i].length;j++)
//			{
//				int[][] quad=new int[4][];
//				quad[0]=new int[3];
//				quad[0][0]=j;
//				quad[0][1]=i;
//				quad[0][2]=heightWeights[i][j];
//				quad[1]=new int[3];
//				quad[1][0]=j+1;
//				quad[1][1]=i;
//				quad[1][2]=heightWeights[i][j+1];
//				quad[2]=new int[3];
//				quad[2][0]=j;
//				quad[2][1]=i+1;
//				quad[2][2]=heightWeights[i+1][j];
//				quad[3]=new int[3];
//				quad[3][0]=j+1;
//				quad[3][1]=i+1;
//				quad[3][2]=heightWeights[i+1][j+1];
//				this.vertexs[i][j]=quad;
//				
//			}
//		}
//	}
	
	private void loadHeightWeight(Image image) throws IOException
	{
		int[] pixels=GLImage.getImagePixels(image);
		imageWidth=image.getWidth(null);
		imageHeight=image.getHeight(null);
		System.out.println("imageWidth:"+imageWidth+",imageHeight:"+imageHeight);
		heightWeights=new short[height][];
		/*计算偏量*/
		int xOffset = imageWidth/width;
        int yOffset = imageHeight/height;
        if(xOffset==0)
        	throw new IOException("width is largh than image width:"+imageWidth);
        if(yOffset==0)
        	throw new IOException("height is largh than image height:"+imageHeight);
        /*计算高度分量*/
        for(int y = 0; y < height; y++)
        {
        	heightWeights[y]=new short[width];
            for(int x = 0; x < width; x++)
            {
                heightWeights[y][x] = (short)((pixels[x * xOffset + y * yOffset * imageWidth] & 0x000000ff));
            }
        }
	}

	public int getWidth() {
		return width;
	}

	public int getHeight() {
		return height;
	}

	public short[][] getHeightWeights() {
		return heightWeights;
	}

//	public int[][][][] getVertexs() {
//		return vertexs;
//	}
	
	

}
