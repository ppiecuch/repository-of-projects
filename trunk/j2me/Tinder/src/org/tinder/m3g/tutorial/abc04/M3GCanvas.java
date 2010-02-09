package org.tinder.m3g.tutorial.abc04;


import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.game.GameCanvas;
import javax.microedition.m3g.Camera;
import javax.microedition.m3g.Graphics3D;
import javax.microedition.m3g.Light;
import javax.microedition.m3g.Loader;
import javax.microedition.m3g.Object3D;
import javax.microedition.m3g.Transform;
import javax.microedition.m3g.World;


public class M3GCanvas extends GameCanvas implements Runnable {
	
	public static final int FPS = 50;	//每秒绘制的帧数
	
//	 Thread-control
	boolean running = false;

	boolean done = true;

	// If the game should end
	public static boolean gameOver = false;

	// Rendering hints
	public static final int STRONG_RENDERING_HINTS = Graphics3D.ANTIALIAS
			| Graphics3D.TRUE_COLOR | Graphics3D.DITHER;

	public static final int WEAK_RENDERING_HINTS = 0;

	public static int RENDERING_HINTS = STRONG_RENDERING_HINTS;

	// Key array
	boolean[] key = new boolean[5];

	// Key constants
	public static final int FIRE = 0;
	public static final int UP = 1;
	public static final int DOWN = 2;
	public static final int LEFT = 3;
	public static final int RIGHT = 4;

	// Global identity matrix
	Transform identity = new Transform();

	// Camera rotation
	float camRot = 0.0f;

	double camSine = 0.0f;

	double camCosine = 0.0f;

	// Head bobbing
	float headDeg = 0.0f;

	private Graphics3D g3d;
	private World world;
	private boolean runnable=true;
	private Thread thread;
	private Camera camera;

	protected M3GCanvas() {
		super(false);
		setFullScreenMode(true);
		g3d = Graphics3D.getInstance();
		
		//Load our world
		loadWorld();

		// Load our camera
		loadCamera();
	}
	
	/** Loads our camera */
	private void loadCamera() {
		// BAD!
		if (world == null)
			return;

		// Get the active camera from the world
		camera = world.getActiveCamera();

		// Create a light
		Light l = new Light();

		// Make sure it's AMBIENT
		l.setMode(Light.AMBIENT);

		// We want a little higher intensity
		l.setIntensity(3.0f);

		// Add it to our world
		world.addChild(l);
	}

	/** Loads our world */
	private void loadWorld() {
		try {
			// Loading the world is very simple. Note that I like to use a
			// res-folder that I keep all files in. If you normally just put
			// your
			// resources in the project root, then load it from the root.
			Object3D[] buffer = Loader.load("/map.m3g");

			// Find the world node, best to do it the "safe" way
			for (int i = 0; i < buffer.length; i++) {
				if (buffer[i] instanceof World) {
					world = (World) buffer[i];
					break;
				}
			}

			// Clean objects
			buffer = null;
		} catch (Exception e) {
			// ERROR!
			System.out.println("Loading error!");
			reportException(e);
		}
	}
	
	private void moveCamera() {
		// Check controls
		if (key[LEFT]) {
			camRot += 5.0f;
		} else if (key[RIGHT]) {
			camRot -= 5.0f;
		}

		// Set the orientation
		camera.setOrientation(camRot, 0.0f, 1.0f, 0.0f);

		// Calculate trigonometry for camera movement
		double rads = Math.toRadians(camRot);
		camSine = Math.sin(rads);
		camCosine = Math.cos(rads);

		if (key[UP]) {
			// Move forward
			camera.translate(-2.0f * (float) camSine, 0.0f, -2.0f
					* (float) camCosine);

			// Bob head
			headDeg += 0.5f;

			// A simple way to "bob" the camera as the user moves
			camera.translate(0.0f, (float) Math.sin(headDeg) / 3.0f, 0.0f);
		} else if (key[DOWN]) {
			// Move backward
			camera.translate(2.0f * (float) camSine, 0.0f,
					2.0f * (float) camCosine);

			// Bob head
			headDeg -= 0.5f;

			// A simple way to "bob" the camera as the user moves
			camera.translate(0.0f, (float) Math.sin(headDeg) / 3.0f, 0.0f);
		}

		// If the user presses the FIRE key, let's quit
		if (key[FIRE])
			System.out.println("Fire");
	}
	
	private void reportException(Exception e) {
		System.out.println(e.getMessage());
		System.out.println(e);
		e.printStackTrace();
	}
	
	protected void process() {
		int keys = getKeyStates();

		if ((keys & GameCanvas.FIRE_PRESSED) != 0)
			key[FIRE] = true;
		else
			key[FIRE] = false;

		if ((keys & GameCanvas.UP_PRESSED) != 0)
			key[UP] = true;
		else
			key[UP] = false;

		if ((keys & GameCanvas.DOWN_PRESSED) != 0)
			key[DOWN] = true;
		else
			key[DOWN] = false;

		if ((keys & GameCanvas.LEFT_PRESSED) != 0)
			key[LEFT] = true;
		else
			key[LEFT] = false;

		if ((keys & GameCanvas.RIGHT_PRESSED) != 0)
			key[RIGHT] = true;
		else
			key[RIGHT] = false;
	}

	public void run() {
		Graphics g = getGraphics();
		while (runnable) {
			long startTime = System.currentTimeMillis();
			
			//Call the process method (computes keys)
			process();

			//Move the camera around
			moveCamera();
			
			try {
				//First bind the graphics object. We use our pre-defined rendering
				// hints.
				g3d.bindTarget(g, true, RENDERING_HINTS);
				//Now, just render the world. Simple as pie!
				g3d.render(world);
			} finally {
				g3d.releaseTarget();
			}
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

}
