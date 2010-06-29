package org.tinder.studio.lwjgl.ms3d;

import java.io.InputStream;
import java.nio.FloatBuffer;
import java.util.Hashtable;
import java.util.Vector;

import org.lwjgl.BufferUtils;
import org.lwjgl.Sys;
import org.lwjgl.opengl.GL11;
import org.tinder.studio.lwjgl.util.BinaryFileReader;
import org.tinder.studio.lwjgl.util.GLApp;
import org.tinder.studio.lwjgl.util.Matrix4f;
import org.tinder.studio.lwjgl.util.Vector3f;
import org.tinder.studio.lwjgl.util.Vector4f;

/**
 * 
 * 
 * @Author Micheal Hong
 * @Email babala_234@163.com
 * @Version 2010-6-29下午10:33:28
 * 
 */
public class MS3DModel {

	MS3DHeader header;

	int numVertices;
	MS3DVertex[] vertices;
	float vertex[];

	int numTriangles;
	MS3DTriangle[] triangles;

	int numMeshs;
	MS3DMesh[] meshs;

	int numMaterials;
	MS3DMaterial[] materials;

	int numJoints;
	MS3DJoint[] joints;

	long lastTime;
	int currentAnimation;
	float currentTime;
	int totalFrames;
	int endFrame;
	float animationFPS;
	boolean loop;
	boolean loopDone;
	Vector<MS3DAnimation> allAnimations;
	Hashtable<String, MS3DJoint> allJoints;
	Vector<MS3DJoint> rootJoints;
	String textureDirectory;

	/***************************************************************************************************************************************************************************************************
	 * MS3DModel () - Constructor
	 **************************************************************************************************************************************************************************************************/
	public MS3DModel(InputStream file,String textureDirectory) {
		this.allJoints = new Hashtable<String, MS3DJoint>();
		this.rootJoints = new Vector<MS3DJoint>();
		this.allAnimations = new Vector<MS3DAnimation>();
		this.currentAnimation = -1;
		this.loop = true;
		this.loopDone = false;
		this.textureDirectory=textureDirectory;
		this.loadModel(file);

	}

	/***************************************************************************************************************************************************************************************************
	 * loadModel() - Load a ms3d model and prepare it for rendering
	 **************************************************************************************************************************************************************************************************/
	protected void loadModel(InputStream file) {

		try {
			// Read the model file
			BinaryFileReader modelFile = new BinaryFileReader(file);

			// Read header
			this.header = new MS3DHeader(modelFile);
			if (!this.header.isValid())
				throw new Exception("Model header is invalid! Only versions 1.3 and 1.4 are supported!");

			// Read vertices
			this.numVertices = modelFile.readShort();
			this.vertices = new MS3DVertex[this.numVertices];
			this.vertex = new float[3 * numVertices];
			for (int i = 0; i < this.numVertices; i++) {
				this.vertices[i] = new MS3DVertex(modelFile);
				this.vertex[3 * i] = this.vertices[i].vertex.x;
				this.vertex[3 * i + 1] = this.vertices[i].vertex.y;
				this.vertex[3 * i + 2] = this.vertices[i].vertex.z;
			}

			// Triangles
			this.numTriangles = modelFile.readShort();
			this.triangles = new MS3DTriangle[this.numTriangles];
			for (int i = 0; i < this.numTriangles; i++)
				this.triangles[i] = new MS3DTriangle(modelFile);

			// Meshs
			this.numMeshs = modelFile.readShort();
			this.meshs = new MS3DMesh[this.numMeshs];
			for (int i = 0; i < this.numMeshs; i++)
				this.meshs[i] = new MS3DMesh(modelFile);

			// Materials
			this.numMaterials = modelFile.readShort();
			this.materials = new MS3DMaterial[this.numMaterials];
			for (int i = 0; i < this.numMaterials; i++)
				this.materials[i] = new MS3DMaterial(modelFile);

			// Keyframe data
			animationFPS = modelFile.readFloat();
			currentTime = (long) (modelFile.readFloat() * 1000);
			totalFrames = modelFile.readInt();

			// Joints
			numJoints = modelFile.readShort();
			this.joints = new MS3DJoint[numJoints];
			for (int i = 0; i < this.numJoints; i++) {
				this.joints[i] = new MS3DJoint(modelFile);
				addJoint(this.joints[i]);
			}

			// Load textures
			this.reloadTextures();

			this.makeBones();

			this.reset();

		} catch (Exception e) {
			Sys.alert("Error", "Failed to load model '" + file.toString()+ "'!\n\n" + e.getMessage());
			e.printStackTrace();
			System.exit(0);
		}

	}

