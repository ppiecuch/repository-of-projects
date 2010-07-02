package org.tinder.studio.lwjgl.heightmap;

import java.awt.Image;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import javax.imageio.ImageIO;

import org.tinder.studio.lwjgl.util.GLImage;
import org.tinder.studio.lwjgl.util.Point3f;

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
				point.x=j;
				point.y=i;
				point.z=heightWeights[i][j];
				triangleStrip[i][j*2]=point;
				
				point=new Point3f();
				point.x=j;
				point.y=i+1;
				point.z=heightWeights[i+1][j];
				triangleStrip[i][j*2+1]=point;
			}
		}
		return triangleStrip;
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
		int imageWidth=image.getWidth(null);
		int imageHeight=image.getHeight(null);
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
