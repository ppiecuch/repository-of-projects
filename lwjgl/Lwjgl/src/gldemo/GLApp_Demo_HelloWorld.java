package gldemo;

import org.lwjgl.opengl.*;
import org.lwjgl.util.glu.*;
import glapp.*;

/**
 * Run a bare-bones GLApp.  Draws one white triangle centered on screen.
 * <P>
 * GLApp initializes the LWJGL environment for OpenGL rendering,
 * ie. creates a window, sets the display mode, inits mouse and keyboard,
 * then runs a loop that calls draw().
 * <P>
 * napier at potatoland dot org
 */
public class GLApp_Demo_HelloWorld extends GLApp {
	
	/**
	 * Start the application.  run() calls setup(), handles mouse and keyboard input,
	 * calls render() in a loop.
	 */
    public static void main(String args[]) {
    	// create the app
    	GLApp_Demo_HelloWorld demo = new GLApp_Demo_HelloWorld();

    	// set title, window size
    	demo.window_title = "Hello World";
    	demo.displayWidth = 640;
    	demo.displayHeight = 480;

    	// start running: will call init(), setup(), draw(), mouse functions
    	demo.run();
    }

    /**
     * Initialize the scene.  Called by GLApp.run().  For now the default
     * settings will be fine, so no code here.
     */
    public void setup()
    {
    }

    /**
     * Render one frame.  Called by GLApp.run().
     */
    public void draw() {
        // Clear screen and depth buffer
        GL11.glClear(GL11.GL_COLOR_BUFFER_BIT | GL11.GL_DEPTH_BUFFER_BIT);
        // Select The Modelview Matrix (controls model orientation)
        GL11.glMatrixMode(GL11.GL_MODELVIEW);
        // Reset the coordinate system to center of screen
        GL11.glLoadIdentity();
        // Place the viewpoint
        GLU.gluLookAt(
            0f, 0f, 10f,   // eye position (10 units in front of the origin)
            0f, 0f, 0f,    // target to look at (the origin)
            0f, 1f, 0f);   // which way is up (Y axis)
        // draw a triangle centered around 0,0,0
        GL11.glBegin(GL11.GL_TRIANGLES);           // draw triangles
            GL11.glVertex3f( 0.0f, 1.0f, 0.0f);         // Top
            GL11.glVertex3f(-1.0f,-1.0f, 0.0f);         // Bottom Left
            GL11.glVertex3f( 1.0f,-1.0f, 0.0f);         // Bottom Right
        GL11.glEnd();                              // done
    }
}