	public void addJoint(MS3DJoint joint) {
		allJoints.put(joint.name, joint);

		if (joint.parentName.equals(""))
			rootJoints.addElement(joint);
	}

	public Hashtable<String, MS3DJoint> getJoints() {
		return allJoints;
	}

	/***************************************************************************************************************************************************************************************************
	 * reloadTextures() - Load the texture of each material
	 **************************************************************************************************************************************************************************************************/
	public void reloadTextures() {

		for (int i = 0; i < this.numMaterials; i++) {
			if (this.materials[i].textureID != 0)
				GL11.glDeleteTextures(BufferUtils.createIntBuffer(1).put(this.materials[i].textureID));
			else {
				try {
					this.materials[i].textureID = GLApp.makeTexture(this.textureDirectory+"/"+this.materials[i].texturemap);
				} catch (Exception e) {
					System.out.println("Failed to open texture file 'this.materials[i].texturemap'!");
				}
			}

		}
	}

	public void updateModel(long time) {
		// update the animation
		updateAnimation(time);

		boolean texturingEnabled = GL11.glIsEnabled(GL11.GL_TEXTURE_2D);

		// update the points
		for (int i = 0; i < numMeshs; i++) {
			if (this.meshs[i].materialIndex >= 0) {
				GL11.glMaterial(GL11.GL_FRONT, GL11.GL_AMBIENT,this.materials[this.meshs[i].materialIndex].ambient);
				GL11.glMaterial(GL11.GL_FRONT, GL11.GL_DIFFUSE,this.materials[this.meshs[i].materialIndex].diffuse);
				GL11.glMaterial(GL11.GL_FRONT, GL11.GL_SPECULAR,this.materials[this.meshs[i].materialIndex].specular);
				GL11.glMaterial(GL11.GL_FRONT, GL11.GL_EMISSION,this.materials[this.meshs[i].materialIndex].emissive);
				GL11.glMaterialf(GL11.GL_FRONT, GL11.GL_SHININESS,this.materials[this.meshs[i].materialIndex].shininess);

				if (this.materials[this.meshs[i].materialIndex].textureID > 0) {
					GL11.glBindTexture(GL11.GL_TEXTURE_2D,this.materials[this.meshs[i].materialIndex].textureID);
					GL11.glEnable(GL11.GL_TEXTURE_2D);
				} else {
					GL11.glDisable(GL11.GL_TEXTURE_2D);
				}
			} else {
				GL11.glDisable(GL11.GL_TEXTURE_2D);
			}
			float[] coords = new float[3];
			Vector4f temp = new Vector4f();
			Matrix4f matrix=null;
			GL11.glBegin(GL11.GL_TRIANGLES);
			for (int j = 0; j < meshs[i].numTriangles; j++) {

				for (int k = 0; k < 3; k++) {

					coords[0] = vertex[3 * triangles[meshs[i].triangleIndices[j]].vertexIndices[k]];
					coords[1] = vertex[3 * triangles[meshs[i].triangleIndices[j]].vertexIndices[k] + 1];
					coords[2] = vertex[3 * triangles[meshs[i].triangleIndices[j]].vertexIndices[k] + 2];

					if (vertices[triangles[meshs[i].triangleIndices[j]].vertexIndices[k]].boneID != -1) {
						matrix = joints[vertices[triangles[meshs[i].triangleIndices[j]].vertexIndices[k]].boneID].finalMatrix;

						temp.x = coords[0] * matrix.m00 + coords[1]
								* matrix.m10 + coords[2] * matrix.m20
								+ matrix.m30;
						temp.y = coords[0] * matrix.m01 + coords[1]
								* matrix.m11 + coords[2] * matrix.m21
								+ matrix.m31;
						temp.z = coords[0] * matrix.m02 + coords[1]
								* matrix.m12 + coords[2] * matrix.m22
								+ matrix.m32;
						temp.w = coords[0] * matrix.m03 + coords[1]
								* matrix.m13 + coords[2] * matrix.m23
								+ matrix.m33;

						GL11.glNormal3f(
										triangles[meshs[i].triangleIndices[j]].vertexNormals[0][0],
										triangles[meshs[i].triangleIndices[j]].vertexNormals[0][1],
										triangles[meshs[i].triangleIndices[j]].vertexNormals[0][2]);
						GL11.glTexCoord2f(
								triangles[meshs[i].triangleIndices[j]].s[k],
								triangles[meshs[i].triangleIndices[j]].t[k]);
						GL11.glVertex3f(temp.x, temp.y, temp.z);
					}

				}
			}
			GL11.glEnd();

		}

		// Reset texture state
		if (texturingEnabled)
			GL11.glEnable(GL11.GL_TEXTURE_2D);
		else
			GL11.glDisable(GL11.GL_TEXTURE_2D);

	}

