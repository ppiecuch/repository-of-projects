package org.java3dgamesdk.graphics.MS3D;

import java.util.*;

import javax.media.j3d.*;
import javax.vecmath.*;

import org.java3dgamesdk.graphics.*;

public class MS3DModel extends Model3D {

	/*
	 * 
	 * Milkshape 3D Specific
	 *  
	 */

	public String id;
	public int version;

	public int nNumVertices;

	public MS3DVertex vertices[];
	public float vertex[];

	public int nNumTriangles;

	public MS3DTriangle triangles[];

	public int nNumGroups;

	public MS3DGroup groups[];

	public int nNumMaterials;

	public MS3DMaterial materials[];

	public float fAnimationFPS;
	public long currentTime;
	public long lastTime;
	public int iTotalFrames;

	private int endFrame;

	public int nNumJoints;

	public MS3DJoint joints[];

	/*
	 * 
	 * Java 3D Specific
	 *  
	 */

	private Hashtable shapes;

	public String name;

	/*
	 * 
	 * Needed for the animation
	 *  
	 */

	private Vector rootJoints;
	private Hashtable allJoints;

	private Vector allAnimations;
	public int currentAnimation;

	public boolean loop;
	private boolean loopDone;

	public MS3DModel(String name) {
		this.shapes = new Hashtable();

		this.name = name;

		this.rootJoints = new Vector();
		this.allJoints = new Hashtable();

		this.allAnimations = new Vector();
		this.currentAnimation = -1;
		this.loop = true;
		this.loopDone = false;
	}

	public void addShape3D(String group, Shape3D shape3d) {
		shapes.put(group, shape3d);
	}

	public void addJoint(MS3DJoint joint) {
		allJoints.put(joint.name, joint);

		if (joint.parentName.equals(""))
			rootJoints.addElement(joint);
	}

	public Hashtable getJoints() {
		return allJoints;
	}

	public void reset() {
		for (int i = 0; i < nNumJoints; i++) {
			joints[i].currentRotationKeyframe = 0;
			joints[i].currentTranslationKeyframe = 0;
			joints[i].finalMatrix = new Matrix4f(joints[i].absoluteMatrix);
		}

		currentTime = 0;
		if (currentAnimation != -1) {
			MS3DAnimation ms3dAnimation = (MS3DAnimation) allAnimations
					.get(currentAnimation);
			currentTime += ms3dAnimation.startFrame * 1000.0 / fAnimationFPS;
		}
		loopDone = false;
	}

	public void updateModel(long time) {
		// update the animation
		updateAnimation(time);

		// update the points
		for (int i = 0; i < nNumGroups; i++) {

			for (int k = 0; k < groups[i].coordsIndex.length; k++) {
				groups[i].coords[3 * k] = vertex[3 * groups[i].coordsIndex[k]];
				groups[i].coords[3 * k + 1] = vertex[3 * groups[i].coordsIndex[k] + 1];
				groups[i].coords[3 * k + 2] = vertex[3 * groups[i].coordsIndex[k] + 2];

				if (vertices[groups[i].coordsIndex[k]].boneId != -1) {
					Matrix4f matrix = joints[vertices[groups[i].coordsIndex[k]].boneId].finalMatrix;

					Vector4f temp = new Vector4f();

					temp.x = groups[i].coords[3 * k] * matrix.m00
							+ groups[i].coords[3 * k + 1] * matrix.m10
							+ groups[i].coords[3 * k + 2] * matrix.m20
							+ matrix.m30;
					temp.y = groups[i].coords[3 * k] * matrix.m01
							+ groups[i].coords[3 * k + 1] * matrix.m11
							+ groups[i].coords[3 * k + 2] * matrix.m21
							+ matrix.m31;
					temp.z = groups[i].coords[3 * k] * matrix.m02
							+ groups[i].coords[3 * k + 1] * matrix.m12
							+ groups[i].coords[3 * k + 2] * matrix.m22
							+ matrix.m32;
					temp.w = groups[i].coords[3 * k] * matrix.m03
							+ groups[i].coords[3 * k + 1] * matrix.m13
							+ groups[i].coords[3 * k + 2] * matrix.m23
							+ matrix.m33;

					groups[i].coords[3 * k] = temp.x;
					groups[i].coords[3 * k + 1] = temp.y;
					groups[i].coords[3 * k + 2] = temp.z;
				}
			}

		}

	}

	public void drawModel(GraphicsContext3D gc) {

		for (Enumeration keys = shapes.keys(); keys.hasMoreElements();) {
			String key = (String) keys.nextElement();

			Shape3D shape = (Shape3D) shapes.get(key);

			gc.draw(shape);
		}

	}

