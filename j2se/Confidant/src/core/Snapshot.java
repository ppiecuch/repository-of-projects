package core;

import java.awt.AWTException;
import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.image.BufferedImage;

/**
 * ÆÁÄ»½ØÆÁ
 * @author Micheal hong
 *
 */
public class Snapshot {
	
	public static Robot robot;
	
	/**
	 * »ñÈ¡ÆÁÄ»Ä³ÏñËØµÄRGBÖµ
	 * @param x
	 * @param y
	 * @return
	 * @throws AWTException
	 */
	public static int getScreenPixelColor(int x,int y) throws AWTException
	{
		BufferedImage screenshot = getRobot().createScreenCapture(new Rectangle(x,y,1,1)); 
		return screenshot.getRGB(0,0);
	}
	
	public static Robot getRobot() throws AWTException
	{
		if(Snapshot.robot==null)
			Snapshot.robot=new Robot();
		return robot;
	}
	
	public static final void main(String[] args) throws AWTException
	{
		System.out.println(Integer.toHexString(getScreenPixelColor(900, 900)));
	}

}
