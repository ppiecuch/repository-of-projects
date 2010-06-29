import java.awt.Color;
import java.awt.FontFormatException;
import java.io.IOException;

import org.lwjgl.input.Keyboard;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;
import org.lwjgl.opengl.GL11;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.UnicodeFont;


public class FontDemo {
	
	public static final int DISPLAY_WIDTH=640;
	public static final int DISPLAY_HEIGHT=480;
	
	private static float COLOR=0.5f;
	
	/**
	 * @param args
	 * @throws Exception
	 */
	public static void main(String[] args) throws Exception {
		
		/*
		 * [org.lwjgl.util.Display]
		 * 
		 * Display initialization utility, that can be used to find display
		 * modes and pick one for you based on your criteria.
		 * 
		 * getAvailableDisplayModes: 
		 * Determine the available display modes that
		 * match the specified minimum and maximum criteria. If any given
		 * criterium is specified as -1 then it is ignored.
		 * 
		 * setDisplayMode:
		 * Create the display by choosing from a list of display modes based on
		 * an order of preference. You must supply a list of allowable display
		 * modes, probably by calling getAvailableDisplayModes(), and an array
		 * with the order in which you would like them sorted in descending
		 * order. This method attempts to create the topmost display mode; if
		 * that fails, it will try the next one, and so on, until there are no
		 * modes left. If no mode is set at the end, an exception is thrown.
		 */
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
		/*
		 * getDesktopDisplayMode():
		 * Return the initial desktop display mode.
		 */
		int width=Display.getDesktopDisplayMode().getWidth();
		int height=Display.getDesktopDisplayMode().getHeight();
//		System.out.println("width:"+width+",height:"+height);
		int x=(width-DISPLAY_WIDTH)/2;
		int y=(height-DISPLAY_HEIGHT)/2;
		Display.setLocation(x,y);
		
		
		/*全屏*/
//		Display.setFullscreen(true);
		
		/*
		 * create():
		 * Create the OpenGL context. If isFullscreen() is true or if windowed
		 * context are not supported on the platform, the display mode will be switched to the mode returned by
		 * getDisplayMode(), and a fullscreen context will be created. If isFullscreen() is false, a windowed context
		 * will be created with the dimensions given in the mode returned by getDisplayMode(). If a context can't be
		 * created with the given parameters, a LWJGLException will be thrown.
		 * <p/>
		 * <p>The window created will be set up in orthographic 2D projection, with 1:1 pixel ratio with GL coordinates.
		 */
		Display.create();
		Display.setTitle("FontDemo");
		
		//设置正交投影
		GL11.glMatrixMode(GL11.GL_PROJECTION);
    	GL11.glLoadIdentity();
    	GL11.glOrtho(0, 640, 480, 0, 0,25);

    	//GL11.glEnable(GL11.GL_LIGHTING);
    	GL11.glEnable(GL11.GL_BLEND);
//    	GL11.glEnable(GL11.GL_DEPTH_TEST);
//    	GL11.glDepthFunc(GL11.GL_ALWAYS);
    	GL11.glEnable(GL11.GL_ALPHA_TEST);
    	GL11.glAlphaFunc(GL11.GL_GREATER, 0);
    	GL11.glEnable(GL11.GL_TEXTURE_2D);
    	
    	GL11.glViewport(0,0,640,480);
    	
    	
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
			
			/*
			 * note:
			 * The display must first have been created. The
			 * reason for this is so the keyboard has a window to "focus" in.
			 * 
			 * next():
			 * Gets the next keyboard event. You can query which key caused the event by using
			 * <code>getEventKey</code>. To get the state of that key, for that event, use
			 * <code>getEventKeyState</code> - finally use <code>getEventCharacter</code> to get the
			 * character for that event.
			 */
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
	static UnicodeFont tfont=null;
	private static void render() {
		GL11.glClearColor(COLOR, COLOR, COLOR, 1f);
		GL11.glClear(GL11.GL_COLOR_BUFFER_BIT);
		
		try {
//			UnicodeFont tfont=FontLoader.loadFontByFile("MSYH.TTF",20,Color.MAGENTA);
			if(tfont==null)
			{
				tfont=FontLoader.loadFontByName("宋体",20,Color.MAGENTA);
			}
			tfont.drawString(100,100,"我1234567890");
		} catch (SlickException e) {
			e.printStackTrace();
		} catch (FontFormatException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} 
	}

}