	public void addAnimation(MS3DAnimation ms3dAnimation) {
		allAnimations.addElement(ms3dAnimation);
	}
	
	/**
	* Added by Arjen Hoekstra
	*/
	
	public void deleteAnimations()
	{
		for(int i=0;i<allAnimations.size();i++)
		{
			allAnimations.remove(i);
		}
	}
	
	public int numberAnimations() {
		return allAnimations.size();
	}

	public boolean loopDone() {
		return loopDone;
	}

	public void updateAnimation(long time) {

		currentTime += time - lastTime;
		lastTime = time;

		if (currentAnimation == -1) {
			endFrame = iTotalFrames;
		} else {
			MS3DAnimation ms3dAnimation = (MS3DAnimation) allAnimations.get(currentAnimation);
			endFrame = ms3dAnimation.endFrame;
		}

		if (currentTime >= (endFrame + 1) * 1000.0 / fAnimationFPS) {
			if (loop) {
				reset();
			} else {
				currentTime = (long) ((endFrame + 1) * 1000.0 / fAnimationFPS);
				loopDone = true;
			}
		}

		for (int i = 0; i < nNumJoints; i++) {
			Vector3f transVec = new Vector3f();
			Matrix4f transform = new Matrix4f();
			transform.setIdentity();

			short frame;
			MS3DJoint joint = joints[i];

			if (joint.numKeyFramesRot == 0 && joint.numKeyFramesTrans == 0) {
				joint.finalMatrix = new Matrix4f(joint.absoluteMatrix);
				continue;
			}

			//
			frame = joint.currentTranslationKeyframe;

			while (frame < joint.numKeyFramesTrans
					&& joint.keyTranslations[frame].time < currentTime) {
				frame++;
			}
			joint.currentTranslationKeyframe = frame;

			if (frame == 0) {
				transVec.x = joint.keyTranslations[0].x;
				transVec.y = joint.keyTranslations[0].y;
				transVec.z = joint.keyTranslations[0].z;
			} else if (frame == joint.numKeyFramesTrans) {
				transVec.x = joint.keyTranslations[frame - 1].x;
				transVec.y = joint.keyTranslations[frame - 1].y;
				transVec.z = joint.keyTranslations[frame - 1].z;
			} else {
				MS3DKeyFrameTranslation curFrame = joint.keyTranslations[frame];
				MS3DKeyFrameTranslation prevFrame = joint.keyTranslations[frame - 1];

				float timeDelta = ((float) (curFrame.time - prevFrame.time));
				float interpValue = (float) ((currentTime - prevFrame.time) / (float) timeDelta);

				transVec.x = prevFrame.x + (curFrame.x - prevFrame.x)
						* interpValue;
				transVec.y = prevFrame.y + (curFrame.y - prevFrame.y)
						* interpValue;
				transVec.z = prevFrame.z + (curFrame.z - prevFrame.z)
						* interpValue;
			}

			//
			frame = joint.currentRotationKeyframe;

			while (frame < joint.numKeyFramesRot
					&& joint.keyRotations[frame].time < currentTime) {
				frame++;
			}
			joint.currentRotationKeyframe = frame;

			if (frame == 0) {

				setRzRyRxRotationTranspose(transform,
						joint.keyRotations[0].rotZ, joint.keyRotations[0].rotY,
						joint.keyRotations[0].rotX);

			} else if (frame == joint.numKeyFramesRot) {

				setRzRyRxRotationTranspose(transform,
						joint.keyRotations[frame - 1].rotZ,
						joint.keyRotations[frame - 1].rotY,
						joint.keyRotations[frame - 1].rotX);

			} else {
				MS3DKeyFrameRotation curFrame = joint.keyRotations[frame];
				MS3DKeyFrameRotation prevFrame = joint.keyRotations[frame - 1];

				float timeDelta = ((float) (curFrame.time - prevFrame.time));
				float interpValue = (float) ((currentTime - prevFrame.time) / timeDelta);

				Vector3f rotVec = new Vector3f();

				rotVec.x = prevFrame.rotX + (curFrame.rotX - prevFrame.rotX)
						* interpValue;
				rotVec.y = prevFrame.rotY + (curFrame.rotY - prevFrame.rotY)
						* interpValue;
				rotVec.z = prevFrame.rotZ + (curFrame.rotZ - prevFrame.rotZ)
						* interpValue;

				setRzRyRxRotationTranspose(transform, rotVec.z, rotVec.y,
						rotVec.x);

			}

			// Translation
			setTranslationTranspose(transform, transVec.x, transVec.y,
					transVec.z);

			Matrix4f relativeFinal = new Matrix4f();
			Matrix4f matrixA = joint.relativeMatrix;
			Matrix4f matrixB = transform;

			mulTransposeBothTanspose(relativeFinal, joint.relativeMatrix,
					transform);

			if (joint.parentName.equals("")) {

				joint.finalMatrix = relativeFinal;

			} else {

				MS3DJoint father = (MS3DJoint) allJoints.get(joint.parentName);

				joint.finalMatrix = new Matrix4f();

				mulTransposeBothTanspose(joint.finalMatrix, father.finalMatrix,
						relativeFinal);

			}

		}

	}

