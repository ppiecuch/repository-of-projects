package org.tinder.m3g.tutorial.particle.explosion;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.game.GameCanvas;
import javax.microedition.m3g.Background;
import javax.microedition.m3g.Camera;
import javax.microedition.m3g.Graphics3D;
import javax.microedition.m3g.Light;
import javax.microedition.m3g.Transform;

public class M3GCanvas extends GameCanvas implements Runnable 
{

    


    
    // Key array
    boolean[] key = new boolean[5];
    boolean isReleased = true;
    // Key constants
    public static final int FIRE = 0;
    public static final int UP = FIRE + 1;
    public static final int DOWN = UP + 1;
    public static final int LEFT = DOWN + 1;
    public static final int RIGHT = LEFT + 1;
    
    // Global identity matrix
    Transform identity = new Transform();
    
    // Global Graphics3D object
    Graphics3D g3d ;
    Graphics g2d;
    // The background
    Background back = null;
    
    // The global camera object
    Camera camera = null;
    
    // The particle system
    ParticleSystem ps = null;
    ExplosionEffect effect = null;
    
    /** Constructs the canvas
     */
    public M3GCanvas()
    {
        // We don't want to capture keys normally
        super(true);
        
        // We want a fullscreen canvas
        setFullScreenMode(false);
        
        // Load our camera
        camera = new Camera();
        
        // Create a new background, set bg color to black
        back = new Background();
        back.setColor(0);

        // Get the instance
        g3d = Graphics3D.getInstance();
        Light light = new Light();
        light.setMode(Light.AMBIENT);
        light.setIntensity(1.0f);
        // Add a light to our scene, so we can see something
        g3d.addLight(light, identity);
        Thread t = new Thread(this);

        t.start();
    }
    




    /** Draws to screen
     */    
    private void draw(Graphics g)
    {
    	 g3d = Graphics3D.getInstance();
        // Envelop all in a try/catch block just in case
        try
        {            
            // Get the Graphics3D context
          g3d.bindTarget(g);
  
	        // First bind the graphics object. We use our pre-defined rendering hints.
	
	        
	        // Clear background
	        g3d.clear(back);
	        
	        // Bind camera at fixed position in origo
	        g3d.setCamera(camera, identity);
	        
	        // Init particles
	        if(ps == null)
	        {
	            effect = new ExplosionEffect();
	            ps = new ParticleSystem(effect, 60);
	        }
	        
	        // Emit the particles
	        ps.emit(g3d);
	        flushGraphics();
	        // Check controls for fountain rotation

   
        }
        catch(Exception e)
        {

                  e.printStackTrace();  
             
             
        }
        finally
        {
            // Always remember to release!
            g3d.releaseTarget();
   
        }
    }

    
    /** Run, runs the whole thread. Also keeps track of FPS
     */
    public void run() {
    	  g2d = getGraphics();
        while(true) {
            try {                
                // Call the process method (computes keys)
                process();
                
                // Draw everything
                draw(g2d);
                
                
                // Sleep to prevent starvation
                try{ Thread.sleep(30); } catch(Exception e) {}
            }
            catch(Exception e) {

                e.printStackTrace();
            }
        }

    }
    

    
    /** Processes keys
     */
    protected void process()
    {
        int keys = getKeyStates();

        
        if((keys & GameCanvas.LEFT_PRESSED) != 0)
            key[LEFT] = true;
        else
            key[LEFT] = false;
        
        if((keys & GameCanvas.RIGHT_PRESSED) != 0)
            key[RIGHT] = true;
        else
            key[RIGHT] = false;
        if(key[LEFT])
	            effect.setAngle(effect.getAngle() + 5);
	        if(key[RIGHT])
	            effect.setAngle(effect.getAngle() - 5);
    }
 
}
