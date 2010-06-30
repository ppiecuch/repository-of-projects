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
	
	protected static final char PRIMARY_CHUNK=0x4D4D;		//������(Primary Chunk)��λ���ļ��Ŀ�ʼ
	/*����(Main Chunks)*/
	protected static final char OBJECT_INFO=0x3D3D;			//�������İ汾��
	protected static final char VERSION=0x0002;				//.3ds�ļ��İ汾
	protected static final char EDIT_KEY_FRAME=0xB000;		//���йؼ�֡��Ϣ��ͷ��
	/*����Ĵμ�����(��������Ĳ��ʺͶ���*/
	protected static final char MATERIAL=0xAFFF;				//����������Ϣ
	protected static final char OBJECT=0x4000;				//���������桢�������Ϣ
	/*���ʵĴμ�����*/
	protected static final char MATERIAL_NAME=0xA000;		//�����������
	protected static final char MATERIAL_DIFFUSE=0xA020;		//����/���ʵ���ɫ
	protected static final char MATERIAL_MAP=0xA200;			//�²��ʵ�ͷ��
	protected static final char MATERIAL_MAP_FILE=0xA300;	//����������ļ���
	
	protected static final char OBJECT_MESH=0x4100;			//�µ��������
	/*OBJECT_MESH�Ĵμ�����*/
	protected static final char OBJECT_VERTICES=0x4110;		//���󶥵�
	protected static final char OBJECT_FACES=0x4120;			//�������
	protected static final char OBJECT_MATERIAL=0x4130;		//����Ĳ���
	protected static final char OBJECT_UV=0x4140;			//�����UV��������
	
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
		 * ���Ƚ��ļ��ʼ�����ݿ�������ж��Ƿ���һ��3ds�ļ�
		 */
		readChunk(chunk);
		if(chunk.ID!=PRIMARY_CHUNK)
		{
			throw new IOException("This is not a 3ds file");
		}
