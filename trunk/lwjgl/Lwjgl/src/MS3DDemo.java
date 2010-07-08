import java.util.Date;

import org.lwjgl.LWJGLException;
import org.lwjgl.Sys;
import org.lwjgl.input.Keyboard;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;
import org.lwjgl.opengl.GL11;
import org.lwjgl.util.glu.GLU;
import org.tinder.studio.lwjgl.ms3d.MS3DAnimation;
import org.tinder.studio.lwjgl.ms3d.MS3DModel;

import de.bloodyblades.ms3dloader.Font;

/**
 *
 * @Author Micheal Hong
 * @Email babala_234@163.com
 * @Version 2010-7-8下午09:31:22
 *
 */
public class MS3DDemo {
	private static ResourceLoader resourceLoader = new ResourceLoader();
	public static final int DISPLAY_WIDTH=800;
	public static final int DISPLAY_HEIGHT=600;
	
	private boolean runnable=true;
	private static final int FPS=50;
	
	private static Font font=null;
	
	MS3DModel g36c;
	private float[] position={0,0,0};
	private int state;
	private static final int INCREMENT=1;
	
	public MS3DDemo() throws LWJGLException{
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
		Display.setTitle("MS3DDemo");
		
		GL11.glViewport(0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT);
		
		//设置正交投影
		GL11.glMatrixMode(GL11.GL_PROJECTION);
    	GL11.glLoadIdentity();
//    	GL11.glOrtho(0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0,25);
    	GLU.gluPerspective(60.0f,(float)width/(float)height,0.1f,1000.0f);
    	
    	GL11.glEnable(GL11.GL_BLEND);
    	GL11.glEnable(GL11.GL_ALPHA_TEST);
    	GL11.glAlphaFunc(GL11.GL_GREATER, 0);
    	GL11.glClearDepth(1.0f);
    	GL11.glEnable(GL11.GL_DEPTH_TEST);
		GL11.glDepthFunc(GL11.GL_LEQUAL);
		GL11.glHint(GL11.GL_PERSPECTIVE_CORRECTION_HINT, GL11.GL_NICEST);
    	
		font = new Font(resourceLoader.loadResourceAsStream("textures/font.bmp"), 25, width, height);
		g36c = new MS3DModel(resourceLoader.loadResourceAsStream("models/assassin.ms3d"),this.getClass().getResource("./textures").getPath());
		MS3DAnimation walk=new MS3DAnimation(0,13);
		MS3DAnimation leftMove=new MS3DAnimation(15,30);
		g36c.addAnimation(walk);
		g36c.addAnimation(leftMove);
		
		GL11.glMatrixMode(GL11.GL_MODELVIEW);
	}
	
	private void run(){
		while (runnable) {
			long startTime = System.currentTimeMillis();
			GL11.glClearColor(0,0,0,1);
			GL11.glClear(GL11.GL_COLOR_BUFFER_BIT| GL11.GL_DEPTH_BUFFER_BIT);
			
			checkInput();
			
			logic();
			
			render();
			
			
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
	
	private void checkInput(){
		
		if (Keyboard.next()) {
			//if is pressing event
			if(Keyboard.getEventKeyState())
			{
				switch(Keyboard.getEventKey())
				{
				case Keyboard.KEY_ESCAPE:
					runnable=false;
					break;
				case Keyboard.KEY_W:
					state|=1;
					g36c.setCurrentAnimation(0);
					break;
				case Keyboard.KEY_S:
					state|=2;
					g36c.setCurrentAnimation(0);
					break;
				case Keyboard.KEY_A:
					state|=4;
					g36c.setCurrentAnimation(1);
					break;
				case Keyboard.KEY_D:
					state|=8;
					g36c.setCurrentAnimation(1);
					break;
				}
			}
			//if is releasing event
			else
			{
				switch(Keyboard.getEventKey())
				{
				case Keyboard.KEY_W:
					state&=~1;
					break;
				case Keyboard.KEY_S:
					state&=~2;
					break;
				case Keyboard.KEY_A:
					state&=~4;
					break;
				case Keyboard.KEY_D:
					state&=~8;
					break;
				}
			}
		}
		
		if(Display.isCloseRequested())
		{
			runnable=false;
		}
	}
	
	private void logic(){
		if((state&1)>0)
		{
			position[2]-=INCREMENT;
		}
		else if((state&2)>0)
		{
			position[2]+=INCREMENT;
		}
		else if((state&4)>0)
		{
			position[0]-=INCREMENT;
		}
		else if((state&8)>0)
		{
			position[0]+=INCREMENT;
		}
	}
	
	private void render() {
		
		GL11.glLoadIdentity();
		
		GL11.glTranslatef(0.0f+position[0], -40.0f, -150.0f+position[2]);
		GL11.glScaled(6.0f,6.0f,6.0f);
		GL11.glColor3f(1,1,1);
		
		g36c.updateModel(System.currentTimeMillis()*2/3);
	}
	
	public static void main(String[] args) throws LWJGLException
	{
		new MS3DDemo();
	}
}
