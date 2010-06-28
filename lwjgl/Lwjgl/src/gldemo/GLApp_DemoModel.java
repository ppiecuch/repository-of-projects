package gldemo;

import org.lwjgl.opengl.*;
import org.lwjgl.input.*;
import org.lwjgl.util.glu.*;
import glapp.*;
import glmodel.*;

/**
 * Load and render an OBJ file.  Demonstrate simple navigation by moving the gluLookat()
 * viewpoint with the arrow keys.
 * <P>
 * Use GLModel to load the mesh and render it with texture and lighting.
 * <P>
 * GLApp initializes the LWJGL environment for OpenGL rendering,
 * ie. creates a window, sets the display mode, inits mouse and keyboard,
 * then runs a loop that calls draw().
 * <P>
 * napier at potatoland dot org
 */
public class GLApp_DemoModel extends GLApp {
    // Model will be loaded from 3DS file
    GLModel object;
    GLImage sky;
    // Light direction: if last value is 0, then this describes light direction.  If 1, then light position.
    float lightDirection[]= { -2f, 2f, 2f, 0f };
    // Camera position
    static float[] cameraPos = {0f,4f,35f};
    float cameraRotation = 0f;
    // A constant used in navigation
    final float piover180 = 0.0174532925f;

    float rotation = 0;

	/**
	 * Start the application.  demo.run() calls setup(), handles mouse and keyboard input,
	 * and calls draw() in a loop.
	 */
    public static void main(String args[]) {
        GLApp_DemoModel demo = new GLApp_DemoModel();
        demo.window_title = "GLApp Demo Model";
        demo.displayWidth = 800;
        demo.displayHeight = 600;
        demo.run();
    }

    /**
     * Initialize the scene.  Called by GLApp.run()
     */
    public void setup()
    {
        // enable lighting and texture rendering
        GL11.glEnable(GL11.GL_LIGHTING);
        GL11.glEnable(GL11.GL_TEXTURE_2D);

        GL11.glClearColor(.5f,.6f,.9f,1f);

        // setup and enable perspective
        setPerspective();

        // Create a light
        setLight( GL11.GL_LIGHT1,
        		new float[] { 1f,  1f,  1f,  1f },    // diffuse color
        		new float[] { .6f, .6f, .9f, 1f },    // ambient
        		new float[] { 1f,  1f,  1f,  1f },    // specular
        		lightDirection );                     // direction/position

        setAmbientLight(new float[] { .6f, .6f, .9f, 1f });

        sky = loadImage("images/sky.jpg");

        // Load the model
        object = new GLModel("models/JetFire/JetFire.3ds");
        object.regenerateNormals();
    }

    /**
     * Render one frame.  Called by GLApp.run().
     */
    public void render() {
    	// use the frames per second to animate
    	rotation += 25f * getSecondsPerFrame();

        // clear depth buffer and color
        GL11.glClear(GL11.GL_COLOR_BUFFER_BIT | GL11.GL_DEPTH_BUFFER_BIT);

        // draw background image 
    	drawImageFullScreen(sky);

        // adjust camera position according to arrow key events
        setCameraPosition();

        // select model view and reset
        GL11.glMatrixMode(GL11.GL_MODELVIEW);
        GL11.glLoadIdentity();

        // set the viewpoint
        GLU.gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], // where is the eye
					// look at a point directly in front of camera
					cameraPos[0]-(float)Math.sin(cameraRotation* piover180), cameraPos[1], cameraPos[2]- (float) Math.cos(cameraRotation* piover180),
        			0f, 1f, 0f);   // which way is up

        // draw object at center
        GL11.glPushMatrix();
        {
            GL11.glRotatef(rotation, 0, 1, 0);  // turn it
            GL11.glScalef(20f,20f,20f);
            object.render();
        }
        GL11.glPopMatrix();

        // Place the light.  Light will move with the rest of the scene
        setLightPosition(GL11.GL_LIGHT1, lightDirection);
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
                           aspectRatio, // shape of viewport rectangle (width/height)
                           .1f,         // Min Z: how far from eye position does view start
                           500f);       // max Z: how far from eye position does view extend
        // return to modelview matrix
        GL11.glMatrixMode(GL11.GL_MODELVIEW);
    }

    /**
     * Adjust the Camera position based on keyboard arrow key input.
     */
    public void setCameraPosition()
    {
        // Turn left
        if (Keyboard.isKeyDown(Keyboard.KEY_LEFT)) {
            cameraRotation += 1.0f;
        }
        // Turn right
        if (Keyboard.isKeyDown(Keyboard.KEY_RIGHT)) {
            cameraRotation -= 1.0f;
        }
        // move forward in current direction
        if (Keyboard.isKeyDown(Keyboard.KEY_UP)) {
            cameraPos[0] -= (float) Math.sin(cameraRotation * piover180) * .3f;
            cameraPos[2] -= (float) Math.cos(cameraRotation * piover180) * .3f;
        }
        // move backward in current direction
        if (Keyboard.isKeyDown(Keyboard.KEY_DOWN)) {
            cameraPos[0] += (float) Math.sin(cameraRotation * piover180) * .3f;
            cameraPos[2] += (float) Math.cos(cameraRotation * piover180) * .3f;
        }
        // move camera down
        if (Keyboard.isKeyDown(Keyboard.KEY_PRIOR)) {
            cameraPos[1] +=  .13f;
        }
        // move camera up
        if (Keyboard.isKeyDown(Keyboard.KEY_NEXT)) {
            cameraPos[1] -=  .13f;
        }
    }


    public void mouseMove(int x, int y) {
    }


    public void mouseDown(int x, int y) {
    }


    public void mouseUp(int x, int y) {
    }


    public void keyDown(int keycode) {
    }


    public void keyUp(int keycode) {
    }
}