package org.tinder.studio.lwjgl.tds;

import java.io.IOException;
import java.io.InputStream;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;
import java.util.Vector;

import org.lwjgl.BufferUtils;
import org.lwjgl.opengl.GL11;
import org.tinder.studio.lwjgl.util.LittleEndianInputStream;
import org.tinder.studio.lwjgl.util.MatrixUtils;

public class TDSLoader {
	
	protected static final char PRIMARY_CHUNK=0x4D4D;		//基本块(Primary Chunk)，位于文件的开始
	/*主块(Main Chunks)*/
	protected static final char OBJECT_INFO=0x3D3D;			//网格对象的版本号
	protected static final char VERSION=0x0002;				//.3ds文件的版本
	protected static final char EDIT_KEY_FRAME=0xB000;		//所有关键帧信息的头部
	/*对象的次级定义(包括对象的材质和对象）*/
	protected static final char MATERIAL=0xAFFF;				//保存纹理信息
	protected static final char OBJECT=0x4000;				//保存对象的面、顶点等信息
	/*材质的次级定义*/
	protected static final char MATERIAL_NAME=0xA000;		//保存材质名称
	protected static final char MATERIAL_DIFFUSE=0xA020;		//对象/材质的颜色
	protected static final char MATERIAL_MAP=0xA200;			//新材质的头部
	protected static final char MATERIAL_MAP_FILE=0xA300;	//保存纹理的文件名
	
	protected static final char OBJECT_MESH=0x4100;			//新的网格对象
	/*OBJECT_MESH的次级定义*/
	protected static final char OBJECT_VERTICES=0x4110;		//对象顶点
	protected static final char OBJECT_FACES=0x4120;			//对象的面
	protected static final char OBJECT_MATERIAL=0x4130;		//对象的材质
	protected static final char OBJECT_UV=0x4140;			//对象的UV纹理坐标
	
	protected LittleEndianInputStream leis;
	protected Chunk chunk;
	protected Chunk prevChunk;
	protected Chunk tempChunk;
	protected TDModel model;
	
	public TDSLoader(){
		chunk=new Chunk();
		tempChunk=new Chunk();
		model=new TDModel();
	}
	
	public void load(InputStream is) throws IOException
	{
		leis = new LittleEndianInputStream(is);
		/*
		 * 首先将文件最开始的数据块读出以判断是否是一个3ds文件
		 */
		readChunk(chunk);
		if(chunk.ID!=PRIMARY_CHUNK)
		{
			throw new IOException("This is not a 3ds file");
		}
//		printChunk(chunk);
		processNextChunk(model, chunk);
		
		// 在读完整个3ds文件之后，计算顶点的法线
		computeNormals(model);
		
		is.close();
		
		if(getModel().objects!=null)
		{
			for(int i=0;i<getModel().objects.size();i++)
			{
				System.out.println(getModel().objects.elementAt(i));
			}
		}
		if(getModel().materials!=null)
		{
			for(int i=0;i<getModel().materials.size();i++)
			{
				System.out.println(getModel().materials.elementAt(i));
			}
		}
	}
	
	/**
	 * 读取下一块信息
	 * @throws IOException
	 */
	private void readChunk(Chunk chunk) throws IOException
	{
		chunk.ID=leis.readUnsignedShort();
		chunk.length=leis.readUnsignedInt();
		chunk.bytesRead=6;
	}
	