//		printChunk(chunk);
		processNextChunk(model, chunk);
		
		// �ڶ�������3ds�ļ�֮�󣬼��㶥��ķ���
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
	 * ��ȡ��һ����Ϣ
	 * @throws IOException
	 */
	private void readChunk(Chunk chunk) throws IOException
	{
		chunk.ID=leis.readUnsignedShort();
		chunk.length=leis.readUnsignedInt();
		chunk.bytesRead=6;
	}
	
	/**
	 * �ݹ����3ds�ļ�����Ҫ����
	 * @throws IOException
	 */
	private void processNextChunk(TDModel model,Chunk prevChunk) throws IOException
	{
		long version=0;									// �����ļ��汾
		chunk=new Chunk();								// Ϊ�µĿ����ռ�
		
		/*
		 * ����ÿ��һ���¿飬��Ҫ�ж�һ�¿��ID������ÿ�����Ҫ�Ķ���ģ����������
		 * ����ǲ���Ҫ����Ŀ飬���Թ�
		 */
		while(prevChunk.bytesRead<prevChunk.length)
		{
			readChunk(chunk);
//			printChunk(chunk);
			byte[] buffer=new byte[4096];
			switch(chunk.ID)
			{
			case VERSION:			// �ļ��汾��
				version=leis.readUnsignedInt();
				chunk.bytesRead+=4;
				// ����ļ��汾�Ŵ���3������һ��������Ϣ
				if (version > 0x03)
					System.out.println("Warning:This 3DS file is over version 3 so it may load incorrectly");
				break;
			case OBJECT_INFO:		// ����汾��Ϣ
				readChunk(tempChunk);
//				printChunk(tempChunk);
				leis.skip(tempChunk.length-tempChunk.bytesRead);
				tempChunk.bytesRead=tempChunk.length;
				chunk.bytesRead+=tempChunk.bytesRead;
				processNextChunk(model, chunk);
				break;
			case MATERIAL:			// ������Ϣ
				// ���ʵ���Ŀ����
				model.numOfMaterials++;
				// ���������������һ���հ�����ṹ
				model.addMaterail(new MaterialInfo());
				// �������װ�뺯��
				processNextMaterialChunk(model, chunk);
				break;
			case OBJECT:			// ���������
				// �ÿ��Ƕ�����Ϣ���ͷ���������˶���������
				// ����������
				model.numOfObjects++;
				// ���һ���µ�TDObject�ڵ㵽����������
				model.addObject(new TDObject());
				// ��ʼ������������������ݳ�Ա
				// ��ò������������ƣ�Ȼ�����Ӷ�����ֽ���
				int length=readString(buffer);
				chunk.bytesRead+=length;
				((TDObject)model.objects.lastElement()).name=new String(buffer,"GBK").substring(0,length-1);
				// ��������Ķ�����Ϣ�Ķ���
				processNextObjectChunk(model, (TDObject)model.objects.lastElement(), chunk);
				break;
			case EDIT_KEY_FRAME:
				// �����ؼ�֡��Ķ��룬������Ҫ������ֽ���
				leis.skip(chunk.length-chunk.bytesRead);
				chunk.bytesRead=chunk.length;
				break;
			default:
				// �ӹ�����Ҫ����Ŀ�
				leis.skip(chunk.length-chunk.bytesRead);
				chunk.bytesRead=chunk.length;
			}
			// ��Ӵ������ж�����ֽ���
			prevChunk.bytesRead+=chunk.bytesRead;
		}
		// ����ǰ��ָ��ǰ��Ŀ�
		chunk=prevChunk;
	}
	
	/**
	 * �������еĲ�����Ϣ
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
			case MATERIAL_NAME:			// ���ʵ�����
				((MaterialInfo)model.materials.lastElement()).name=leis.readString((int)(chunk.length-chunk.bytesRead)).trim();
				chunk.bytesRead=chunk.length;
				break;
			case MATERIAL_DIFFUSE:		// �����RGB��ɫ
				readColorChunk((MaterialInfo)model.materials.lastElement(),chunk);
				break;
			case MATERIAL_MAP:			// ������Ϣ��ͷ��
				// ������һ�����ʿ���Ϣ
				processNextMaterialChunk(model, chunk);
				break;
			case MATERIAL_MAP_FILE:		// �����ļ�������
				((MaterialInfo)model.materials.lastElement()).fileName=leis.readString((int)(chunk.length-chunk.bytesRead));
				chunk.bytesRead=chunk.length;
				break;
			default:
				// �ӹ�����Ҫ����Ŀ�
				leis.skip(chunk.length-chunk.bytesRead);
				chunk.bytesRead=chunk.length;
			}
			// ��Ӵ������ж�����ֽ���
			prevChunk.bytesRead+=chunk.bytesRead;
		}
		// ����ǰ��ָ��ǰ��Ŀ�
		chunk=prevChunk;
	}
	
	/**
	 * �������е��ļ��ж������Ϣ
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
			case OBJECT_MESH:			// ���������һ���¿�
				// ʹ�õݹ麯�����ã�������¿�
				processNextObjectChunk(model, object, chunk);
				break;
			case OBJECT_VERTICES:		// �����Ƕ��󶥵�
				readVertice(object, chunk);
				break;
			case OBJECT_FACES:			// ������Ƕ������
				readVerticeIndices(object, chunk);
				break;
			case OBJECT_MATERIAL:		// ������Ƕ���Ĳ�������
				// �ÿ鱣���˶�����ʵ����ƣ�������һ����ɫ��Ҳ������һ������ӳ�䡣ͬʱ�ڸÿ���Ҳ������
				// ����������������
				// ����������Ĳ�������
				readObjectMaterial(model, object, chunk);
				break;
			case OBJECT_UV:				// ��������UV��������
				// ��������UV��������
				readUVCoordinates(object, chunk);
			default:  
				// �Թ�����Ҫ����Ŀ�
				leis.skip(chunk.length-chunk.bytesRead);
				chunk.bytesRead=chunk.length;
			}
			// ��Ӵ������ж�����ֽ�����ǰ��Ķ�����ֽ���
			prevChunk.bytesRead+=chunk.bytesRead;
		}
		// ����ǰ��ָ��ǰ��Ŀ�
		chunk=prevChunk;
	}
	
	/**
	 * ����RGB��ɫ
	 * @param material
	 * @param prevChunk
	 * @throws IOException
	 */
	private void readColorChunk(MaterialInfo material,Chunk prevChunk) throws IOException
	{
		// ������ɫ����Ϣ
		readChunk(tempChunk);
		// ����RGB��ɫ
		material.color=leis.readString((int)(tempChunk.length-tempChunk.bytesRead)).getBytes();
		tempChunk.bytesRead=tempChunk.length;
		// ���Ӷ�����ֽ���
		prevChunk.bytesRead+=tempChunk.bytesRead;
	}
	
	/**
	 * ��������UV����
	 * @param object
	 * @param prevChunk
	 * @throws IOException
	 */
	private void readUVCoordinates(TDObject object,Chunk prevChunk) throws IOException
	{
		// Ϊ�˶�������UV���꣬������Ҫ����UV�����������Ȼ��Ŷ�����������
		// ����UV���������
		object.numTexVertex=leis.readUnsignedShort();
		prevChunk.bytesRead+=2;
		// ���䱣��UV������ڴ�ռ�
		object.texCoords=new float[object.numTexVertex*2];
		// ������������
		for(int i=0;i<object.numTexVertex;i++)
		{
			object.texCoords[i*2]=leis.readFloat();
			object.texCoords[i*2+1]=1-leis.readFloat();
			prevChunk.bytesRead+=8;
		}
	}
	
	/**
	 * �������Ĳ�������
	 * @param model
	 * @param object
	 * @param prevChunk
	 * @throws IOException
	 */
	private void readObjectMaterial(TDModel model,TDObject object,Chunk prevChunk) throws IOException
	{
		String strMaterial;
		byte[] buffer=new byte[4096];
		// ���ʻ�������ɫ�������Ƕ��������Ҳ���ܱ������������ȡ�����ȵ���Ϣ��
		// ������븳�赱ǰ����Ĳ�������
		int length=readString(buffer);
		prevChunk.bytesRead+=length;
		strMaterial=new String(buffer,"GBK").substring(0,length-1);
		System.out.println(strMaterial);
		
		String fileName=null;
		// �������е�����
		for(int i=0;i<model.numOfMaterials;i++)
		{
			//�������������뵱ǰ����������ƥ��
			if(((MaterialInfo)model.materials.elementAt(i)).name.equals(strMaterial))
			{
				// ���ò���ID
				object.materialID=i;
				// �ж��Ƿ�������ӳ�䣬���strFile��һ�����ȴ���1���ַ�������������
				fileName=((MaterialInfo)model.materials.elementAt(i)).fileName;
				if(fileName!=null&&fileName.length()>0)
				{
					// ���ö��������ӳ���־
					object.hasTexture=true;
				}
				break;
			}
			else
			{
				// ����ö���û�в��ʣ�������IDΪ-1
				object.materialID=-1;
			}
		}
		
		leis.skip(prevChunk.length-prevChunk.bytesRead);
		prevChunk.bytesRead=prevChunk.length;
	}
	
	/**
	 * ���붥������
	 * @param object
	 * @param prevChunk
	 * @throws IOException
	 */
	private void readVerticeIndices(TDObject object,Chunk prevChunk) throws IOException
	{
		char index = 0;					// ���ڶ��뵱ǰ�������
		// ����ö����������Ŀ
		object.numOfFaces=leis.readUnsignedShort();
		prevChunk.bytesRead+=2;
		// ����������Ĵ洢�ռ䣬����ʼ���ṹ
		object.faces=new Face[object.numOfFaces];
		// �������������е���
		for(int i=0;i<object.numOfFaces;i++)
		{
			object.faces[i]=new Face();
			object.faces[i].verticeIndexs=new int[3];
			for(int j=0;j<4;j++)
			{
				// ���뵱ǰ��ĵ�һ���� 
				index=leis.readUnsignedShort();
				prevChunk.bytesRead+=2;
				if(j<3)
				{
					// ��������������Ľṹ��
					object.faces[i].verticeIndexs[j]=index;
				}
			}
		}
	}
	
	/**
	 * �������Ķ���
	 * @param object
	 * @param prevChunk
	 * @throws IOException
	 */
	private void readVertice(TDObject object,Chunk prevChunk) throws IOException
	{
		// �ڶ���ʵ�ʵĶ���֮ǰ�����ȱ���ȷ����Ҫ������ٸ����㡣
		// ���붥�����Ŀ
		object.numOfVerts=leis.readUnsignedShort();
		prevChunk.bytesRead+=2;
		// ���䶥��Ĵ洢�ռ䣬Ȼ���ʼ���ṹ��
		object.vertices=new float[object.numOfVerts*3];
		// ���붥������
		// ��Ϊ3D Studio Max��ģ�͵�Z����ָ���ϵģ������Ҫ��y���z�ᷭת������
		// ����������ǽ�Y���Z�ύ����Ȼ��Z�ᷴ��
		for(int i=0;i<object.numOfVerts;i++)
		{
			object.vertices[i*3]=leis.readFloat();
			object.vertices[i*3+2]=-leis.readFloat();
			object.vertices[i*3+1]=leis.readFloat();
			prevChunk.bytesRead+=12;
		}
	}
	
	/**
	 * ����һ���ַ����������ַ����ĳ���
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
		// ���ģ����û�ж����򷵻�
		if(model.numOfObjects<=0)
			return;
		
		TDObject object=null;
		float[][] normals=null;
		float[][] tempNormals=null;
		
		// ����ģ�������еĶ���
		for(int i=0;i<model.numOfObjects;i++)
		{
			// ��õ�ǰ�Ķ���
			object=(TDObject)model.objects.elementAt(i);
			// ������Ҫ�Ĵ洢�ռ�
			normals=new float[object.numOfFaces][];
			tempNormals=new float[object.numOfFaces][];
			object.normals=new float[object.numOfVerts*3];
			// ���������������
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
				
				//������ķ�����
				MatrixUtils.minus(poly[0],poly[2],vertor1);
				MatrixUtils.minus(poly[2],poly[1],vertor2);
				
				//������ķ�����
				MatrixUtils.cross(vertor1,vertor2,normal);
				//����ǹ淶��������
				tempNormals[j]=new float[3];
				MatrixUtils.copy(normal,tempNormals[j]);
				
				MatrixUtils.normalize(normal);
				normals[j]=normal;
				
			}
			//�����󶥵㷨����
			float[] sum={0,0,0};
			int shared=0;
			//�������еĶ���
			for(int j=0;j<object.numOfVerts;j++)
			{
				//�������е���������
				for(int k=0;k<object.numOfFaces;k++)
				{
					// �жϸõ��Ƿ����������湲��
					if(object.faces[k].verticeIndexs[0]==j||
					   object.faces[k].verticeIndexs[1]==j||
					   object.faces[k].verticeIndexs[2]==j)
					{
						MatrixUtils.plus(sum,tempNormals[k],sum);
						shared++;
					}
				}
				MatrixUtils.scalarMultiply(sum,-shared);
				//�淶�����Ķ��㷨��
				MatrixUtils.normalize(sum);
				
				object.normals[j*3]=sum[0];
				object.normals[j*3+1]=sum[1];
				object.normals[j*3+2]=sum[2];
				
				MatrixUtils.zero(sum);
				shared=0;
			}
			//�ͷŴ洢�ռ䣬��ʼ��һ������
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
		//��������
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
 * �������Ϣ�Ľṹ
 */
