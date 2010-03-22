package org.tinder.m3g.tutorial.camera;



import javax.microedition.lcdui.*;
import javax.microedition.m3g.*;
import javax.microedition.lcdui.game.*;

class M3GCanvas  extends GameCanvas implements Runnable
{
    private Graphics3D g3d; // Graphics object used to render the 3d world.
    private Graphics g2d; 
    private World world; // This world contains the camera and the pyramidMesh.
    private MobileCamera mobileCamera; // the camera in the scene
    private Mesh mesh;
    private    IndexBuffer indexBuffer;
    private Appearance  appearance = new Appearance();
    private Transform cubeTransform = new Transform();
    private   VertexBuffer vertexBuffer = new VertexBuffer();
    public M3GCanvas(){
        super(false);
        world = new World();
        setFullScreenMode(false);       
         g3d = Graphics3D.getInstance();  
        mobileCamera = new MobileCamera( getWidth(), getHeight());
    world.addChild( mobileCamera.getCameraGroup() );
    world.setActiveCamera( mobileCamera.getCamera() ); 

        createCube(); 
        world.addChild(mesh);
    mobileCamera.getCamera().setAlignment(mesh, Node.ORIGIN, world, Node.Y_AXIS);
    mobileCamera.getCamera().scale(-1, 1, -1);
              
        Thread t = new Thread(this);
        t.start();
    }
    public void run() {
        g2d = getGraphics();
        while(true){
        try{
            g3d.bindTarget(g2d); // Binds the given Graphics or mutable Image2D as the rendering target of this Graphics3D
            //g3d.clear(null);
            g3d.render(world);
        }finally{
            g3d.releaseTarget();
        }
            mobileCamera.update();
          mobileCamera.getCamera().align(mesh); 

            g2d.setColor(255,255,255);
            g2d.drawString( mobileCamera.getKeyMode(), 5,5, Graphics.TOP|Graphics.LEFT);
            g2d.drawString( mobileCamera.getPosition(), 5,18, Graphics.TOP|Graphics.LEFT);
            g2d.drawString( mobileCamera.getRotation(), 5,31, Graphics.TOP|Graphics.LEFT);
            flushGraphics();
        }
    }
  protected void keyPressed(int keyCode)
  { int gameAction = getGameAction(keyCode);
    mobileCamera.pressedKey(gameAction);
  }

  protected void keyReleased(int keyCode)
  { int gameAction = getGameAction(keyCode);
    mobileCamera.releasedKey(gameAction);
  }

  private void createCube(){
  	/** The cube's vertex positions (x, y, z). */
  	  byte []POINTS = new byte[] {    
  	     -1, -1,  1,    1, -1,  1,   -1,  1,  1,    1,  1,  1,
         -1, -1, -1,    1, -1, -1,   -1,  1, -1,    1,  1, -1};
                                                            
                                                            
        /** Indices that define how to connect the vertices to build 
   * triangles. */
       
        byte []COLORS = new byte[] {    0, 0, (byte) 128,             0, 0, (byte) 255,
    0, (byte) 255, 0,             0, (byte) 255, (byte) 255,
    (byte) 255, 0, 0,             (byte) 255, 0, (byte) 255,
    (byte) 255, (byte) 255, 0,    (byte) 255, (byte) 255, (byte) 255};//B
                                                        
        int []INDICES = new int[]{
    0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1
    };
        // The length of each sequence in the indices array.
      //  int []LENGTH = new int[] {3, 3, 3, 3, 3, 3,3, 3, 3, 3, 3, 3}; // the pyramid is built by 12 triangles
        
        VertexArray POSITION_ARRAY, COLOR_ARRAY;

        
        // Create a VertexArray to be used by the VertexBuffer
        POSITION_ARRAY = new VertexArray(POINTS.length / 3, 3, 1);
        POSITION_ARRAY.set(0, POINTS.length / 3, POINTS);
        COLOR_ARRAY = new VertexArray(COLORS.length / 3, 3, 1);
        COLOR_ARRAY.set(0, COLORS.length / 3, COLORS);
        indexBuffer = new TriangleStripArray(INDICES, new int[] {INDICES.length});
        
        // VertexBuffer holds references to VertexArrays that contain the positions, colors, normals, 
        // and texture coordinates for a set of vertices

        vertexBuffer.setPositions(POSITION_ARRAY, 1.0f, null);
        vertexBuffer.setColors(COLOR_ARRAY);
   

       mesh = new Mesh(vertexBuffer, indexBuffer,appearance);


  }
}
