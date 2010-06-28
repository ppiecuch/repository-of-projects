package glapp;

import org.lwjgl.input.Keyboard;

/**
 * Change the position and orientation of a GLCamera based on input keys.  This
 * class moves the camera according to how much time has elapsed, so will 
 * produce consistent response to input, regardless of processor speed.
 * 
 * To use:
 * 
 * 
 * @see GLCamera.java
 */
public class GLCam {
	// camera motion speed
	public float camSpeedR = 90;         // degrees per second
	public float camSpeedXZ = 10;        // units per second 
	public float camSpeedY  = 10;        // units per second
	
	public GLCamera camera;  // camera that we'll be moving
	
	double avgSecsPerFrame=.01;   // to smooth out motion, keep a moving average of frame time deltas
	double numToAvg = 50;         // number of frames to average (about one second)
	
	
	/**
	 * Create a GLCameraMover with the camera that it will be moving.
	 * 
	 * @param cam
	 */
	public GLCam(GLCamera cam) {
		camera = cam;
	}
	
	/**
	 * Create a GLCameraMover.  Will create a default camera.
	 * 
	 * @param cam
	 */
	public GLCam() { 
		camera = new GLCamera(0,0,10, 0,0,0, 0,1,0);
	}
	
	
	/**
	 * Create a GLCameraMover with the camera that it will be moving.
	 * 
	 * @param cam
	 */
	public void setCamera(GLCamera cam) {
		camera = cam;
	}
	
	
	/**
	 * Tell camera to adjust modelview matrix to new view position and orientation.
	 *
	 */public void render() {
		 camera.Render();
	 }
	 
	 /**
	  * Adjust the Camera position based on keyboard arrow key input.
	  * These are repeating events (camera will move as long as key is held
	  * down).  Moves the camera according to time elapsed, so camera motion
	  * is consistent across varied cpu speeds.
	  * 
	  * @param seconds  seconds elapsed since last frame was drawn
	  */
	 public void handleNavKeys(float seconds)
	 {
		 //boolean altKeyDown = Keyboard.isKeyDown(Keyboard.KEY_RMENU)||Keyboard.isKeyDown(Keyboard.KEY_LMENU);
		 //boolean ctrlKeyDown = Keyboard.isKeyDown(Keyboard.KEY_RCONTROL)||Keyboard.isKeyDown(Keyboard.KEY_LCONTROL);
		 
		 // Basic motion
		 // Turn left
		 if (Keyboard.isKeyDown(Keyboard.KEY_LEFT)) {
			 camera.RotateV(camSpeedR*seconds);
		 }
		 // Turn right
		 if (Keyboard.isKeyDown(Keyboard.KEY_RIGHT)) {
			 camera.RotateV(-camSpeedR*seconds);
		 }
		 // move forward
		 if (Keyboard.isKeyDown(Keyboard.KEY_UP)) {
			 camera.MoveForward(-camSpeedXZ*seconds);
		 }
		 // move backward
		 if (Keyboard.isKeyDown(Keyboard.KEY_DOWN)) {
			 camera.MoveForward(camSpeedXZ*seconds);
		 }
		 // move down
		 if (Keyboard.isKeyDown(Keyboard.KEY_PRIOR)) {
			 camera.MoveUpward(camSpeedY*seconds);
		 }
		 // move up
		 if (Keyboard.isKeyDown(Keyboard.KEY_NEXT)) {
			 camera.MoveUpward(-camSpeedY*seconds);
		 }
		 
		 // additional motion
		 // Pan left
		 if (Keyboard.isKeyDown(Keyboard.KEY_A)) {
			 camera.StrafeRight(-camSpeedXZ*seconds);
		 }
		 // Pan right
		 if (Keyboard.isKeyDown(Keyboard.KEY_D)) {
			 camera.StrafeRight(camSpeedXZ*seconds);
		 }
		 // tilt down
		 if (Keyboard.isKeyDown(Keyboard.KEY_S)) {
			 camera.RotateX(-camSpeedR*seconds);
		 }
		 // tilt up
		 if (Keyboard.isKeyDown(Keyboard.KEY_W)) {
			 camera.RotateX(camSpeedR*seconds);
		 }
		 // tilt left
		 if (Keyboard.isKeyDown(Keyboard.KEY_Z)) {
			 camera.RotateZ(camSpeedR*seconds);
		 }
		 // tilt right
		 if (Keyboard.isKeyDown(Keyboard.KEY_C)) {
			 camera.RotateZ(-camSpeedR*seconds);
		 }
	 }
	 