class Chunk{
	
	char ID;			//���ID
	long length;		//��ĳ���
	long bytesRead;		//��Ҫ���Ŀ����ݵ��ֽ���	
}
/**
 * ��Ľṹ����
 */
class Face{
	int verticeIndexs[];				// ��������
	int coordinateIndexs[];				// ������������
}
/**
 * ������Ϣ�ṹ��
 */
class TDObject{
	int  numOfVerts;					//ģ���ж������Ŀ
	int  numOfFaces;					//ģ���������Ŀ
	int  numTexVertex;					//ģ���������������Ŀ
	int  materialID;					//����ID
	boolean hasTexture;				//�Ƿ��������ӳ��
	String name;						//���������
	float[] vertices;					//����Ķ���
	float[] normals;					//����ķ�����
	float[] texCoords;					//����UV����
	Face[] faces;						//���������Ϣ
	
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
 * ������Ϣ�ṹ��
 */
class MaterialInfo{
	String name;				// ��������
	String fileName;			// �����������ӳ�䣬���ʾ�����ļ�����
	byte[] color;				// �����RGB��ɫ
	int texureId;				// ����ID
//	float uTile;				// u �ظ�
//	float vTile;				// v �ظ�
//	float uOffset;			    // u ����ƫ��
//	float vOffset;				// v ����ƫ��
	
	public String toString() {
		return "name:"+name+",fileName:"+fileName+",texId:"+texureId;
	}
}
/**
 * ģ����Ϣ�ṹ��
 */
class TDModel{
	int numOfObjects;					// ģ���ж������Ŀ
	int numOfMaterials;					// ģ���в��ʵ���Ŀ
	Vector<MaterialInfo> materials;		// ����������Ϣ
	Vector<TDObject> objects;			// ģ���ж���������Ϣ
	
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
