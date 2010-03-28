package org.tinder.m3g.tutorial.heightmap;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.game.GameCanvas;
import javax.microedition.m3g.*;


public class M3GCanvas extends GameCanvas implements Runnable {

    private World world;
    // Key array
    boolean[] key = new boolean[9];
    
  
    // Global Graphics3D object
    private Graphics3D g3d = null;
    
    // The background
    private Background back = null;
    
    // The global camera object
    private Camera camera = null;
    private Transform camTrans = new Transform();
    
    private HeightMap heightmap;
    private Mesh[][] map;
    // Transforms
    
    /** Constructs the canvas
     */
    public M3GCanvas()
    {
        // We don't want to capture keys normally
        super(true);
        setFullScreenMode(true);
        g3d = Graphics3D.getInstance();     
        world = new World();
        // Create our scene
        createMap();
        map = heightmap.getQuads();	
        for(int x = 0; x < heightmap.getMapWidth()-1; x++)
        {
            for(int y = 0; y < heightmap.getMapHeight()-1; y++)
            {
               world.addChild(map[x][y]);
            }
        }  
       // world.addChild(heightmap.getWater());
        // Create a new camera
        camera = new Camera();
        float aspect = (float) getWidth() / (float) getHeight();
        camera.setPerspective(60.0f, aspect, 0.1f, 150.0f);
	      camTrans.postTranslate(0.0f, 5.0f, 0.0f);
        camera.setTransform(camTrans);
        world.addChild(camera);
        world.setActiveCamera(camera);
        addWater();
        // Create a new background, set bg color to black
        back = new Background();
        back.setColor(0xFF0000FF);
        world.setBackground(back);
        // Set up graphics 3d

        Thread t = new Thread(this);
        t.start();
    }
    
    
    // The heightmap and its transform

    /** Creates our scene */
    private void createMap()
    {
        try
        {
            // We're using a pretty high resolution. If you want to test this on an actual
            // handheld, try using a lower resolution, such as 0.20 or 0.10
	        heightmap = new HeightMap();        
	        Transform t = new Transform();
	        t.postTranslate(0.0f, -5.0f, -5.0f);
	        heightmap.setTransform(t);

	        //camTrans.postTranslate(0.0f, 5.0f, 2.0f);
        }
        catch(Exception e)
        {
            System.out.println("Heightmap error: " + e.getMessage());
            e.printStackTrace();

        }
    }

    /** Draws to screen
     */    
    private void draw(Graphics g)
    {
        // Envelop all in a try/catch block just in case
        try
        {            
            // Get the Graphics3D context
          g3d = Graphics3D.getInstance();
            
	        // First bind the graphics object. We use our pre-defined rendering hints.
	        g3d.bindTarget(g, true, Graphics3D.TRUE_COLOR | Graphics3D.DITHER);

	        g3d.render(world);
	        // Check controls for camera movement

        }
        catch(Exception e)
        {
          System.out.println(e.getMessage());
          System.out.println(e);
          e.printStackTrace();
        }
        finally
        {
            // Always remember to release!
            g3d.releaseTarget();
        }
    }
  private void addWater()

  {
  
      Image2D waterIm = null;
    try {
       waterIm = (Image2D)Loader.load("/water.png")[0];
    }
    catch (Exception e)
    { System.out.println("Cannot load image " ); }

    TiledWater w = new TiledWater(waterIm,8);   
    Mesh water = w.getWaterMesh();
    water.scale(2,2,2); 
      // 8 by 8 size, made up of 1 by 1 tiles
   // f.getWaterMesh().postTranslate(0.0f, 0.0f, 0.0f);
    world.addChild( w.getWaterMesh() );  // add the water
  }  // end of addWater()
    
    /** Runs the whole thread. Also keeps track of FPS
     */
    public void run() {
        while(true) {
            try {                
                // Call the process method (computes keys)
                input();
                
                // Draw everything
                draw(getGraphics());
                flushGraphics();
                
                // Sleep to prevent starvation
                try{ Thread.sleep(30); } catch(Exception e) {}
            }
            catch(Exception e) {
               System.out.println(e.getMessage());
               System.out.println(e);
               e.printStackTrace();
            }
        }

    }

    protected void input()
    {
        int keys = getKeyStates();
	      if((keys & GameCanvas.FIRE_PRESSED) != 0)
          camTrans.postTranslate(0.0f, 0.0f, -1.0f);
        
        if((keys & GameCanvas.UP_PRESSED) != 0)
          camTrans.postTranslate(0.0f, 1.0f, 0.0f);
        
        if((keys & GameCanvas.DOWN_PRESSED) != 0)
          camTrans.postTranslate(0.0f, -1.0f, 0.0f);
        
        if((keys & GameCanvas.LEFT_PRESSED) != 0)
	        camTrans.postRotate(5, 0.0f, 1.0f, 0.0f);
        
        if((keys & GameCanvas.RIGHT_PRESSED) != 0)
	        camTrans.postRotate(-5, 0.0f, 1.0f, 0.0f);
	        camera.setTransform(camTrans) ;

    }
    

}
