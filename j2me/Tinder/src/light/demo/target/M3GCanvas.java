package light.demo.target;



import javax.microedition.lcdui.*;
import javax.microedition.m3g.*;
import javax.microedition.lcdui.game.*;

class M3GCanvas  extends GameCanvas implements Runnable
{
    private Graphics3D g3d ; // Graphics object used to render the 3d world.
    private Graphics g2d; 
    private World world; // This world contains the camera and the pyramidMesh.
    private Light light;
    private Mesh mesh;
    private static final float MOVE_INCR = 0.1f;
    static final int LIGHT_ID = 2;
    static final int MESH_ID = 11;
    private Transform transform = new Transform();

    public M3GCanvas(){
    	super(false);

   			try {
			// Load the world from the m3g file							
			world = (World) Loader.load("/ball.m3g")[0];	
	  	light = (Light) world.find(LIGHT_ID);
	  	mesh = (Mesh) world.find(MESH_ID);
      
      light.setMode(Light.SPOT);
      light.setColor(0xFFFF0000);
      light.setIntensity(1.0f);
      light.setTranslation(0, 0,100);
      light.setSpotAngle(50);
      light.setSpotExponent(128.0f);
      light.setAlignment(mesh, Node.ORIGIN, mesh, Node.NONE);
      light.scale(-1, 1, -1);


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
            light.align(mesh);
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
		camera.setTranslation(0, 0,100) ;
		camera.postRotate(90.0f,1.0f, 0.0f,0.0f);
	} 
	public void keyPressed(int key){
        System.out.println(key);
        mesh.getTransform(transform);
        switch(key){
            case -1: // up
                transform.postTranslate(0, MOVE_INCR,0 );
                break;
            case -2:// down
                transform.postTranslate(0, -MOVE_INCR,0);
                break;
            case -3: // left
                transform.postTranslate(-MOVE_INCR, 0, 0);
                break;
            case -4: // right
                transform.postTranslate(MOVE_INCR, 0, 0);
                break;
        }
        mesh.setTransform(transform);
     }
     public void keyRepeated(int key){
        keyPressed(key);
    }
  
}
