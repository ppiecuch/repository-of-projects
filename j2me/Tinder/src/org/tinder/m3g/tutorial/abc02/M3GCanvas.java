package org.tinder.m3g.tutorial.abc02;

import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.game.GameCanvas;
import javax.microedition.m3g.Appearance;
import javax.microedition.m3g.Camera;
import javax.microedition.m3g.Graphics3D;
import javax.microedition.m3g.IndexBuffer;
import javax.microedition.m3g.Light;
import javax.microedition.m3g.Material;
import javax.microedition.m3g.Mesh;
import javax.microedition.m3g.PolygonMode;
import javax.microedition.m3g.Transform;
import javax.microedition.m3g.TriangleStripArray;
import javax.microedition.m3g.VertexArray;
import javax.microedition.m3g.VertexBuffer;
import javax.microedition.m3g.World;


public class M3GCanvas extends GameCanvas implements Runnable {
	
	public static final int FPS = 20;	//每秒绘制的帧数

	private Graphics3D g3d;
	private World world;
	private boolean runnable=true;
	private Thread thread;
	private Mesh mesh;
	private Camera camera;
	private Light light;
	private Material material;

	protected M3GCanvas() {
		super(false);
		setFullScreenMode(true);
		g3d = Graphics3D.getInstance();
		world = new World();
		
		camera = new Camera();
		world.addChild(camera);
		
		float w = getWidth();
		float h = getHeight();
		camera.setPerspective(60.0f, w / h, 0.1f, 80f);
		
		Transform cameraTransform = new Transform();
		cameraTransform.postTranslate(0.0f, 15.0f, 10.0f);
		camera.setTransform(cameraTransform);
		camera.setOrientation(-10,1,0,0);
		
		mesh = createCube();
		mesh.translate(0.0f, 0.0f, -50.0f);
        
        light=new Light();
        nextLightMode(light);
        Transform lightTransform = new Transform();
        lightTransform.postTranslate(0.0f, 0.0f, -4.5f);
        light.setOrientation(-30,1,0,0);
//        light.setOrientation(30,0,1,0);
        light.setColor(0xFFFFFF);
        g3d.resetLights();
        g3d.addLight(light,lightTransform);
        
        world.addChild(light);
        
        world.addChild(mesh);
        world.setActiveCamera(camera);
	}

	public void run() {
		Graphics g = getGraphics();
		while (runnable) {
			long startTime = System.currentTimeMillis();
			
			mesh.postRotate(1.0f, 0.0f, 1.0f, 0.0f);
			
			try {
//				g3d.bindTarget(g);
				g3d.bindTarget(g, true, Graphics3D.DITHER | Graphics3D.TRUE_COLOR);
				g3d.render(world);
			} finally {
				g3d.releaseTarget();
			}
			g.setColor(0xFF00FF);
            g.drawString("material:"+materailInfo, 10, 10, Graphics.LEFT|Graphics.TOP);
            g.drawString("light:"+lightInfo, 10, 30, Graphics.LEFT|Graphics.TOP);
			flushGraphics();
			
			long endTime = System.currentTimeMillis();
            long costTime = endTime - startTime;
            if(costTime<1000/FPS)
            {
                try{
                  Thread.sleep(1000/FPS-costTime);
                }
                catch(Exception e){
                   e.printStackTrace();
                }
            }
		}
		System.out.println("Canvas stopped");

	}
	
	public void start()
	{
		thread=new Thread(this);
		thread.start();
	}
	
