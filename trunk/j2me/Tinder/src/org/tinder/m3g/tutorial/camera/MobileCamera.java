package org.tinder.m3g.tutorial.camera;




import javax.microedition.lcdui.*;
import javax.microedition.m3g.*;


public class MobileCamera
{
  // initial x-axis rotation and position
  private static final float X_ANGLE = -10.0f;  // degrees
  private static final float X_POS = 0.0f; 
  private static final float Y_POS = 0.0f; 
  private static final float Z_POS = 5.0f; 
    // move back and up, rotate down

  // translation and rotation increments
  private static final float ANGLE_INCR = 1.0f;   // degrees
  private static final float MOVE_INCR = 0.1f;

  // key mode constants
  private static final int MOVE = 0;    // move left, right, fwd, back
  private static final int ROTATE = 1;  // turn left, right, up, down
  private static final int FLOAT = 2;   // move up, down
  private static final int NUM_MODES = 3;

  // booleans for remembering key presses
  private boolean upPressed = false;
  private boolean downPressed = false;
  private boolean leftPressed = false;
  private boolean rightPressed = false;

  private int keyMode = MOVE;   // current key mode

  // for examining the camera's (x,y,z) position
  private Transform trans = new Transform();
  private float transMat[] = new float[16];
  private float xCoord, yCoord, zCoord;

  // for storing the camera's current x- and y- rotations
  private float xAngle, yAngle;

  // scene graph elements for the camera
  private Group transGroup;
  private Camera cam;


  public MobileCamera(int width, int height)
  // transGroup --> camera
  {
    cam = new Camera();
    float aspectRatio = ((float) width) / ((float) height);
    cam.setPerspective(70.0f, aspectRatio, 0.1f, 50.0f);
    cam.postRotate(X_ANGLE, 1.0f, 0, 0);  // apply x-axis rotations to camera

    // translation an y-axis rotation group for the model
    transGroup = new Group();      // no initial rotation
    trans.postTranslate(X_POS, Y_POS, Z_POS);
    transGroup.setTransform(trans);
    transGroup.addChild(cam);

    // store initial rotations
    xAngle = X_ANGLE; yAngle = 0.0f;
  }  // end of MobileCamera()


  public Group getCameraGroup()
  {  return transGroup; }

  public Camera getCamera()
  {  return cam; }


  // ----- handle key presses / releases, and return details ------

  public void pressedKey(int gameAction)
  {
    switch(gameAction) {
      case Canvas.UP:    upPressed = true;    break;
      case Canvas.DOWN:  downPressed = true;  break;
      case Canvas.LEFT:  leftPressed = true;  break;
      case Canvas.RIGHT: rightPressed = true; break;
      case Canvas.FIRE: keyMode = (keyMode + 1) % NUM_MODES;  break;
      default: break;
    }
  }  // end of pressedKey()


  public void releasedKey(int gameAction)
  {
    switch(gameAction) {
      case Canvas.UP:    upPressed = false;    break;
      case Canvas.DOWN:  downPressed = false;  break;
      case Canvas.LEFT:  leftPressed = false;  break;
      case Canvas.RIGHT: rightPressed = false; break;
      default: break;
    }
  }  // end of releasedKey()


  public String getKeyMode()
  {
    switch(keyMode) {
      case MOVE: return "Select Mode: move";
      case ROTATE: return "Select Mode: rotate";
      case FLOAT: return "Select Mode: float";
      default: return "Select Mode: ??";
    }
  } // end of getKeyMode()


  public String getPosition()
  {  
    storePosition();

    // round coords to 2 dp
    float x = ((int)((xCoord+0.005)*100.0f))/100.0f;
    float y = ((int)((yCoord+0.005)*100.0f))/100.0f;
    float z = ((int)((zCoord+0.005)*100.0f))/100.0f;
    return "Pos: (" + x + ", " + y + ", " + z + ")";  
  }  // end of getPosition()


  private void storePosition()
  // extract the current (x,y,z) position from transGroup
  {
    transGroup.getTransform(trans);
    trans.get(transMat);
    xCoord = transMat[3];
    yCoord = transMat[7];
    zCoord = transMat[11]; 
  }  // end of storePosition()


  public String getRotation()
  {  return "Rot: (" + xAngle + ", " + yAngle + ", 0.0)";  }


  // ------------- update camera position and rotation ---------------

  public void update()
  {
    switch(keyMode) {
      case MOVE: updateMove();  break;
      case ROTATE: updateRotation();  break;
      case FLOAT: updateFloating();  break;
      default: break;
    }
  }  // end of update()


  private void updateMove()
  {
    transGroup.getTransform(trans);
    if (upPressed)          // move forward
      trans.postTranslate(0, 0, -MOVE_INCR);
    else if (downPressed)   // move backwards
      trans.postTranslate(0, 0, MOVE_INCR);
    else if (leftPressed)   // move to the left
      trans.postTranslate(-MOVE_INCR, 0, 0);
    else if (rightPressed)  // move to the right
      trans.postTranslate(MOVE_INCR, 0, 0);
    transGroup.setTransform(trans);
  }  // end of updateMove()


  private void updateRotation()
  {
    if (upPressed) {  // rotate up around x-axis
      cam.postRotate(ANGLE_INCR, 1.0f, 0, 0);
      xAngle += ANGLE_INCR;
    }
    else if (downPressed) {  // rotate down around x-axis
      cam.postRotate(-ANGLE_INCR, 1.0f, 0, 0);
      xAngle -= ANGLE_INCR;
    }
    else if (leftPressed)   // rotate left around the y-axis
      yAngle += ANGLE_INCR;
    else if (rightPressed)   // rotate right around the y-axis
      yAngle -= ANGLE_INCR;

    // angle values are modulo 360 degrees
    if (xAngle >= 360.0f)
      xAngle -= 360.0f;
    else if (xAngle <= -360.0f)
      xAngle += 360.0f;

    if (yAngle >= 360.0f)
      yAngle -= 360.0f;
    else if (yAngle <= -360.0f)
      yAngle += 360.0f;

    // apply the y-axis rotation to transGroup
    if (leftPressed || rightPressed) {
      storePosition();
      trans.setIdentity();
      trans.postTranslate(xCoord, yCoord, zCoord);
      trans.postRotate(yAngle, 0, 1.0f, 0);
      transGroup.setTransform(trans);
    }
  }  // end of updateRotation()


  private void updateFloating()
  {
    transGroup.getTransform(trans);
    if (upPressed)          // move up
      trans.postTranslate(0, MOVE_INCR, 0);
    else if (downPressed)   // move down
      trans.postTranslate(0, -MOVE_INCR, 0);
    else if (leftPressed)   // move to the left (same as updateMove())
      trans.postTranslate(-MOVE_INCR, 0, 0);
    else if (rightPressed)   // move to the right (same as updateMove())
      trans.postTranslate(MOVE_INCR, 0, 0);
    transGroup.setTransform(trans);
  }  // end of updateFloating()

} // end of MobileCamera class
