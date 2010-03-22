package org.tinder.m3g.tutorial.abc04;

import javax.microedition.lcdui.*;
import javax.microedition.m3g.*;
import javax.microedition.lcdui.game.*;

public class M3GCanvas2 extends GameCanvas implements Runnable 
{


  private static final float X_CAMERA = 0.0f; 
  private static final float Y_CAMERA = 0.0f; 
  private static final float Z_CAMERA = 3.0f; 
    // camera coordinates; change as required
  private static final float MODEL_MOVE = 0.2f; 
    // distance to move the model up/down/left/right/fwd/back for a key press
  private static final float MODEL_ROT = 5.0f;     // 5 degrees
    // angle to rotate the model left/right for a key press
  private Graphics3D g3d;
  private Camera camera;
  private Light light;
  private Background bg;
  private VertexBuffer vertexBuffer; 
  private IndexBuffer indexBuffer;   
  private Appearance appearance;

  private Transform camTrans, modelTrans;
    // transforms used by the camera and the model

  // model position and angle information
  private float xTrans, yTrans, zTrans;
  private int totalDegrees;




  public M3GCanvas2()
  {
    super(true);


    try {
      createScene();
    }
    catch (Exception e) 
    {  e.printStackTrace(); }
    Thread t = new Thread(this);
    t.start();
  } // end of SwordCanvas()





  private void createScene() throws Exception
  {
    g3d = Graphics3D.getInstance();
    // create a camera
    camera = new Camera();
    float aspectRatio = ((float) getWidth()) / ((float) getHeight());
    camera.setPerspective(45.0f, aspectRatio, 0.1f, 50.0f);
    // was 60, aspectRatio, 1, 1000
    // set up the camera's position
    camTrans = new Transform();
    camTrans.postTranslate(X_CAMERA, Y_CAMERA, Z_CAMERA);
    // create a light
    light = new Light();
    light.setColor(0xffffff);  // white light
    light.setIntensity(1.25f); // over bright
    // initialise the model's transform vars
    modelTrans = new Transform();
    xTrans = 0.0f; yTrans = 0.0f; zTrans = 0.0f;
    totalDegrees = 0;
    makeGeometry();
    makeAppearance();
    bg = new Background();
    bg.setColor(0x9EFEFE); // light blue 
  } // end of createScene()
  // ------------------- model creation -----------------------------
  private void makeGeometry()
  /* Read in the values for the vertices, normals, texture coords,
     and colour coords, and create a VertexBuffer object. Also
     read in the strip lengths array to create the index for the
     triangle strips used in the vertex buffer.

     Depending on the model, there may not be any texture or colour
     coordinates, and so the calls related to them should be commented
     out.

     The texture coordinates can use 2, 3, or 4 components, and the
     colour coordinates 3 or 4, which will require changes to the array
     creation code. The code here assumes (s,t) format for texture coords
     (2 components) and RGB for the colours (3 components), the most common 
     formats.
  */
  {
    // create vertices
    short[] POINTS = getVerts();
    VertexArray POSITION_ARRAY  = new VertexArray(POINTS.length/3, 3, 2);
    POSITION_ARRAY.set(0, POINTS.length/3,POINTS);

    // create normals
    byte[] NORMALS = getNormals();
    VertexArray NORMAL_ARRAY = new VertexArray(NORMALS.length/3, 3, 1);
    NORMAL_ARRAY.set(0, NORMALS.length/3, NORMALS);

    // create texture coordinates
    short[] TEXCOORDS = getTexCoords();
    VertexArray TEXCOORD_ARRAY = new VertexArray(TEXCOORDS.length/2, 2, 2);
                          // this assumes (s,t) texture coordinates
    TEXCOORD_ARRAY.set(0, TEXCOORDS.length/2, TEXCOORDS);
    vertexBuffer = new VertexBuffer();
    float[] pbias = {(1.0f/255.0f), (1.0f/255.0f), (1.0f/255.0f)};
    vertexBuffer.setPositions(POSITION_ARRAY, (2.0f/255.0f), pbias); // scale, bias
       // fix the scale and bias to create points in range [-1 to 1]

    vertexBuffer.setNormals(NORMAL_ARRAY);
    vertexBuffer.setTexCoords(0, TEXCOORD_ARRAY, (1.0f/255.0f), null);
    int[] LENGTHS = {82, 33, 6, 24, 60, 6, 50, 16, 182, 47};
     indexBuffer = new TriangleStripArray(0, LENGTHS);
  }  // end of makeGeometry()

