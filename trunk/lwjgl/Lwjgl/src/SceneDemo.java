import glapp.GLApp;

import java.awt.Color;
import java.awt.FontFormatException;
import java.io.IOException;

import org.lwjgl.LWJGLException;
import org.lwjgl.Sys;
import org.lwjgl.input.Keyboard;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;
import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL12;
import org.lwjgl.util.glu.GLU;
import org.newdawn.slick.SlickException;
import org.tinder.studio.lwjgl.heightmap.HeightMap;

import de.bloodyblades.ms3dloader.Font;

/**
 *
 * @Author Micheal Hong
 * @Email babala_234@163.com
 * @Version 2010-7-1ÏÂÎç07:45:35
 *
 */
public class SceneDemo{
	
	ResourceLoader resourceLoader = new ResourceLoader();
	
	private static final int WIDTH=800;
	private static final int HEIGHT=600;
	
	private boolean runnable=true;
	private static final int FPS=50;
	
	private static Font font=null;
	private static HeightMap heightMap=null;
	private static int textureId;
	
	public SceneDemo(){
		this.init();
		this.run();
	}
	
	private void init()
	{
		try {
			Display.setDisplayMode(new DisplayMode(WIDTH,HEIGHT));
//			Display.setVSyncEnabled(true);
			Display.setTitle("SceneDemo");
			Display.create();

			Keyboard.create();

		} catch (LWJGLException e) {
			Sys.alert("Error", "Initialization failed!\n\n" + e.getMessage());
			System.exit(0);
		}
		
		int width = Display.getDisplayMode().getWidth();
		int height = Display.getDisplayMode().getHeight();
		
		font = new Font(resourceLoader.loadResourceAsStream("textures/font.bmp"), 16, width, height);
		try {
			heightMap=new HeightMap(30, 30,resourceLoader.loadResourceAsStream("textures/heightmap.png"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		initGL();
		textureId=GLApp.makeTexture(SceneDemo.class.getResource(".").getPath()+"textures/sod.jpg");
		
//		GL11.glViewport(0, 0, width, height); // Reset The Current Viewport
//		GL11.glMatrixMode(GL11.GL_PROJECTION); // Select The Projection Matrix
//		GL11.glLoadIdentity(); // Reset The Projection Matrix
//		GLU.gluPerspective(60.0f, ((float) width / (float) height), 0.1f, 1000.0f); // Calculate The Aspect Ratio Of The Window
//		GL11.glOrtho(0, width, height, 0, 0,25);
//		GLU.gluLookAt(0f, 0f, 10f, 0f, 0f, 0f, 0f, 1f, 0f);
		
		GL11.glEnable(GL11.GL_BLEND);
		GL11.glEnable(GL11.GL_ALPHA_TEST);
    	GL11.glAlphaFunc(GL11.GL_GREATER, 0);
//    	GL11.glEnable(GL11.GL_TEXTURE_2D);
	}
	int angle=0;
	public void run() {
		while (runnable) {
			long startTime = System.currentTimeMillis();
			GL11.glClearColor(0,0,0,1);
			GL11.glClear(GL11.GL_COLOR_BUFFER_BIT| GL11.GL_DEPTH_BUFFER_BIT);
			
			
			GL11.glMatrixMode(GL11.GL_MODELVIEW);
			GL11.glLoadIdentity();
//			angle+=2;
//			GL11.glRotatef(angle, 0,1,0);
//			GL11.glTranslated(0,-55,0);
			GL11.glBindTexture(GL11.GL_TEXTURE_2D, textureId);
			GL11.glEnable(GL11.GL_TEXTURE_2D);
			
			GL11.glBegin(GL11.GL_TRIANGLES);
//			GL11.glBegin(GL11.GL_QUADS);
//			int[][][][] vertexs=heightMap.getVertexs();
//			for(int i=0;i<vertexs.length;i++)
//			{
//				for(int j=0;j<vertexs[i].length;j++)
//				{
//					for(int k=0;k<vertexs[i][j].length-1;k++)
//					{
//						GL11.glVertex3f(vertexs[i][j][k][0],vertexs[i][j][k][2],vertexs[i][j][k][1]);
//					}
//				}
//			}
			GL11.glVertex3f( 0.0f, 1.0f, 0.0f);         // Top
            GL11.glVertex3f(-1.0f,-1.0f, 0.0f);         // Bottom Left
            GL11.glVertex3f( 1.0f,-1.0f, 0.0f);         // Bottom Right
			GL11.glEnd();
			GL11.glDisable(GL11.GL_TEXTURE_2D);
			
			if (Keyboard.next() && Keyboard.getEventKeyState()&&Keyboard.getEventKey()==Keyboard.KEY_ESCAPE||Display.isCloseRequested()) {
					runnable=false;
					break;
			}
            long costTime = System.currentTimeMillis() - startTime;
            if(costTime<1000/FPS)
            {
                try{
                  Thread.sleep(1000/FPS-costTime);
                }
                catch(Exception e){
                   e.printStackTrace();
                }
            }
            GL11.glColor3f(0,1,0);
            font.print("FPS:"+String.valueOf(costTime==0?FPS:1000/costTime),5,22,0);
            Display.update();
		}
		Keyboard.destroy();
		Display.destroy();
		System.out.println("Program stopped");
		
	}
	
	 public void initGL() {
	        try {
	            // Depth test setup
	            GL11.glEnable(GL11.GL_DEPTH_TEST); // Enables Depth Testing
	            GL11.glDepthFunc(GL11.GL_LEQUAL);  // The Type Of Depth Testing To Do

	            // Some basic settings
	            GL11.glClearColor(0f, 0f, 0f, 1f); // Black Background
	            GL11.glEnable(GL11.GL_NORMALIZE);  // force normal lengths to 1
	            GL11.glEnable(GL11.GL_CULL_FACE);  // don't render hidden faces
	        	GL11.glEnable(GL11.GL_TEXTURE_2D); // use textures
	            GL11.glEnable(GL11.GL_BLEND);      // enable transparency

	            // How to handle transparency: average colors together
	            GL11.glBlendFunc(GL11.GL_SRC_ALPHA, GL11.GL_ONE_MINUS_SRC_ALPHA);

	            // Enable alpha test so the transparent backgrounds in texture images don't draw.
	            // This prevents transparent areas from affecting the depth or stencil buffer.
	            // alpha func will accept only fragments with alpha greater than 0
	            GL11.glEnable(GL11.GL_ALPHA_TEST);
	            GL11.glAlphaFunc(GL11.GL_GREATER, 0f);

	            // Draw specular highlghts on top of textures
	            GL11.glLightModeli(GL12.GL_LIGHT_MODEL_COLOR_CONTROL, GL12.GL_SEPARATE_SPECULAR_COLOR );

	            // Perspective quality
	            GL11.glHint(GL11.GL_PERSPECTIVE_CORRECTION_HINT, GL11.GL_NICEST);

	            // Set the size and shape of the screen area
	            int width = Display.getDisplayMode().getWidth();
	    		int height = Display.getDisplayMode().getHeight();
	            GL11.glViewport(0, 0, width, height);

	            // setup perspective (see setOrtho() for 2D)
	            setPerspective();

	            // select model view for subsequent transformations
	            GL11.glMatrixMode(GL11.GL_MODELVIEW);
	            GL11.glLoadIdentity();
	        }
	        catch (Exception e) {
	           e.printStackTrace();
	        }
	    }
	 
	 /**
	     * Set OpenGL to render in 3D perspective.  Set the projection matrix using
	     * GLU.gluPerspective().  The projection matrix controls how the scene is
	     * "projected" onto the screen.  Think of it as the lens on a camera, which
	     * defines how wide the field of vision is, how deep the scene is, and how
	     * what the aspect ratio will be.
	     */
	    public static void setPerspective()
	    {
	        // select projection matrix (controls perspective)
	        GL11.glMatrixMode(GL11.GL_PROJECTION);
	        GL11.glLoadIdentity();
	        int width = Display.getDisplayMode().getWidth();
			int height = Display.getDisplayMode().getHeight();
	        GLU.gluPerspective(40f, ((float) width / (float) height), 1f, 1000f);
	        // return to modelview matrix
	        GL11.glMatrixMode(GL11.GL_MODELVIEW);
	    }
	
	public static void main(String[] args)
	{
		new SceneDemo();
	}

}