	/**
	 * 递归读出3ds文件的主要部分
	 * @throws IOException
	 */
	private void processNextChunk(TDModel model,Chunk prevChunk) throws IOException
	{
		long version=0;									// 保存文件版本
		chunk=new Chunk();								// 为新的块分配空间
		
		/*
		 * 下面每读一个新块，都要判断一下块的ID，如果该块是需要的读入的，则继续进行
		 * 如果是不需要读入的块，则略过
		 */
		while(prevChunk.bytesRead<prevChunk.length)
		{
			readChunk(chunk);
//			printChunk(chunk);
			byte[] buffer=new byte[4096];
			switch(chunk.ID)
			{
			case VERSION:			// 文件版本号
				version=leis.readUnsignedInt();
				chunk.bytesRead+=4;
				// 如果文件版本号大于3，给出一个警告信息
				if (version > 0x03)
					System.out.println("Warning:This 3DS file is over version 3 so it may load incorrectly");
				break;
			case OBJECT_INFO:		// 网格版本信息
				readChunk(tempChunk);
//				printChunk(tempChunk);
				leis.skip(tempChunk.length-tempChunk.bytesRead);
				tempChunk.bytesRead=tempChunk.length;
				chunk.bytesRead+=tempChunk.bytesRead;
				processNextChunk(model, chunk);
				break;
			case MATERIAL:			// 材质信息
				// 材质的数目递增
				model.numOfMaterials++;
				// 在纹理链表中添加一个空白纹理结构
				model.addMaterail(new MaterialInfo());
				// 进入材质装入函数
				processNextMaterialChunk(model, chunk);
				break;
			case OBJECT:			// 对象的名称
				// 该块是对象信息块的头部，保存了对象了名称
				// 对象数递增
				model.numOfObjects++;
				// 添加一个新的TDObject节点到对象链表中
				model.addObject(new TDObject());
				// 初始化对象和它的所有数据成员
				// 获得并保存对象的名称，然后增加读入的字节数
				int length=readString(buffer);
				chunk.bytesRead+=length;
				((TDObject)model.objects.lastElement()).name=new String(buffer,"GBK").substring(0,length-1);
				// 进入其余的对象信息的读入
				processNextObjectChunk(model, (TDObject)model.objects.lastElement(), chunk);
				break;
			case EDIT_KEY_FRAME:
				// 跳过关键帧块的读入，增加需要读入的字节数
				leis.skip(chunk.length-chunk.bytesRead);
				chunk.bytesRead=chunk.length;
				break;
			default:
				// 掠过不需要读入的块
				leis.skip(chunk.length-chunk.bytesRead);
				chunk.bytesRead=chunk.length;
			}
			// 添加从最后块中读入的字节数
			prevChunk.bytesRead+=chunk.bytesRead;
		}
		// 将当前块指向前面的块
		chunk=prevChunk;
	}
	
	/**
	 * 处理所有的材质信息
	 * @param model
	 * @param prevChunk
	 * @throws IOException
	 */
	private void processNextMaterialChunk(TDModel model,Chunk prevChunk) throws IOException
	{
		chunk=new Chunk();
		while(prevChunk.bytesRead<prevChunk.length)
		{
			readChunk(chunk);
//			printChunk(chunk);
			switch(chunk.ID)
			{
			case MATERIAL_NAME:			// 材质的名称
				((MaterialInfo)model.materials.lastElement()).name=leis.readString((int)(chunk.length-chunk.bytesRead)).trim();
				chunk.bytesRead=chunk.length;
				break;
			case MATERIAL_DIFFUSE:		// 对象的RGB颜色
				readColorChunk((MaterialInfo)model.materials.lastElement(),chunk);
				break;
			case MATERIAL_MAP:			// 纹理信息的头部
				// 进入下一个材质块信息
				processNextMaterialChunk(model, chunk);
				break;
			case MATERIAL_MAP_FILE:		// 材质文件的名称
				((MaterialInfo)model.materials.lastElement()).fileName=leis.readString((int)(chunk.length-chunk.bytesRead));
				chunk.bytesRead=chunk.length;
				break;
			default:
				// 掠过不需要读入的块
				leis.skip(chunk.length-chunk.bytesRead);
				chunk.bytesRead=chunk.length;
			}
			// 添加从最后块中读入的字节数
			prevChunk.bytesRead+=chunk.bytesRead;
		}
		// 将当前块指向前面的块
		chunk=prevChunk;
	}
	