  private void makeAppearance() throws Exception
  {
    // load the image for the texture
    Image im = Image.createImage("/swordTextures.png");

    // create an Image2D for the Texture2D
    Image2D image2D = new Image2D(Image2D.RGB, im);

    // create the Texture2D and enable mip mapping
    // the texture color is modulated with the lit material color
    Texture2D texture = new Texture2D(image2D);
    texture.setFiltering(Texture2D.FILTER_NEAREST, Texture2D.FILTER_NEAREST);
    texture.setWrapping(Texture2D.WRAP_CLAMP, Texture2D.WRAP_CLAMP);
    // texture.setWrapping(Texture2D.WRAP_REPEAT, Texture2D.WRAP_REPEAT);
    texture.setBlending(Texture2D.FUNC_MODULATE);
    // create the appearance
    appearance = new Appearance();
    appearance.setTexture(0, texture);
    Material material = new Material();

    material.setColor(Material.AMBIENT, 0x00000000);
    material.setColor(Material.EMISSIVE, 0x00000000);
    material.setColor(Material.DIFFUSE, 0xFFFFFFFF);
    material.setColor(Material.SPECULAR, 0x00000000);
    material.setShininess(1.0f);
    appearance.setMaterial(material);
  }  // end of makeAppearance()


  private void updateModelTrans()
  // the model's transform = a translation * a rotation
  {
    modelTrans.setIdentity();   // reset
    modelTrans.postTranslate(xTrans, yTrans, zTrans);
    modelTrans.postRotate(totalDegrees, 0, 1, 0);  // around y-axis
   }  // end of updateModelTrans()



  // -----------------key operation methods ---------------------


  public void keyRepeated(int keyCode)
  {
    int gameAction = getGameAction(keyCode);
    if (hasRepeatEvents())
      performAction( gameAction );
  }


  protected void keyPressed(int keyCode)
  {
    int gameAction = getGameAction(keyCode);
    if (!hasRepeatEvents())
      performAction( gameAction );
  } // end of keyPressed()




  public void performAction(int gameAction) 
  // called by KeyRepeat object or the keyRepeated() method
  {
    if (gameAction == UP) {
      moveModel(0,MODEL_MOVE,0);  // y up
    }
    else if (gameAction == DOWN) {
      moveModel(0,-MODEL_MOVE,0);  // y down
    }
    else if (gameAction == LEFT) {
      moveModel(-MODEL_MOVE,0,0);   // x left
    }
    else if (gameAction == RIGHT) {
      moveModel(MODEL_MOVE,0,0);   // x right
    }
    else if (gameAction == GAME_A) {
      // System.out.println("fwd");
      moveModel(0,0,MODEL_MOVE);   // z fwd
    }
    else if (gameAction == GAME_B) {
      // System.out.println("back");
      moveModel(0,0,-MODEL_MOVE);   // z back
    }
    else if (gameAction == GAME_C) {
      // System.out.println("rot left");
      rotYModel(-MODEL_ROT);   // rotate left
    }
    else if (gameAction == GAME_D) {
      // System.out.println("rot right");
      rotYModel(MODEL_ROT);   // rotate right
    }
  } // end of keyPressed()


  private void moveModel(float x, float y, float z)
  // update the model's translation values
  { xTrans += x; yTrans += y; zTrans += z;
    repaint();
  }


  private void rotYModel(float degrees)
  // update the model's y-axis rotation value
  { totalDegrees += degrees;
    repaint();
  }  // end of rotYModel()


  // ----------------- data-specific methods added here -------
/* 
  The following methods can be pasted in from the ObjView application
  output stored in examObj.txt:

  private short[] getVerts() {...}
  private byte[] getNormals() {...}
  private short[] getTexCoords() (...}
  private short[] getColourCoords() (...}
  private int[] getStripLengths() {...}
  private Material setMatColours() {...}
*/
    public void run() {
        Graphics g = getGraphics();
        while(true) {

        g.setColor( 255, 255, 255 );
       g.fillRect( 0, 0, getWidth(), getHeight() ); 
       g3d = Graphics3D.getInstance();
  
    g3d.bindTarget(g, true, Graphics3D.DITHER | Graphics3D.TRUE_COLOR);

    g3d.clear(bg);    // clear the color and depth buffers

    g3d.setCamera(camera, camTrans);   // position the camera

    // set up a "headlight": a directional light shining
    // from the direction of the camera
    g3d.resetLights();
    g3d.addLight(light, camTrans);

    updateModelTrans();

    // Render the model. We provide the vertex and index buffers
    // to specify the geometry; the appearance so we know what
    // material and texture to use; and a tranform to position
    // the model
    g3d.render(vertexBuffer, indexBuffer, appearance, modelTrans);

    g3d.releaseTarget();      // flush
    flushGraphics();
     try {
                Thread.sleep(200); 
            }
            catch (InterruptedException e) {
                
    }
  }
        
           
    
    }

// Methods for model in sword.obj

