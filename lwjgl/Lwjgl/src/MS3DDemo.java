import glapp.GLApp;

import java.awt.event.MouseEvent;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.Date;

import org.lwjgl.BufferUtils;
import org.lwjgl.LWJGLException;
import org.lwjgl.input.Keyboard;
import org.lwjgl.input.Mouse;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;
import org.lwjgl.opengl.GL11;
import org.lwjgl.util.glu.GLU;
import org.tinder.studio.lwjgl.camera.ThirdPersonCamera;
import org.tinder.studio.lwjgl.heightmap.HeightMap;
import org.tinder.studio.lwjgl.ms3d.MS3DAnimation;
import org.tinder.studio.lwjgl.ms3d.MS3DModel;
import org.tinder.studio.lwjgl.util.Dome;
import org.tinder.studio.lwjgl.util.Point3f;

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
	
	private HeightMap heightMap;
	private Point3f[][][] lines;
	private Point3f[][] strips;
	private float[] mufactors={20,20,0.5f};
	private int[] steps={80,80};
	
	private int width,height;
	private int textureId;
	
	private boolean isPress3=false;
	private float yaw=0;	 //the rotation around the Y axis of the camera
	private float pitch=0; //the rotation around the X axis of the camera
	private float[] cameraPosition={0,0,0};
	
	private ThirdPersonCamera thirdCamera;
	
	private Dome dome;
	private int domeTextureId;
	private float domeRadius=1500;
	
	private float fogDensity=0.0055f;
	private float[] fogColor={0.9f,0.9f,0.9f,1.0f};
	
	public MS3DDemo() throws LWJGLException{
		this.init();
		this.run();
	}

	private void init() throws LWJGLException
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
		thirdCamera=new ThirdPersonCamera(25, 0.2f);
		
		Display.create();
		Display.setTitle("MS3DDemo");
		
		GL11.glViewport(0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT);
		
		
        
        /*设置纹理参数*/
        GL11.glTexEnvf(GL11.GL_TEXTURE_ENV,GL11.GL_TEXTURE_ENV_MODE,GL11.GL_REPLACE);
		
		//设置正交投影
		GL11.glMatrixMode(GL11.GL_PROJECTION);
    	GL11.glLoadIdentity();
//    	GL11.glOrtho(0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0,25);
    	GLU.gluPerspective(60.0f,(float)width/(float)height,0.1f,8000.0f);
    	
    	GL11.glEnable(GL11.GL_BLEND);
    	GL11.glEnable(GL11.GL_ALPHA_TEST);
//    	GL11.glShadeModel(GL11.GL_FLAT);
//    	GL11.glShadeModel(GL11.GL_SMOOTH);
//    	GL11.glAlphaFunc(GL11.GL_GREATER, 0);
    	GL11.glClearDepth(1.0f);
    	GL11.glEnable(GL11.GL_DEPTH_TEST);