	/***************************************************************************************************************************************************************************************************
	 * render() - Render this model
	 **************************************************************************************************************************************************************************************************/
	public void render() {

		// Save texture state
		boolean texturingEnabled = GL11.glIsEnabled(GL11.GL_TEXTURE_2D);

		// Render each mesh
		for (int m = 0; m < this.numMeshs; m++) {

			// Set texture for this mesh
			if (this.meshs[m].materialIndex >= 0) {
				GL11.glMaterial(GL11.GL_FRONT, GL11.GL_AMBIENT,this.materials[this.meshs[m].materialIndex].ambient);
				GL11.glMaterial(GL11.GL_FRONT, GL11.GL_DIFFUSE,this.materials[this.meshs[m].materialIndex].diffuse);
				GL11.glMaterial(GL11.GL_FRONT, GL11.GL_SPECULAR,this.materials[this.meshs[m].materialIndex].specular);
				GL11.glMaterial(GL11.GL_FRONT, GL11.GL_EMISSION,this.materials[this.meshs[m].materialIndex].emissive);
				GL11.glMaterialf(GL11.GL_FRONT, GL11.GL_SHININESS,this.materials[this.meshs[m].materialIndex].shininess);

				if (this.materials[this.meshs[m].materialIndex].textureID > 0) {
					GL11.glBindTexture(GL11.GL_TEXTURE_2D,this.materials[this.meshs[m].materialIndex].textureID);
					GL11.glEnable(GL11.GL_TEXTURE_2D);
				} else {
					GL11.glDisable(GL11.GL_TEXTURE_2D);
				}
			} else {
				GL11.glDisable(GL11.GL_TEXTURE_2D);
			}

			// Render each mesh
			GL11.glBegin(GL11.GL_TRIANGLES);
			{
				for (int t = 0; t < this.meshs[m].numTriangles; t++) {
					for (int v = 0; v < 3; v++) {
						GL11.glNormal3f(
										this.triangles[this.meshs[m].triangleIndices[t]].vertexNormals[v][0],
										this.triangles[this.meshs[m].triangleIndices[t]].vertexNormals[v][1],
										this.triangles[this.meshs[m].triangleIndices[t]].vertexNormals[v][2]);
						GL11.glTexCoord2f(
										this.triangles[this.meshs[m].triangleIndices[t]].s[v],
										this.triangles[this.meshs[m].triangleIndices[t]].t[v]);
						GL11.glVertex3f(
										this.vertices[this.triangles[this.meshs[m].triangleIndices[t]].vertexIndices[v]].vertex.x,
										this.vertices[this.triangles[this.meshs[m].triangleIndices[t]].vertexIndices[v]].vertex.y,
										this.vertices[this.triangles[this.meshs[m].triangleIndices[t]].vertexIndices[v]].vertex.z);
					}
				}
			}
			GL11.glEnd();

		}

		// Reset texture state
		if (texturingEnabled)
			GL11.glEnable(GL11.GL_TEXTURE_2D);
		else
			GL11.glDisable(GL11.GL_TEXTURE_2D);
	}