  private short[] getVerts()
  // return an array holding Verts [1518 values / 3 = 506 points] 
  {
    short[] POINTS = {
      	-3,60,-6,  	-1,60,-7,  	-1,60,-2,  	-1,68,-2,  
	-15,67,-2,  	-1,68,-2,  	-1,68,-7,  	-1,60,-7,  
	-1,68,-7,  	-15,67,-7,  	-15,67,-2,  	-15,60,-2,  
	-1,60,-2,  	-15,60,-2,  	-14,60,-4,  	-15,60,-2,  
	-15,60,-7,  	-15,67,-7,  	-15,60,-7,  	-1,60,-7,  
	-15,60,-7,  	-3,60,-6,  	-14,60,-4,  	-13,32,-4,  
	-14,60,-4,  	-3,60,-4,  	-1,60,-2,  	-3,60,-4,  
	-3,60,-6,  	-2,33,-6,  	-13,32,-4,  	-2,33,-6,  
	0,5,-6,  	-2,33,-6,  	-2,33,-4,  	-3,60,-4,  
	-2,33,-4,  	-13,32,-4,  	-2,33,-4,  	0,5,-4,  
	0,5,-6,  	2,-21,-6,  	0,5,-6,  	-11,3,-4,  
	-13,32,-4,  	-11,3,-4,  	0,5,-4,  	2,-21,-4,  
	2,-21,-6,  	5,-48,-6,  	2,-21,-6,  	-8,-26,-4,  
	-11,3,-4,  	-8,-26,-4,  	2,-21,-4,  	5,-48,-4,  
	5,-48,-6,  	10,-74,-6,  	5,-48,-6,  	-3,-54,-4,  
	-8,-26,-4,  	-3,-54,-4,  	5,-48,-4,  	10,-74,-4,  
	10,-74,-6,  	14,-101,-6,  	10,-74,-6,  	1,-80,-4,  
	-3,-54,-4,  	1,-80,-4,  	10,-74,-4,  	14,-101,-4,  
	14,-101,-6,  	21,-128,-4,  	11,-122,-4,  	14,-101,-4,  
	6,-103,-4,  	1,-80,-4,  	6,-103,-4,  	14,-101,-6,  
	6,-103,-4,  	11,-122,-4,  	-14,68,3,  	-13,68,4,  
	-13,67,4,  	-13,68,4,  	-13,67,5,  	-13,68,5,  
	-13,67,5,  	-12,67,5,  	-12,67,6,  	-12,67,5,  
	-11,67,6,  	-12,67,5,  	-10,67,6,  	-10,67,5,  
	-10,68,5,  	-10,68,4,  	-12,68,5,  	-10,68,4,  
	-11,68,4,  	-10,68,4,  	-10,67,4,  	-10,67,5,  
	-10,67,4,  	-12,67,5,  	-10,67,4,  	-11,67,4,  
	-11,68,4,  	-11,67,4,  	-12,68,5,  	-12,67,5,  
	-12,68,5,  	-12,67,5,  	-13,68,5,  	7,68,-5,  
	9,68,-5,  	6,68,-13,  	9,67,-5,  	6,67,-13,  
	7,67,-5,  	6,67,-13,  	0,68,-19,  	6,68,-13,  
	4,68,-13,  	6,68,-13,  	6,68,-9,  	7,68,-5,  
	6,67,-9,  	7,67,-5,  	6,67,-9,  	6,67,-13,  
	6,67,-9,  	4,67,-13,  	6,68,-9,  	4,67,-13,  
	4,68,-13,  	0,67,-17,  	4,68,-13,  	0,68,-17,  
	0,68,-19,  	0,68,-17,  	-2,68,-18,  	-2,67,-18,  
	-6,67,-19,  	0,68,-17,  	-2,67,-18,  	0,67,-17,  
	0,67,-19,  	4,67,-13,  	0,67,-19,  	6,67,-13,  
	0,67,-19,  	0,68,-19,  	-8,68,-21,  	-2,68,-18,  
	-6,68,-19,  	-6,67,-19,  	-10,67,-18,  	-6,67,-19,  
	-8,67,-21,  	-2,67,-18,  	-8,67,-21,  	0,67,-19,  
	-8,67,-21,  	-8,68,-21,  	-16,68,-19,  	-8,68,-21,  
	-10,68,-18,  	-6,68,-19,  	-10,68,-18,  	-10,67,-18,  
	-12,67,-15,  	-10,67,-18,  	-16,67,-19,  	-8,67,-21,  
	-16,67,-19,  	-16,68,-19,  	-22,68,-13,  	-16,68,-19,  
	-12,68,-15,  	-10,68,-18,  	-12,68,-15,  	-12,67,-15,  
	-12,67,-13,  	-12,67,-15,  	-22,67,-13,  	-16,67,-19,  
	-22,67,-13,  	-22,68,-13,  	-24,68,-5,  	-22,68,-13,  
	-22,68,-5,  	-22,68,-13,  	-12,68,-13,  	-12,68,-15,  
	-12,68,-13,  	-12,67,-13,  	-11,67,-11,  	-12,67,-13,  
	-22,67,-5,  	-22,67,-13,  	-22,67,-5,  	-24,67,-5,  
	-22,67,-1,  	-22,67,-13,  	-24,67,-5,  	-24,68,-5,  
	-22,68,3,  	-22,68,-1,  	-21,68,4,  	-24,68,-5,  
	-22,68,-1,  	-22,68,-5,  	-22,67,-1,  	-22,68,-5,  
	-22,67,-5,  	-21,68,0,  	-22,67,-5,  	-21,67,0,  
	-11,67,-11,  	-21,67,0,  	-17,67,4,  	-17,68,4,  
	-17,67,4,  	-12,68,6,  	-12,67,6,  	-11,68,6,  
	-11,67,6,  	-10,68,6,  	-10,67,6,  	-10,68,6,  
	-10,68,5,  	-10,68,6,  	-12,68,5,  	-11,68,6,  
	-12,68,5,  	-12,68,6,  	-13,68,5,  	-12,68,6,  
	-13,68,4,  	-17,68,4,  	-14,68,3,  	-17,68,4,  
	-13,68,2,  	-12,68,1,  	-13,67,2,  	-12,67,1,  
	-13,67,2,  	-17,67,4,  	-13,67,2,  	-14,67,3,  
	-13,68,2,  	-14,67,3,  	-14,68,3,  	-14,67,3,  
	-13,67,4,  	-17,67,4,  	-13,67,4,  	-12,67,6,  
	-13,67,5,  	-21,67,0,  	-17,68,4,  	-21,68,0,  
	-11,68,-11,  	-22,68,-5,  	-11,68,-11,  	-12,68,-13,  
	-11,68,-11,  	-11,67,-11,  	-9,67,-10,  	-17,67,4,  
	-9,67,-10,  	-12,67,1,  	-10,67,1,  	-10,68,1,  
	-8,68,1,  	-10,68,10,  	-6,68,9,  	-6,67,9,  
	-4,67,6,  	-6,67,9,  	0,67,9,  	-8,67,11,  
	0,68,9,  	-8,67,11,  	-8,68,11,  	-8,67,11,  
	-16,67,9,  	-8,67,11,  	-10,67,10,  	-6,67,9,  
	-10,67,10,  	-10,68,10,  	-14,68,9,  	-16,68,9,  
	-14,68,9,  	-17,68,7,  	-14,68,9,  	-14,67,9,  
	-10,67,10,  	-14,67,9,  	-16,67,9,  	-14,67,9,  
	-17,67,7,  	-17,68,7,  	-21,68,4,  	-17,68,7,  
	-22,68,3,  	-16,68,9,  	-22,68,3,  	-22,67,3,  
	-24,67,-5,  	-22,67,3,  	-22,67,-1,  	-21,67,4,  
	-22,68,-1,  	-21,67,4,  	-21,68,4,  	-21,67,4,  
	-17,67,7,  	-22,67,3,  	-16,67,9,  	-16,68,9,  
	-8,68,11,  	-10,68,10,  	-8,68,11,  	-6,68,9,  
	0,68,9,  	-6,68,9,  	-4,68,6,  	-4,67,6,  
	-4,67,3,  	-4,67,6,  	6,67,3,  	0,67,9,  
	6,68,3,  	0,68,9,  	6,68,3,  	-4,68,6,  
	-4,68,3,  	-4,67,3,  	-5,67,2,  	-4,67,3,  
	7,67,-5,  	6,67,3,  	9,67,-5,  	6,67,3,  
	9,68,-5,  	6,68,3,  	7,68,-5,  	-4,68,3,  
	-5,68,2,  	-5,67,2,  	-8,67,1,  	-5,67,2,  
	0,67,-14,  	-5,67,2,  	4,67,-9,  	7,67,-5,  
	4,68,-9,  	7,68,-5,  	4,68,-9,  	-5,68,2,  
	4,68,-9,  	0,68,-14,  	4,67,-9,  	0,68,-14,  
	0,67,-14,  	-5,68,-16,  	-5,67,-16,  	-6,68,-16,  
	-6,67,-16,  	-7,68,-15,  	-7,67,-15,  	-7,68,-15,  
	-7,68,-14,  	-7,68,-15,  	-6,68,-13,  	-6,68,-16,  
	-5,68,-14,  	-6,68,-16,  	-5,68,-15,  	-5,68,-16,  
	-5,68,-15,  	0,68,-14,  	-4,68,-15,  	-3,68,-14,  
	-4,68,-15,  	-4,67,-15,  	-4,68,-15,  	-5,67,-15,  
	-5,68,-15,  	-5,67,-15,  	-5,68,-15,  	-5,67,-14,  
	-5,68,-14,  	-6,67,-13,  	-6,68,-13,  	-7,67,-14,  
	-7,68,-14,  	-7,67,-14,  	-7,67,-15,  	-6,67,-13,  
	-6,67,-16,  	-5,67,-14,  	-6,67,-16,  	-5,67,-15,  
	-5,67,-16,  	-5,67,-15,  	0,67,-14,  	-4,67,-15,  
	-3,67,-14,  	-3,68,-14,  	-3,68,-13,  	0,68,-14,  
	-3,68,-13,  	-3,68,-11,  	-3,68,-13,  	-3,67,-13,  
	-3,67,-14,  	-3,67,-13,  	0,67,-14,  	-3,67,-13,  
	-3,67,-11,  	-3,68,-11,  	-4,68,-11,  	-3,68,-11,  
	-10,68,1,  	0,68,-14,  	-8,68,1,  	-5,68,2,  
	-8,68,1,  	-8,67,1,  	-10,67,1,  	0,67,-14,  
	-10,67,1,  	-3,67,-11,  	-4,67,-11,  	-4,68,-11,  
	-7,68,-10,  	-10,68,1,  	-7,68,-10,  	-9,68,-10,  
	-7,68,-10,  	-7,67,-10,  	-4,67,-11,  	-7,67,-10,  
	-10,67,1,  	-7,67,-10,  	-9,67,-10,  	-9,68,-10,  
	-11,68,-11,  	-9,68,-10,  	-17,68,4,  	-9,68,-10,  
	-12,68,1,  	-10,68,1,  	-12,67,1,  	-8,127,-1,  
	-8,127,-7,  	-12,127,-1,  	-12,127,-7,  	-12,127,-1,  
	-15,125,-1,  	-8,127,-1,  	-3,126,-1,  	-8,127,-1,  
	-3,126,-7,  	-8,127,-7,  	-15,125,-7,  	-12,127,-7,  
	-15,125,-7,  	-15,125,-1,  	-15,107,-1,  	-15,125,-1,  
	-3,107,-1,  	-3,126,-1,  	-3,107,-7,  	-3,126,-7,  
	-3,107,-7,  	-15,125,-7,  	-15,107,-7,  	-15,107,-1,  
	-15,107,-7,  	-15,86,-1,  	-15,107,-7,  	-15,86,-7,  
	-15,107,-7,  	-3,87,-7,  	-3,107,-7,  	-3,87,-7,  
	-3,107,-1,  	-3,87,-1,  	-15,107,-1,  	-3,87,-1,  
	-15,86,-1,  	-2,68,-1,  	-15,86,-1,  	-15,68,-1,  
	-15,86,-7,  	-15,68,-7,  	-3,87,-7,  	-2,68,-7,  
	-3,87,-1,  	-2,68,-1      };
    return POINTS;
  }  // end of getVerts()


