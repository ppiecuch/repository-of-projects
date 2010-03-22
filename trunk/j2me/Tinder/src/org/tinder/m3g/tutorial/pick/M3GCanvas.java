package org.tinder.m3g.tutorial.pick;


import javax.microedition.lcdui.*;
import javax.microedition.m3g.*;
import javax.microedition.lcdui.game.*;

class M3GCanvas  extends GameCanvas implements Runnable
{
    private Graphics3D g3d; // Graphics object used to render the 3d world.
    private Graphics g2d; 
    private World world; // This world contains the camera and the pyramidMesh.

    private Mesh cube;


    private Image pointerImage = null;
    private int POINTER_X=0, POINTER_Y=0;
    private RayIntersection ri;
    private Node pickedNode;
    private Appearance oldAppearance;
    private Appearance newAppearance;
    private Camera camera;
    private final int WIDTH, HEIGHT; 
    private float p1,p2;
    private float distance;
    private float px,py,pz;
    private float nx,ny,nz;
    private float s,t;
    private int index; 
    private float [] ray = new float[6]; 

    

    public M3GCanvas(){
        super(false);
        WIDTH = getWidth();
        HEIGHT = getHeight();
 
        POINTER_X = WIDTH/2;
       POINTER_Y = HEIGHT/2;
       POINTER_X = 50;
       POINTER_Y = 55;
        camera = new Camera();
         float aspectRatio = ((float) WIDTH) / ((float) HEIGHT);
        camera.setPerspective(70.0f, aspectRatio, 0.1f, 50.0f);
        camera.translate(0,0,6);
        world = new World();
        setFullScreenMode(false);
        world.addChild(camera );
        world.setActiveCamera(camera ); 
        g3d = Graphics3D.getInstance();  

        int i ,j;
        for(i = 1;i<4;i++){
        	for ( j = 0;j<2;j++){
        		cube = createCube();
        		cube.translate(1.5f*(float)Math.cos(j*Math.PI),2.0f*(float)Math.sin(i*Math.PI/2),0);
        		world.addChild(cube);
       		
        		}
        		
        	}
        
        try{
             pointerImage = Image.createImage("/pointer.png");
        }catch(Exception e){
            System.out.println("Failed to load image");
        }
        ri = new RayIntersection();
       // world.pick(-1, POINTER_X,POINTER_Y, world.getActiveCamera(), ray);      
        
        oldAppearance = null;
        Fog fog = new Fog();
        fog.setColor(0xFFFF0000);
        fog.setMode(Fog.LINEAR);
        fog.setDensity(100); 
        fog.setLinear(-1.0f, 1.0f);
        newAppearance = new Appearance();
        newAppearance.setFog(fog);
        Thread t = new Thread(this);
        t.start();
    }
    public void run() {
        g2d = getGraphics();
        while(true){
        try{
            g3d.bindTarget(g2d); // Binds the given Graphics or mutable Image2D as the rendering target of this Graphics3D
            //g3d.clear(null);
            p1 = (float)POINTER_X / (float)WIDTH;
            p2 = (float)POINTER_Y / (float)HEIGHT;
             world.pick(-1, p1,p2, camera, ri);    
         //   boolean a = world.pick(-1, POINTER_X,POINTER_Y, world.getActiveCamera(), ray);    
            
            if (ri.getIntersected() != null)
            {
            	
                  
                  distance = ri.getDistance();
               //   distance = ((int)((distance+0.005)*100.0f))/100.0f;
                  index = ri.getSubmeshIndex();
                  ri.getRay(ray);
                  px = ray[0] + ray[3] * ri.getDistance();
                  py = ray[1] + ray[4] * ri.getDistance();
                  pz = ray[2] + ray[5] * ri.getDistance();
              //    distance = distance*(float)Math.sqrt((double)(ray[3]*ray[3]+ray[4]*ray[4]+ray[5]*ray[5]));
               //   px = ((int)((px+0.005)*100.0f))/100.0f;
              //    py = ((int)((px+0.005)*100.0f))/100.0f;
              //    pz = ((int)((px+0.005)*100.0f))/100.0f;
                  nx = ri.getNormalX();
                  ny = ri.getNormalY();
                  nz = ri.getNormalZ();
              //    nx = ((int)((nx+0.005)*100.0f))/100.0f;
              //    ny = ((int)((nx+0.005)*100.0f))/100.0f;
              //    nz = ((int)((nx+0.005)*100.0f))/100.0f;
                  

               pickedNode = ri.getIntersected();

               if (pickedNode instanceof Mesh)
               {
                  oldAppearance = ((Mesh)pickedNode).getAppearance(0);
                  ((Mesh)pickedNode).setAppearance(0, newAppearance);
              }
            }
            g3d.render(world);

        }finally{
            g3d.releaseTarget();
        }
            g2d.setColor(255,255,255);
            g2d.drawString( "Distance: "+distance , 5,5, Graphics.TOP|Graphics.LEFT);
            g2d.drawString( "SubMeshindex: "+index, 5,18, Graphics.TOP|Graphics.LEFT);
            g2d.drawString( "Pos: "+"("+px+","+py+","+pz+")", 5,31, Graphics.TOP|Graphics.LEFT);
          //  g2d.drawString( "Normal: "+"("+nx+","+ny+","+nz+")", 5,44, Graphics.TOP|Graphics.LEFT);
         //   g2d.drawString( "Origin: "+"("+ray[0]+","+ray[1]+","+ray[2]+")", 5,57, Graphics.TOP|Graphics.LEFT);
         //   g2d.drawString( "Direction: "+"("+ray[3]+","+ray[4]+","+ray[5]+")", 5,70, Graphics.TOP|Graphics.LEFT);
            g2d.drawImage(pointerImage, POINTER_X, POINTER_Y, Graphics.TOP|Graphics.LEFT);
            flushGraphics();
            if (pickedNode != null)
            {
               if (pickedNode instanceof Mesh)
               {
                  ((Mesh)pickedNode).setAppearance(0, oldAppearance);
               }
            }
        }
    }
  protected void keyPressed(int keyCode)
  {   
  	int gameAction = getGameAction(keyCode);
  	switch(gameAction) {
      case Canvas.UP:    POINTER_Y -=3;    break;
      case Canvas.DOWN:  POINTER_Y +=3;  break;
      case Canvas.LEFT:  POINTER_X -=3;  break;
      case Canvas.RIGHT: POINTER_X +=3; break;
      default: break;
    }
  }

