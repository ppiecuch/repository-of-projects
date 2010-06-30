package org.java3dgamesdk.graphics.MS3D;

import java.nio.*;
import java.io.*;
import java.util.*;
import javax.media.j3d.*;
import javax.vecmath.*;
import com.sun.j3d.utils.image.*;

public class MS3DLoader {

	// Boundings
	private float maxX;
	private float maxY;
	private float maxZ;
	private float minX;
	private float minY;
	private float minZ;

	private static java.awt.Component component;

	private MS3DModel model;

	private ByteBuffer byteBuffer;

	private Hashtable textureCache;

	public MS3DLoader() {
		model = null;

		byteBuffer = null;

		textureCache = new Hashtable();
	}

	public MS3DModel loadModel(String name, String filename) {

		model = null;
		maxX = 0.0f;
		maxY = 0.0f;
		maxZ = 0.0f;
		minX = 0.0f;
		minY = 0.0f;
		minZ = 0.0f;

		byte data[] = getRawData(filename);

		if (data != null) {
			byteBuffer = ByteBuffer.wrap(data).order(ByteOrder.LITTLE_ENDIAN);

			model = new MS3DModel(name);

			readHeader();

			readNumberVertices();
			readVertices();

			readNumberTriangles();
			readTriangles();

			readNumberGroups();
			readGroups();

			readNumberMaterials();
			readMaterials();

			readKeyFrameData();

			readNumberJoints();
			readJoints();

			buildModel();

			makeBones();

			reset();
		}

		return model;
	}

	/*
	 * Loads the raw data from the ms3d file.
	 */
	private byte[] getRawData(String filename) {
		byte buffer[] = null;

		try {
			File file = new File(filename);

			int length = (int) file.length();

			buffer = new byte[length];

			FileInputStream fis = new FileInputStream(file);

			fis.read(buffer);

			fis.close();
		} catch (Exception e) {
			buffer = null;
		}

		return buffer;
	}

	// read the Header
	private void readHeader() {
		byte buffer[] = new byte[10];
		for (int k = 0; k < 10; k++)
			buffer[k] = byteBuffer.get();
		model.id = makeString(buffer);

		model.version = byteBuffer.getInt();
	}

	// read number of vertices
	private void readNumberVertices() {
		model.nNumVertices = getUnsignedShortValue(byteBuffer.getShort());

		model.vertices = new MS3DVertex[model.nNumVertices];
		model.vertex = new float[3 * model.nNumVertices];
	}

	// read vertices
	private void readVertices() {
		MS3DVertex vertex = null;

		for (int i = 0; i < model.nNumVertices; i++) {
			vertex = new MS3DVertex();

			vertex.flags = byteBuffer.get();

			vertex.x = byteBuffer.getFloat();
			vertex.y = byteBuffer.getFloat();
			vertex.z = byteBuffer.getFloat();
			vertex.boneId = byteBuffer.get();
			vertex.referenceCount = byteBuffer.get();

			model.vertices[i] = vertex;
			model.vertex[3 * i] = vertex.x;
			model.vertex[3 * i + 1] = vertex.y;
			model.vertex[3 * i + 2] = vertex.z;

			// new for bounds
			if (i == 0) {
				minX = maxX = vertex.x;
				minY = maxY = vertex.y;
				minZ = maxZ = vertex.z;
			} else {
				if (vertex.x < minX)
					minX = vertex.x;
				else if (vertex.x > maxX)
					maxX = vertex.x;

				if (vertex.y < minY)
					minY = vertex.y;
				else if (vertex.y > maxY)
					maxY = vertex.y;

				if (vertex.z < minZ)
					minZ = vertex.z;
				else if (vertex.z > maxZ)
					maxZ = vertex.z;

			}
		}
	}

	// read number of triangles
	private void readNumberTriangles() {
		model.nNumTriangles = getUnsignedShortValue(byteBuffer.getShort());

		model.triangles = new MS3DTriangle[model.nNumTriangles];
	}

	// read triangles
	private void readTriangles() {
		MS3DTriangle triangle = null;

		for (int i = 0; i < model.nNumTriangles; i++) {
			triangle = new MS3DTriangle();

			triangle.flags = byteBuffer.getShort();

			for (int k = 0; k < 3; k++)
				triangle.vertexIndices[k] = getUnsignedShortValue(byteBuffer
						.getShort());

			for (int k = 0; k < 3; k++) {
				triangle.vertexNormals[k] = new Vector3f(byteBuffer.getFloat(),
						byteBuffer.getFloat(), byteBuffer.getFloat());
			}

			for (int k = 0; k < 3; k++) {
				triangle.texCoord[k] = new TexCoord2f(byteBuffer.getFloat(),
						0.0f);
			}

			for (int k = 0; k < 3; k++)
				triangle.texCoord[k].y = 1.0f - byteBuffer.getFloat();

			triangle.smoothingGroup = byteBuffer.get();
			triangle.groupIndex = byteBuffer.get();

			model.triangles[i] = triangle;
		}
	}

