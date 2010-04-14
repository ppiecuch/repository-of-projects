package org.tinder.m3g.tutorial.collsion.aabb;



import javax.microedition.lcdui.*;
import javax.microedition.m3g.*;
import javax.microedition.lcdui.game.*;

class M3GCanvas  extends GameCanvas implements Runnable
{
    private Graphics3D g3d; // Graphics object used to render the 3d world.
    private Graphics g2d; 
    private World world; // This world contains the camera and the pyramidMesh.
    private Camera camera; // the camera in the scene
    private Mesh mesh1;
    private Mesh mesh2;
    private AABB box1;
    private AABB box2;
    private IndexBuffer indexBuffer;
    private boolean isCollided = false;
    private Appearance  appearance = new Appearance();
    private Transform cubeTransform = new Transform();
    private VertexBuffer vertexBuffer = new VertexBuffer();
    public M3GCanvas(){
        super(false);
        setFullScreenMode(false);
        world = new World();
        g3d = Graphics3D.getInstance();  
        camera = new Camera();
        float aspect = (float) getWidth() / (float) getHeight();
        camera.setPerspective(45.0f, aspect, 1.0f, 1000.0f);
        Transform cameraTransform = new Transform();
        cameraTransform.postTranslate(0, 0, 6.0f);
        camera.setTransform(cameraTransform);
        world.addChild(camera);
        world.setActiveCamera(camera) ;

        
        mesh1 = createCube(); 
        mesh1.setTranslation(1.0f, 0.0f,0.0f) ;
        mesh1.setOrientation(90,0.0f,1.0f,0.0f);
        mesh1.setScale(0.5f,0.5f,0.5f); 
        box1 = new AABB();
        box1.setMin(-1.0f,-1.0f,-1.0f);
        box1.setMax(1.0f,1.0f,1.0f); 
        mesh1.getCompositeTransform(cubeTransform);  
        box1.setToTransformedBox(cubeTransform);     
        world.addChild(mesh1);
        
        mesh2 = createCube();
        mesh2.setTranslation(-0.5f, 0.0f,0.0f) ;
        mesh2.setScale(0.5f,0.5f,0.5f);
        box2 = new AABB();
        box2.setMin(-1.0f,-1.0f,-1.0f);//设置包装盒2的最小顶点
        box2.setMax(1.0f,1.0f,1.0f); //设置包装盒2的最大顶点
        mesh2.getCompositeTransform(cubeTransform); //获取立方体2的混合矩阵
        box2.setToTransformedBox(cubeTransform); //将变换矩阵应用到包装盒2中
        world.addChild(mesh2);
     //   mesh.setOrientation(45,1.0f,1.0f,0.0f);
     
        
        float[] speed = new float []{3.0f,0.0f,0.0f};
        float tEnter = intersectMovingAABB(box1,box2,speed);
        System.out.println(tEnter);
        Thread t = new Thread(this);
        t.start();
    }
    public void run() {
        g2d = getGraphics();
        Transform transform=new Transform();
    	transform.postTranslate(-0.001f,0,0);
        while(true){
        	mesh1.translate(-0.001f,0,0);
            box1.setToTransformedBox(transform);    
        	isCollided = box1.intersectAABBs(box2,null);
	        try{
	            g3d.bindTarget(g2d); // Binds the given Graphics or mutable Image2D as the rendering target of this Graphics3D
	            g3d.render(world);
	        }finally{
	            g3d.releaseTarget();
	        }
	        g2d.setColor(255,0,0);
	        String str = "collision:   "+isCollided;
	        g2d.drawString( str, 5,5, Graphics.TOP|Graphics.LEFT);
	        flushGraphics();
        }
    }