	public void stop()
	{
		this.runnable=false;
		try {
			thread.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	protected void keyPressed(int keyCode) {
		switch(keyCode)
		{
		case KEY_NUM2:
			nextMaterial();
			break;
		case KEY_NUM4:
			nextLightMode(light);
			break;
		case KEY_NUM6:
		case KEY_NUM8:
		}
	}
	
	private int materialTypeIndex=0;
	private String materailInfo=null;
	private void nextMaterial()
	{
		materialTypeIndex++;
		materialTypeIndex=(materialTypeIndex+4)%4;
		switch(materialTypeIndex)
		{
		case 0:
			material.setColor(Material.EMISSIVE, 0x00FF0000);
			materailInfo="EMISSIVE";
			System.out.println("use material:EMISSIVE");
			break;
		case 1:
			material.setColor(Material.AMBIENT, 0x00FF0000);
			materailInfo="AMBIENT";
			System.out.println("use material:AMBIENT");
			break;
		case 2:
			material.setColor(Material.DIFFUSE, 0x00FF0000);
			materailInfo="DIFFUSE";
			System.out.println("use material:DIFFUSE");
			break;
		case 3:
			material.setColor(Material.SPECULAR, 0x00FF0000);
			materailInfo="SPECULAR";
			System.out.println("use material:SPECULAR");
			break;
		}
		
	}
	
	private int modeIndex=0;
	private String lightInfo=null;
	protected void nextLightMode(Light light)
	{
		modeIndex++;
		modeIndex=(modeIndex+4)%4;
		switch (modeIndex)
		{
	    case 0:
	    	light.setMode(Light.AMBIENT);
	        light.setIntensity(1.0f);
	        lightInfo="AMBIENT";
	        System.out.println("use light mode:AMBIENT");
	        break;
	    case 1:
	        light.setMode(Light.DIRECTIONAL);
	        light.setIntensity(1.0f);
	        lightInfo="DIRECTIONAL";
	        System.out.println("use light mode:DIRECTIONAL");
	        break;
	    case 2:
	        light.setMode(Light.OMNI);
	        light.setIntensity(1.0f);
	        lightInfo="OMNI";
	        System.out.println("use light mode:OMNI");
	        break;
	    case 3:
	        light.setMode(Light.SPOT);
	        light.setSpotAngle(45.0f);
	        light.setIntensity(2.0f);
	        lightInfo="SPOT";
	        System.out.println("use light mode:SPOT");
	        break;
	   }
	}
	
	private Mesh createCube(){
		
		short[] vert = { 10, 10, 10, -10, 10, 10, 10, -10, 10, -10, -10,10, // front
				-10, 10, -10, 10, 10, -10, -10, -10, -10, 10, -10, -10, // back
				-10, 10, 10, -10, 10, -10, -10, -10, 10, -10, -10, -10, // left
				10, 10, -10, 10, 10, 10, 10, -10, -10, 10, -10, 10, // right
				10, 10, -10, -10, 10, -10, 10, 10, 10, -10, 10, 10, // top
				10, -10, 10, -10, -10, 10, 10, -10, -10, -10, -10, -10 }; // bottom
		VertexArray vertArray = new VertexArray(vert.length / 3, 3, 2);
		vertArray.set(0, vert.length / 3, vert);
		byte[] norm = { 0, 0, 127, 0, 0, 127, 0, 0, 127, 0, 0, 127, 0, 0,
				-127, 0, 0, -127, 0, 0, -127, 0, 0, -127, -127, 0, 0, -127,
				0, 0, -127, 0, 0, -127, 0, 0, 127, 0, 0, 127, 0, 0, 127, 0,
				0, 127, 0, 0, 0, 127, 0, 0, 127, 0, 0, 127, 0, 0, 127, 0,
				0, -127, 0, 0, -127, 0, 0, -127, 0, 0, -127, 0 };
		VertexArray normArray = new VertexArray(norm.length / 3, 3, 1);
		normArray.set(0, norm.length / 3, norm);
		int[] stripLen = { 4, 4, 4, 4, 4, 4 };
		IndexBuffer indexBuffer = new TriangleStripArray(0, stripLen);
		VertexBuffer vertexBuffer = new VertexBuffer();
		vertexBuffer.setPositions(vertArray, 1.0f, null);
		vertexBuffer.setNormals(normArray);
		Appearance cubeAppearance = new Appearance();
		PolygonMode pm = new PolygonMode();
        pm.setShading(PolygonMode.SHADE_SMOOTH);
        pm.setCulling(PolygonMode.CULL_NONE);

        material = new Material();
        nextMaterial();

        Appearance app = new Appearance();
        app.setPolygonMode(pm);
		cubeAppearance.setMaterial(material);
		Mesh mesh = new Mesh(vertexBuffer, indexBuffer, cubeAppearance);
		return mesh;
	}

}