	/**
	 * 处理所有的文件中对象的信息
	 * @param model
	 * @param prevChunk
	 * @throws IOException
	 */
	private void processNextObjectChunk(TDModel model,TDObject object,Chunk prevChunk) throws IOException
	{
		chunk=new Chunk();
		while(prevChunk.bytesRead<prevChunk.length)
		{
			readChunk(chunk);
//			printChunk(chunk);
			switch(chunk.ID)
			{
			case OBJECT_MESH:			// 正读入的是一个新块
				// 使用递归函数调用，处理该新块
				processNextObjectChunk(model, object, chunk);
				break;
			case OBJECT_VERTICES:		// 读入是对象顶点
				readVertice(object, chunk);
				break;
			case OBJECT_FACES:			// 读入的是对象的面
				readVerticeIndices(object, chunk);
				break;
			case OBJECT_MATERIAL:		// 读入的是对象的材质名称
				// 该块保存了对象材质的名称，可能是一个颜色，也可能是一个纹理映射。同时在该块中也保存了
				// 纹理对象所赋予的面
				// 下面读入对象的材质名称
				readObjectMaterial(model, object, chunk);
				break;
			case OBJECT_UV:				// 读入对象的UV纹理坐标
				// 读入对象的UV纹理坐标
				readUVCoordinates(object, chunk);
			default:  
				// 略过不需要读入的块
				leis.skip(chunk.length-chunk.bytesRead);
				chunk.bytesRead=chunk.length;
			}
			// 添加从最后块中读入的字节数到前面的读入的字节中
			prevChunk.bytesRead+=chunk.bytesRead;
		}
		// 将当前块指向前面的块
		chunk=prevChunk;
	}
	
	/**
	 * 读入RGB颜色
	 * @param material
	 * @param prevChunk
	 * @throws IOException
	 */
	private void readColorChunk(MaterialInfo material,Chunk prevChunk) throws IOException
	{
		// 读入颜色块信息
		readChunk(tempChunk);
		// 读入RGB颜色
		material.color=leis.readString((int)(tempChunk.length-tempChunk.bytesRead)).getBytes();
		tempChunk.bytesRead=tempChunk.length;
		// 增加读入的字节数
		prevChunk.bytesRead+=tempChunk.bytesRead;
	}
	
	/**
	 * 读入对象的UV坐标
	 * @param object
	 * @param prevChunk
	 * @throws IOException
	 */
	private void readUVCoordinates(TDObject object,Chunk prevChunk) throws IOException
	{
		// 为了读入对象的UV坐标，首先需要读入UV坐标的数量，然后才读入具体的数据
		// 读入UV坐标的数量
		object.numTexVertex=leis.readUnsignedShort();
		prevChunk.bytesRead+=2;
		// 分配保存UV坐标的内存空间
		object.texCoords=new float[object.numTexVertex*2];
		// 读入纹理坐标
		for(int i=0;i<object.numTexVertex;i++)
		{
			object.texCoords[i*2]=leis.readFloat();
			object.texCoords[i*2+1]=1-leis.readFloat();
			prevChunk.bytesRead+=8;
		}
	}
	
	/**
	 * 读入对象的材质名称
	 * @param model
	 * @param object
	 * @param prevChunk
	 * @throws IOException
	 */
	private void readObjectMaterial(TDModel model,TDObject object,Chunk prevChunk) throws IOException
	{
		String strMaterial;
		byte[] buffer=new byte[4096];
		// 材质或者是颜色，或者是对象的纹理，也可能保存了象明亮度、发光度等信息。
		// 下面读入赋予当前对象的材质名称
		int length=readString(buffer);
		prevChunk.bytesRead+=length;
		strMaterial=new String(buffer,"GBK").substring(0,length-1);
		System.out.println(strMaterial);
		
		String fileName=null;
		// 遍历所有的纹理
		for(int i=0;i<model.numOfMaterials;i++)
		{
			//如果读入的纹理与当前的纹理名称匹配
			if(((MaterialInfo)model.materials.elementAt(i)).name.equals(strMaterial))
			{
				// 设置材质ID
				object.materialID=i;
				// 判断是否是纹理映射，如果strFile是一个长度大于1的字符串，则是纹理
				fileName=((MaterialInfo)model.materials.elementAt(i)).fileName;
				if(fileName!=null&&fileName.length()>0)
				{
					// 设置对象的纹理映射标志
					object.hasTexture=true;
				}
				break;
			}
			else
			{
				// 如果该对象没有材质，则设置ID为-1
				object.materialID=-1;
			}
		}
		
		leis.skip(prevChunk.length-prevChunk.bytesRead);
		prevChunk.bytesRead=prevChunk.length;
	}
	