	// read number of groups
	private void readNumberGroups() {
		model.nNumGroups = getUnsignedShortValue(byteBuffer.getShort());

		model.groups = new MS3DGroup[model.nNumGroups];
	}

	// read groups
	private void readGroups() {
		MS3DGroup group = null;

		for (int i = 0; i < model.nNumGroups; i++) {
			group = new MS3DGroup();

			group.flags = byteBuffer.get();

			byte buffer[] = new byte[32];
			for (int k = 0; k < 32; k++)
				buffer[k] = byteBuffer.get();
			group.name = makeString(buffer);

			group.numtriangles = getUnsignedShortValue(byteBuffer.getShort());

			group.triangleIndices = new int[group.numtriangles];

			for (int k = 0; k < group.numtriangles; k++)
				group.triangleIndices[k] = getUnsignedShortValue(byteBuffer
						.getShort());
			group.materialIndex = byteBuffer.get();

			model.groups[i] = group;
		}
	}

	// read number of materials
	private void readNumberMaterials() {
		model.nNumMaterials = getUnsignedShortValue(byteBuffer.getShort());

		model.materials = new MS3DMaterial[model.nNumMaterials];
	}

	// read materials
	private void readMaterials() {
		MS3DMaterial material = null;

		for (int i = 0; i < model.nNumMaterials; i++) {
			material = new MS3DMaterial();

			byte buffer[] = new byte[32];
			for (int k = 0; k < 32; k++)
				buffer[k] = byteBuffer.get();
			material.name = makeString(buffer);

			material.ambient = new Color4f(byteBuffer.getFloat(), byteBuffer
					.getFloat(), byteBuffer.getFloat(), byteBuffer.getFloat());

			material.diffuse = new Color4f(byteBuffer.getFloat(), byteBuffer
					.getFloat(), byteBuffer.getFloat(), byteBuffer.getFloat());

			material.specular = new Color4f(byteBuffer.getFloat(), byteBuffer
					.getFloat(), byteBuffer.getFloat(), byteBuffer.getFloat());

			material.emissive = new Color4f(byteBuffer.getFloat(), byteBuffer
					.getFloat(), byteBuffer.getFloat(), byteBuffer.getFloat());

			material.shininess = byteBuffer.getFloat();

			material.transparency = byteBuffer.getFloat();

			material.mode = (char) byteBuffer.get();

			buffer = new byte[128];
			for (int k = 0; k < 128; k++)
				buffer[k] = byteBuffer.get();
			material.texture = makeString(buffer);

			buffer = new byte[128];
			for (int k = 0; k < 128; k++)
				buffer[k] = byteBuffer.get();
			material.alphamap = makeString(buffer);

			model.materials[i] = material;
		}
	}

	// read key frame data
	private void readKeyFrameData() {
		model.fAnimationFPS = byteBuffer.getFloat();
		model.currentTime = (long) (byteBuffer.getFloat() * 1000);
		model.iTotalFrames = byteBuffer.getInt();
	}

	// read number of joints
	private void readNumberJoints() {
		model.nNumJoints = getUnsignedShortValue(byteBuffer.getShort());

		model.joints = new MS3DJoint[model.nNumJoints];
	}

	// read joints
	private void readJoints() {
		MS3DJoint joint = null;

		for (int i = 0; i < model.nNumJoints; i++) {
			joint = new MS3DJoint();

			joint.flags = byteBuffer.get();

			byte buffer[] = new byte[32];
			for (int k = 0; k < 32; k++)
				buffer[k] = byteBuffer.get();
			joint.name = makeString(buffer);

			buffer = new byte[32];
			for (int k = 0; k < 32; k++)
				buffer[k] = byteBuffer.get();
			joint.parentName = makeString(buffer);

			joint.rotX = byteBuffer.getFloat();
			joint.rotY = byteBuffer.getFloat();
			joint.rotZ = byteBuffer.getFloat();

			joint.x = byteBuffer.getFloat();
			joint.y = byteBuffer.getFloat();
			joint.z = byteBuffer.getFloat();

			joint.numKeyFramesRot = getUnsignedShortValue(byteBuffer.getShort());
			joint.keyRotations = new MS3DKeyFrameRotation[joint.numKeyFramesRot];
			joint.numKeyFramesTrans = getUnsignedShortValue(byteBuffer
					.getShort());
			joint.keyTranslations = new MS3DKeyFrameTranslation[joint.numKeyFramesTrans];

			MS3DKeyFrameRotation keyRotation = null;
			for (int k = 0; k < joint.numKeyFramesRot; k++) {
				keyRotation = new MS3DKeyFrameRotation();

				keyRotation.time = (long) (byteBuffer.getFloat() * 1000);

				keyRotation.rotX = byteBuffer.getFloat();
				keyRotation.rotY = byteBuffer.getFloat();
				keyRotation.rotZ = byteBuffer.getFloat();

				joint.keyRotations[k] = keyRotation;
			}

			MS3DKeyFrameTranslation keyTranslation = null;
			for (int k = 0; k < joint.numKeyFramesTrans; k++) {
				keyTranslation = new MS3DKeyFrameTranslation();

				keyTranslation.time = (long) (byteBuffer.getFloat() * 1000);

				keyTranslation.x = byteBuffer.getFloat();
				keyTranslation.y = byteBuffer.getFloat();
				keyTranslation.z = byteBuffer.getFloat();

				joint.keyTranslations[k] = keyTranslation;
			}

			model.joints[i] = joint;
			// add to the animation list
			model.addJoint(joint);
		}
	}

