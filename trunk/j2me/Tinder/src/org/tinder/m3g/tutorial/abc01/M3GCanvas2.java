package org.tinder.m3g.tutorial.abc01;

import javax.microedition.lcdui.*;
import javax.microedition.m3g.*;
import javax.microedition.lcdui.game.*;

class M3GCanvas2 extends GameCanvas implements Runnable {
	private Graphics3D g3d; // Graphics object used to render the 3d world.

	private Graphics g2d;

	private World world; // This world contains the camera and the
							// pyramidMesh.

	private Camera camera; // the camera in the scene

	private IndexBuffer indexBuffer;

	private Transform cubeTransform = new Transform();

	private VertexBuffer vertexBuffer = new VertexBuffer();

	private Appearance appearance = new Appearance();

	private Image image;

	public M3GCanvas2() {
		super(false);

		setFullScreenMode(false);
		camera = new Camera();
		float aspect = (float) getWidth() / (float) getHeight();
		camera.setPerspective(45.0f, aspect, 1.0f, 1000.0f);
		Transform cameraTransform = new Transform();
		cameraTransform.postTranslate(0, 0, 6.0f);
		g3d = Graphics3D.getInstance();
		g3d.setCamera(camera, cameraTransform);
		createCube();
		Thread t = new Thread(this);
		t.start();
	}

	public void run() {
		g2d = getGraphics();
		while (true) {
			try {
				g3d.bindTarget(g2d); // Binds the given Graphics or mutable
										// Image2D as the rendering target of
										// this Graphics3D
				g3d.clear(null);
				g3d
						.render(vertexBuffer, indexBuffer, appearance,
								cubeTransform);
			} finally {
				g3d.releaseTarget();
			}
			flushGraphics();
		}
	}