	/**
	 * 读入顶点索引
	 * @param object
	 * @param prevChunk
	 * @throws IOException
	 */
	private void readVerticeIndices(TDObject object,Chunk prevChunk) throws IOException
	{
		char index = 0;					// 用于读入当前面的索引
		// 读入该对象中面的数目
		object.numOfFaces=leis.readUnsignedShort();
		prevChunk.bytesRead+=2;
		// 分配所有面的存储空间，并初始化结构
		object.faces=new Face[object.numOfFaces];
		// 遍历对象中所有的面
		for(int i=0;i<object.numOfFaces;i++)
		{
			object.faces[i]=new Face();
			object.faces[i].verticeIndexs=new int[3];
			for(int j=0;j<4;j++)
			{
				// 读入当前面的第一个点 
				index=leis.readUnsignedShort();
				prevChunk.bytesRead+=2;
				if(j<3)
				{
					// 将索引保存在面的结构中
					object.faces[i].verticeIndexs[j]=index;
				}
			}
		}
	}
	
	/**
	 * 读入对象的顶点
	 * @param object
	 * @param prevChunk
	 * @throws IOException
	 */
	private void readVertice(TDObject object,Chunk prevChunk) throws IOException
	{
		// 在读入实际的顶点之前，首先必须确定需要读入多少个顶点。
		// 读入顶点的数目
		object.numOfVerts=leis.readUnsignedShort();
		prevChunk.bytesRead+=2;
		// 分配顶点的存储空间，然后初始化结构体
		object.vertices=new float[object.numOfVerts*3];
		// 读入顶点序列
		// 因为3D Studio Max的模型的Z轴是指向上的，因此需要将y轴和z轴翻转过来。
		// 具体的做法是将Y轴和Z轴交换，然后将Z轴反向。
		for(int i=0;i<object.numOfVerts;i++)
		{
			object.vertices[i*3]=leis.readFloat();
			object.vertices[i*3+2]=-leis.readFloat();
			object.vertices[i*3+1]=leis.readFloat();
			prevChunk.bytesRead+=12;
		}
	}
	
	/**
	 * 读入一个字符串，返回字符串的长度
	 * @param buffer
	 * @return
	 * @throws IOException
	 */
	private int readString(byte[] buffer) throws IOException
	{
		int index=0;
		do{
			leis.read(buffer, index, 1);
		}while(buffer[index++]!=0);
		return index;
	}
	
//	private void printChunk(Chunk chunk){
//		System.out.println("ID:0x"+Integer.toHexString(chunk.ID)+",length:"+chunk.length+",bytesRead:"+chunk.bytesRead);
//	}
	
