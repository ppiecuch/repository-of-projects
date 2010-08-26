package org.tinder.studio.lwjgl.util;
/**
 * 
 * @author Micheal Hong 
 * @email babala_234@163.com
 * @version 2010-7-6 下午02:38:34
 *
 */
public class Util {
	
	public static float TOLERATION=0.0000005f;
	
	/**
	 * 计算三角形的方向向量,点按逆时针顺序
	 * Points p1, p2, & p3 specified in counter clock-wise order
	 * @param points
	 * @param normal
	 */
	public static void calculateNormal(Point3f p1,Point3f p2,Point3f p3,Vector3f normal)
	{
		Vector3f v1=new Vector3f();
		v1.sub(p1,p2);
		Vector3f v2=new Vector3f();
		v2.sub(p2,p3);
		normal.cross(v1, v2);
		normal.normalize();
	}
	
	// 计算三角形的面向,即可见性（不明白数学根据）
	// 顺时针为-1，逆时针为1
	// Discover the orientation of a triangle's points:
	// Taken from "Programming Principles in Computer Graphics", L. Ammeraal (Wiley)
	//
	public static int orientation( int pX, int pY, int qX, int qY, int rX, int rY )
	{
		int aX, aY, bX, bY;
		float d;

		aX = qX - pX;
		aY = qY - pY;

		bX = rX - pX;
		bY = rY - pY;

		d = (float)aX * (float)bY - (float)aY * (float)bX;
		return (d<0)?-1:(d>0)?1:0;
	}
	
	/**
	 * 平方
	 * @param a
	 * @return
	 */
	public static float square(float a)
	{
		return a*a;
	}
	
	/**
	 * 三角形线性插值,a为共点,b跟c注意顺序,按右手法则
	 * 原理：求P.Z的更好的公式是:d·(d1×d2)=0.其中d为向量<A,P>,d1为向量<A,B>,d2为向量<A,C>.
	 * @param a
	 * @param b
	 * @param c
	 * @param p
	 */
	public static void linearInterpolate(Point3f a,Point3f b,Point3f c,Point3f p)
	{
		Vector3f ab=new Vector3f();
		ab.sub(b,a);
		Vector3f ac=new Vector3f();
		ac.sub(c,a);
		Vector3f temp=new Vector3f();
		temp.cross(ab,ac);
		p.z=a.z-(temp.x*(p.x-a.x)+temp.y*(p.y-a.y)/temp.z);
	}
	
	/**
	 * 计算三角形的面积Sabc=1/2|AB*AC|
	 * @param a
	 * @param b
	 * @param c
	 * @return
	 */
	public static float getArea(Point3f a,Point3f b,Point3f c)
	{
		Vector3f ab=new Vector3f();
		ab.sub(b,a);
		Vector3f ac=new Vector3f();
		ac.sub(c,a);
		Vector3f temp=new Vector3f();
		temp.cross(ab,ac);
		return temp.length()/2;
	}
	
//	/**
//	 * 计算三角形的面积Sabc=1/2|AB*AC|
//	 * @param a
//	 * @param b
//	 * @param c
//	 * @return
//	 */
//	public static float getArea(Point2f a,Point2f b,Point2f c)
//	{
//		Vector2f ab=new Vector2f();
//		ab.sub(b,a);
//		Vector3f ac=new Vector3f();
//		ac.sub(c,a);
//		Vector2f temp=new Vector2f();
//		temp.cross(ab,ac);
//		return temp.length()/2;
//	}
	
	public static void main(String[] args){
		Point3f a=new Point3f(new float[]{0,0,0});
		Point3f b=new Point3f(new float[]{1,0,0});
		Point3f c=new Point3f(new float[]{0,2,0});
		Point3f p=new Point3f(new float[]{0,0.5f,0});
		linearInterpolate(a, b, c, p);
		System.out.println(p.z);
		System.out.println(getArea(a, b, c));
	}

}