  private byte[] getNormals()
  // return an array holding Normals [1518 values / 3 = 506 points] 
  {
    byte[] NORMALS = {
      	23,-113,-56,  	75,-74,-74,  	29,-112,57,  	105,54,54,  
	-47,85,86,  	105,54,54,  	48,108,-53,  	75,-74,-74,  
	48,108,-53,  	-87,43,-84,  	-47,85,86,  	-85,-85,44,  
	29,-112,57,  	-85,-85,44,  	-5,-128,1,  	-85,-85,44,  
	-38,-116,-38,  	-87,43,-84,  	-38,-116,-38,  	75,-74,-74,  
	-38,-116,-38,  	23,-113,-56,  	-5,-128,1,  	-128,-7,4,  
	-5,-128,1,  	73,-73,78,  	29,-112,57,  	73,-73,78,  
	23,-113,-56,  	105,6,-74,  	-128,-7,4,  	105,6,-74,  
	86,7,-96,  	105,6,-74,  	106,7,74,  	73,-73,78,  
	106,7,74,  	-128,-7,4,  	106,7,74,  	89,9,94,  
	86,7,-96,  	86,10,-96,  	86,7,-96,  	-128,-11,4,  
	-128,-7,4,  	-128,-11,4,  	89,9,94,  	89,11,95,  
	86,10,-96,  	85,12,-96,  	86,10,-96,  	-127,-16,5,  
	-128,-11,4,  	-127,-16,5,  	89,11,95,  	88,14,95,  
	85,12,-96,  	85,15,-96,  	85,12,-96,  	-126,-20,5,  
	-127,-16,5,  	-126,-20,5,  	88,14,95,  	88,15,95,  
	85,15,-96,  	69,12,-108,  	85,15,-96,  	-126,-23,5,  
	-126,-20,5,  	-126,-23,5,  	88,15,95,  	52,10,119,  
	69,12,-108,  	127,21,1,  	-113,-59,5,  	52,10,119,  
	-124,-30,6,  	-126,-23,5,  	-124,-30,6,  	69,12,-108,  
	-124,-30,6,  	-113,-59,5,  	103,77,-16,  	121,42,-18,  
	117,-41,-37,  	121,42,-18,  	116,1,-57,  	58,1,-115,  
	116,1,-57,  	1,-90,-90,  	-14,-72,106,  	1,-90,-90,  
	28,-91,86,  	-15,-112,-60,  	97,-78,36,  	91,-74,-52,  
	102,76,-26,  	54,76,-90,  	-31,113,-54,  	54,76,-90,  
	-30,44,-117,  	54,76,-90,  	27,-74,-101,  	91,-74,-52,  
	27,-74,-101,  	-15,-112,-60,  	27,-74,-101,  	-61,-43,-104,  
	-30,44,-117,  	-61,-43,-104,  	-31,113,-54,  	-15,-112,-60,  
	1,92,-90,  	1,-90,-90,  	58,1,-115,  	-23,127,-4,  
	106,74,12,  	69,105,-33,  	106,-73,-10,  	62,-103,-44,  
	16,-126,-17,  	62,-103,-44,  	44,105,-62,  	69,105,-33,  
	-70,92,58,  	69,105,-33,  	-99,73,38,  	-23,127,-4,  
	-103,-71,27,  	16,-126,-17,  	-103,-71,27,  	62,-103,-44,  
	-103,-71,27,  	-76,-91,50,  	-99,73,38,  	-76,-91,50,  
	-70,92,58,  	-66,-71,84,  	-70,92,58,  	-57,73,91,  
	44,105,-62,  	-57,73,91,  	-23,92,88,  	-32,-91,85,  
	-1,-72,107,  	-57,73,91,  	-32,-91,85,  	-66,-71,84,  
	31,-104,-69,  	-76,-91,50,  	31,-104,-69,  	62,-103,-44,  
	31,-104,-69,  	44,105,-62,  	7,105,-75,  	-23,92,88,  
	15,73,106,  	-1,-72,107,  	43,-93,79,  	-1,-72,107,  
	-5,-103,-75,  	-32,-91,85,  	-5,-103,-75,  	31,-104,-69,  
	-5,-103,-75,  	7,105,-75,  	-35,93,-81,  	7,105,-75,  
	59,94,68,  	15,73,106,  	59,94,68,  	43,-93,79,  
	82,-92,38,  	43,-93,79,  	-50,-92,-73,  	-5,-103,-75,  
	-50,-92,-73,  	-35,93,-81,  	-61,105,-44,  	-35,93,-81,  
	88,94,19,  	59,94,68,  	88,94,19,  	82,-92,38,  
	88,-92,-16,  	82,-92,38,  	-67,-103,-33,  	-50,-92,-73,  
	-67,-103,-33,  	-61,105,-44,  	-88,94,-8,  	-61,105,-44,  
	27,126,10,  	-61,105,-44,  	83,94,-34,  	88,94,19,  
	83,94,-34,  	88,-92,-16,  	51,-105,-54,  	88,-92,-16,  
	-21,-125,18,  	-67,-103,-33,  	-21,-125,18,  	-88,-92,10,  
	90,-91,-11,  	-67,-103,-33,  	-88,-92,10,  	-88,94,-8,  
	-67,105,34,  	88,93,-23,  	85,73,-65,  	-88,94,-8,  
	88,93,-23,  	27,126,10,  	90,-91,-11,  	27,126,10,  
	-21,-125,18,  	-94,73,50,  	-21,-125,18,  	-88,-71,60,  
	51,-105,-54,  	-88,-71,60,  	-34,-110,56,  	-42,112,50,  
	-34,-110,56,  	-28,73,103,  	-14,-72,106,  	14,93,90,  
	28,-91,86,  	75,79,71,  	97,-78,36,  	75,79,71,  
	102,76,-26,  	75,79,71,  	-31,113,-54,  	14,93,90,  
	1,92,-90,  	-28,73,103,  	58,1,-115,  	-28,73,103,  
	121,42,-18,  	-42,112,50,  	103,77,-16,  	-42,112,50,  
	101,1,81,  	50,42,113,  	81,1,101,  	45,-40,115,  
	81,1,101,  	-34,-110,56,  	81,1,101,  	103,-76,18,  
	101,1,81,  	103,-76,18,  	103,77,-16,  	103,-76,18,  
	117,-41,-37,  	-34,-110,56,  	117,-41,-37,  	-14,-72,106,  
	116,1,-57,  	-88,-71,60,  	-42,112,50,  	-94,73,50,  
	35,106,-65,  	27,126,10,  	35,106,-65,  	83,94,-34,  
	35,106,-65,  	51,-105,-54,  	13,-103,-75,  	-34,-110,56,  
	13,-103,-75,  	45,-40,115,  	7,-115,58,  	13,116,57,  
	-16,73,106,  	-3,92,-90,  	-54,95,-68,  	-33,-93,-80,  
	-80,-92,-37,  	-33,-93,-80,  	53,-92,74,  	10,-92,90,  
	41,93,81,  	10,-92,90,  	-8,94,90,  	10,-92,90,  
	-33,-103,69,  	10,-92,90,  	6,-91,-90,  	-33,-93,-80,  
	6,-91,-90,  	-3,92,-90,  	31,73,-102,  	-44,105,62,  
	31,73,-102,  	54,92,-73,  	31,73,-102,  	44,-72,-97,  
	6,-91,-90,  	44,-72,-97,  	-33,-103,69,  	44,-72,-97,  
	60,-91,-68,  	54,92,-73,  	85,73,-65,  	54,92,-73,  
	-67,105,34,  	-44,105,62,  	-67,105,34,  	-61,-103,45,  
	-88,-92,10,  	-61,-103,45,  	90,-91,-11,  	93,-72,-53,  
	88,93,-23,  	93,-72,-53,  	85,73,-65,  	93,-72,-53,  
	60,-91,-68,  	-61,-103,45,  	-33,-103,69,  	-44,105,62,  
	-8,94,90,  	-3,92,-90,  	-8,94,90,  	-54,95,-68,  
	41,93,81,  	-54,95,-68,  	-86,94,-18,  	-80,-92,-37,  
	-84,-93,23,  	-80,-92,-37,  	69,-103,34,  	53,-92,74,  
	62,105,45,  	41,93,81,  	62,105,45,  	-86,94,-18,  
	-75,95,45,  	-84,-93,23,  	-47,-103,60,  	-84,-93,23,  
	16,-126,-17,  	69,-103,34,  	106,-73,-10,  	69,-103,34,  
	106,74,12,  	62,105,45,  	-23,127,-4,  	-75,95,45,  
	-41,104,65,  	-47,-103,60,  	-32,-72,102,  	-47,-103,60,  
	26,-118,-42,  	-47,-103,60,  	86,-71,-64,  	16,-126,-17,  
	90,72,-58,  	-23,127,-4,  	90,72,-58,  	-41,104,65,  
	90,72,-58,  	33,120,-37,  	86,-71,-64,  	33,120,-37,  
	26,-118,-42,  	22,93,-87,  	11,-91,-89,  	-11,105,-74,  
	-23,-103,-72,  	-74,79,-69,  	-95,-78,-34,  	-74,79,-69,  
	-112,47,42,  	-74,79,-69,  	-5,100,83,  	-11,105,-74,  
	91,76,54,  	-11,105,-74,  	70,104,35,  	22,93,-87,  
	1,41,123,  	33,120,-37,  	-32,1,125,  	-90,76,54,  
	-32,1,125,  	-64,1,112,  	-32,1,125,  	1,-40,123,  
	1,41,123,  	35,-103,70,  	70,104,35,  	102,-74,27,  
	91,76,54,  	33,-98,77,  	-5,100,83,  	-87,-45,83,  
	-112,47,42,  	-87,-45,83,  	-95,-78,-34,  	33,-98,77,  
	-23,-103,-72,  	102,-74,27,  	-23,-103,-72,  	35,-103,70,  
	11,-91,-89,  	1,-40,123,  	26,-118,-42,  	-64,1,112,  
	-101,-74,27,  	-90,76,54,  	-106,72,1,  	33,120,-37,  
	-106,72,1,  	-84,95,-21,  	-106,72,1,  	-106,-71,1,  
	-101,-74,27,  	-106,-71,1,  	26,-118,-42,  	-106,-71,1,  
	-75,-93,-44,  	-84,95,-21,  	-60,74,-86,  	-84,95,-21,  
	13,116,57,  	33,120,-37,  	-16,73,106,  	-41,104,65,  
	-16,73,106,  	-32,-72,102,  	7,-115,58,  	26,-118,-42,  
	7,-115,58,  	-75,-93,-44,  	-43,-72,-96,  	-60,74,-86,  
	-17,73,-105,  	13,116,57,  	-17,73,-105,  	7,104,-76,  
	-17,73,-105,  	-8,-71,-106,  	-43,-72,-96,  	-8,-71,-106,  
	7,-115,58,  	-8,-71,-106,  	13,-103,-75,  	7,104,-76,  
	35,106,-65,  	7,104,-76,  	-42,112,50,  	7,104,-76,  
	50,42,113,  	13,116,57,  	45,-40,115,  	23,104,74,  
	1,107,-73,  	-39,116,42,  	-55,109,-41,  	-39,116,42,  
	-57,44,108,  	23,104,74,  	96,34,80,  	23,104,74,  
	73,72,-79,  	1,107,-73,  	-68,23,-106,  	-55,109,-41,  
	-68,23,-106,  	-57,44,108,  	-90,1,92,  	-57,44,108,  
	92,0,92,  	96,34,80,  	92,-1,-90,  	73,72,-79,  
	92,-1,-90,  	-68,23,-106,  	-90,1,-90,  	-90,1,92,  
	-90,1,-90,  	-90,1,92,  	-90,1,-90,  	-106,1,-71,  
	-90,1,-90,  	78,3,-102,  	92,-1,-90,  	78,3,-102,  
	92,0,92,  	92,7,92,  	-90,1,92,  	92,7,92,  
	-90,1,92,  	58,7,116,  	-90,1,92,  	-115,1,58,  
	-106,1,-71,  	-57,1,-115,  	78,3,-102,  	115,12,-57,  
	92,7,92,  	58,7,116      };
    return NORMALS;
  }  // end of getNormals()


