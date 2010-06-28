/*
 * TextureManager.java
 *
 * Created on 25. Juni 2006, 15:27
 */

package de.bloodyblades.ms3dloader;



import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.HashMap;

import org.lwjgl.BufferUtils;
import org.lwjgl.LWJGLException;
import org.lwjgl.opengl.GL11;
import org.lwjgl.util.glu.GLU;


/**
 * Static texture class to load textures and lightmaps
 * 
 * @author xindon
 */
public class TextureManager {

	public static final boolean DEBUG = false; // Debug flag

	/* Texture filter definitions */
	public static int FILTER_NONE = 0;
	public static int FILTER_LINEAR = 1;
	public static int FILTER_MIPMAP = 2;

	// Num textures loaded
	private static int numTotalLoaded = 0;
	private static int numLoaded = 0;

	// Textures loaded
	private static HashMap<String, Number> loadedTextures = new HashMap<String, Number>();


	/***************************************************************************************************************************************************************************************************
	 * loadTexture() - Load a texture in OpenGL
	 **************************************************************************************************************************************************************************************************/
//	public static int loadTexture(InputStream file, boolean flip, int filter) {
//
//		// Make sure DevIL is created
//		checkDevIL();
//
//		// Is Texture already loaded?
//		if (loadedTextures.get(file) != null) {
//			if (DEBUG)
//				System.out.println("[TextureManager] Texture '" + file.toString() + "' was already loaded! ID is " + loadedTextures.get(file.toString()).intValue() + ".");
//
//			return loadedTextures.get(file.toString()).intValue();
//
//		} else {
//
//			if (DEBUG)
//				System.out.println("[TextureManager] Loading texture '" + file.toString() + "'...");
//
//			ByteBuffer imageData = null;
//			IntBuffer tmp = BufferUtils.createIntBuffer(1);
//			int ilImageHandle;
//			int oglImageHandle;
//
//			// Create and bind empty image to DevIL
//			IL.ilGenImages(tmp);
//			ilImageHandle = tmp.get(0);
//			IL.ilBindImage(ilImageHandle);
//
//			try {
//				if (!IL.ilLoadFromStream(file, 0)) {
//					if (DEBUG)
//						System.out.println("[TextureManager] Failed to load texture '" + file.toString() + "'!");
//					return -1;
//				}
//			} catch (Exception e) {
//				if (DEBUG)
//					System.out.println("[TextureManager] Failed to load texture '" + file.toString() + "'!");
//			}
//
//			// Convert the image to RGBA
//			IL.ilConvertImage(IL.IL_RGBA, IL.IL_BYTE);
//
//			// Flip the image if needed
//			if (flip)
//				ILU.iluFlipImage();
//
//			// Image attributes
//			int width = IL.ilGetInteger(IL.IL_IMAGE_WIDTH);
//			int height = IL.ilGetInteger(IL.IL_IMAGE_HEIGHT);
//			int desiredWidth = getNextPowerOfTwo(width);
//			int desiredHeight = getNextPowerOfTwo(height);
//
//			// Resize image
//			if (desiredWidth != width || desiredHeight != height) {
//
//				// System.out.println("Resize...");
//				ILU.iluImageParameter(ILU.ILU_FILTER, ILU.ILU_BILINEAR);
//				ILU.iluScale(desiredWidth, desiredHeight, IL.ilGetInteger(IL.IL_IMAGE_DEPTH));
//
//				// Update texture variables
//				width = desiredWidth;
//				height = desiredHeight;
//			}
//
//			// Get Image data
//			imageData = IL.ilGetData();
//
//			// Create the texture in OpenGL
//			GL11.glGenTextures(tmp);
//			oglImageHandle = tmp.get(0);
//
//			// Bind this texture
//			GL11.glBindTexture(GL11.GL_TEXTURE_2D, oglImageHandle);
//
//			// If a jpg texture, need to set UNPACK_ALIGNMENT to 1
//			if (file.toString().endsWith(".jpg") || file.toString().endsWith(".jpeg"))
//				GL11.glPixelStorei(GL11.GL_UNPACK_ALIGNMENT, 1);
//
//			// Create the texture depending on the chosen filter
//			if (filter == FILTER_NONE) { // No Texture Filtering
//
//				GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MAG_FILTER, GL11.GL_NEAREST);
//				GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MIN_FILTER, GL11.GL_NEAREST);
//				GL11.glTexImage2D(GL11.GL_TEXTURE_2D, 0, GL11.GL_RGBA, width, height, 0, GL11.GL_RGBA, GL11.GL_UNSIGNED_BYTE, imageData);
//
//			} else if (filter == FILTER_LINEAR) { // Linear Filtering
//
//				GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MIN_FILTER, GL11.GL_LINEAR);
//				GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MAG_FILTER, GL11.GL_LINEAR);
//				GL11.glTexImage2D(GL11.GL_TEXTURE_2D, 0, GL11.GL_RGBA, width, height, 0, GL11.GL_RGBA, GL11.GL_UNSIGNED_BYTE, imageData);
//
//			} else if (filter == FILTER_MIPMAP) { // MipMapping
//
//				GLU.gluBuild2DMipmaps(GL11.GL_TEXTURE_2D, GL11.GL_RGBA, width, height, GL11.GL_RGBA, GL11.GL_UNSIGNED_BYTE, imageData); // ( NEW )
//
//				GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MIN_FILTER, GL11.GL_LINEAR_MIPMAP_LINEAR);
//				GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MAG_FILTER, GL11.GL_LINEAR);
//				GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_WRAP_S, GL11.GL_REPEAT);
//				GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_WRAP_T, GL11.GL_REPEAT);
//			}
//
//			GL11.glPixelStorei(GL11.GL_UNPACK_ALIGNMENT, 4);
//
//			// Delete image in DevIL
//			tmp.put(0, ilImageHandle);
//			IL.ilDeleteImages(tmp);
//
//			// Reset OpenGL texture state
//			GL11.glBindTexture(GL11.GL_TEXTURE_2D, 0);
//
//			// Add texture to Hashmap
//			loadedTextures.put(file.toString(), oglImageHandle);
//
//			// Increment counter
//			numTotalLoaded++;
//			numLoaded++;
//
//			return oglImageHandle;
//		}
//	}
//
//
//	/***************************************************************************************************************************************************************************************************
//	 * generateLightmap() - Generates a lightmap texture from the lightmapData
//	 * 
//	 * @param lightmapData
//	 *            The data from which the lightmap is generated
//	 * @return The texture ID of the generated texture
//	 **************************************************************************************************************************************************************************************************/
//	public static int generateLightmap(int[] lightmapData) {
//
//		IntBuffer tmp = BufferUtils.createIntBuffer(1);
//
//		// Generate the texture identifiers
//		GL11.glGenTextures(tmp);
//		int textureID = tmp.get(0);
//
//		// Is lightmap data provided?
//		if (lightmapData != null) {
//
//			// Change the gamma settings on the lightmaps (make them brighter)
//			float gamma = 2.5f;
//
//			for (int j = 0; j < 128 * 128; j++) {
//
//				float r, g, b;
//				r = lightmapData[j * 3 + 0];
//				g = lightmapData[j * 3 + 1];
//				b = lightmapData[j * 3 + 2];
//
//				r *= gamma / 255.0f;
//				g *= gamma / 255.0f;
//				b *= gamma / 255.0f;
//
//				// Find the value to scale back up
//				float scale = 1.0f;
//				float temp;
//
//				if (r > 1.0f && (temp = (1.0f / r)) < scale)
//					scale = temp;
//				if (g > 1.0f && (temp = (1.0f / g)) < scale)
//					scale = temp;
//				if (b > 1.0f && (temp = (1.0f / b)) < scale)
//					scale = temp;
//
//				// Scale up color values
//				scale *= 255.0f;
//				r *= scale;
//				g *= scale;
//				b *= scale;
//
//				// Fill data back in
//				lightmapData[j * 3 + 0] = (int) r;
//				lightmapData[j * 3 + 1] = (int) g;
//				lightmapData[j * 3 + 2] = (int) b;
//			}
//
//			// Generate texture in OpenGL
//			GL11.glBindTexture(GL11.GL_TEXTURE_2D, textureID);
//
//			ByteBuffer data = BufferUtils.createByteBuffer(128 * 128 * 3);
//
//			for (int i = 0; i < 128 * 128 * 3; i++)
//				data.put(i, (byte) lightmapData[i]);
//
//			GLU.gluBuild2DMipmaps(GL11.GL_TEXTURE_2D, GL11.GL_RGB, 128, 128, GL11.GL_RGB, GL11.GL_UNSIGNED_BYTE, data);
//
//			// Set Parameters
//			GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MIN_FILTER, GL11.GL_LINEAR_MIPMAP_LINEAR);
//			GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MAG_FILTER, GL11.GL_LINEAR);
//			// GL11.glTexEnvf(GL11.GL_TEXTURE_ENV, GL11.GL_TEXTURE_ENV_MODE, GL11.GL_MODULATE);
//			GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_WRAP_S, GL11.GL_REPEAT);
//			GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_WRAP_T, GL11.GL_REPEAT);
//
//		} else {
//
//			// Create white texture if no lightmap specified
//			GL11.glBindTexture(GL11.GL_TEXTURE_2D, textureID);
//
//			ByteBuffer white = BufferUtils.createByteBuffer(4);
//			for (int i = 0; i < 4; i++)
//				white.put(i, (byte) 255);
//
//			// Create texture
//			GLU.gluBuild2DMipmaps(GL11.GL_TEXTURE_2D, GL11.GL_RGB, 1, 1, GL11.GL_RGB, GL11.GL_UNSIGNED_BYTE, white);
//
//			// Set Parameters
//			GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MIN_FILTER, GL11.GL_LINEAR_MIPMAP_LINEAR);
//			GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MAG_FILTER, GL11.GL_LINEAR);
//			GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_WRAP_S, GL11.GL_REPEAT);
//			GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_WRAP_T, GL11.GL_REPEAT);
//		}
//
//		return textureID;
//	}
//
//
//	/***************************************************************************************************************************************************************************************************
//	 * getNumTotalLoadedTextures() - Returns the number of successfully loaded textures
//	 * 
//	 * @return The total number of textures the TextureManager ever loaded.
//	 **************************************************************************************************************************************************************************************************/
//	public static int getNumTotalLoadedTextures() {
//
//		return numTotalLoaded;
//	}
//
//
//	/***************************************************************************************************************************************************************************************************
//	 * getNumLoadedTextures() - Returns the number of successfully loaded textures since last counter reset
//	 * 
//	 * @return The number of loaded textures since last counter reset.
//	 **************************************************************************************************************************************************************************************************/
//	public static int getNumLoadedTextures() {
//
//		return numLoaded;
//	}
//
//
//	/***************************************************************************************************************************************************************************************************
//	 * resetLoadCounter() - Sets the counter of loaded textures to zero.
//	 **************************************************************************************************************************************************************************************************/
//	public static void resetLoadCounter() {
//
//		numLoaded = 0;
//	}
//
//
//	/***************************************************************************************************************************************************************************************************
//	 * checkDevIL() - Make sure DevIL is created
//	 **************************************************************************************************************************************************************************************************/
//	private static void checkDevIL() {
//
//		try {
//			if (!IL.isCreated())
//				IL.create();
//
//			if (!ILU.isCreated())
//				ILU.create();
//		} catch (LWJGLException e) {
//			e.printStackTrace();
//		}
//	}
//
//
//	/***************************************************************************************************************************************************************************************************
//	 * getNextPowerOfTwo() - Get the closest greater power of 2 to the fold number
//	 * 
//	 * @param fold
//	 *            The target number
//	 * @return The power of 2
//	 **************************************************************************************************************************************************************************************************/
//	private static int getNextPowerOfTwo(int fold) {
//
//		int pow = 2;
//		while (pow < fold) {
//			pow *= 2;
//		}
//		return pow;
//	}

}