//		GL11.glDepthFunc(GL11.GL_LEQUAL);
//		GL11.glHint(GL11.GL_PERSPECTIVE_CORRECTION_HINT, GL11.GL_NICEST);
		GL11.glEnable(GL11.GL_CULL_FACE);
		
		/*设置雾的参数*/
		FloatBuffer fogColorBuffer=BufferUtils.createFloatBuffer(fogColor.length);
		fogColorBuffer.put(fogColor);
        fogColorBuffer.flip(); 
        GL11.glFogi(GL11.GL_FOG_MODE, GL11.GL_EXP); //set the fog mode to GL_EXP
		GL11.glFogf (GL11.GL_FOG_DENSITY, fogDensity); //set the density to thevalue above
		GL11.glHint (GL11.GL_FOG_HINT, GL11.GL_DONT_CARE); // set the fog to look the nicest, may slow down on older cards
        GL11.glFog(GL11.GL_FOG_COLOR, fogColorBuffer);  //set the fog color to our color chosen above
		
		dome=new Dome(domeRadius, 24,5);
		font = new Font(resourceLoader.loadResourceAsStream("textures/font.bmp"), 25, width, height);
		g36c = new MS3DModel(resourceLoader.loadResourceAsStream("models/assassin.ms3d"),this.getClass().getResource("./textures").getPath());
		MS3DAnimation walk=new MS3DAnimation(0,13);
		MS3DAnimation leftMove=new MS3DAnimation(15,30);
		g36c.addAnimation(walk);
		g36c.addAnimation(leftMove);
		
		position[0]=200;
		position[2]=200;
		
		
		
		try {
			heightMap=new HeightMap(mufactors,steps, resourceLoader.loadResourceAsStream("textures/terrain.png"));
			textureId=GLApp.makeTexture(SceneDemo.class.getResource(".").getPath()+"textures/terrain.jpg");
			domeTextureId=GLApp.makeTexture(SceneDemo.class.getResource(".").getPath()+"textures/sky01.jpg");
		} catch (IOException e) {
			e.printStackTrace();
		}
		lines=heightMap.generateTriangles();
		strips=heightMap.generateTriangleStrip();
		
		
	}
	
	private void run(){
		while (runnable) {
			long startTime = System.currentTimeMillis();
			GL11.glClearColor(0.5f,0.5f,0.5f,1);
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
//            GL11.glColor3f(0,1,0);
//            font.print("FPS:"+String.valueOf(costTime==0?FPS:1000/costTime),5,22,0);
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
					g36c.reset();
					break;
				case Keyboard.KEY_S:
					state|=2;
					g36c.setCurrentAnimation(0);
					g36c.reset();
					break;
				case Keyboard.KEY_A:
					state|=4;
					g36c.setCurrentAnimation(1);
					g36c.reset();
					break;
				case Keyboard.KEY_D:
					state|=8;
					g36c.setCurrentAnimation(1);
					g36c.reset();
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
		
		if (Mouse.next()) {
		    if (Mouse.getEventButton() != -1) {
		        int btn = Mouse.getEventButton();

		        if (btn == 0)btn = MouseEvent.BUTTON1;
		        else if (btn == 1)btn = MouseEvent.BUTTON3;
		        else if (btn == 2)btn = MouseEvent.BUTTON2;

		        if (Mouse.getEventButtonState()) {
		        	//Press
		        	switch(btn)
		        	{
		        	case MouseEvent.BUTTON1:
		        		break;
		        	case MouseEvent.BUTTON2:
		        		break;
		        	case MouseEvent.BUTTON3:
		        		isPress3=true;
		        		break;
		        	}
		        } else {
		        	//Release
		        	switch(btn)
		        	{
		        	case MouseEvent.BUTTON1:
		        		break;
		        	case MouseEvent.BUTTON2:
		        		break;
		        	case MouseEvent.BUTTON3:
		        		isPress3=false;
		        		break;
		        	}
		        }
		    }
		}

		if(Display.isCloseRequested())
		{
			runnable=false;
		}
	}
	
	private void logic(){
		if((state&1)>0)			//W
		{
			thirdCamera.walkForward(position, yaw);
		}
		else if((state&2)>0)	//S
		{
			thirdCamera.walkBackward(position, yaw);
		}
		else if((state&4)>0)	//A
		{
			thirdCamera.walkLeft(position, yaw);
		}
		else if((state&8)>0)	//D
		{
			thirdCamera.walkRight(position, yaw);
		}
		if(isPress3)
		{
			yaw=thirdCamera.horizontalTurn(yaw,-Mouse.getDX());
			pitch=thirdCamera.portraitTurn(pitch,Mouse.getDY()*0.3f);
		}
		/*第三人称视角,使用极坐标运算公式*/
		thirdCamera.calculateCameraPosition(position, yaw, cameraPosition);
//		System.out.println(position[0]+","+position[2]+","+cameraPosition[0]+","+cameraPosition[2]+"("+((cameraPosition[0]-position[0])*(cameraPosition[0]-position[0])+(cameraPosition[2]-position[2])*(cameraPosition[2]-position[2]))+")");
	}
	
	private void render() {
		GL11.glMatrixMode(GL11.GL_PROJECTION);
    	GL11.glLoadIdentity();
    	GLU.gluPerspective(60.0f,(float)width/(float)height,0.1f,3000.0f);
    	GLU.gluLookAt(cameraPosition[0],10+position[1],cameraPosition[2], position[0],5+position[1]+pitch,position[2], 0,1,0);
		
		GL11.glMatrixMode(GL11.GL_MODELVIEW);
		GL11.glDisable(GL11.GL_FOG);
		/*画出天空*/
		GL11.glLoadIdentity();
		GL11.glEnable(GL11.GL_TEXTURE_2D);
		GL11.glBindTexture(GL11.GL_TEXTURE_2D, domeTextureId);
		GL11.glTranslatef(domeRadius*0.7f,-domeRadius*0.2f,domeRadius*0.7f);
		GL11.glRotatef(180,0,1,0);
		dome.render();
		GL11.glDisable(GL11.GL_TEXTURE_2D);
		
		/*启用雾化效果*/
		GL11.glEnable(GL11.GL_FOG);
//		GL11.glFogf(GL11.GL_FOG_START, 10+cameraPosition[2]);       // Fog Start Depth
//		GL11.glFogf(GL11.GL_FOG_END, 50+cameraPosition[2]);        // Fog End Depth
		
		/*画出线条*/
//		GL11.glLoadIdentity();
//		for(int i=0;i<lines.length;i++)
//		{
//			for(int j=0;j<lines[i].length;j++)
//			{
//				for(int k=0;k<lines[i][j].length;k++)
//				{
//					if(k%3==0)
//						GL11.glBegin(GL11.GL_LINE_LOOP);
//					Point3f point=lines[i][j][k];
////					GL11.glColor3f(point.x/(point.x+point.z+point.y),point.z/(point.x+point.z+point.y),point.y/(point.x+point.z+point.y));
//					GL11.glVertex3f(point.x,point.z+0.01f,point.y);
//					if(k%3==2)
//						GL11.glEnd();
//				}
//			}
//		}
		/*画出坐标轴*/
		GL11.glLoadIdentity();
		GL11.glTranslated(-20,-20,-20);
		GL11.glBegin(GL11.GL_LINES);
		GL11.glColor3f(1,0,0);
		GL11.glVertex3f(0,0,0);
		GL11.glVertex3f(500,0,0);
		GL11.glColor3f(0,0,1);
		GL11.glVertex3f(0,0,0);
		GL11.glVertex3f(0,500,0);
		GL11.glColor3f(0,1,0);
		GL11.glVertex3f(0,0,0);
		GL11.glVertex3f(0,0,500);
		GL11.glEnd();
		/*绘制地形*/
		GL11.glLoadIdentity();
		GL11.glColor3f(1,1,1);
		GL11.glBindTexture(GL11.GL_TEXTURE_2D, textureId);
		GL11.glEnable(GL11.GL_TEXTURE_2D);
		for(int i=0;i<this.strips.length;i++)
		{
			GL11.glBegin(GL11.GL_TRIANGLE_STRIP);
			for(int j=0;j<this.strips[i].length;j++)
			{
				Point3f point=this.strips[i][j];
				GL11.glTexCoord2f(point.x/80,point.y/80);
				GL11.glVertex3f(point.x,point.z,point.y);
			}
			GL11.glEnd();
		}
		GL11.glDisable(GL11.GL_TEXTURE_2D);
		GL11.glLoadIdentity();
		position[1]=heightMap.getHeight(position[0], position[2]);
		GL11.glTranslated(position[0], position[1], position[2]);
		GL11.glRotatef(yaw,0,1,0);
		GL11.glColor3f(1,1,1);
		g36c.updateModel(System.currentTimeMillis()*2/3);
		
		
	}
	
	public static void main(String[] args) throws LWJGLException
	{
		new MS3DDemo();
	}
}
