package gldemo;

import java.awt.Font;
import java.util.ArrayList;
import org.lwjgl.opengl.*;
import org.lwjgl.util.glu.*;
import glapp.*;
import glmodel.*;

/**
 * Render 2D text using texture mapping.  Text is drawn over a simple 3D
 * scene and is incorporated into the scene as a texture on an object.
 * <P>
 * The GLApp.buildfont() function creates a character set by texture-mapping
 * images of characters onto quads.
 * <P>
 * GLApp.print(String) renders text strings as a series of texture-mapped quads.
 * <P>
 * The GLFont class takes a similar approach, but builds a character set
 * dynamically using the Java Font class.  Character images can drawn using
 * any font that your system supports, with italics, bold, and in a wide
 * ranges of sizes.
 * <P>
 * GLApp initializes the LWJGL environment for OpenGL rendering,
 * ie. creates a window, sets the display mode, inits mouse and keyboard,
 * then runs a loop that calls draw().
 * <P>
 * Special thanks to NeHe and Giuseppe D'Agata for the "2D Texture Font"
 * tutorial (http://nehe.gamedev.net).
 * <P>
 * napier at potatoland dot org
 */
public class GLApp_DemoText extends GLApp {
    // Light position: if last value is 0, then this describes light direction.  If 1, then light position.
    float lightPosition[]= { 0f, 2f, 2f, 0f };

    // sphere displaylist
    int sphereDL;

    // holds a texture mapped font
    GLFont font;

    // will hold the last seven characters types
    ArrayList lastchars = new ArrayList();

	/**
	 * Start the application.  demo.run() calls setup(), handles mouse and keyboard input,
	 * and calls draw() in a loop.
	 */
    public static void main(String args[]) {
        GLApp_DemoText demo = new GLApp_DemoText();
        demo.window_title = "GLApp Text Demo";
        demo.displayWidth = 640;
        demo.displayHeight = 480;
        demo.run();
    }

    /**
     * Initialize the scene.  Called by GLApp.run()
     */
    public void setup()
    {
        // setup and enable perspective
        setPerspective();

        // Create a white light
        setLight( GL11.GL_LIGHT1,
        		new float[] { 1f, 1f, 1f, 1f },     // diffuse
        		new float[] { .8f, .8f, .8f, 1f },  // ambient
        		new float[] { 1f, 1f, 1f, 1f },     // specular
        		lightPosition );

        // off-white material, very shiny
        setMaterial(new float[] {.6f,.6f,.45f,1}, 1f);

        // lighten up the shadows a bit
        setAmbientLight(new float[] {.35f, .45f, .5f, 1f});

        // make a sphere display list
        sphereDL = beginDisplayList(); {
        	renderSphere();
        }
        endDisplayList();

        // Make a font object to render text
		font = new GLFont( new Font("Verdana", Font.BOLD, 18));
    }

    /**
     * set the field of view and view depth.
     */
    public static void setPerspective()
    {
        // select projection matrix (controls perspective)
        GL11.glMatrixMode(GL11.GL_PROJECTION);
        GL11.glLoadIdentity();
        // fovy, aspect ratio, zNear, zFar
        GLU.gluPerspective(40f,         // zoom in or out of view
                           aspectRatio, // shape of viewport rectangle
                           .1f,         // Min Z: how far from eye position does view start
                           500f);       // max Z: how far from eye position does view extend
        // return to modelview matrix
        GL11.glMatrixMode(GL11.GL_MODELVIEW);
    }

    //float rotation=0;
    /**
     * Render one frame.  Called by GLApp.run().
     */
    public void draw() {
        // clear depth buffer and color
        GL11.glClear(GL11.GL_COLOR_BUFFER_BIT | GL11.GL_DEPTH_BUFFER_BIT);

        // select model view for subsequent transforms
        GL11.glMatrixMode(GL11.GL_MODELVIEW);
        GL11.glLoadIdentity();

        // do gluLookAt() with viewpoint position, direction, orientation
        GLU.gluLookAt(0f, -3f, 33f, // where is the eye
        			0f, -3f, 0f,    // what point are we looking at
        			0f, 1f, 0f);   // which way is up

        // draw the scene (after we draw the shadows, so everything layers correctly)
        //drawSpheres();

        // Place the light.  Light will move with the rest of the scene
        setLightPosition(GL11.GL_LIGHT1, lightPosition);

        // render some text using a font
		font.print(40, DM.getHeight()-80, "Type something:");

		// render some text using texture-mapped font
		//print( 40, 210, "Text rendered with the default GLApp.print() function uses");
        //print( 40, 190, "a texture mapped fixed width font, loaded from an image file.", 1);

        // render some text using the GLFont class
        font.print( 40,125, "Text rendered with the GLFont class generates a proportional spaced");
        font.print( 40,100, "character set from a Java Font object.");
    }

    public void drawSpheres() {
    	int leftpos = -12;
    	int texture = 0;
    	GL11.glEnable(GL11.GL_LIGHTING);
    	for (int i=0; i < 7; i++) {
    		texture = 0;
    		if (lastchars.size() > i) {
    			texture = ((Integer)lastchars.get(i)).intValue();
    		}
			GL11.glBindTexture(GL11.GL_TEXTURE_2D, texture);
    		GL11.glPushMatrix();
    		{
    			GL11.glTranslatef(leftpos, 0, 0); // move
    			GL11.glScalef(2f, 2f, 2f); // scale up
    			callDisplayList(sphereDL); //renderSphere();
    		}
    		GL11.glPopMatrix();
    		leftpos += 4;
    	}
    }


    public void mouseMove(int x, int y) {
    }


    public void mouseDown(int x, int y) {
    }


    public void mouseUp(int x, int y) {
    }

    /**
     * Use GLFont to make texture images of the last 7 characters typed
     */
    public void keyDown(int keycode) {
    	if (keyChar() >= 32) {   // only printable characters
    		// pick a font:
    		Font font = new Font("Times New Roman", Font.BOLD, 120);
			//Font font = new Font("Comic Sans MS", Font.BOLD, 120);
			//Font font = new Font("Courier New", Font.BOLD, 120);

    		// key that was typed
    	    String key = String.valueOf(keyChar());
    	    
    	    // make a texture image of the character
    		int textureHandle = GLFont.makeCharTexture(font, key, GLMaterial.colorBlack, GLMaterial.colorWhite);
    		lastchars.add(new Integer(textureHandle));
    		if (lastchars.size() > 7) {
    			int oldTxtr = ((Integer)lastchars.get(0)).intValue();
     			deleteTexture(oldTxtr);
       			lastchars.remove(0);
    		}
    	}
    }


    public void keyUp(int keycode) {
    }

}