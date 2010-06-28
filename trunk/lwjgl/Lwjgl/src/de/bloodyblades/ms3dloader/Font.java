package de.bloodyblades.ms3dloader;




import gldemo.GLApp;
import gldemo.GLImage;

import java.io.InputStream;

import org.lwjgl.opengl.ARBMultitexture;
import org.lwjgl.opengl.GL11;


public class Font {

	private int textureID;
	private int baseList;

	private int size;

	private int windowWidth, windowHeight;


	/***************************************************************************************************************************************************************************************************
	 * Font() - Constructor
	 **************************************************************************************************************************************************************************************************/
	public Font() {

	}


	/***************************************************************************************************************************************************************************************************
	 * Font() - Constructor
	 **************************************************************************************************************************************************************************************************/
	public Font(InputStream file, int size, int windowWidth, int windowHeight) {

		this.createFont(file, size, windowWidth, windowHeight);
	}


	/***************************************************************************************************************************************************************************************************
	 * createFont() - Create the font display lists
	 **************************************************************************************************************************************************************************************************/
	public boolean createFont(InputStream file, int size, int windowWidth, int windowHeight) {

		this.size = size;

		this.windowWidth = windowWidth;
		this.windowHeight = windowHeight;

//		this.textureID = TextureManager.loadTexture(file, false, TextureManager.FILTER_LINEAR);
		this.textureID = GLApp.makeTexture(new GLImage(GLApp.getBytesFromStream(file)));
		if (this.textureID == -1)
			return false; // failed

		/* Build the font */

		float cx; // Holds Our X Character Coord
		float cy; // Holds Our Y Character Coord

		this.baseList = GL11.glGenLists(256); // Creating 256 Display Lists

		ARBMultitexture.glActiveTextureARB(ARBMultitexture.GL_TEXTURE0_ARB);
		GL11.glEnable(GL11.GL_TEXTURE_2D);
		GL11.glBindTexture(GL11.GL_TEXTURE_2D, this.textureID); // Select Our Font Texture

		for (int loop = 0; loop < 256; loop++) // Loop Through All 256 Lists
		{
			cx = (float) (loop % 16) / 16.0f; // X Position Of Current Character
			cy = (float) (loop / 16) / 16.0f; // Y Position Of Current Character

			GL11.glNewList(this.baseList + loop, GL11.GL_COMPILE); // Start Building A List
			GL11.glBegin(GL11.GL_QUADS); // Use A Quad For Each Character
			GL11.glTexCoord2f(cx, 1 - cy - 0.0625f); // Texture Coord (Bottom Left)
			GL11.glVertex2i(0, 0); // Vertex Coord (Bottom Left)
			GL11.glTexCoord2f(cx + 0.0625f, 1 - cy - 0.0625f); // Texture Coord (Bottom Right)
			GL11.glVertex2i(size, 0); // Vertex Coord (Bottom Right)
			GL11.glTexCoord2f(cx + 0.0625f, 1 - cy); // Texture Coord (Top Right)
			GL11.glVertex2i(size, size); // Vertex Coord (Top Right)
			GL11.glTexCoord2f(cx, 1 - cy); // Texture Coord (Top Left)
			GL11.glVertex2i(0, size); // Vertex Coord (Top Left)
			GL11.glEnd(); // Done Building Our Quad (Character)
			GL11.glTranslated((int) ((float) size * 0.625f), 0, 0); // Move To The Right Of The Character
			GL11.glEndList(); // Done Building The Display List
		} // Loop Until All 256 Are Built
		
		

		return true;
	}


	/***************************************************************************************************************************************************************************************************
	 * killFont() - Delete the font display lists
	 **************************************************************************************************************************************************************************************************/
	public void killFont() {

		GL11.glDeleteLists(this.baseList, 256); // Delete All 256 Display Lists
	}


	/***************************************************************************************************************************************************************************************************
	 * print() - Prints a text to the screen
	 **************************************************************************************************************************************************************************************************/
	public void print(String text, int x, int y, int set) {

		if (set > 1)
			set = 1;

		ARBMultitexture.glActiveTextureARB(ARBMultitexture.GL_TEXTURE0_ARB);
		GL11.glEnable(GL11.GL_TEXTURE_2D);
		GL11.glBindTexture(GL11.GL_TEXTURE_2D, this.textureID); // Select Our Font Texture

		GL11.glDisable(GL11.GL_DEPTH_TEST); // Disables Depth Testing
		GL11.glEnable(GL11.GL_BLEND);
		GL11.glBlendFunc(GL11.GL_SRC_ALPHA, GL11.GL_ONE); // Select The Type Of Blending

		GL11.glMatrixMode(GL11.GL_PROJECTION); // Select The Projection Matrix
		GL11.glPushMatrix(); // Store The Projection Matrix
		GL11.glLoadIdentity(); // Reset The Projection Matrix
		GL11.glOrtho(0, windowWidth, 0, windowHeight, -1, 1); // Set Up An Ortho Screen
		GL11.glMatrixMode(GL11.GL_MODELVIEW); // Select The Modelview Matrix
		GL11.glPushMatrix(); // Store The Modelview Matrix

		GL11.glLoadIdentity(); // Reset The Modelview Matrix
		GL11.glTranslated(x, windowHeight - y, 0); // Position The Text (0,0 - Bottom Left)

		for (int i = 0; i < text.length(); i++)
			GL11.glCallList(this.baseList - 128 + text.charAt(i) + 96 + set * 128); // Write The Text To The Screen

		GL11.glMatrixMode(GL11.GL_PROJECTION); // Select The Projection Matrix
		GL11.glPopMatrix(); // Restore The Old Projection Matrix
		GL11.glMatrixMode(GL11.GL_MODELVIEW); // Select The Modelview Matrix
		GL11.glPopMatrix(); // Restore The Old Projection Matrix

		GL11.glDisable(GL11.GL_BLEND);
		GL11.glEnable(GL11.GL_DEPTH_TEST); // Enables Depth Testing
	}


	/***************************************************************************************************************************************************************************************************
	 * getSize() - Get the size of this font
	 **************************************************************************************************************************************************************************************************/
	public int getSize() {

		return this.size;
	}

}
