package com.tinder.loader;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import javax.microedition.m3g.SkinnedMesh;

import util.LittleEndianInputStream;

/**
 * A loader for MD2 (Quake II and others) files
 * 
 * MD2开头的就是MD2文件里的块结构
 * Header比较重要，基本上MD2所有的信息都在这里面了
 * 不过也很简单，对于不关心的块，根据各自的Offset跳过去就行了
 *
 */
public class MD2Loader {
	
	private HeaderInfo headerInfo;
	
	public void load(InputStream in) throws IOException
	{
		LittleEndianInputStream is = new LittleEndianInputStream(in);
		byte[] fileID=new byte[4];
		is.read(fileID);// 对应MD2/MD3应是IDP2或者IDP3
		if(new String(fileID).equals("IDP2")==false)
		{
			in.close();
			throw new IOException("This is not a md2 file");
		}
		int version = is.readInt();//对应MD2/MD3应是8或者15
		if(version!=8)
		{
			in.close();
			throw new IOException("This is not a md2 file");
		}
		headerInfo=new HeaderInfo();
		headerInfo.setSkinWidth(is.readInt());
		headerInfo.setSkinHeight(is.readInt());
		headerInfo.setFrameSize(is.readInt());
		headerInfo.setSkinNum(is.readInt());
		headerInfo.setVerticeNum(is.readInt());
		headerInfo.setTexCoordNum(is.readInt());
		headerInfo.setTriangleNum(is.readInt());
		headerInfo.setGlCommandNum(is.readInt());
		headerInfo.setFrameNum(is.readInt());
		headerInfo.setSkinOffset(is.readInt());
		headerInfo.setTexCoordOffset(is.readInt());
		headerInfo.setTriangeOffset(is.readInt());
		headerInfo.setFrameOffset(is.readInt());
		headerInfo.setGlCommandOffset(is.readInt());
		headerInfo.setEndOffset(is.readInt());
		
		byte[] bytes = new byte[headerInfo.getEndOffset()-68];
		is.read(bytes);
		
		ByteArrayInputStream bs = 
			new ByteArrayInputStream(bytes, headerInfo.getSkinOffset()-68, bytes.length-headerInfo.getSkinOffset());
		is = new LittleEndianInputStream(bs);
		
		for (int i=0;i<headerInfo.getSkinNum();i++) {
			String path = is.readString(64);
			System.out.println(path);
		}
		
		bs = new ByteArrayInputStream(bytes, headerInfo.getFrameNum()-68, bytes.length-headerInfo.getFrameNum());
		is = new LittleEndianInputStream(bs);
		
		
//		float[] vertices=new float[headerInfo.getVerticeNum()*3];
//		float[] texCoords=new float[headerInfo.getTexCoordNum()*3];
//		float[] triangls=new float[headerInfo.getTriangleNum()*3];
		
		for (int i=0;i<headerInfo.getFrameNum();i++) {
			float[] scle = new float[3];
			scle[0] = is.readFloat();
			scle[1] = is.readFloat();
			scle[2] = is.readFloat();
//			System.out.println(scle[0]+","+scle[1]+","+scle[2]);
			float[] translate = new float[3];
			translate[0] = is.readFloat();
			translate[1] = is.readFloat();
			translate[2] = is.readFloat();
//			System.out.println(translate[0]+","+translate[1]+","+translate[2]);
			String name = is.readString(16);
			System.out.println(name);
			for (int j=0;j<headerInfo.getVerticeNum();j++) {
				float[] vert = new float[3];
				vert[0] = scle[0]*is.readUnsignedChar()+translate[0];
				vert[2] = scle[1]*is.readUnsignedChar()+translate[1];
				vert[1] = scle[2]*is.readUnsignedChar()+translate[2];
//				System.out.println(vert[0]+","+vert[1]+","+vert[2]);
				int nIx = is.readUnsignedChar();
//				System.out.println("nIx:"+nIx);
//				m.addVertex(vert);
//				m.addNormal(anorms[nIx]);
			}
//			frames[i] = new Frame(name, m);
		}
		
		bs = 
			new ByteArrayInputStream(bytes, headerInfo.getTriangeOffset()-68, bytes.length-headerInfo.getTriangeOffset());
		is = new LittleEndianInputStream(bs);
		
		for (int i=0;i<headerInfo.getTriangleNum();i++) {
			int[] face = new int[3];
			int[] face_t = new int[3];
			face[0] = is.readUnsignedShort();
			face[1] = is.readUnsignedShort();
			face[2] = is.readUnsignedShort();
			face_t[0] = is.readUnsignedShort();
			face_t[1] = is.readUnsignedShort();
			face_t[2] = is.readUnsignedShort();
//			System.out.println(face[0]+","+face[1]+","+face[2]);
//			for (int j=0;j<headerInfo.getFrameNum();j++) {
//				m = frames[j].getMesh();
//				m.addFace(face);
//				m.setSharedVertexNormals(true);
				//Not necessary because shared with vertices, saves memory...
				//m.addFaceNormals(face);
//				m.addTextureIndices(face_t);
//			}
			//for (int j=1;j<numFrames;j++) {
			//	frames[j].getMesh().setIndexDelegate(m);
			//}
		}
		
		bs =
			new ByteArrayInputStream(bytes, headerInfo.getTexCoordOffset()-68, bytes.length-headerInfo.getTexCoordOffset());
		is = new LittleEndianInputStream(bs);
		
		for (int i=0;i<headerInfo.getTexCoordNum();i++) {
			short s = is.readShort();
			short t = is.readShort();
			float[] coord = new float[2];
			coord[0] = ((float)s)/headerInfo.getSkinWidth();
			coord[1] = ((float)t)/headerInfo.getSkinHeight();
//			System.out.println(coord[0]+","+coord[1]);
//			for (int j=0;j<headerInfo.getFrameNum();j++) 
//				frames[j].getMesh().addTextureCoordinate(coord);
		}
		
		in.close();
	}
	
	
	
