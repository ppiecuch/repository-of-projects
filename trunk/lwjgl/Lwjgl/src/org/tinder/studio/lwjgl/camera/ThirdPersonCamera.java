package org.tinder.studio.lwjgl.camera;

/**
 * 
 * @author Micheal Hong 
 * @email babala_234@163.com
 * @version 2010-7-13 ÏÂÎç03:55:58
 *
 */
public class ThirdPersonCamera {
	
	private float stadia;				//the distance from the camera to the role
	private float walkStepLength;		//the walk step length of the role
	
	public ThirdPersonCamera(float stadia,float walkStepLength) {
		this.stadia=stadia;
		this.walkStepLength=walkStepLength;
	}
	
	/**
	 * 
	 * @param position the position of the role
	 * @param yaw the rotation around the Y axis of the camera
	 */
	public void walkForward(float[] position,float yaw)
	{
		position[0]+=walkStepLength*Math.sin(-yaw* Math.PI/180);
		position[2]-=walkStepLength*Math.cos(-yaw* Math.PI/180);
	}
	public void walkBackward(float[] position,float yaw)
	{
		position[0]-=walkStepLength*Math.sin(-yaw* Math.PI/180);
		position[2]+=walkStepLength*Math.cos(-yaw* Math.PI/180);
	}
	public void walkLeft(float[] position,float yaw)
	{
		position[0]-=walkStepLength*Math.cos(-yaw* Math.PI/180);
		position[2]-=walkStepLength*Math.sin(-yaw* Math.PI/180);
	}
	public void walkRight(float[] position,float yaw)
	{
		position[0]+=walkStepLength*Math.cos(-yaw* Math.PI/180);
		position[2]+=walkStepLength*Math.sin(-yaw* Math.PI/180);
	}
	
	/**
	 * 
	 * @param yaw the rotation around the Y axis of the camera
	 * @param angle the rotation of change
	 * @return the result
	 */
	public float horizontalTurn(float yaw,float angle)
	{
		yaw+=angle;
		return yaw;
	}

	/**
	 * Calculate the position of the camera
	 * @param position the position of the role
	 * @param yaw the rotation around the Y axis of the camera
	 * @param cameraPosition the position of the camera
	 */
	public void calculateCameraPosition(float[] position,float yaw,float[] cameraPosition) {
		cameraPosition[0] = (float) (stadia * Math.sin(yaw*Math.PI/180)+position[0]);
		cameraPosition[2] = (float) (stadia * Math.cos(yaw*Math.PI/180)+position[2]);
	}
	
	

}