  private short[] getTexCoords()
  // return an array holding TexCoords [1012 values / 2 = 506 points] 
  {
    short[] TEXCOORDS = {
26,101,26,101,
26,101,0,101,
0,127,0,101,
0,101,26,101,
0,101,0,127,
0,127,26,127,
26,101,26,127,
26,127,26,127,
26,127,0,127,
26,127,26,101,
26,127,26,101,
26,127,51,127,
26,127,26,101,
26,101,26,101,
26,101,51,101,
51,127,51,101,
77,101,51,101,
51,101,26,101,
51,101,51,127,
51,101,77,101,
77,101,102,101,
77,101,77,127,
51,127,77,127,
77,101,102,101,
102,101,154,101,
102,101,128,127,
77,127,128,127,
102,101,154,101,
154,101,179,101,
154,101,154,127,
128,127,154,127,
154,101,179,101,
179,101,205,101,
179,101,179,127,
154,127,179,127,
179,101,205,101,
205,101,255,101,
230,101,205,101,
205,127,179,127,
205,127,205,101,
205,127,230,101,
77,153,77,153,
77,153,77,153,
77,153,77,153,
77,153,77,153,
77,153,77,153,
77,153,77,153,
77,178,77,178,
77,178,77,153,
77,153,77,153,
77,153,77,153,
77,153,77,178,
77,153,77,153,
77,153,77,153,
77,153,77,153,
77,153,77,153,
77,153,77,153,
77,153,51,229,
51,229,26,204,
51,229,26,204,
51,229,26,204,
26,204,26,204,
26,204,26,204,
51,204,51,229,
51,204,51,229,
51,204,26,204,
51,204,26,204,
51,204,26,204,
26,204,26,204,
26,204,26,204,
26,204,26,204,
26,178,26,178,
26,178,26,204,
26,178,26,204,
26,204,26,204,
26,204,26,204,
26,204,26,204,
0,178,26,178,
26,178,26,178,
26,153,26,178,
0,178,26,178,
0,178,26,204,
0,178,0,178,
26,153,0,178,
26,153,26,178,
26,153,26,153,
26,153,26,153,
26,153,0,178,
26,153,26,153,
26,127,26,153,
26,153,26,153,
26,153,26,153,
26,153,26,153,
26,127,26,153,
26,127,26,127,
51,127,26,127,
51,127,26,127,
26,153,26,153,
26,153,26,153,
51,153,26,153,
51,127,26,127,
51,127,51,127,
77,127,26,127,
51,127,51,127,
77,127,77,127,
77,127,51,127,
77,127,51,127,
77,127,51,127,
51,127,77,127,
51,127,77,127,
51,153,77,127,
77,153,77,153,
77,153,77,153,
77,153,77,153,
77,153,77,178,
77,178,77,178,
77,178,77,178,
77,153,77,153,
77,153,77,153,
77,153,77,153,
77,153,77,153,
77,153,77,153,
77,153,77,153,
77,153,77,153,
77,153,77,153,
77,153,77,153,
77,153,77,153,
77,153,77,153,
77,153,77,153,
77,153,77,153,
77,153,77,127,
77,153,77,127,
51,153,51,127,
51,153,26,153,
51,153,51,153,
51,178,77,153,
51,178,77,153,
77,178,77,178,
77,178,102,153,
102,178,102,178,
77,178,102,178,
102,204,102,178,
102,204,102,178,
102,178,102,178,
102,153,102,178,
102,153,102,178,
102,153,102,153,
102,153,102,153,
102,153,102,153,
102,153,102,153,
102,153,102,153,
102,153,102,153,
102,153,102,153,
77,127,102,153,
77,127,102,153,
77,127,77,127,
51,127,77,127,
77,127,77,127,
77,127,77,127,
77,127,77,127,
102,153,77,127,
102,153,102,153,
102,178,102,153,
102,178,102,178,
102,204,102,178,
77,178,77,178,
77,178,77,178,
77,204,102,204,
77,204,102,204,
77,204,77,178,
77,178,77,178,
77,178,77,178,
51,229,77,204,
51,229,77,204,
51,229,77,204,
51,229,77,178,
77,178,77,178,
77,178,77,178,
26,204,77,178,
51,204,51,229,
51,204,51,229,
51,204,77,178,
51,204,26,204,
51,204,26,204,
26,204,26,178,
26,178,26,178,
26,178,26,178,
26,178,26,178,
26,178,26,178,
26,178,26,178,
26,178,26,178,
26,178,26,178,
26,178,26,204,
26,178,26,178,
26,178,26,178,
26,178,26,178,
26,178,26,178,
26,178,26,178,
26,178,26,178,
26,178,26,178,
26,178,26,178,
26,178,26,178,
26,178,26,178,
26,178,26,178,
26,178,26,178,
26,204,26,178,
26,178,26,178,
26,178,26,204,
26,178,26,178,
26,178,26,178,
26,178,26,178,
26,204,26,178,
26,178,26,178,
51,178,26,178,
77,178,26,204,
77,178,77,178,
77,178,77,178,
77,178,26,204,
77,178,26,178,
51,178,51,178,
51,178,77,178,
51,178,51,178,
51,178,51,178,
51,178,51,178,
77,178,51,178,
51,178,51,178,
51,153,51,178,
77,153,51,178,
77,153,77,178,
77,153,26,76,
26,76,26,76,
26,76,26,76,
26,50,26,76,
26,101,26,76,
26,101,26,76,
26,50,26,76,
26,50,26,50,
102,50,26,50,
102,76,26,101,
102,76,26,101,
102,76,26,50,
102,50,102,50,
102,50,179,50,
102,50,179,50,
102,50,179,76,
102,76,179,76,
102,76,179,76,
102,50,179,76,
179,50,255,76,
179,50,255,50,
179,50,230,50,
179,76,255,76,
179,76,255,76
      };
    return TEXCOORDS;
  }  // end of getTexCoords()
} // end of SwordCanvas class
