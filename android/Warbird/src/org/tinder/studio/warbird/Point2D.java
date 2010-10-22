package org.tinder.studio.warbird;

public class Point2D {
	
	public Point2D() {}
	
	public Point2D(double x,double y){
		this.x=x;
		this.y=y;
	}
	
	public double x,y;
	
	@Override
	public String toString() {
		return "[x:"+x+",y:"+y+"]";
	}

}
