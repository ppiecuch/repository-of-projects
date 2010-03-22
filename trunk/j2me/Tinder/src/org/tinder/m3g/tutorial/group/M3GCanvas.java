package org.tinder.m3g.tutorial.group;


import javax.microedition.lcdui.*;
import javax.microedition.m3g.*;
import javax.microedition.lcdui.game.*;

class M3GCanvas  extends GameCanvas implements Runnable
{
    private Graphics3D g3d; // Graphics object used to render the 3d world.
    private Graphics g2d; 
    private World world; // This world contains the camera and the pyramidMesh.



  private static final int USER_ID_ALL_MESHES  = 322;
  private static final int USER_ID_BLUE_MESHES = 242;
  private static final int USER_ID_RED_MESHES  = 735;


    private Camera camera;
    private final int WIDTH, HEIGHT; 
    private Mesh cube;


  private static int COUNTER_MAX = 20;
  private int counter = COUNTER_MAX/2;
  private float scale = 0.02f;


    

    public M3GCanvas(){
        super(false);
        setFullScreenMode(false);
        WIDTH = getWidth();
        HEIGHT = getHeight();
        world = new World();
        camera = new Camera();
         float aspectRatio = ((float) WIDTH) / ((float) HEIGHT);
        camera.setPerspective(30.0f, aspectRatio, 1.0f, 1000.0f);
        camera.setTranslation(0.0f, 0.0f, 10.0f);
        world.addChild(camera );
        world.setActiveCamera(camera ); 
        
        g3d = Graphics3D.getInstance();  
     
        Group groupRed = new Group();
       groupRed.setUserID(USER_ID_RED_MESHES);
        Group groupBlue = new Group();
        groupBlue.setUserID(USER_ID_BLUE_MESHES);
        Group groupAll = new Group();
       groupAll.setUserID(USER_ID_ALL_MESHES);
        groupAll.addChild(groupRed);
        groupAll.addChild(groupBlue );
        world.addChild(groupAll );

    for (int i=0; i<8; i++)
    {
      
      
      if ((i%2) == 0)
      {
      	cube = createCube(0xFFFF0000);
      	
        
        groupRed.addChild(cube);
      }
      else
      {
      	cube = createCube(0xFF0000FF);

        
        groupBlue.addChild(cube);
      }

      cube.setTranslation(1.5f * (float) Math.cos(i*Math.PI/4), 1.5f * (float) Math.sin(i*Math.PI/4), 0.0f);
     cube.setScale(0.4f, 0.4f, 0.4f);
    }
     
       

        Thread t = new Thread(this);
        t.start();
    }
    public void run() {
        g2d = getGraphics();
        while(true){
     Group group1 = (Group) world.find(USER_ID_ALL_MESHES);
      group1.postRotate(2, 0.0f, 0.0f, 1.0f);

      counter++;
      if ((counter%COUNTER_MAX) == 0)
      {
        scale *= -1;
      }
      Group group2 = (Group) world.find(USER_ID_BLUE_MESHES);
      group2.scale(1 + scale, 1 + scale, 1 + scale);
            
      // Scale red meshes.
      Group group3 = (Group) world.find(USER_ID_RED_MESHES);
      group3.scale(1 - scale, 1 - scale, 1 - scale);
    
        try{
            g3d.bindTarget(g2d); // Binds the given Graphics or mutable Image2D as the rendering target of this Graphics3D
    
         //   boolean a = world.pick(-1, POINTER_X,POINTER_Y, world.getActiveCamera(), ray);    
            
     
            g3d.render(world);


        }finally{
            g3d.releaseTarget();
        }
        flushGraphics();
      try
      {
        Thread.sleep(50);
      }
      catch (Exception e){}       
            
        }
    }
 



  private Mesh createCube(int RGBA){
  	IndexBuffer indexBuffer;
    Appearance  appearance = new Appearance();
    VertexBuffer vertexBuffer = new VertexBuffer();
  	/** The cube's vertex positions (x, y, z). */
  	
  	  byte []POINTS = new byte[] {    
  	     -1, -1,  1,    1, -1,  1,   -1,  1,  1,    1,  1,  1,
         -1, -1, -1,    1, -1, -1,   -1,  1, -1,    1,  1, -1};
                                                            
                                                            
        /** Indices that define how to connect the vertices to build 
   * triangles. */
       
    
    short[] TEXCOORDS = {
    0, 1,   1, 1,   0, 0,   1, 0,   // front
    0, 1,   1, 1,   0, 0,   1, 0,   // back

                  };                                     
        int []INDICES = new int[]{
    0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1    };
        // The length of each sequence in the indices array.
      //  int []LENGTH = new int[] {3, 3, 3, 3, 3, 3,3, 3, 3, 3, 3, 3}; // the pyramid is built by 12 triangles
        
        VertexArray POSITION_ARRAY, COLOR_ARRAY,TEXCOORD_ARRAY;

        
        // Create a VertexArray to be used by the VertexBuffer
        POSITION_ARRAY = new VertexArray(POINTS.length / 3, 3, 1);
        POSITION_ARRAY.set(0, POINTS.length / 3, POINTS);

        TEXCOORD_ARRAY = new VertexArray( TEXCOORDS.length / 2, 2, 2 );
        TEXCOORD_ARRAY.set( 0, TEXCOORDS.length/2,TEXCOORDS );
        indexBuffer = new TriangleStripArray(INDICES, new int[] {INDICES.length});
        
        // VertexBuffer holds references to VertexArrays that contain the positions, colors, normals, 
        // and texture coordinates for a set of vertices

        vertexBuffer.setPositions(POSITION_ARRAY, 1, null);
        vertexBuffer.setDefaultColor(RGBA);
        vertexBuffer.setTexCoords( 0, TEXCOORD_ARRAY, 1.0f, null  ); 


       Mesh mesh = new Mesh(vertexBuffer, indexBuffer,null);
       mesh.setAppearance (0,appearance);
       return mesh;


  }
}
