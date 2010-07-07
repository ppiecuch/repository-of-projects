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
	 * 三角形线性插值
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
		p.z=-(temp.x*p.x+temp.y*p.y/temp.z);
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