  private Mesh createCube(){
  	/** The cube's vertex positions (x, y, z). */
  	  byte []POINTS = new byte[] {    
  	     -1, -1,  1,    1, -1,  1,   -1,  1,  1,    1,  1,  1,
         -1, -1, -1,    1, -1, -1,   -1,  1, -1,    1,  1, -1};
                                                            
                                                            
        /** Indices that define how to connect the vertices to build 
   * triangles. */
       
        byte []COLORS = new byte[] { 0, 0, (byte) 128,             0, 0, (byte) 255,
 																		  0, (byte) 255, 0,             0, (byte) 255, (byte) 255,
 																		  (byte) 255, 0, 0,             (byte) 255, 0, (byte) 255,
  																   (byte) 255, (byte) 255, 0,    (byte) 255, (byte) 255, (byte) 255};//B
                                                        
        int []INDICES = new int[]{
			    0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1
        };
          
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

       Mesh mesh = new Mesh(vertexBuffer, indexBuffer,appearance);
       return mesh;
      }



float intersectMovingAABB(AABB stationaryBox,AABB movingBox,float []s) 
{
// We'll return this huge number if no intersection
float NoIntersection = 1e30f;
// Initialize interval to contain all the time under consideration
float tEnter = 0.0f;
float tLeave = 1.0f;
float Swap = 0.0f;
//
// Compute interval of overlap on each dimension, and intersect
// this interval with the interval accumulated so far. As soon as
// an empty interval is detected, return a negative result
// (no intersection). In each case, we have to be careful for
// an infinite of empty interval on each dimension.
//
// Check x-axis
float [] sBoxmin= stationaryBox.getMin();
float [] sBoxmax= stationaryBox.getMax();
float [] mBoxmin= movingBox.getMin();
float [] mBoxmax= movingBox.getMax();

if (s[0] == 0.0f) {
// Empty or infinite inverval on x
if (
(sBoxmin[0] >= mBoxmax[0]) ||(sBoxmax[0] <= mBoxmin[0])
) {
// Empty time interval so no intersection
return NoIntersection;
}
// Inifinite time interval - no update necessary
} else {

// Compute time value when they begin and end overlapping
float xEnter = (sBoxmin[0]-mBoxmax[0])/s[0];

float xLeave = (sBoxmax[0]-mBoxmin[0])/ s[0];

// Check for interval out of order
if (xEnter > xLeave) {
	Swap = xEnter;
	xEnter = xLeave;
	xLeave = Swap;
}
// Update interval
if (xEnter > tEnter) tEnter = xEnter;
if (xLeave < tLeave) tLeave = xLeave;
// Check if this resulted in empty interval
if (tEnter > tLeave) {
return NoIntersection;
}
}
// Check y-axis
if (s[1] == 0.0f) {
// Empty or infinite inverval on y
if (
(sBoxmin[1] >= mBoxmax[1]) || (sBoxmax[1] <= mBoxmin[1])
) {
// Empty time interval, so no intersection
return NoIntersection;
}
// Infinite time interval - no update necessary
} else {

// Compute time value when they begin and end overlapping
float yEnter = (sBoxmin[1]-mBoxmax[1]) / s[1];
float yLeave = (sBoxmax[1]-mBoxmin[1]) / s[1];
// Check for interval out of order
if (yEnter > yLeave) {
	Swap = yEnter;
	yEnter = yLeave;
	yLeave = Swap;
}
// Update interval
if (yEnter > tEnter) tEnter = yEnter;
if (yLeave < tLeave) tLeave = yLeave;
// Check if this resulted in empty interval

if (tEnter > tLeave) {
return NoIntersection;
}
}
// Check z-axis
if (s[2] == 0.0f) {
// Empty or infinite inverval on z
if (
(sBoxmin[2] >= mBoxmax[2]) ||
(sBoxmax[2] <= mBoxmin[2])
) {
// Empty time interval, so no intersection
return NoIntersection;
}
// Infinite time interval - no update necessary
} else {
// Divide once
float oneOverD = 1.0f / s[2];
// Compute time value when they begin and end overlapping
float zEnter = (sBoxmin[2]-mBoxmax[2]) / s[2];
float zLeave = (sBoxmax[2]- mBoxmin[2]) / s[2];
// Check for interval out of order
if (zEnter > zLeave) {
	Swap = zEnter;
	zEnter = zLeave;
	zLeave = Swap;
}
// Update interval
if (zEnter > tEnter) tEnter = zEnter;
if (zLeave < tLeave) tLeave = zLeave;
// Check if this resulted in empty interval
if (tEnter > tLeave) {
return NoIntersection;
}
}
// OK, we have an intersection. Return the parametric point in time
// where the intersection occurs
return tEnter;
}


}
