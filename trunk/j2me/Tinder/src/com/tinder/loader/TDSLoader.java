package com.tinder.loader;

import java.io.IOException;
import java.io.InputStream;
import java.util.Vector;

import util.LittleEndianInputStream;

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
	protected StringBuffer buffer;
	
	public TDSLoader(){
		chunk=new Chunk();
		tempChunk=new Chunk();
		model=new TDModel();
		buffer=new StringBuffer();
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
		printChunk();
//		if(leis.readUnsignedShort()!=PRIMARY_CHUNK)
//		{
//			throw new IOException("This is not a 3ds file");
//		}
		processNextChunk(model, chunk);
		
		// �ڶ�������3ds�ļ�֮�󣬼��㶥��ķ���
		
		
		/*System.out.println(leis.readUnsignedInt()+","+is.available());
		System.out.println(Long.toString(leis.readUnsignedInt(),16));
		System.out.println(Long.toString(leis.readUnsignedInt(),16));
		System.out.println(Long.toString(leis.readUnsignedInt(),16));
		System.out.println(Long.toString(leis.readUnsignedInt(),16));*/
//		System.out.println(Integer.toHexString(leis.readInt()));
//		System.out.println(Integer.toHexString(leis.readInt()));
//		System.out.println(Integer.toHexString(leis.readInt()));
//		System.out.println(Integer.toHexString(leis.readInt()));
//		System.out.println(Integer.toHexString(leis.readUnsignedShort()));
//		System.out.println(Integer.toHexString(leis.readUnsignedShort()));
//		System.out.println(Integer.toHexString(leis.readUnsignedShort()));
//		System.out.println(Integer.toHexString(leis.readUnsignedShort()));
//		System.out.println(Integer.toHexString(leis.readUnsignedShort()));
//		System.out.println(Integer.toHexString(leis.readUnsignedShort()));
//		System.out.println(Integer.toHexString(leis.readUnsignedShort()));
//		System.out.println(Integer.toHexString(leis.readUnsignedShort()));
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
//		TDObject newObject = new TDObject();			// ������ӵ���������
//		MaterialInfo newTexture =new MaterialInfo();	// ������ӵ���������
		long version=0;									// �����ļ��汾
		chunk=new Chunk();								// Ϊ�µĿ����ռ�
		
		/*
		 * ����ÿ��һ���¿飬��Ҫ�ж�һ�¿��ID������ÿ�����Ҫ�Ķ���ģ����������
		 * ����ǲ���Ҫ����Ŀ飬���Թ�
		 */
		while(prevChunk.bytesRead<prevChunk.length)
		{
			readChunk(chunk);
			switch(chunk.ID)
			{
			case VERSION:			// �ļ��汾��
				version=Long.parseLong(leis.readString((int)(chunk.length-chunk.bytesRead)));
				chunk.bytesRead+=chunk.length-chunk.bytesRead;
				// ����ļ��汾�Ŵ���3������һ��������Ϣ
				if (version > 0x03)
					System.out.println("Warning:This 3DS file is over version 3 so it may load incorrectly");
				break;
			case OBJECT_INFO:		// ����汾��Ϣ
				readChunk(tempChunk);
				version=Long.parseLong(leis.readString((int)(tempChunk.length-tempChunk.bytesRead)));
				tempChunk.bytesRead+=tempChunk.length-tempChunk.bytesRead;
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
				chunk.bytesRead+=readString(buffer);
				((TDObject)model.objects.lastElement()).name=new String(buffer);
				// ��������Ķ�����Ϣ�Ķ���
				processNextObjectChunk(model, (TDObject)model.objects.lastElement(), chunk);
				break;
			case EDIT_KEY_FRAME:
				// �����ؼ�֡��Ķ��룬������Ҫ������ֽ���
				leis.skip(chunk.length-chunk.bytesRead);
				chunk.bytesRead+=chunk.length-chunk.bytesRead;
				break;
			default:
				// �ӹ�����Ҫ����Ŀ�
				leis.skip(chunk.length-chunk.bytesRead);
				chunk.bytesRead+=chunk.length-chunk.bytesRead;
			}
			// ��Ӵ������ж�����ֽ���
			prevChunk.bytesRead+=chunk.bytesRead;
		}
		// ɾ����ǰ�飬������ǰ��ָ��ǰ��Ŀ�
		chunk=null;
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
			switch(chunk.ID)
			{
			case MATERIAL_NAME:			// ���ʵ�����
				((MaterialInfo)model.materials.lastElement()).name=leis.readString((int)(chunk.length-chunk.bytesRead));
				chunk.bytesRead+=chunk.length-chunk.bytesRead;
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
				chunk.bytesRead+=chunk.length-chunk.bytesRead;
				break;
			default:
				// �ӹ�����Ҫ����Ŀ�
				leis.skip(chunk.length-chunk.bytesRead);
				chunk.bytesRead+=chunk.length-chunk.bytesRead;
			}
			// ��Ӵ������ж�����ֽ���
			prevChunk.bytesRead+=chunk.bytesRead;
		}
		// ɾ����ǰ�飬������ǰ��ָ��ǰ��Ŀ�
		chunk=null;
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
				chunk.bytesRead+=chunk.length-chunk.bytesRead;
			}
			// ��Ӵ������ж�����ֽ�����ǰ��Ķ�����ֽ���
			prevChunk.bytesRead+=chunk.bytesRead;
		}
		// ɾ����ǰ�飬������ǰ��ָ��ǰ��Ŀ�
		chunk=null;
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
		tempChunk.bytesRead+=tempChunk.length-tempChunk.bytesRead;
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
		for(int i=0;i<object.texCoords.length;i+=2)
		{
			object.texCoords[i*2]=leis.readFloat();
			object.texCoords[i*2+1]=leis.readFloat();
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
		// ���ʻ�������ɫ�������Ƕ��������Ҳ���ܱ������������ȡ�����ȵ���Ϣ��
		// ������븳�赱ǰ����Ĳ�������
		prevChunk.bytesRead+=readString(buffer);
		strMaterial=new String(buffer);
		
		String fileName=null;
		// �������е�����
		for(int i=0;i<model.numOfMaterials;i++)
		{
			//�������������뵱ǰ����������ƥ��
			if(((String)model.materials.elementAt(i)).equals(strMaterial))
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
		prevChunk.bytesRead+=prevChunk.length-prevChunk.bytesRead;
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
		for(int i=0;i<object.vertices.length;i+=3)
		{
			object.vertices[i*3]=leis.readFloat();
			object.vertices[i*3+1]=leis.readFloat();
			object.vertices[i*3+2]=leis.readFloat();
			prevChunk.bytesRead+=12;
		}
		// �����Ѿ����������еĶ��㡣
		// ��Ϊ3D Studio Max��ģ�͵�Z����ָ���ϵģ������Ҫ��y���z�ᷭת������
		// ����������ǽ�Y���Z�ύ����Ȼ��Z�ᷴ��
		// �������еĶ���
		float temp;
		for(int i=0;i<object.vertices.length;i+=3)
		{
			// ����Y���ֵ
			temp=object.vertices[i*3+1];
			// ����Y���ֵ����Z���ֵ
			object.vertices[i*3+1]=object.vertices[i*3+2];
			// ����Z���ֵ����-Y���ֵ 
			object.vertices[i*3+2]=-temp;
		}
	}
	
	/**
	 * ����һ���ַ����������ַ����ĳ���
	 * @param buffer
	 * @return
	 * @throws IOException
	 */
	private int readString(StringBuffer buffer) throws IOException
	{
		buffer.setLength(0);
		int b=0;
		do{
			b=leis.readByte();
			buffer.append(b);
		}while(b!=0);	// ����һ���ַ�ֱ��NULL
		return buffer.length();		/**???�Ƿ��1*/
	}
	
	private void printChunk(){
		System.out.println("ID:0x"+Integer.toHexString(chunk.ID)+",length:"+chunk.length);
	}
	
	private void computeNormals(TDModel model)
	{
		// ���ģ����û�ж����򷵻�
		if(model.numOfObjects<=0)
			return;
		
		TDObject object=null;
		float[] normals=null;
		float[] tempNormals=null;
		
		// ����ģ�������еĶ���
		for(int i=0;i<model.numOfObjects;i++)
		{
			// ��õ�ǰ�Ķ���
			object=(TDObject)model.objects.elementAt(i);
			// ������Ҫ�Ĵ洢�ռ�
			normals=new float[object.numOfFaces*3];
			tempNormals=new float[object.numOfFaces*3];
			object.normals=new float[object.numOfVerts*3];
			// ���������������
			for(int j=0;j<normals.length;j+=3)
			{
				
			}
		}
	}

	public static void main(String[] args) throws IOException {
		TDSLoader loader=new TDSLoader();
		InputStream is=loader.getClass().getResourceAsStream("/face.3ds");
		loader.load(is);
		is.close();
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
	int verticeIndexs[]=new int[3];	// ��������
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
}
/**
 * ������Ϣ�ṹ��
 */
class MaterialInfo{
	String name;				// ��������
	String fileName;			// �����������ӳ�䣬���ʾ�����ļ�����
	byte[] color;				// �����RGB��ɫ
	int texureId;				// ����ID
	float uTile;				// u �ظ�
	float vTile;				// v �ظ�
	float uOffset;			    // u ����ƫ��
	float vOffset;				// v ����ƫ��
}
/**
 * ģ����Ϣ�ṹ��
 */
class TDModel{
	int numOfObjects;					// ģ���ж������Ŀ
	int numOfMaterials;					// ģ���в��ʵ���Ŀ
	Vector materials;					// ����������Ϣ
	Vector objects;						// ģ���ж���������Ϣ
	
	void addObject(TDObject obj)
	{
		if(this.objects==null)
			this.objects=new Vector();
		this.objects.addElement(obj);
	}
	
	void addMaterail(MaterialInfo material)
	{
		if(this.materials==null)
			this.materials=new Vector();
		this.materials.addElement(material);
	}
}
