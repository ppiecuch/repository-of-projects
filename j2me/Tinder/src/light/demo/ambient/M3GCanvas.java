package light.demo.ambient;


import javax.microedition.lcdui.*;
import javax.microedition.m3g.*;
import javax.microedition.lcdui.game.*;

class M3GCanvas  extends GameCanvas implements Runnable
{
    private Graphics3D g3d ; // Graphics object used to render the 3d world.
    private Graphics g2d; 
    private World world; // This world contains the camera and the pyramidMesh.
    private Light light;
    private float Intensity=1;
    static final int LIGHT_ID = 16;
    private Transform transform = new Transform();

    public M3GCanvas(){
    	super(false);
   		try {
			// Load the world from the m3g file							
			world = (World) Loader.load("/ball.m3g")[0];
		//	light = (Light) world.find(CAMERA_ID);
      light = new Light();
      light.setMode(Light.AMBIENT);
      light.setColor(0xFFFF0000);
      light.setIntensity(10.0f);
      world.addChild(light);
      world.align(null);
      setupAspectRatio();
		} catch (Exception e) {
			e.printStackTrace();
		}

        Thread t = new Thread(this);
        t.start();
    }
    public void run() {
        g3d = Graphics3D.getInstance();
        
        g2d = getGraphics();
        while(true){
        try{
            g3d.bindTarget(g2d); // Binds the given Graphics or mutable Image2D as the rendering target of this Graphics3D
            g3d.clear(null);
            g3d.render(world);
        }finally{
            g3d.releaseTarget();
        }
  
            flushGraphics();
        }
    }
 	void setupAspectRatio() {

    float aspectratio = (float)getWidth() / getHeight();
		Camera camera = world.getActiveCamera();
		float[] params = new float[4];
		int type = camera.getProjection(params);
		camera.setPerspective(60.0f,aspectratio,1.0f, 1000.0f);

	} 
	    public void keyPressed(int key){
	    	
        System.out.println(key);

        switch(key){

            case -1: // up
                light.setColor(0xFF0000ff);
                break;
            case -2:// down
                light.setColor(0xFF00ff00);
                break;
            case -3: // left
                light.setColor(0xFFff0000);
                break;
            case -4: // right
                light.setColor(0xFFffffff);
                break;
           case 50: // 2
           
                Intensity =  light.getIntensity()-2;
                light.setIntensity(Intensity);
            
                break;
           case 56: // 8
                Intensity =  light.getIntensity()+2;
                light.setIntensity(Intensity);
               
                break;
            default:
                System.out.println(key);
                break;
        }
     }
     public void keyRepeated(int key){
        keyPressed(key);
    }


  
}