	private void computeNormals(TDModel model)
	{
		float[][] poly=new float[3][];
		poly[0]=new float[3];
		poly[1]=new float[3];
		poly[2]=new float[3];
		float[] vertor1=new float[3];
		float[] vertor2=new float[3];
		float[] normal=new float[3];
		// 如果模型中没有对象，则返回
		if(model.numOfObjects<=0)
			return;
		
		TDObject object=null;
		float[][] normals=null;
		float[][] tempNormals=null;
		
		// 遍历模型中所有的对象
		for(int i=0;i<model.numOfObjects;i++)
		{
			// 获得当前的对象
			object=(TDObject)model.objects.elementAt(i);
			// 分配需要的存储空间
			normals=new float[object.numOfFaces][];
			tempNormals=new float[object.numOfFaces][];
			object.normals=new float[object.numOfVerts*3];
			// 遍历对象的所有面
			for(int j=0;j<object.numOfFaces;j++)
			{
				poly[0][0]=object.vertices[object.faces[j].verticeIndexs[0]*3];
				poly[0][1]=object.vertices[object.faces[j].verticeIndexs[0]*3+1];
				poly[0][2]=object.vertices[object.faces[j].verticeIndexs[0]*3+2];
				
				poly[1][0]=object.vertices[object.faces[j].verticeIndexs[1]*3];
				poly[1][1]=object.vertices[object.faces[j].verticeIndexs[1]*3+1];
				poly[1][2]=object.vertices[object.faces[j].verticeIndexs[1]*3+2];
				
				poly[2][0]=object.vertices[object.faces[j].verticeIndexs[2]*3];
				poly[2][1]=object.vertices[object.faces[j].verticeIndexs[2]*3+1];
				poly[2][2]=object.vertices[object.faces[j].verticeIndexs[2]*3+2];
				
				//计算面的法向量
				MatrixUtils.minus(poly[0],poly[2],vertor1);
				MatrixUtils.minus(poly[2],poly[1],vertor2);
				
				//计算面的法向量
				MatrixUtils.cross(vertor1,vertor2,normal);
				//保存非规范化法向量
				tempNormals[j]=new float[3];
				MatrixUtils.copy(normal,tempNormals[j]);
				
				MatrixUtils.normalize(normal);
				normals[j]=normal;
				
			}
			//下面求顶点法向量
			float[] sum={0,0,0};
			int shared=0;
			//遍历所有的顶点
			for(int j=0;j<object.numOfVerts;j++)
			{
				//遍历所有的三角形面
				for(int k=0;k<object.numOfFaces;k++)
				{
					// 判断该点是否与其它的面共享
					if(object.faces[k].verticeIndexs[0]==j||
					   object.faces[k].verticeIndexs[1]==j||
					   object.faces[k].verticeIndexs[2]==j)
					{
						MatrixUtils.plus(sum,tempNormals[k],sum);
						shared++;
					}
				}
				MatrixUtils.scalarMultiply(sum,-shared);
				//规范化最后的顶点法向
				MatrixUtils.normalize(sum);
				
				object.normals[j*3]=sum[0];
				object.normals[j*3+1]=sum[1];
				object.normals[j*3+2]=sum[2];
				
				MatrixUtils.zero(sum);
				shared=0;
			}
			//释放存储空间，开始下一个对象
			tempNormals=null;
			normals=null;
		}
	}
	
	public TDModel getModel() {
		return model;
	}

	public static void main(String[] args) throws IOException {
		TDSLoader loader=new TDSLoader();
		InputStream is=loader.getClass().getResourceAsStream("/car.3ds");
		loader.load(is);
		System.out.println("model:"+loader.getModel());
	}
	
	public int getObjectSize(){
		if(model!=null&&model.objects!=null)
			return model.objects.size();
		return 0;
	}
	
	public float[] getVertices(int objectIndex)
	{
		return ((TDObject)model.objects.elementAt(objectIndex)).vertices;
	}
	
	public short[] getVerticeIndices(int objectIndex)
	{
		return ((TDObject)model.objects.elementAt(objectIndex)).getVerticeIndice();
	}
	
	public String getMaterialFileName(int materialIndex)
	{
		return ((MaterialInfo)model.materials.elementAt(materialIndex)).fileName;
	}
	
	public float[] getTextureCoordinates(int objectIndex)
	{
		return ((TDObject)model.objects.elementAt(objectIndex)).texCoords;
	}
	
	public void render()
	{
		//激活数组
		GL11.glEnableClientState(GL11.GL_VERTEX_ARRAY);
//		GL11.glEnableClientState(GL11.GL_COLOR_ARRAY);
		GL11.glEnableClientState(GL11.GL_NORMAL_ARRAY);
		GL11.glEnableClientState(GL11.GL_TEXTURE_COORD_ARRAY);
		
		TDObject obj=this.getModel().objects.get(0);
		
		FloatBuffer vertexBuffer=BufferUtils.createFloatBuffer(obj.vertices.length);
		vertexBuffer.put(obj.vertices);
		GL11.glVertexPointer(3, GL11.GL_FLOAT, vertexBuffer);
		
		
		FloatBuffer normalBuffer=BufferUtils.createFloatBuffer(obj.normals.length);
		normalBuffer.put(obj.normals);
		GL11.glNormalPointer(GL11.GL_FLOAT, normalBuffer);
		
		FloatBuffer textureBuffer=BufferUtils.createFloatBuffer(obj.texCoords.length);
		textureBuffer.put(obj.texCoords);
		GL11.glTexCoordPointer(2, GL11.GL_FLOAT, textureBuffer);
		
		short[] indices=obj.getVerticeIndice();
		ShortBuffer indiceBuffer=BufferUtils.createShortBuffer(indices.length);
		indiceBuffer.put(indices);
		GL11.glDrawElements(GL11.GL_TRIANGLES, indiceBuffer);
		
		GL11.glDisableClientState(GL11.GL_VERTEX_ARRAY);
		GL11.glDisableClientState(GL11.GL_COLOR_ARRAY);
		GL11.glDisableClientState(GL11.GL_NORMAL_ARRAY);
//		GL11.glDisableClientState(GL11.GL_TEXTURE_COORD_ARRAY);
		
//		TDObject obj=this.getModel().objects.get(0);
//		short[] indices=obj.getVerticeIndice();
////		GL11.glBegin(GL11.GL_TRIANGLE_FAN);
////		for(int i=0;i<obj.numOfVerts;i++)
////		{
////			GL11.glVertex3f(obj.vertices[i*3], obj.vertices[i*3+1], obj.vertices[i*3+2]);
////		}
////		GL11.glEnd();
//		GL11.glBegin(GL11.GL_TRIANGLE_STRIP);
//		for(int i=0;i<obj.numOfFaces;i++)
//		{
//			GL11.glVertex3f(obj.vertices[indices[i*3]], obj.vertices[indices[i*3+1]], obj.vertices[indices[i*3+2]]);
//		}
//		GL11.glEnd();
	}

}
/**
 * 保存块信息的结构
 */