	//
	//
	//

	private String makeString(byte data[]) {
		for (int i = 0; i < data.length; i++) {
			if (data[i] == 0)
				return new String(data, 0, i);
		}

		return new String(data);
	}

	//
	//
	//

	private void buildModel() {

		Shape3D shape3d = null;
		TriangleArray triangleArray = null;

		Appearance appearance = null;

		MS3DGroup group = null;
		MS3DMaterial groupMaterial = null;
		int vertex = 0;
		int texvertex = 0;
		for (int i = 0; i < model.groups.length; i++) {
			group = model.groups[i];
			if (group.materialIndex != -1)
				groupMaterial = model.materials[group.materialIndex];
			vertex = 0;
			texvertex = 0;

			shape3d = new Shape3D();

			appearance = new Appearance();

			group.coords = new float[group.numtriangles * 3 * 3];
			group.coordsIndex = new int[group.numtriangles * 3];
			group.normals = new float[group.numtriangles * 3 * 3];
			group.texcoords = new float[group.numtriangles * 3 * 2];

			if (group.numtriangles > 0) {
				triangleArray = new TriangleArray(group.numtriangles * 3,
						GeometryArray.COORDINATES | GeometryArray.NORMALS
								| GeometryArray.TEXTURE_COORDINATE_2
								| GeometryArray.BY_REFERENCE);
				MS3DTriangle triangle = null;
				for (int k = 0; k < group.numtriangles; k++) {
					triangle = model.triangles[group.triangleIndices[k]];

					for (int l = 0; l < 3; l++) {

						if (group.materialIndex != -1) {
							if (groupMaterial.texture.length() > 0) {
								group.texcoords[texvertex++] = triangle.texCoord[l].x;
								group.texcoords[texvertex++] = triangle.texCoord[l].y;
							}
						}

						group.coordsIndex[k * 3 + l] = triangle.vertexIndices[l];

						group.coords[vertex] = model.vertices[triangle.vertexIndices[l]].x;
						group.normals[vertex++] = triangle.vertexNormals[l].x;
						group.coords[vertex] = model.vertices[triangle.vertexIndices[l]].y;
						group.normals[vertex++] = triangle.vertexNormals[l].y;
						group.coords[vertex] = model.vertices[triangle.vertexIndices[l]].z;
						group.normals[vertex++] = triangle.vertexNormals[l].z;

					}
				}
				triangleArray.setCoordRefFloat(group.coords);
				triangleArray
						.setCapability(GeometryArray.ALLOW_COORDINATE_READ);
				triangleArray
						.setCapability(GeometryArray.ALLOW_COORDINATE_WRITE);

				triangleArray.setNormalRefFloat(group.normals);
				triangleArray.setCapability(GeometryArray.ALLOW_NORMAL_READ);
				triangleArray.setCapability(GeometryArray.ALLOW_NORMAL_WRITE);

				triangleArray.setTexCoordRefFloat(0, group.texcoords);
				triangleArray.setCapability(GeometryArray.ALLOW_TEXCOORD_READ);
				triangleArray.setCapability(GeometryArray.ALLOW_TEXCOORD_WRITE);

				triangleArray.setCapability(GeometryArray.ALLOW_REF_DATA_READ);
				triangleArray.setCapability(GeometryArray.ALLOW_REF_DATA_READ);

				shape3d.setGeometry(triangleArray);
				shape3d.setCapability(Shape3D.ALLOW_GEOMETRY_READ);
				shape3d.setCapability(Shape3D.ALLOW_GEOMETRY_WRITE);

				// create the material
				if (group.materialIndex != -1) {

					Material material = new Material(new Color3f(
							groupMaterial.ambient.x, groupMaterial.ambient.y,
							groupMaterial.ambient.z), new Color3f(
							groupMaterial.emissive.x, groupMaterial.emissive.y,
							groupMaterial.emissive.z), new Color3f(
							groupMaterial.diffuse.x, groupMaterial.diffuse.y,
							groupMaterial.diffuse.z), new Color3f(
							groupMaterial.specular.x, groupMaterial.specular.y,
							groupMaterial.specular.z), groupMaterial.shininess);

					appearance.setMaterial(material);

					// Texture
					if (groupMaterial.texture.length() > 0) {

						Texture texture = (Texture) textureCache
								.get(groupMaterial.texture);

						if (texture == null) {
							// needed for the different operating systems
							String filename = groupMaterial.texture;
							filename = filename.replace('\\',
									File.separatorChar);
							//
							// @@ changed by Boris
							TextureLoader textureLoader = new TextureLoader(
									filename, TextureLoader.GENERATE_MIPMAP,
									component);
							texture = textureLoader.getTexture();

							// @@ added by Boris
							texture.setMinFilter(Texture.NICEST);
							texture.setMagFilter(Texture.NICEST);

							textureCache.put(groupMaterial.texture, texture);
						}

						appearance.setTexture(texture);
						TextureAttributes ta = new TextureAttributes();
						ta.setCapability(TextureAttributes.ALLOW_MODE_READ);
						ta.setCapability(TextureAttributes.ALLOW_MODE_WRITE);
						ta.setTextureMode(TextureAttributes.MODULATE);
						appearance.setTextureAttributes(ta);
					}
				}
				shape3d.setAppearance(appearance);

				model.addShape3D(model.groups[i].name, shape3d);
			} else {
				triangleArray = null;
				System.out.println("Attention: The group \""
						+ model.groups[i].name + "\" in the model named \""
						+ model.name + "\" doesn't contain any triangle!");
			}
		}
	}

