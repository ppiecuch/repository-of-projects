package org.tinder.m3g.tutorial.abc05;



import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.game.GameCanvas;
import javax.microedition.m3g.AnimationController;
import javax.microedition.m3g.AnimationTrack;
import javax.microedition.m3g.Camera;
import javax.microedition.m3g.Graphics3D;
import javax.microedition.m3g.Group;
import javax.microedition.m3g.KeyframeSequence;
import javax.microedition.m3g.Loader;
import javax.microedition.m3g.Object3D;
import javax.microedition.m3g.World;


public class M3GCanvas extends GameCanvas implements Runnable {
	
	public static final int FPS = 50;	//每秒绘制的帧数
	
//	 Thread-control
	boolean running = false;

	boolean done = true;

	//UserIDs for objects we use in the scene.
    static final int POGOROO_TRANSFORM_ID = 347178853;
    static final int ROO_BOUNCE_ID = 418071423;

    // Control objects for game play
    // control for 'roo - group transform and cameras
    private AnimationController animRoo = null;
    private Group acRoo = null;
    private int animTime = 0;
    private int animLength = 0;
    private int animLastTime = 0;
    int viewport_x;
    int viewport_y;
    int viewport_width;
    int viewport_height;

	// Key array
	private boolean[] key = new boolean[5];

	// Key constants
	public static final int FIRE = 0;
	public static final int UP = 1;
	public static final int DOWN = 2;
	public static final int LEFT = 3;
	public static final int RIGHT = 4;

	// Camera rotation
	private float camRot = 0.0f;

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
		
		getObjects();
		
		setupAspectRatio();
	}
	
	/** Loads our camera */
	private void loadCamera() {
		// BAD!
		if (world == null)
			return;
		
		// Get the active camera from the world
		camera = world.getActiveCamera();
	}

	/** Loads our world */
	private void loadWorld() {
		try {
			// Loading the world is very simple. Note that I like to use a
			// res-folder that I keep all files in. If you normally just put
			// your
			// resources in the project root, then load it from the root.
			Object3D[] buffer = Loader.load("/pogoroo.m3g");
	
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
		}
	}
	
	/**
     * Make sure that the content is rendered with the correct aspect ratio.
     */
    void setupAspectRatio() {
        viewport_x = 0;
        viewport_y = 0;
        viewport_width = getWidth();
        viewport_height = getHeight();

        float[] params = new float[4];
        int type = camera.getProjection(params);

        if (type != Camera.GENERIC) {
            //calculate window aspect ratio
            float waspect = viewport_width / viewport_height;

            if (waspect < params[1]) {
                float height = viewport_width / params[1];
                viewport_height = (int)height;
                viewport_y = (getHeight() - viewport_height) / 2;
            } else {
                float width = viewport_height * params[1];
                viewport_width = (int)width;
                viewport_x = (getWidth() - viewport_width) / 2;
            }
        }
    }
    
    /**
     * getObjects()
     * get objects from the scene tree for use in the game AI
     */
    public void getObjects() {
        try {
            acRoo = (Group)world.find(POGOROO_TRANSFORM_ID);
            animRoo = (AnimationController)world.find(ROO_BOUNCE_ID);

            // get length of animation
            AnimationTrack track = acRoo.getAnimationTrack(0);
            animLength = 1000; // default length, 1 second

            if (track != null) {
                KeyframeSequence ks = track.getKeyframeSequence();

                if (ks != null) {
                    animLength = ks.getDuration();
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    /**
     * animateRoo()
     * Makes sure that the hopping animation loops correctly.
     */
    private void animateRoo(int worldTime) {
        // control the kangaroo animation sequence
        if (animLastTime == 0) {
            animLastTime = worldTime;
        }

        animTime += (worldTime - animLastTime);

        // initialise animation at end of sequence
        if (animTime > animLength) // sequence is ~1000ms
         {
            animRoo.setActiveInterval(worldTime, worldTime+2000);
            //setPosition(float sequenceTime, int worldTime) 
            //Sets a new playback position, relative to world time, for this animation controller. 
            animRoo.setPosition(0, worldTime);
            animTime = 0;
        }

        // update storage of last position and time
        animLastTime = worldTime;
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

		// If the user presses the FIRE key, let's quit
		if (key[FIRE])
			System.out.println("Fire");
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
				g3d.bindTarget(g);
				
				int st=(int)startTime;
	            // update the control and game AI
				animateRoo(st);
	            // Update the world to the current time.
	            world.animate(st);
	            
				g3d.setViewport(viewport_x, viewport_y, viewport_width, viewport_height);
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
