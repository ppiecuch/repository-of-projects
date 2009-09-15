package beta01;


public interface Mode {
	
	public static final int F_12=123;
	public static final int NUM_1=97;
	public static final int NUM_2=98;
	
	public void start();
	public void doPress(int keyNum);
	public void doReleased(int keyNum);
	
	public void doLeftPressed(int x,int y);
	public void doLeftReleased(int x,int y);
	public void doRightPressed(int x,int y);
	public void doRightReleased(int x,int y);
	public void doMiddlePressed(int x,int y);
	public void doMiddleReleased(int x,int y);

}