	public static void setRzRyRxRotationTranspose(Matrix4f matrix, float z,
			float y, float x) {
		if (matrix == null) {
			matrix = new Matrix4f();
			matrix.setIdentity();
		}

		double cosinusX = Math.cos(x);
		double sinusX = Math.sin(x);
		double cosinusY = Math.cos(y);
		double sinusY = Math.sin(y);
		double cosinusZ = Math.cos(z);
		double sinusZ = Math.sin(z);

		matrix.m00 = (float) (cosinusY * cosinusZ);
		matrix.m01 = (float) (cosinusY * sinusZ);
		matrix.m02 = (float) (-sinusY);
		// do not alter m03

		matrix.m10 = (float) (sinusX * sinusY * cosinusZ - cosinusX * sinusZ);
		matrix.m11 = (float) (sinusX * sinusY * sinusZ + cosinusX * cosinusZ);
		matrix.m12 = (float) (sinusX * cosinusY);
		// do not alter m13

		matrix.m20 = (float) (cosinusX * sinusY * cosinusZ + sinusX * sinusZ);
		matrix.m21 = (float) (cosinusX * sinusY * sinusZ - sinusX * cosinusZ);
		matrix.m22 = (float) (cosinusX * cosinusY);
		// do not alter m23

		// do not alter m30 - m33
	}

	public static void setTranslationTranspose(Matrix4f matrix, float x,
			float y, float z) {
		if (matrix == null) {
			matrix = new Matrix4f();
			matrix.setIdentity();
		}

		// do not alter m00 - m23

		matrix.m30 = x;
		matrix.m31 = y;
		matrix.m32 = z;

		matrix.m33 = 1.0f;

	}

	public static void mulTransposeBothTanspose(Matrix4f matrix,
			Matrix4f matrixA, Matrix4f matrixB) {
		if (matrix == null) {
			matrix = new Matrix4f();
		}

		matrix.m00 = matrixA.m00 * matrixB.m00 + matrixA.m10 * matrixB.m01
				+ matrixA.m20 * matrixB.m02;
		matrix.m01 = matrixA.m01 * matrixB.m00 + matrixA.m11 * matrixB.m01
				+ matrixA.m21 * matrixB.m02;
		matrix.m02 = matrixA.m02 * matrixB.m00 + matrixA.m12 * matrixB.m01
				+ matrixA.m22 * matrixB.m02;
		matrix.m03 = 0.0f;

		matrix.m10 = matrixA.m00 * matrixB.m10 + matrixA.m10 * matrixB.m11
				+ matrixA.m20 * matrixB.m12;
		matrix.m11 = matrixA.m01 * matrixB.m10 + matrixA.m11 * matrixB.m11
				+ matrixA.m21 * matrixB.m12;
		matrix.m12 = matrixA.m02 * matrixB.m10 + matrixA.m12 * matrixB.m11
				+ matrixA.m22 * matrixB.m12;
		matrix.m13 = 0.0f;

		matrix.m20 = matrixA.m00 * matrixB.m20 + matrixA.m10 * matrixB.m21
				+ matrixA.m20 * matrixB.m22;
		matrix.m21 = matrixA.m01 * matrixB.m20 + matrixA.m11 * matrixB.m21
				+ matrixA.m21 * matrixB.m22;
		matrix.m22 = matrixA.m02 * matrixB.m20 + matrixA.m12 * matrixB.m21
				+ matrixA.m22 * matrixB.m22;
		matrix.m23 = 0.0f;

		matrix.m30 = matrixA.m00 * matrixB.m30 + matrixA.m10 * matrixB.m31
				+ matrixA.m20 * matrixB.m32 + matrixA.m30;
		matrix.m31 = matrixA.m01 * matrixB.m30 + matrixA.m11 * matrixB.m31
				+ matrixA.m21 * matrixB.m32 + matrixA.m31;
		matrix.m32 = matrixA.m02 * matrixB.m30 + matrixA.m12 * matrixB.m31
				+ matrixA.m22 * matrixB.m32 + matrixA.m32;
		matrix.m33 = 1.0f;
	}
}