

import java.nio.FloatBuffer;

import org.lwjgl.BufferUtils;
import org.lwjgl.input.Keyboard;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;
import org.lwjgl.opengl.GL11;

/**
 * 
 * @author Micheal Hong 
 * @email babala_234@163.com
 * @version 2010-7-1 上午09:30:00
 *
 */
public class TDSDemo {

	public static final int DISPLAY_WIDTH=300;
	public static final int DISPLAY_HEIGHT=200;
	
	private static float COLOR=0.5f;
	
	static float[] vertices = { 
		 	.0f, 0.0f, 0.0f,
			 1.0f, 0.0f, 0.0f,
			 0.0f, 1.0f, 0.0f
			 }; 
	
	private static FloatBuffer vertexBuffer;
	
	/**
	 * @param args
	 * @throws Exception
	 */
	public static void main(String[] args) throws Exception {
		Display.setDisplayMode(new DisplayMode(DISPLAY_WIDTH, DISPLAY_HEIGHT));
		
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
		Display.setTitle("TDSDemo");
		
		//设置正交投影
		GL11.glMatrixMode(GL11.GL_PROJECTION);
    	GL11.glLoadIdentity();
    	GL11.glOrtho(0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0,25);
    	
    	GL11.glEnable(GL11.GL_BLEND);
    	GL11.glEnable(GL11.GL_ALPHA_TEST);
    	GL11.glAlphaFunc(GL11.GL_GREATER, 0);
    	
    	GL11.glViewport(0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT);
    	
    	
	    vertexBuffer=BufferUtils.createFloatBuffer(vertices.length);
		vertexBuffer.put(vertices);
		
    	boolean running=true;
		while (running) {
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
	
	

	private static void render() {
		GL11.glClearColor(COLOR, COLOR, COLOR, 1f);
		GL11.glClear(GL11.GL_COLOR_BUFFER_BIT|GL11.GL_DEPTH_BUFFER_BIT);
		
		
		GL11.glMatrixMode(GL11.GL_MODELVIEW);
		GL11.glLoadIdentity();
		GL11.glVertexPointer(3,GL11.GL_FLOAT,vertexBuffer);
		GL11.glEnableClientState(GL11.GL_VERTEX_ARRAY);
		GL11.glDrawArrays(GL11.GL_TRIANGLES, 0, 3);
		GL11.glDisableClientState(GL11.GL_VERTEX_ARRAY);

	}

}