	 /**
	  * Adjust the Camera position based on keyboard arrow key input.
	  * These are repeating events (camera will move as long as key is held
	  * down).  Moves the camera according to time elapsed, so camera motion
	  * is consistent across varied cpu speeds.
	  * 
	  * @param seconds  seconds elapsed since last frame was drawn
	  */
	 public void handleArrowKeysOnly(float seconds)
	 {
		 // Keep a moving average of recent frames, to smooth out jumps or lags in processing time
		 //seconds = (float)updateAvgFrameTime(seconds);
		 
		 // Basic motion
		 // Turn left
		 if (Keyboard.isKeyDown(Keyboard.KEY_LEFT)) {
			 camera.RotateY(camSpeedR*seconds);
		 }
		 // Turn right
		 if (Keyboard.isKeyDown(Keyboard.KEY_RIGHT)) {
			 camera.RotateY(-camSpeedR*seconds);
		 }
		 // move forward
		 if (Keyboard.isKeyDown(Keyboard.KEY_UP)) {
			 camera.MoveForward(-camSpeedXZ*seconds);
		 }
		 // move backward
		 if (Keyboard.isKeyDown(Keyboard.KEY_DOWN)) {
			 camera.MoveForward(camSpeedXZ*seconds);
		 }
		 // move down
		 if (Keyboard.isKeyDown(Keyboard.KEY_PRIOR)) {
			 camera.MoveUpward(camSpeedY*seconds);
		 }
		 // move up
		 if (Keyboard.isKeyDown(Keyboard.KEY_NEXT)) {
			 camera.MoveUpward(-camSpeedY*seconds);
		 }
	 }
	 
	 /**
	  * Adjust the Camera view direction based on key press events.  Do not 
	  * change camera position.
	  * 
	  * @param seconds  seconds elapsed since last frame was drawn
	  */
	 public void handleNavKeysViewOnly(float seconds)
	 {
		 // Keep a moving average of recent frames, to smooth out jumps or lags in processing time
		 avgSecsPerFrame = (double) ((avgSecsPerFrame*numToAvg)+seconds) / (numToAvg+1) ;
		 seconds = (float)avgSecsPerFrame;
		 
		 //System.out.println("seconds elapsed = "+ seconds);
		 
		 // alt key combo
		 if (Keyboard.isKeyDown(Keyboard.KEY_RMENU)||Keyboard.isKeyDown(Keyboard.KEY_LMENU)) {
			 // Pan left
			 if (Keyboard.isKeyDown(Keyboard.KEY_LEFT)) {
				 camera.RotateZ(camSpeedR*seconds);
			 }
			 // Pan right
			 if (Keyboard.isKeyDown(Keyboard.KEY_RIGHT)) {
				 camera.RotateZ(-camSpeedR*seconds);
			 }
		 }
		 // control key combo
		 else if (Keyboard.isKeyDown(Keyboard.KEY_RCONTROL)||Keyboard.isKeyDown(Keyboard.KEY_LCONTROL)) {
			 // tilt down
			 if (Keyboard.isKeyDown(Keyboard.KEY_UP)) {
				 camera.RotateX(-camSpeedR*seconds);
			 }
			 // tilt up
			 if (Keyboard.isKeyDown(Keyboard.KEY_DOWN)) {
				 camera.RotateX(camSpeedR*seconds);
			 }
		 }
		 else {
			 // Turn left
			 if (Keyboard.isKeyDown(Keyboard.KEY_LEFT)) {
				 camera.RotateY(camSpeedR*seconds);
			 }
			 // Turn right
			 if (Keyboard.isKeyDown(Keyboard.KEY_RIGHT)) {
				 camera.RotateY(-camSpeedR*seconds);
			 }
		 }
	 }
	
}