  protected void keyRepeated(int keyCode)
  { 
  	keyPressed(keyCode);
  }

  private Mesh createCube(){
  	IndexBuffer indexBuffer;
    Appearance  appearance = new Appearance();
    VertexBuffer vertexBuffer = new VertexBuffer();
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
        COLOR_ARRAY = new VertexArray(COLORS.length / 3, 3, 1);
        COLOR_ARRAY.set(0, COLORS.length / 3, COLORS);
        TEXCOORD_ARRAY = new VertexArray( TEXCOORDS.length / 2, 2, 2 );
        TEXCOORD_ARRAY.set( 0, TEXCOORDS.length/2,TEXCOORDS );
        indexBuffer = new TriangleStripArray(INDICES, new int[] {INDICES.length});
        
        // VertexBuffer holds references to VertexArrays that contain the positions, colors, normals, 
        // and texture coordinates for a set of vertices

        vertexBuffer.setPositions(POSITION_ARRAY, 0.5f, null);
        vertexBuffer.setColors(COLOR_ARRAY);
        vertexBuffer.setTexCoords( 0, TEXCOORD_ARRAY, 1.0f, null  ); 


       Mesh mesh = new Mesh(vertexBuffer, indexBuffer,null);
       mesh.setAppearance (0,appearance);
       return mesh;


  }
}
