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
import org.tinder.studio.lwjgl.ms3d.MS3DModel;
import org.tinder.studio.lwjgl.util.FPSRecorder;
import org.tinder.studio.lwjgl.util.Point3f;

import de.bloodyblades.ms3dloader.Font;

/**
 *
 * @Author Micheal Hong
 * @Email babala_234@163.com
 * @Version 2010-7-1下午07:45:35
 *
 */
public class SceneDemo{
	
	private static ResourceLoader resourceLoader = new ResourceLoader();
	public static final int DISPLAY_WIDTH=800;
	public static final int DISPLAY_HEIGHT=600;
	
	private boolean runnable=true;
	private static final int FPS=40;
	
	private static Font font=null;
	private int textureId;
	
	private MS3DModel g36c;
	
	private HeightMap heightMap;
	private Point3f[][] strips;
	private Point3f[][][] lines;
	
	private int width;
	private int height;
	
	private float[] position={0,0,0};
	
	public SceneDemo() throws LWJGLException, IOException{
		this.init();
		this.run();
	}

	private void init() throws LWJGLException, IOException
	{
		Display.setDisplayMode(new DisplayMode(DISPLAY_WIDTH, DISPLAY_HEIGHT));
		
		/*居中*/
		width=Display.getDesktopDisplayMode().getWidth();
		height=Display.getDesktopDisplayMode().getHeight();
		int x=(width-DISPLAY_WIDTH)/2;
		int y=(height-DISPLAY_HEIGHT)/2;
		Display.setLocation(x,y);
		
		/*全屏*/
//		Display.setFullscreen(true);
		
		Display.create();
		Display.setTitle("SceneDemo");
		
		GL11.glViewport(0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT);
		
		//设置正交投影
//		GL11.glMatrixMode(GL11.GL_PROJECTION);
//    	GL11.glLoadIdentity();
//    	
////    	GL11.glOrtho(0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0,25);
//    	GLU.gluPerspective(90.0f,(float)width/(float)height,0.1f,1000.0f);
//    	GL11.glTranslated(position[0],0, position[2]-500);
    	
    	GL11.glEnable(GL11.GL_BLEND);
    	GL11.glEnable(GL11.GL_ALPHA_TEST);
    	GL11.glAlphaFunc(GL11.GL_GREATER, 0);
    	
		font = new Font(resourceLoader.loadResourceAsStream("textures/font.bmp"), 25, width, height);
//		heightMap=new HeightMap(50,50,resourceLoader.loadResourceAsStream("textures/heightmap.png"));
		heightMap=new HeightMap(new float[]{40,40,1},new int[]{50,50}, resourceLoader.loadResourceAsStream("textures/heightmap.png"));
		strips=heightMap.generateTriangleStrip();
		lines=heightMap.generateTriangles();
		
		g36c = new MS3DModel(resourceLoader.loadResourceAsStream("models/assassin.ms3d"),this.getClass().getResource("./textures").getPath());
		
//		GL11.glMatrixMode(GL11.GL_MODELVIEW);
		textureId=GLApp.makeTexture(SceneDemo.class.getResource(".").getPath()+"textures/sod.jpg");
		
	}
	
	private void run(){
		while (runnable) {
			long startTime = System.currentTimeMillis();
			GL11.glClearColor(0.5f,0.5f,0.5f,1);
			GL11.glClear(GL11.GL_COLOR_BUFFER_BIT| GL11.GL_DEPTH_BUFFER_BIT);
			render();
			
			if (Keyboard.next() && Keyboard.getEventKeyState()) {
				switch(Keyboard.getEventKey())
				{
				case Keyboard.KEY_ESCAPE:
					runnable=false;
					break;
				case Keyboard.KEY_W:
					position[2]+=50;
					break;
				case Keyboard.KEY_S:
					position[2]-=50;
					break;
				case Keyboard.KEY_A:
					position[0]+=50;
					break;
				case Keyboard.KEY_D:
					position[0]-=50;
					break;
				}
					
			}
			if(Display.isCloseRequested())
			{
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
//            FPSRecorder.push(costTime);
//            costTime=FPSRecorder.getAverage();
//            font.print("FPS:"+String.valueOf(costTime==0?FPS:1000/costTime),5,25,0);
            Display.update();
		}
		Keyboard.destroy();
		Display.destroy();
		System.out.println("Program stopped");
	}
	int angle=0;
	private void render() {
		GL11.glMatrixMode(GL11.GL_PROJECTION);
    	GL11.glLoadIdentity();
    	
//    	GL11.glOrtho(0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0,25);
    	GLU.gluPerspective(90.0f,(float)width/(float)height,0.1f,1000.0f);
    	GL11.glTranslated(position[0],0, position[2]-500);
    	GL11.glMatrixMode(GL11.GL_MODELVIEW);
		GL11.glLoadIdentity();
		
//		GL11.glTranslated(position[0],-400, position[2]);
		GL11.glTranslated(0,-400, 0);
//		System.out.println(position[0]+","+position[1]+","+position[2]+":"+heightMap.getHeight(position[0],position[2]));
//		GL11.glRotatef(angle++, 0,1,0);
//		GL11.glScaled(40,0.8f,40);
//		GL11.glBindTexture(GL11.GL_TEXTURE_2D, textureId);
//		GL11.glEnable(GL11.GL_TEXTURE_2D);
//		for(int i=0;i<this.strips.length;i++)
//		{
//			GL11.glBegin(GL11.GL_TRIANGLE_STRIP);
//			for(int j=0;j<this.strips[i].length;j++)
//			{
//				Point3f point=this.strips[i][j];
//				GL11.glTexCoord2f(point.x/80,point.y/80);
//				GL11.glVertex3f(point.x,point.z,point.y);
//			}
//			GL11.glEnd();
//		}
//		GL11.glDisable(GL11.GL_TEXTURE_2D);
		
		for(int i=0;i<lines.length;i++)
		{
			for(int j=0;j<lines[i].length;j++)
			{
				for(int k=0;k<lines[i][j].length;k++)
				{
					if(k%3==0)
						GL11.glBegin(GL11.GL_LINE_LOOP);
					Point3f point=lines[i][j][k];
					GL11.glColor3f(point.x/(point.x+point.z+point.y),point.z/(point.x+point.z+point.y),point.y/(point.x+point.z+point.y));
					GL11.glVertex3f(point.x,point.z,point.y);
					if(k%3==2)
						GL11.glEnd();
				}
			}
		}
		GL11.glPushMatrix();
		GL11.glLoadIdentity();
		GL11.glTranslatef(position[0], -40.0f, position[2]+450.0f);
		GL11.glScaled(6.0f,6.0f,6.0f);
		g36c.render();
		GL11.glPopMatrix();
		
		
//		GL11.glColor3f(1,0,0);
//		GL11.glBegin(GL11.GL_LINES);
//		GL11.glVertex3f(position[0],position[1],position[2]);
//		GL11.glVertex3f(position[0],position[1]+100,position[2]);
//		GL11.glVertex3f(position[0]-50,position[1]+100,position[2]);
//		GL11.glVertex3f(position[0]+50,position[1]+100,position[2]);
//		GL11.glVertex3f(position[0],position[1]+100,position[2]-50);
//		GL11.glVertex3f(position[0],position[1]+100,position[2]+50);
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
	}
	
	public static void main(String[] args) throws LWJGLException, IOException
	{
		new SceneDemo();
	}

}
