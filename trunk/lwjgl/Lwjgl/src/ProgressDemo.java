

import java.util.Date;

import glapp.GLApp;

import org.lwjgl.input.Keyboard;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;
import org.lwjgl.opengl.GL11;


/**
 * 
 * @author Micheal Hong 
 * @email babala_234@163.com
 * @version 2010-6-30 上午10:27:49
 *
 */
public class ProgressDemo {
	
	public static final int DISPLAY_WIDTH=640;
	public static final int DISPLAY_HEIGHT=480;
	
	private static float COLOR=0.5f;
	
	private static int textureId;
	
	/**
	 * @param args
	 * @throws Exception
	 */
	public static void main(String[] args) throws Exception {
		
		DisplayMode[] displayModes = org.lwjgl.util.Display
				.getAvailableDisplayModes(DISPLAY_WIDTH,DISPLAY_HEIGHT, -1, -1, -1, -1, 60, 60);
		
		for(DisplayMode dm:displayModes)
		{
			System.out.println(dm.getWidth()+","+dm.getHeight()+","+dm.getFrequency());
		}

		org.lwjgl.util.Display.setDisplayMode(displayModes, new String[] {
				"width=" + DISPLAY_WIDTH,
				"height=" + DISPLAY_HEIGHT,
				"freq=" + 60,
				"bpp="
						+ org.lwjgl.opengl.Display.getDisplayMode()
								.getBitsPerPixel() });
		
		/*居中*/
		int width=Display.getDesktopDisplayMode().getWidth();
		int height=Display.getDesktopDisplayMode().getHeight();
//		System.out.println("width:"+width+",height:"+height);
		int x=(width-DISPLAY_WIDTH)/2;
		int y=(height-DISPLAY_HEIGHT)/2;
		Display.setLocation(x,y);
		
		
		/*全屏*/
//		Display.setFullscreen(true);
		
		Display.create();
		Display.setTitle("ProgressDemo");
		
		//设置正交投影
		GL11.glMatrixMode(GL11.GL_PROJECTION);
    	GL11.glLoadIdentity();
    	GL11.glOrtho(0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0,25);
    	
    	//GL11.glEnable(GL11.GL_LIGHTING);
    	GL11.glEnable(GL11.GL_BLEND);
//    	GL11.glEnable(GL11.GL_DEPTH_TEST);
//    	GL11.glDepthFunc(GL11.GL_ALWAYS);
    	GL11.glEnable(GL11.GL_ALPHA_TEST);
    	GL11.glAlphaFunc(GL11.GL_GREATER, 0);
    	GL11.glEnable(GL11.GL_TEXTURE_2D);
    	
    	
    	textureId=GLApp.makeTexture(ProgressDemo.class.getResource(".").getPath()+"progress.png");
    	
    	GL11.glViewport(0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT);
    	
    	
    	boolean running=true;
		int count=128;
		while (running && count-- > 0) {
			render();
			Display.update();
			try {
				Thread.sleep(100);
			} catch (Exception e) {
				e.printStackTrace();
			}
			
			while (Keyboard.next()) {
				// we only want key down events
				if (!Keyboard.getEventKeyState()) {
					continue;
				}
				// check for exit
				if (Keyboard.getEventKey() == Keyboard.KEY_ESCAPE) {
					System.out.println("you press ESC");
					running = false;

				}
			}
		}

	}
	static long lastTime=0;
	static int progress=0;
	static int length=500;
	private static void render() {
		GL11.glClearColor(COLOR, COLOR, COLOR, 1f);
		GL11.glClear(GL11.GL_COLOR_BUFFER_BIT);
		
		GL11.glMatrixMode(GL11.GL_MODELVIEW);
		GL11.glBindTexture(GL11.GL_TEXTURE_2D, textureId);
		GL11.glEnable(GL11.GL_TEXTURE_2D);
    	GL11.glBegin(GL11.GL_QUADS);
    	GL11.glTexCoord2f(0,1);
    	GL11.glVertex2f(1,50);
    	GL11.glTexCoord2f((float)progress/100,1);
    	GL11.glVertex2f(length*progress/100,50);
    	GL11.glTexCoord2f((float)progress/100,0);
    	GL11.glVertex2f(length*progress/100,65);
    	GL11.glTexCoord2f(0,0);
    	GL11.glVertex2f(1,65);
    	GL11.glEnd();
    	GL11.glDisable(GL11.GL_TEXTURE_2D);
    	long currentTime=new Date().getTime();
    	if(currentTime-lastTime>200)
    	{
    		progress++;
    		lastTime=currentTime;
    	}
    	if(progress==100)
    		progress=0;
	}

}
