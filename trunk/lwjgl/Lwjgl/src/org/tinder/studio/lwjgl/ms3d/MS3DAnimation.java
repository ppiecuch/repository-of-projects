package org.tinder.studio.lwjgl.ms3d;
/**
 *
 * @Author Micheal Hong
 * @Email babala_234@163.com
 * @Version 2010-7-8обнГ11:44:32
 *
 */
public class MS3DAnimation {

	short startFrame;
	short endFrame;

	public MS3DAnimation() {
		this.startFrame = 0;
		this.endFrame = 0;
	}

	/** Creates a new instance of MS3DAnimation */
	public MS3DAnimation(short startFrame, short endFrame) {
		this.startFrame = startFrame;
		this.endFrame = endFrame;
	}

	public MS3DAnimation(int startFrame, int endFrame) {
		this.startFrame = (short) startFrame;
		this.endFrame = (short) endFrame;
	}

}
