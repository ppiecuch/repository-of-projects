
import java.io.IOException;

import org.lwjgl.LWJGLException;
import org.lwjgl.input.Keyboard;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;
import org.lwjgl.opengl.GL11;
import org.lwjgl.util.glu.GLU;
import org.tinder.studio.lwjgl.heightmap.HeightMap;
import org.tinder.studio.lwjgl.roam.Roam;
import org.tinder.studio.lwjgl.util.Util;

import de.bloodyblades.ms3dloader.Font;

/**
 * 
 * @author Micheal Hong 
 * @email babala_234@163.com
 * @version 2010-7-2 下午02:30:14
 *
 */
public class RoamDemo {
	private static ResourceLoader resourceLoader = new ResourceLoader();
	public static final int DISPLAY_WIDTH=300;
	public static final int DISPLAY_HEIGHT=200;
	
	private boolean runnable=true;
	private static final int FPS=40;
	
	private static Font font=null;
	private static Roam roam=null;
	
	public RoamDemo() throws LWJGLException{
		this.init();
		this.run();
	}

	private void init() throws LWJGLException
	{
		Display.setDisplayMode(new DisplayMode(DISPLAY_WIDTH, DISPLAY_HEIGHT));
		
		/*居中*/
		int width=Display.getDesktopDisplayMode().getWidth();
		int height=Display.getDesktopDisplayMode().getHeight();
		int x=(width-DISPLAY_WIDTH)/2;
		int y=(height-DISPLAY_HEIGHT)/2;
		Display.setLocation(x,y);
		
		/*全屏*/
//		Display.setFullscreen(true);
		try {
			HeightMap heightMap = new HeightMap(new float[]{1,1,1},new int[]{1024,1024}, resourceLoader.loadResourceAsStream("textures/Terrain.png"));
			roam=new Roam(heightMap.getHeightWeights(), null, 0);
			roam.init();
			roam.setViewPosition(new float[]{270,0,270});
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		
		Display.create();
		Display.setTitle("BaseDemo");
		
		GL11.glViewport(0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT);
		
		//设置正交投影
		GL11.glMatrixMode(GL11.GL_PROJECTION);
    	GL11.glLoadIdentity();
//    	GL11.glOrtho(0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0,25);
    	GLU.gluPerspective(60.0f,(float)width/(float)height,0.1f,4000.0f);
    	GLU.gluLookAt(500, 1050, 3000,500,0,-100, 0,1,0);
    	
    	GL11.glEnable(GL11.GL_BLEND);
    	GL11.glEnable(GL11.GL_ALPHA_TEST);
    	GL11.glAlphaFunc(GL11.GL_GREATER, 0);
    	
		font = new Font(resourceLoader.loadResourceAsStream("textures/font.bmp"), 16, width, height);
		
		GL11.glMatrixMode(GL11.GL_MODELVIEW);
		GL11.glLoadIdentity();
	}
	
	private void run(){
		while (runnable) {
			long startTime = System.currentTimeMillis();
			GL11.glClearColor(0,0,0,1);
			GL11.glClear(GL11.GL_COLOR_BUFFER_BIT| GL11.GL_DEPTH_BUFFER_BIT);
			
			render();
			
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
//            font.print("FPS:"+String.valueOf(costTime==0?FPS:1000/costTime),5,22,0);
//            System.out.println("FPS:"+String.valueOf(costTime==0?FPS:1000/costTime));
            Display.update();
		}
		Keyboard.destroy();
		Display.destroy();
		System.out.println("Program stopped");
	}
	private float angle=0;
	private void render() {
		GL11.glClearColor(0.5f,0.5f,0.5f, 1f);
		GL11.glClear(GL11.GL_COLOR_BUFFER_BIT|GL11.GL_DEPTH_BUFFER_BIT);

//		GL11.glBegin(GL11.GL_TRIANGLES);
//		for(int i=0;i<3;i++)
//		{
//			GL11.glNormal3f(normals[i*3], normals[i*3+1], normals[i*3+2]);
//			GL11.glVertex3f(vertices[i*3],vertices[i*3+1],vertices[i*3+2]);
//		}
//		GL11.glEnd();

//		GL11.glEnableClientState(GL11.GL_VERTEX_ARRAY);
//		GL11.glEnableClientState(GL11.GL_NORMAL_ARRAY);
//		GL11.glEnableClientState(GL11.GL_TEXTURE_COORD_ARRAY);
//		GL11.glEnable(GL11.GL_TEXTURE_2D);
//		GL11.glDrawArrays(GL11.GL_TRIANGLES, 0, 3);
//		GL11.glDisable(GL11.GL_TEXTURE_2D);
//		GL11.glDisableClientState(GL11.GL_VERTEX_ARRAY);
//		GL11.glDisableClientState(GL11.GL_NORMAL_ARRAY);
//		GL11.glDisableClientState(GL11.GL_TEXTURE_COORD_ARRAY);
		angle+=2;
		if(angle==360)angle=0;
		roam.setgClipAngle(angle);
		roam.reset();
		roam.tessellate();
		GL11.glLoadIdentity();
		GL11.glScaled(6,1,6);
		roam.render();
//		System.out.println(roam.getgNumTrisRendered());
//		roam.drawFrustum();
	}
	
	public static void main(String[] args) throws LWJGLException
	{
		new RoamDemo();
	}
}
