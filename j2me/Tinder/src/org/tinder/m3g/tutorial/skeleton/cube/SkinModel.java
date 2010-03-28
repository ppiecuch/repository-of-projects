package org.tinder.m3g.tutorial.skeleton.cube;


import javax.microedition.lcdui.*;
import javax.microedition.m3g.*;


public class SkinModel 
{
  private SkinnedMesh blockModel;
  private Group Bone1, Bone2, Bone3;
  private AnimationController animController;


  public SkinModel() 
  { makeBlock();
    setUpAnimation();
  }

  public SkinnedMesh getModel()
  {  return blockModel;  }


  // ------------------- model creation -----------------------------

  private void makeBlock()
  /* The block has coloured vertices, but no normals or texture coordinates. 
     It has 3 joints attached to it.
  */
  { // create vertices
    short[] POINTS = {  -1,0,1,  1,0,1,  1,0,-1,  -1,0,-1,     // bottom coords
                      -1,4,1,  1,4,1,  1,4,-1,  -1,4,-1,     // middle
                      -1,8,1,  1,8,1,  1,8,-1,  -1,8,-1   }; // top
    VertexArray POSITION_ARRAY = new VertexArray(POINTS.length/3, 3, 2);
    POSITION_ARRAY.set(0, POINTS.length/3, POINTS);

	// create vertex colours
    byte[] COLORS = {  -1,0,0,  -1,0,0,  -1,0,0,   -1,0,0,    // red
                     0,-1,0,  0,-1,0,   0,-1,0,  0,-1,0,    // green
                     0,0,-1,  0,0,-1,   0,0,-1,  0,0,-1  }; // blue
  	VertexArray COLOR_ARRAY = new VertexArray(COLORS.length/3, 3, 1);
  	COLOR_ARRAY.set(0, COLORS.length/3, COLORS);

    VertexBuffer vertexBuffer = new VertexBuffer(); 
    vertexBuffer.setPositions(POSITION_ARRAY, 1.0f, null); // no scale, bias
	// vertexBuffer.setDefaultColor(0x00FF0000); // default to red vertices
   	vertexBuffer.setColors(COLOR_ARRAY);
    int[] INDEX = {   // the block is made from 10 quads
                  1,5,0,4,  5,9,4,8,    // front 2 quads
                  2,6,1,5,  6,10,5,9,   // right 2 quads
                  0,4,3,7,  4,8,7,11,   // left 2 quads
                  3,7,2,6,  7,11,6,10,  // back 2 quads
                  2,1,3,0,     // bottom quad
                  9,10,8,11    // top quad
                };
    int[] LENGTHS = { 4,4, 4,4, 4,4, 4,4, 4,4 };
    IndexBuffer indexBuffer = new TriangleStripArray(INDEX, LENGTHS);

    Appearance appearance =  new Appearance();   // no material

    // create skinned mesh
    blockModel = new SkinnedMesh(vertexBuffer, indexBuffer, appearance, makeSkeleton());
    blockModel.scale(0.1f, 0.1f, 0.1f);

    /* Link joints to the block's vertices. Each joint is linked to
       the four vertices at that level in the block. */
    blockModel.addTransform(Bone1, 1, 0, 4); 
    blockModel.addTransform(Bone2, 1, 4, 4);
    blockModel.addTransform(Bone3, 1, 8, 4);
  }  // end of makeBlock()


  private Group makeSkeleton()
  /* The skeleton is a chain of three joints (Group nodes).
     Bone1 --> mid --> end

     The Bone1 and mid joints will be rotated.

     The Bone1 joint will be linked to the bottom of the 
     block, the middle joint to the block's middle,
     and the top joint to the block's top. 
  */
  { Group skeleton = new Group();

    Bone1 = new Group();
    skeleton.addChild(Bone1);

    Bone2 = new Group();
    Bone2.setTranslation(0.0f, 4.0f, 0.0f);
    Bone1.addChild(Bone2);

    Bone3= new Group();
    Bone3.setTranslation(0.0f, 8.0f, 0.0f);
    Bone2.addChild(Bone3);

    return skeleton;
  } // end of makeSkeleton()

  private void setUpAnimation()

  {
    // creation animation controller 
    animController = new AnimationController();
    animController.setActiveInterval(0, 1500);
    animController.setSpeed(2.0f, 0);	 // double speed

    // create rotation animation track for Bone1 joint
    int[] Bone1Angles = {0, -15, -30, -15, 0, 15, 30, 15};   // z-axis angles
  //  setJointRotation(Bone1, startAngles);
    KeyframeSequence rotKS = rotationFrames(Bone1Angles);
    AnimationTrack rotTrack = 
           new AnimationTrack(rotKS, AnimationTrack.ORIENTATION); 
    rotTrack.setController(animController);
    Bone1.addAnimationTrack(rotTrack);   // affect joint
    // create rotation animation track for mid joint
    int[] Bone2Angles = {0, 30, 60, 30, 0, -30, -60, -30};    // z-axis angles
    rotKS = rotationFrames(Bone2Angles);
    rotTrack = 
           new AnimationTrack(rotKS, AnimationTrack.ORIENTATION); 
    rotTrack.setController(animController);
    Bone2.addAnimationTrack(rotTrack);   // affect joint
  }  // end of setUpAnimation()





  private KeyframeSequence rotationFrames(int[] angles)
  /* The rotation is carried out over 8 frames, then repeated.
     Each sequence taking 80 time units.
  */
  {
    KeyframeSequence ks = new KeyframeSequence(8, 4, KeyframeSequence.SLERP);
      /* the interpolation is a constant speed rotation */

    // each frame is separated by 10 sequence time units
    ks.setKeyframe(0,  0, rotZQuat(angles[0]) );
    ks.setKeyframe(1, 10, rotZQuat(angles[1]) );
    ks.setKeyframe(2, 20, rotZQuat(angles[2]) );
    ks.setKeyframe(3, 30, rotZQuat(angles[3]) );
    ks.setKeyframe(4, 40, rotZQuat(angles[4]) );
    ks.setKeyframe(5, 50, rotZQuat(angles[5]) );
    ks.setKeyframe(6, 60, rotZQuat(angles[6]) );
    ks.setKeyframe(7, 70, rotZQuat(angles[7]) );

    ks.setDuration(80);   // one cycle takes 80 sequence time units
    ks.setValidRange(0, 7);
    ks.setRepeatMode(KeyframeSequence.LOOP);

    return ks;
  }  // end of rotationFrames()


  private float[] rotZQuat(double angle)
  /* Calculate the quaternion for a clockwise rotation of 
     angle degrees about the z-axis. */
  {
    double radianAngle = Math.toRadians(angle)/2.0;
    float[] quat = new float[4];
    quat[0] = 0.0f;    // i coefficient
    quat[1] = 0.0f;    // j coef
    quat[2] = (float) Math.sin(radianAngle);    // k coef
    quat[3] = (float) Math.cos(radianAngle);   // scalar component a
    return quat;
  }  // end of rotZQuat()


} // end of SkinModel class