	public void reset() {
		for (int i = 0; i < numJoints; i++) {
			joints[i].currentRotationKeyframe = 0;
			joints[i].currentTranslationKeyframe = 0;
			joints[i].finalMatrix = new Matrix4f(joints[i].absoluteMatrix);
		}

		currentTime = 0;
		if (currentAnimation != -1) {
			MS3DAnimation ms3dAnimation = (MS3DAnimation) allAnimations.get(currentAnimation);
			currentTime += ms3dAnimation.startFrame * 1000.0 / animationFPS;
		}
		loopDone = false;
	}

	public void updateAnimation(long time) {

		currentTime += time - lastTime;
		lastTime = time;

		if (currentAnimation == -1) {
			endFrame = totalFrames;
		} else {
			MS3DAnimation ms3dAnimation = (MS3DAnimation) allAnimations.get(currentAnimation);
			endFrame = ms3dAnimation.endFrame;
		}

		if (currentTime >= (endFrame + 1) * 1000.0 / animationFPS) {
			if (loop) {
				reset();
			} else {
				currentTime = (long) ((endFrame + 1) * 1000.0 / animationFPS);
				loopDone = true;
			}
		}

		for (int i = 0; i < numJoints; i++) {
			Vector3f transVec = new Vector3f();
			Matrix4f transform = new Matrix4f();
			transform.setIdentity();

			short frame;
			MS3DJoint joint = joints[i];

			if (joint.numRotationKeyframes == 0
					&& joint.numTranslationKeyframes == 0) {
				joint.finalMatrix = new Matrix4f(joint.absoluteMatrix);
				continue;
			}

			frame = joint.currentTranslationKeyframe;
			while (frame < joint.numTranslationKeyframes
					&& joint.translationKeyframes[frame].time < currentTime) {
				frame++;
			}
			// System.out.println("frame:"+frame);
			joint.currentTranslationKeyframe = frame;

			if (frame == 0) {
				transVec.x = joint.translationKeyframes[0].x;
				transVec.y = joint.translationKeyframes[0].y;
				transVec.z = joint.translationKeyframes[0].z;
			} else if (frame == joint.numTranslationKeyframes) {
				transVec.x = joint.translationKeyframes[frame - 1].x;
				transVec.y = joint.translationKeyframes[frame - 1].y;
				transVec.z = joint.translationKeyframes[frame - 1].z;
			} else {
				MS3DKeyframe curFrame = joint.translationKeyframes[frame];
				MS3DKeyframe prevFrame = joint.translationKeyframes[frame - 1];

				float timeDelta = ((float) (curFrame.time - prevFrame.time));
				float interpValue = (float) ((currentTime - prevFrame.time) / (float) timeDelta);

				transVec.x = prevFrame.x + (curFrame.x - prevFrame.x) * interpValue;
				transVec.y = prevFrame.y + (curFrame.y - prevFrame.y) * interpValue;
				transVec.z = prevFrame.z + (curFrame.z - prevFrame.z) * interpValue;
			}

			//
			frame = joint.currentRotationKeyframe;

			while (frame < joint.numRotationKeyframes
					&& joint.rotationKeyframes[frame].time < currentTime) {
				frame++;
			}
			joint.currentRotationKeyframe = frame;

			if (frame == 0) {
				setRzRyRxRotationTranspose(transform,joint.rotationKeyframes[0].z,joint.rotationKeyframes[0].y,joint.rotationKeyframes[0].x);
			} else if (frame == joint.numRotationKeyframes) {
				setRzRyRxRotationTranspose(transform,joint.rotationKeyframes[frame - 1].z,joint.rotationKeyframes[frame - 1].y,joint.rotationKeyframes[frame - 1].x);
			} else {
				MS3DKeyframe curFrame = joint.rotationKeyframes[frame];
				MS3DKeyframe prevFrame = joint.rotationKeyframes[frame - 1];

				float timeDelta = ((float) (curFrame.time - prevFrame.time));
				float interpValue = (float) ((currentTime - prevFrame.time) / timeDelta);

				Vector3f rotVec = new Vector3f();

				rotVec.x = prevFrame.x + (curFrame.x - prevFrame.x) * interpValue;
				rotVec.y = prevFrame.y + (curFrame.y - prevFrame.y) * interpValue;
				rotVec.z = prevFrame.z + (curFrame.z - prevFrame.z) * interpValue;

				setRzRyRxRotationTranspose(transform, rotVec.z, rotVec.y, rotVec.x);

			}

			// Translation
			setTranslationTranspose(transform, transVec.x, transVec.y, transVec.z);

			Matrix4f relativeFinal = new Matrix4f();

			mulTransposeBothTanspose(relativeFinal, joint.relativeMatrix, transform);

			if (joint.parentName.equals("")) {

				joint.finalMatrix = relativeFinal;

			} else {

				MS3DJoint father = (MS3DJoint) allJoints.get(joint.parentName);

				joint.finalMatrix = new Matrix4f();

				mulTransposeBothTanspose(joint.finalMatrix, father.finalMatrix, relativeFinal);

			}

		}

	}