class Chunk{
	
	char ID;			//块的ID
	long length;		//块的长度
	long bytesRead;		//需要读的块数据的字节数	
}
/**
 * 面的结构定义
 */
class Face{
	int verticeIndexs[];				// 顶点索引
	int coordinateIndexs[];				// 纹理坐标索引
}
/**
 * 对象信息结构体
 */
class TDObject{
	int  numOfVerts;					//模型中顶点的数目
	int  numOfFaces;					//模型中面的数目
	int  numTexVertex;					//模型中纹理坐标的数目
	int  materialID;					//纹理ID
	boolean hasTexture;				//是否具有纹理映射
	String name;						//对象的名称
	float[] vertices;					//对象的顶点
	float[] normals;					//对象的法向量
	float[] texCoords;					//纹理UV坐标
	Face[] faces;						//对象的面信息
	
	public short[] getVerticeIndice()
	{
		short[] verticeIndices=new short[numOfFaces*3];
		for(int i=0;i<numOfFaces;i++)
		{
			verticeIndices[i*3]=(short)faces[i].verticeIndexs[0];
			verticeIndices[i*3+1]=(short)faces[i].verticeIndexs[1];
			verticeIndices[i*3+2]=(short)faces[i].verticeIndexs[2];
		}
		return verticeIndices;
	}
	
	public String toString() {
		return "name:"+name+",v:"+numOfVerts+",vf:"+numOfFaces+",vu:"+numTexVertex+",maId:"+materialID;
	}
}
/**
 * 材质信息结构体
 */
class MaterialInfo{
	String name;				// 纹理名称
	String fileName;			// 如果存在纹理映射，则表示纹理文件名称
	byte[] color;				// 对象的RGB颜色
	int texureId;				// 纹理ID
//	float uTile;				// u 重复
//	float vTile;				// v 重复
//	float uOffset;			    // u 纹理偏移
//	float vOffset;				// v 纹理偏移
	
	public String toString() {
		return "name:"+name+",fileName:"+fileName+",texId:"+texureId;
	}
}
/**
 * 模型信息结构体
 */
class TDModel{
	int numOfObjects;					// 模型中对象的数目
	int numOfMaterials;					// 模型中材质的数目
	Vector<MaterialInfo> materials;		// 材质链表信息
	Vector<TDObject> objects;			// 模型中对象链表信息
	
	void addObject(TDObject obj)
	{
		if(this.objects==null)
			this.objects=new Vector<TDObject>();
		this.objects.addElement(obj);
	}
	
	void addMaterail(MaterialInfo material)
	{
		if(this.materials==null)
			this.materials=new Vector<MaterialInfo>();
		this.materials.addElement(material);
	}

	public String toString() {
		return "objects:"+(objects==null?null:String.valueOf(objects.size()))+
				",materials:"+(materials==null?null:String.valueOf(materials.size()));
	}
}
