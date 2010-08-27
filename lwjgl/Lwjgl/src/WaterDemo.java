
import org.lwjgl.LWJGLException;
import org.lwjgl.input.Keyboard;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;
import org.lwjgl.opengl.GL11;
import org.lwjgl.util.glu.GLU;
import org.tinder.studio.lwjgl.util.Dome;
import org.tinder.studio.lwjgl.water.Water;

import de.bloodyblades.ms3dloader.Font;

/**
 * 
 * @author Micheal Hong 
 * @email babala_234@163.com
 * @version 2010-7-2 下午02:30:14
 *
 */
public class WaterDemo {
	private static ResourceLoader resourceLoader = new ResourceLoader();
	public static final int DISPLAY_WIDTH=300;
	public static final int DISPLAY_HEIGHT=200;
	
	private boolean runnable=true;
	private static final int FPS=50;
	
	private static Font font=null;
	
	private static Water water;
	
	public WaterDemo() throws LWJGLException{
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
		
		Display.create();
		Display.setTitle("BaseDemo");
		
		GL11.glViewport(0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT);
		
		//设置正交投影
		GL11.glMatrixMode(GL11.GL_PROJECTION);
    	GL11.glLoadIdentity();
//    	GL11.glOrtho(0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0,25);
    	GLU.gluPerspective(60.0f,(float)width/(float)height,0.1f,1000.0f);
    	GLU.gluLookAt(100, 100, 300,100,50,0, 0,1,0);
    	
    	GL11.glEnable(GL11.GL_BLEND);
    	GL11.glEnable(GL11.GL_ALPHA_TEST);
    	GL11.glAlphaFunc(GL11.GL_GREATER, 0);
    	
		font = new Font(resourceLoader.loadResourceAsStream("textures/font.bmp"), 16, width, height);
		water=new Water(new float[]{200,200}, new int[]{30,30});
		water.init();
		
		GL11.glMatrixMode(GL11.GL_MODELVIEW);
		GL11.glLoadIdentity();
	}
	
	private void run(){
		while (runnable) {
			long startTime = System.currentTimeMillis();
//			GL11.glClearColor(0,0,0,1);
//			GL11.glClear(GL11.GL_COLOR_BUFFER_BIT| GL11.GL_DEPTH_BUFFER_BIT);
			
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
//            GL11.glColor3f(0,1,0);
//            font.print("FPS:"+String.valueOf(costTime==0?FPS:1000/costTime),5,22,0);
            Display.update();
		}
		Keyboard.destroy();
		Display.destroy();
		System.out.println("Program stopped");
	}
	
	private void render() {
		GL11.glClearColor(0.5f,0.5f,0.5f, 1f);
		GL11.glClear(GL11.GL_COLOR_BUFFER_BIT|GL11.GL_DEPTH_BUFFER_BIT);

		water.render();
	}
	
	public static void main(String[] args) throws LWJGLException
	{
		new WaterDemo();
	}
}