	private void makeBones() {
		MS3DJoint joint = null;
		MS3DVertex vertex = null;

		// refer vertices to the joints
		for (int i = 0; i < model.nNumVertices; i++) {
			vertex = model.vertices[i];

			if (vertex.boneId != -1) {
				joint = model.joints[vertex.boneId];
			}
		}

		MS3DJoint father = null;

		// make the bone hierarchy
		for (int i = 0; i < model.nNumJoints; i++) {
			joint = model.joints[i];

			joint.relativeMatrix = new Matrix4f();

			// Rotation
			MS3DModel.setRzRyRxRotationTranspose(joint.relativeMatrix,
					joint.rotZ, joint.rotY, joint.rotX);

			// Translation
			MS3DModel.setTranslationTranspose(joint.relativeMatrix, joint.x,
					joint.y, joint.z);

			if (!joint.parentName.equals("")) {
				father = (MS3DJoint) model.getJoints().get(joint.parentName);

				joint.absoluteMatrix = new Matrix4f();

				MS3DModel.mulTransposeBothTanspose(joint.absoluteMatrix,
						father.absoluteMatrix, joint.relativeMatrix);

			} else {

				joint.absoluteMatrix = new Matrix4f(joint.relativeMatrix);

			}
		}

		// initialize the vertices
		for (int i = 0; i < model.nNumVertices; i++) {
			vertex = model.vertices[i];

			if (vertex.boneId != -1) {
				Matrix4f matrix = model.joints[vertex.boneId].absoluteMatrix;

				float x = vertex.x - matrix.m30;
				float y = vertex.y - matrix.m31;
				float z = vertex.z - matrix.m32;

				model.vertex[3 * i] = x * matrix.m00 + y * matrix.m01 + z
						* matrix.m02;
				model.vertex[3 * i + 1] = x * matrix.m10 + y * matrix.m11 + z
						* matrix.m12;
				model.vertex[3 * i + 2] = x * matrix.m20 + y * matrix.m21 + z
						* matrix.m22;

			}
		}

	}

	private void reset() {

		model.reset();

	}

	public BoundingSphere getBoundingSphere() {
		Point3d lower = new Point3d(minX, minY, minZ);
		Point3d upper = new Point3d(maxX, maxY, maxZ);

		// Point3d center = new Point3d((maxX + minX)/2, (maxY + minY)/2, (maxZ
		// + minZ)/2);
		Point3d center = new Point3d(0, 0, 0);

		double distance1 = center.distance(upper);
		double distance2 = center.distance(lower);
		if (distance1 < distance2)
			distance1 = distance2;

		return new BoundingSphere(center, distance1 / 2);
	}

	public BoundingBox getBoundingBox() {

		Point3d lower = new Point3d(minX, minY, minZ);
		Point3d upper = new Point3d(maxX, maxY, maxZ);

		return new BoundingBox(lower, upper);
	}

	public int getUnsignedShortValue(short value) {
		if (value < 0) {
			return Short.MAX_VALUE + (value - Short.MIN_VALUE) + 1;
		} else {
			return value;
		}
	}

}
