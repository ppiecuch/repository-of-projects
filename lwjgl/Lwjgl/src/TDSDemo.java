

import glapp.GLApp;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;

import org.lwjgl.BufferUtils;
import org.lwjgl.input.Keyboard;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;
import org.lwjgl.opengl.GL11;
import org.lwjgl.util.glu.GLU;

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
		 	.0f, 0.0f, -20.0f,
			 20.0f, 0.0f, -20.0f,
			 0.0f, 20.0f, -20.0f
			 }; 
	static int[] normals ={
		0,0,1,
		0,0,1,
		0,0,1
	};
	static float[] texcoords={
		0,0,
		0,1,
		1,1
	};
	
	private static int textureId;
	
	private static FloatBuffer vertexBuffer;
	private static IntBuffer normalBuffer;
	private static FloatBuffer texcoordBuffer;
	
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
		
		GL11.glViewport(0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT);
		
		//设置正交投影
		GL11.glMatrixMode(GL11.GL_PROJECTION);
    	GL11.glLoadIdentity();
//    	GL11.glOrtho(0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0,25);
    	GLU.gluPerspective(90.0f,(float)width/(float)height,0.1f,100.0f);
    	
    	GL11.glEnable(GL11.GL_BLEND);
    	GL11.glEnable(GL11.GL_ALPHA_TEST);
    	GL11.glAlphaFunc(GL11.GL_GREATER, 0);
    	
//    	GL11.glShadeModel(GL11.GL_SMOOTH);
//    	initRendering();
    	
    	textureId=GLApp.makeTexture(TDSDemo.class.getResource(".").getPath()+"textures/sod.jpg");
    	
	    vertexBuffer=BufferUtils.createFloatBuffer(vertices.length);
		vertexBuffer.put(vertices);
		vertexBuffer.flip();
		
		normalBuffer=BufferUtils.createIntBuffer(normals.length);
		normalBuffer.put(normals);
		normalBuffer.flip();
		
		texcoordBuffer=BufferUtils.createFloatBuffer(texcoords.length);
		texcoordBuffer.put(texcoords);
		texcoordBuffer.flip();
		
		GL11.glMatrixMode(GL11.GL_MODELVIEW);
		GL11.glLoadIdentity();
		GL11.glBindTexture(GL11.GL_TEXTURE_2D, textureId);
		
		GL11.glVertexPointer(3,0,vertexBuffer);
		GL11.glNormalPointer(0,normalBuffer);
		GL11.glTexCoordPointer(2,0,texcoordBuffer);
		
		
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
	
	// Initialize OpenGL's rendering modes   
	static void initRendering()   
	{   
	  
	 GL11.glEnable ( GL11.GL_DEPTH_TEST );   
	  
	 // Uncomment out the first block of code below, and then the second block,   
	 //  to see how they affect line and point drawing.   
	  
	 // The following commands should cause points and line to be drawn larger   
	 // than a single pixel width.   
	 GL11.glPointSize(8);   
	 GL11.glLineWidth(5);   
	  
	 // The following commands should induce OpenGL to create round points and    
	 // antialias points and lines.  (This is implementation dependent unfortunately).   
	 //RGBA mode antialias need cooperate with blend function.   
	 GL11.glEnable(GL11.GL_POINT_SMOOTH);
	 GL11.glEnable(GL11.GL_LINE_SMOOTH);   
	 GL11.glHint(GL11.GL_POINT_SMOOTH_HINT, GL11.GL_NICEST); // Make round points, not square points   
	 GL11.glHint(GL11.GL_LINE_SMOOTH_HINT, GL11.GL_NICEST);  // Antialias the lines   
	 GL11.glEnable(GL11.GL_BLEND);   
	 GL11.glBlendFunc(GL11.GL_SRC_ALPHA, GL11.GL_ONE_MINUS_SRC_ALPHA);   
	}  
	
	

	private static void render() {
		GL11.glClearColor(COLOR, COLOR, COLOR, 1f);
		GL11.glClear(GL11.GL_COLOR_BUFFER_BIT|GL11.GL_DEPTH_BUFFER_BIT);

//		GL11.glBegin(GL11.GL_TRIANGLES);
//		for(int i=0;i<3;i++)
//		{
//			GL11.glNormal3f(normals[i*3], normals[i*3+1], normals[i*3+2]);
//			GL11.glVertex3f(vertices[i*3],vertices[i*3+1],vertices[i*3+2]);
//		}
//		GL11.glEnd();

		GL11.glEnableClientState(GL11.GL_VERTEX_ARRAY);
		GL11.glEnableClientState(GL11.GL_NORMAL_ARRAY);
		GL11.glEnableClientState(GL11.GL_TEXTURE_COORD_ARRAY);
		GL11.glEnable(GL11.GL_TEXTURE_2D);
		GL11.glDrawArrays(GL11.GL_TRIANGLES, 0, 3);
		GL11.glDisable(GL11.GL_TEXTURE_2D);
		GL11.glDisableClientState(GL11.GL_VERTEX_ARRAY);
		GL11.glDisableClientState(GL11.GL_NORMAL_ARRAY);
		GL11.glDisableClientState(GL11.GL_TEXTURE_COORD_ARRAY);
	}

}
