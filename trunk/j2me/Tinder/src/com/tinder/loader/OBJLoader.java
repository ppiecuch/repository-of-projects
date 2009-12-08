package com.tinder.loader;


import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Vector;

import util.StringTokenizer;

public class OBJLoader {
	
	private Vector vertices;			//顶点组
	private Vector coordinates;		//纹理坐标组
	private Vector normals;			//法线向量组
	private Vector faceIndices;		//顶点索引组
	private Vector coordIndices;		//纹理索引组
	private Vector normalIndices;		//法线索引组
	
	public void load(InputStream is) throws IOException
	{
		DataInputStream dis=new DataInputStream(is);
		StringBuffer buffer=new StringBuffer();
		vertices=new Vector();
		coordinates=new Vector();
		normals=new Vector();
		faceIndices=new Vector();
		coordIndices=new Vector();
		normalIndices=new Vector();
		byte b1=-1;
		byte b2=-1;
		String line=null;
		while(dis.available()!=0)
		{
			buffer.delete(0,buffer.length());
			b2=dis.readByte();
			while((b1!=13||b2!=10)&&dis.available()!=0){
				buffer.append((char)b1);
				b1=b2;
				b2=dis.readByte();
			}
			buffer.append((char)10);
			line=buffer.toString();
			
			if (line.length() > 0) {
				if (line.startsWith("v ")) {
					float[] vertex = new float[3];
					StringTokenizer tok = new StringTokenizer(line);
					tok.nextToken();
					vertex[0] = Float.parseFloat(tok.nextToken());
					vertex[1] = Float.parseFloat(tok.nextToken());
					vertex[2] = Float.parseFloat(tok.nextToken());
					vertices.addElement(vertex);
				}
				else if (line.startsWith("vt ")) {
					StringTokenizer tok = new StringTokenizer(line);
					tok.nextToken();
					float[] coord=new float[2];
					coord[0] = Float.parseFloat(tok.nextToken());
					coord[1] = Float.parseFloat(tok.nextToken());
					coordinates.addElement(coord);
				}
				else if (line.startsWith("f ")) {
					int[] face = new int[3];
					int[] face_n_ix = new int[3];
					int[] face_tx_ix = new int[3];
					int[] val;

					StringTokenizer tok = new StringTokenizer(line);
					tok.nextToken();
					val = parseIntTriple(tok.nextToken());
					face[0] = val[0];
					if (val.length > 1 && val[1] > -1)
						face_tx_ix[0] = val[1];
					if (val.length > 2 && val[2] > -1)
						face_n_ix[0] = val[2];

					val = parseIntTriple(tok.nextToken());
					face[1] = val[0];
					if (val.length > 1 && val[1] > -1)
						face_tx_ix[1] = val[1];
					if (val.length > 2 && val[2] > -1)
						face_n_ix[1] = val[2];

					val = parseIntTriple(tok.nextToken());
					face[2] = val[0];
					if (val.length > 1 && val[1] > -1) {
						face_tx_ix[2] = val[1];
						coordIndices.addElement(face_tx_ix);
					}
					if (val.length > 2 && val[2] > -1) {
						face_n_ix[2] = val[2];
						normalIndices.addElement(face_n_ix);
					}
					faceIndices.addElement(face);
					if (tok.hasMoreTokens()) {
						val = parseIntTriple(tok.nextToken());
						face[1] = face[2];
						face[2] = val[0];
						if (val.length > 1 && val[1] > -1) {
							face_tx_ix[1] = face_tx_ix[2];
							face_tx_ix[2] = val[1];
							coordIndices.addElement(face_tx_ix);
						}
						if (val.length > 2 && val[2] > -1) {
							face_n_ix[1] = face_n_ix[2];
							face_n_ix[2] = val[2];
							normalIndices.addElement(face_n_ix);
						}
						faceIndices.addElement(face);
					}

				}
				else if (line.startsWith("vn ")) {
					float[] norm = new float[3];
					StringTokenizer tok = new StringTokenizer(line);
					tok.nextToken();
					norm[0] = Float.parseFloat(tok.nextToken());
					norm[1] = Float.parseFloat(tok.nextToken());
					norm[2] = Float.parseFloat(tok.nextToken());
					normals.addElement(norm);
				}
			}
		}
//		if (normals.size()>0) {
//			m.calculateFaceNormals(coordinate_hand);
//			m.calculateVertexNormals();
//			m.copyNormals();
//		}
	}
	
	protected static int parseInt(String val) {
		if (val.length() == 0) {
			return -1;
		}
		return Integer.parseInt(val);
	}
	
	protected static int[] parseIntTriple(String face) {
		int ix = face.indexOf("/");
		if (ix == -1)
			return new int[] {Integer.parseInt(face)-1};
		else {
			int ix2 = face.indexOf("/", ix+1);
			if (ix2 == -1) {
				return new int[] 
				       {Integer.parseInt(face.substring(0,ix))-1,
						Integer.parseInt(face.substring(ix+1))-1};
			}else {
				return new int[]
				        {parseInt(face.substring(0,ix))-1,
						parseInt(face.substring(ix+1,ix2))-1,
						parseInt(face.substring(ix2+1))-1};
			}
		}
	}
	
	public float[] getNormals() {
		float[] normalArray=new float[normals.size()*3];
		for(int i=0;i<this.normals.size();i++)
		{
			normalArray[i*3]=((float[])this.normals.elementAt(i))[0];
			normalArray[i*3+1]=((float[])this.normals.elementAt(i))[1];
			normalArray[i*3+2]=((float[])this.normals.elementAt(i))[2];
		}
		return normalArray;
	}
	
	public float[] getVertices(){
		float[] verticeArray=new float[vertices.size()*3];
		for(int i=0;i<this.vertices.size();i++)
		{
			verticeArray[i*3]=((float[])this.vertices.elementAt(i))[0];
			verticeArray[i*3+1]=((float[])this.vertices.elementAt(i))[1];
			verticeArray[i*3+2]=((float[])this.vertices.elementAt(i))[2];
		}
		return verticeArray;
	}

	public Vector getCoordinates() {
		return coordinates;
	}

	public int[] getFaceIndices() {
		int[] verticeIndiceArray=new int[faceIndices.size()*3];
		for(int i=0;i<this.faceIndices.size();i++)
		{
			verticeIndiceArray[i*3]=((int[])this.faceIndices.elementAt(i))[0];
			verticeIndiceArray[i*3+1]=((int[])this.faceIndices.elementAt(i))[1];
			verticeIndiceArray[i*3+2]=((int[])this.faceIndices.elementAt(i))[2];
		}
		return verticeIndiceArray;
	}



	public static void main(String[] args) throws IOException
	{
		OBJLoader loader=new OBJLoader();
		InputStream is = loader.getClass().getResourceAsStream("/Liit.obj");
		loader.load(is);
		
		
		System.out.println("v:"+loader.getVertices().length/3+",vt:"+loader.getCoordinates().size()+",vn:"+loader.getNormals().length/3+",vi:"+loader.getFaceIndices().length/3);
	}

}