	public HeaderInfo getHeaderInfo() {
		return headerInfo;
	}



	public static void main(String[] args) throws IOException
	{
		MD2Loader loader=new MD2Loader();
		InputStream is = loader.getClass().getResourceAsStream("/tris.md2");
		loader.load(is);
		System.out.print(loader.getHeaderInfo());
	}

}

class HeaderInfo{
	private int skinWidth;			//纹理宽度（像素）
	private int skinHeight;		//纹理高度（像素）
	private int frameSize;			//每一帧的大小（字节）
	private int skinNum;			//纹理总数量
	private int verticeNum;		//顶点总数量
	private int texCoordNum;		//纹理坐标总数量
	private int triangleNum;		//三角形总数量
	private int glCommandNum;		//OpenGL命令总数量
	private int frameNum;			//动画帧总数量
	private int skinOffset;		//纹理偏移量（字节）
	private int texCoordOffset;	//纹理坐标偏移量（字节）
	private int triangeOffset;		//三角形偏移量（字节）
	private int frameOffset;		//动画帧偏移量（字节）
	private int glCommandOffset;	//OpenGL命令偏移量（字节）
	private int endOffset;			//本文件结束的位置（字节）
	
	public int getEndOffset() {
		return endOffset;
	}
	public void setEndOffset(int endOffset) {
		this.endOffset = endOffset;
	}
	public int getFrameNum() {
		return frameNum;
	}
	public void setFrameNum(int frameNum) {
		this.frameNum = frameNum;
	}
	public int getFrameOffset() {
		return frameOffset;
	}
	public void setFrameOffset(int frameOffset) {
		this.frameOffset = frameOffset;
	}
	public int getFrameSize() {
		return frameSize;
	}
	public void setFrameSize(int frameSize) {
		this.frameSize = frameSize;
	}
	public int getGlCommandNum() {
		return glCommandNum;
	}
	public void setGlCommandNum(int glCommandNum) {
		this.glCommandNum = glCommandNum;
	}
	public int getGlCommandOffset() {
		return glCommandOffset;
	}
	public void setGlCommandOffset(int glCommandOffset) {
		this.glCommandOffset = glCommandOffset;
	}
	public int getSkinHeight() {
		return skinHeight;
	}
	public void setSkinHeight(int skinHeight) {
		this.skinHeight = skinHeight;
	}
	public int getSkinNum() {
		return skinNum;
	}
	public void setSkinNum(int skinNum) {
		this.skinNum = skinNum;
	}
	public int getSkinOffset() {
		return skinOffset;
	}
	public void setSkinOffset(int skinOffset) {
		this.skinOffset = skinOffset;
	}
	public int getSkinWidth() {
		return skinWidth;
	}
	public void setSkinWidth(int skinWidth) {
		this.skinWidth = skinWidth;
	}
	public int getTexCoordNum() {
		return texCoordNum;
	}
	public void setTexCoordNum(int texCoordNum) {
		this.texCoordNum = texCoordNum;
	}
	public int getTexCoordOffset() {
		return texCoordOffset;
	}
	public void setTexCoordOffset(int texCoordOffset) {
		this.texCoordOffset = texCoordOffset;
	}
	public int getTriangeOffset() {
		return triangeOffset;
	}
	public void setTriangeOffset(int triangeOffset) {
		this.triangeOffset = triangeOffset;
	}
	public int getTriangleNum() {
		return triangleNum;
	}
	public void setTriangleNum(int triangleNum) {
		this.triangleNum = triangleNum;
	}
	public int getVerticeNum() {
		return verticeNum;
	}
	public void setVerticeNum(int verticeNum) {
		this.verticeNum = verticeNum;
	}
	public String toString() {
		return "skinWidth:"+skinWidth+",\n"+
		"skinHeight:"+skinHeight+",\n"+
		"frameSize:"+frameSize+",\n"+
		"skinNum:"+skinNum+",\n"+
		"verticeNum:"+verticeNum+",\n"+
		"texCoordNum:"+texCoordNum+",\n"+
		"triangleNum:"+triangleNum+",\n"+
		"glCommandNum:"+glCommandNum+",\n"+
		"frameNum:"+frameNum+",\n"+
		"skinOffset:"+skinOffset+",\n"+
		"texCoordOffset:"+texCoordOffset+",\n"+
		"triangleOffset:"+triangeOffset+",\n"+
		"frameOffset:"+frameOffset+",\n"+
		"glCommandOffset:"+glCommandOffset+",\n"+
		"endOffset:"+endOffset;
	}
}