	public static void setRzRyRxRotationTranspose(Matrix4f matrix, float z, float y, float x) {
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

	public static void setTranslationTranspose(Matrix4f matrix, float x, float y, float z) {
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

	public static void mulTransposeBothTanspose(Matrix4f matrix, Matrix4f matrixA, Matrix4f matrixB) {
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

	public void addAnimation(MS3DAnimation ms3dAnimation) {
		allAnimations.addElement(ms3dAnimation);
	}

	public void deleteAnimations() {
		for (int i = 0; i < allAnimations.size(); i++) {
			allAnimations.remove(i);
		}
	}

	public int numberAnimations() {
		return allAnimations.size();
	}

	/**
	 * 初始化骨骼和绑定顶点
	 */
	private void makeBones() {
		MS3DJoint joint = null;
		MS3DVertex vertex = null;

		MS3DJoint father = null;

		// make the bone hierarchy
		for (int i = 0; i < numJoints; i++) {
			joint = joints[i];

			joint.relativeMatrix = new Matrix4f();

			// Rotation
			MS3DModel.setRzRyRxRotationTranspose(joint.relativeMatrix,joint.rotation[2], joint.rotation[1], joint.rotation[0]);

			// Translation
			MS3DModel.setTranslationTranspose(joint.relativeMatrix,joint.translation[0], joint.translation[1],joint.translation[2]);

			if (!joint.parentName.equals("")) {
				// 如果是子关节，那么将上一个节点的绝对矩阵乘上这个节点的变幻矩 阵就可以得到这个子节点的绝对矩阵了.
				father = (MS3DJoint) allJoints.get(joint.parentName);

				joint.absoluteMatrix = new Matrix4f();

				MS3DModel.mulTransposeBothTanspose(joint.absoluteMatrix,
						father.absoluteMatrix, joint.relativeMatrix);

			} else {
				// 如果是根关节那么它的绝对矩阵就是它的相对变换矩阵
				joint.absoluteMatrix = new Matrix4f(joint.relativeMatrix);

			}
		}

		// initialize the vertices
		for (int i = 0; i < numVertices; i++) {
			vertex = vertices[i];

			if (vertex.boneID != -1) {
				Matrix4f matrix = joints[vertex.boneID].absoluteMatrix;

				float x = vertex.vertex.x - matrix.m30;
				float y = vertex.vertex.y - matrix.m31;
				float z = vertex.vertex.z - matrix.m32;

				this.vertex[3 * i] = x * matrix.m00 + y * matrix.m01 + z * matrix.m02;
				this.vertex[3 * i + 1] = x * matrix.m10 + y * matrix.m11 + z * matrix.m12;
				this.vertex[3 * i + 2] = x * matrix.m20 + y * matrix.m21 + z * matrix.m22;

			}
		}

	}

	/***************************************************************************************************************************************************************************************************
	 * Header of a ms3d file
	 **************************************************************************************************************************************************************************************************/
	private class MS3DHeader {

		/** Read the header */
		public MS3DHeader(BinaryFileReader modelFile) {

			this.ID = modelFile.readString(10);
			this.version = modelFile.readInt();
		}

		/** Check whether the header is valid */
		public boolean isValid() {

			if ((this.ID.equals("MS3D000000") && this.version >= 3 && this.version <= 4))
				return true;
			else
				return false;
		}

		String ID;
		int version;
	}

	/***************************************************************************************************************************************************************************************************
	 * Vertex as used in ms3d files
	 **************************************************************************************************************************************************************************************************/
	@SuppressWarnings("unused")
	private class MS3DVertex {

		/** Read a vertex */
		public MS3DVertex(BinaryFileReader modelFile) {

			this.flags = modelFile.readByte();
			this.vertex = new Vector3f(modelFile.readFloat(), modelFile.readFloat(), modelFile.readFloat());
			this.boneID = modelFile.readByte();
			this.refCount = modelFile.readByte();
		}

		Vector3f vertex;
		int boneID;
		int flags;
		int refCount;
	}

	/***************************************************************************************************************************************************************************************************
	 * Triangle as used in ms3d files
	 **************************************************************************************************************************************************************************************************/
	@SuppressWarnings("unused")
	private class MS3DTriangle {

		/** Read a triangle */
		public MS3DTriangle(BinaryFileReader modelFile) {

			this.flags = modelFile.readShort();
			this.vertexIndices[0] = modelFile.readShort();
			this.vertexIndices[1] = modelFile.readShort();
			this.vertexIndices[2] = modelFile.readShort();
			this.vertexNormals[0][0] = modelFile.readFloat();
			this.vertexNormals[0][1] = modelFile.readFloat();
			this.vertexNormals[0][2] = modelFile.readFloat();
			this.vertexNormals[1][0] = modelFile.readFloat();
			this.vertexNormals[1][1] = modelFile.readFloat();
			this.vertexNormals[1][2] = modelFile.readFloat();
			this.vertexNormals[2][0] = modelFile.readFloat();
			this.vertexNormals[2][1] = modelFile.readFloat();
			this.vertexNormals[2][2] = modelFile.readFloat();
			this.s[0] = modelFile.readFloat();
			this.s[1] = modelFile.readFloat();
			this.s[2] = modelFile.readFloat();
			this.t[0] = 1 - modelFile.readFloat();
			this.t[1] = 1 - modelFile.readFloat();
			this.t[2] = 1 - modelFile.readFloat();
			this.smoothingMesh = modelFile.readByte();
			this.meshIndex = modelFile.readByte();
		}

		int flags;
		int vertexIndices[] = new int[3];
		float[][] vertexNormals = new float[3][3];
		float[] s = new float[3];
		float[] t = new float[3];
		int smoothingMesh;
		int meshIndex;
	}

	/***************************************************************************************************************************************************************************************************
	 * Mesh to group triangles which use the same material
	 **************************************************************************************************************************************************************************************************/
	@SuppressWarnings("unused")
	private class MS3DMesh {

		// Read a mesh
		public MS3DMesh(BinaryFileReader modelFile) {

			this.flags = modelFile.readByte();
			this.name = modelFile.readString(32);
			this.numTriangles = modelFile.readShort();
			this.triangleIndices = new int[this.numTriangles];
			for (int i = 0; i < this.numTriangles; i++)
				this.triangleIndices[i] = modelFile.readShort();
			this.materialIndex = modelFile.readByte();
		}

		String name;
		int materialIndex;
		int numTriangles;
		int[] triangleIndices;
		int flags;
	}

	/***************************************************************************************************************************************************************************************************
	 * Material as used in ms3d files
	 **************************************************************************************************************************************************************************************************/
	@SuppressWarnings("unused")
	private class MS3DMaterial {

		// Read a material
		public MS3DMaterial(BinaryFileReader modelFile) {

			this.name = modelFile.readString(32);
			float[] ambientArray = new float[] { modelFile.readFloat(),modelFile.readFloat(), modelFile.readFloat(),modelFile.readFloat() };
			float[] diffuseArray = new float[] { modelFile.readFloat(),modelFile.readFloat(), modelFile.readFloat(),modelFile.readFloat() };
			float[] specularArray = new float[] { modelFile.readFloat(),modelFile.readFloat(), modelFile.readFloat(),modelFile.readFloat() };
			float[] emissiveArray = new float[] { modelFile.readFloat(),modelFile.readFloat(), modelFile.readFloat(),modelFile.readFloat() };
			this.shininess = modelFile.readFloat();
			this.transparency = modelFile.readFloat();
			this.mode = modelFile.readByte();
			this.texturemap = modelFile.readString(128);
			this.alphamap = modelFile.readString(128);

			// // Fix paths
			if (this.texturemap.startsWith("..\\"))
				this.texturemap = this.texturemap.substring(3);

			if (this.texturemap.startsWith(".\\"))
				this.texturemap = this.texturemap.substring(2);

			if (this.alphamap.startsWith("..\\"))
				this.alphamap = this.alphamap.substring(3);

			if (this.alphamap.startsWith(".\\"))
				this.alphamap = this.alphamap.substring(2);

			// Fill buffers
			this.ambient.put(ambientArray).flip();
			this.diffuse.put(diffuseArray).flip();
			this.specular.put(specularArray).flip();
			this.emissive.put(emissiveArray).flip();
		}

		String name;
		String texturemap;
		int textureID;
		String alphamap;
		FloatBuffer ambient = BufferUtils.createFloatBuffer(4);
		FloatBuffer diffuse = BufferUtils.createFloatBuffer(4);
		FloatBuffer specular = BufferUtils.createFloatBuffer(4);
		FloatBuffer emissive = BufferUtils.createFloatBuffer(4);
		float shininess;
		float transparency;
		int mode;
	}

	/***************************************************************************************************************************************************************************************************
	 * Joint information as used in ms3d files
	 **************************************************************************************************************************************************************************************************/
	@SuppressWarnings("unused")
	private class MS3DJoint {

		// Read Joint
		public MS3DJoint(BinaryFileReader modelFile) {
			this.flag = modelFile.readByte();
			this.name = modelFile.readString(32);
			System.out.print(name + ":");
			this.parentName = modelFile.readString(32);
			this.rotation[0] = modelFile.readFloat();
			this.rotation[1] = modelFile.readFloat();
			this.rotation[2] = modelFile.readFloat();
			this.translation[0] = modelFile.readFloat();
			this.translation[1] = modelFile.readFloat();
			this.translation[2] = modelFile.readFloat();
			this.numRotationKeyframes = modelFile.readShort();
			this.numTranslationKeyframes = modelFile.readShort();
			System.out.println(this.numRotationKeyframes + ","
					+ this.numTranslationKeyframes);

			this.rotationKeyframes = new MS3DKeyframe[numRotationKeyframes];
			for (int i = 0; i < this.numRotationKeyframes; i++)
				this.rotationKeyframes[i] = new MS3DKeyframe(modelFile);

			this.translationKeyframes = new MS3DKeyframe[numTranslationKeyframes];
			for (int i = 0; i < this.numTranslationKeyframes; i++)
				this.translationKeyframes[i] = new MS3DKeyframe(modelFile);

		}

		int flag;
		String name;
		String parentName;
		float[] rotation = new float[3];
		float[] translation = new float[3];
		int numRotationKeyframes;
		int numTranslationKeyframes;
		MS3DKeyframe[] rotationKeyframes;
		MS3DKeyframe[] translationKeyframes;

		Matrix4f absoluteMatrix;
		Matrix4f relativeMatrix;
		Matrix4f finalMatrix;
		short currentRotationKeyframe;
		short currentTranslationKeyframe;
	}

	/***************************************************************************************************************************************************************************************************
	 * Keyframe as used in ms3d files
	 **************************************************************************************************************************************************************************************************/
	private class MS3DKeyframe {

		// Read Keyframe
		public MS3DKeyframe(BinaryFileReader modelFile) {
			this.time = (long) (modelFile.readFloat() * 1000);
			this.x = modelFile.readFloat();
			this.y = modelFile.readFloat();
			this.z = modelFile.readFloat();
		}

		float time; // time in seconds
		float x, y, z;
	}

	@SuppressWarnings("unused")
	private class MS3DAnimation {

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

}
