package beta04;

import java.awt.Point;

public class Rect {

	public int topLeftX;
	public int topLeftY;
	public int bottomRightX;
	public int bottomRightY;
	
	public Rect() {
	}
	
	/*public Rect(int x,int y,int width,int height) {
		topLeftX=x;
		topLeftY=y;
		bottomRightX=x+width;
		bottomRightY=y+height;
	}*/
	public Rect(int tlx,int tly,int brx,int bry) {
		topLeftX=tlx;
		topLeftY=tly;
		bottomRightX=brx;
		bottomRightY=bry;
	}
	
	public int getWidth(){
		return bottomRightX-topLeftX;
	}
	
	public int getHeight(){
		return bottomRightY-topLeftY;
	}
	
	public Point getCenter(){
		return new Point((topLeftX+bottomRightX)/2, (topLeftY+bottomRightY)/2);
	}
	
	@Override
	public String toString() {
		return "{"+topLeftX+","+topLeftY+","+bottomRightX+","+bottomRightY+"}";
	}
}