	private void createCube() {
		/** The cube's vertex positions (x, y, z). */
		byte[] POINTS = new byte[] { -1, -1, 1, 1, -1, 1, -1, 1, 1, 1, 1, 1, // front
				1, -1, -1, -1, -1, -1, 1, 1, -1, -1, 1, -1, // back
				1, -1, 1, 1, -1, -1, 1, 1, 1, 1, 1, -1, // right
				-1, -1, -1, -1, -1, 1, -1, 1, -1, -1, 1, 1, // left
				-1, 1, 1, 1, 1, 1, -1, 1, -1, 1, 1, -1, // top
				-1, -1, -1, 1, -1, -1, -1, -1, 1, 1, -1, 1 // bottom
		};

		/**
		 * Indices that define how to connect the vertices to build triangles.
		 */
		byte[] NORMALS = { 0, 0, 127, 0, 0, 127, 0, 0, 127, 0, 0, 127, // front
				0, 0, -128, 0, 0, -128, 0, 0, -128, 0, 0, -128, // back
				127, 0, 0, 127, 0, 0, 127, 0, 0, 127, 0, 0, // right
				-128, 0, 0, -128, 0, 0, -128, 0, 0, -128, 0, 0, // left
				0, 127, 0, 0, 127, 0, 0, 127, 0, 0, 127, 0, // top
				0, -128, 0, 0, -128, 0, 0, -128, 0, 0, -128, 0, // bottom
		};

		byte[] COLORS = new byte[] {

		0, 0, (byte) 128, 0, 0, (byte) 255, 0, (byte) 255, 0, 0, (byte) 255,
				(byte) 255, (byte) 255, 0, (byte) 255, (byte) 255, 0, 0,
				(byte) 255, (byte) 255, (byte) 255, (byte) 255, (byte) 255, 0,
				0, 0, (byte) 255, (byte) 255, 0, 0, 0, (byte) 255, (byte) 255,
				(byte) 255, (byte) 255, (byte) 255, (byte) 255, 0, 0, 0, 0,
				(byte) 128, (byte) 255, (byte) 255, 0, 0, (byte) 255, 0, 0,
				(byte) 255, 0, 0, 0, (byte) 128, (byte) 255, (byte) 255, 0, 0,
				(byte) 255, 0, (byte) 255, 0, 0, (byte) 255, 0, (byte) 255, 0,
				0, (byte) 128, 0, 0, (byte) 255 };// B

		short[] TEXCOORDS = { 96, 32, 64, 32, 96, 64, 64, 64, 64, 32, 32, 32,
				64, 64, 32, 64, 64, 0, 32, 0, 64, 32, 32, 32, 32, 0, 0, 0, 32,
				32, 0, 32, 32, 32, 0, 32, 32, 64, 0, 64, 96, 0, 64, 0, 96, 32,
				64, 32 };
		int[] INDICES = new int[] { 0, 1, 2, 3, // front
				4, 5, 6, 7, // back
				8, 9, 10, 11, // right
				12, 13, 14, 15, // left
				16, 17, 18, 19, // top
				20, 21, 22, 23, // bottom
		};
		// The length of each sequence in the indices array.
		int[] LENGTH = new int[] { 4, 4, 4, 4, 4, 4 }; // the pyramid is built
														// by 12 triangles

		VertexArray POSITION_ARRAY, COLOR_ARRAY;

		// Create a VertexArray to be used by the VertexBuffer
		POSITION_ARRAY = new VertexArray(POINTS.length / 3, 3, 1);
		POSITION_ARRAY.set(0, POINTS.length / 3, POINTS);
		COLOR_ARRAY = new VertexArray(COLORS.length / 3, 3, 1);
		COLOR_ARRAY.set(0, COLORS.length / 3, COLORS);
		VertexArray NORMAL_ARRAY = new VertexArray(NORMALS.length / 3, 3, 1);
		// NORMAL_ARRAY.set(0, NORMALS.length / 3, NORMALS);
		VertexArray TEXCOORD_ARRAY = new VertexArray(TEXCOORDS.length / 2, 2, 2);
		TEXCOORD_ARRAY.set(0, TEXCOORDS.length / 2, TEXCOORDS);

		indexBuffer = new TriangleStripArray(INDICES, LENGTH);

		// VertexBuffer holds references to VertexArrays that contain the
		// positions, colors, normals,
		// and texture coordinates for a set of vertices

		vertexBuffer.setPositions(POSITION_ARRAY, 1.0f, null);
		vertexBuffer.setColors(COLOR_ARRAY);
		vertexBuffer.setNormals(NORMAL_ARRAY);
		vertexBuffer.setTexCoords(0, TEXCOORD_ARRAY, (1.0f / 128.0f), null); // 128-pixel
																				// scale,
																				// zero
																				// bias
		System.out.println(vertexBuffer.getVertexCount());
		float[] matrix = new float[] { 0.7615467f, 0.44544515f, 0.47077128f,
				0.0f, -0.059390657f, 0.77128255f, -0.6337161f, 0.0f,
				-0.64538336f, 0.45464504f, 0.61382306f, 0.0f, 0.0f, 0.0f, 0.0f,
				1.0f };
		/*
		 * try{ image = Image.createImage("/cubeface.png");
		 * 
		 * }catch(Exception e) { e.printStackTrace(); }Image2D image2D = new
		 * Image2D( Image2D.RGB, image ); Texture2D texture = new Texture2D(
		 * image2D ); texture.setFiltering( Texture2D.FILTER_NEAREST,
		 * Texture2D.FILTER_NEAREST ); texture.setWrapping(
		 * Texture2D.WRAP_CLAMP, Texture2D.WRAP_CLAMP ); texture.setBlending(
		 * Texture2D.FUNC_MODULATE );
		 */
		PolygonMode polygonMode = new PolygonMode(); // An Appearance
														// component
														// encapsulating
														// polygon-level
														// attributes
		polygonMode.setPerspectiveCorrectionEnable(true);
		polygonMode.setWinding(PolygonMode.WINDING_CCW);
		polygonMode.setCulling(PolygonMode.CULL_BACK); // By using CULL_NONE
														// all faces of the
														// pyramid will be
														// shown.
		polygonMode.setShading(PolygonMode.SHADE_SMOOTH);
		// appearance.setTexture( 0, texture );
		appearance.setPolygonMode(polygonMode);
		cubeTransform.set(matrix);
		// Mesh cubeMesh = new Mesh(vertexBuffer, indexBuffer, null);

	}
}
