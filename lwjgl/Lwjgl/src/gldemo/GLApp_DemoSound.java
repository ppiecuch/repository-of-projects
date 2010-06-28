package gldemo;

import org.lwjgl.opengl.*;
import org.lwjgl.input.*;
import org.lwjgl.util.glu.*;
import glapp.*;
import glsound.*;

/**
 * Demo SoundScape class.  Loads four sounds from wav files, places the sounds
 * in a space that user can navigate with arrow keys.  Click mouse to move
 * one sound.
 * <P>
 * The SoundScape class wraps OpenAL functions.  The class is static so can be
 * used without instantiation, similar to LWJGL Display, Mouse and Keyboard
 * classes:
 * <PRE>
 *     SoundScape.create();      // init sound environment
 *     ...
 *     int soundData sd = SoundScape.loadSoundData("a_noise.wav");
 *     int soundSource ss = SoundScape.makeSoundSource(soundData);
 *     SoundScape.setSoundPosition(ss, 10, 2, 0);
 *     SoundScape.play(ss);
 *     ...
 *     SoundScape.destroy();
 * </PRE>
 * The demo application uses GLApp.java for OpenGL rendering.
 * <P>
 * napier at potatoland dot org
 */
public class GLApp_DemoSound extends GLApp {
    // Handles for textures
    int sphereTextureHandle = 0;
    int marbleTextureHandle = 0;
    int groundTextureHandle = 0;

    // display list ID
    int sphereListID;

    // Point Light position
    float lightPosition[]= { -2f, 2f, 2f, 1.0f };

    // Camera position
    static float[] cameraPos = {0f,1f,20f};
    // Camera rotation in degrees
    float cameraRotation = 0f;
    // A constant used in navigation
    final float piover180 = 0.0174532925f;

    // sound sources
    int s1, s2, s3, s4;
    // World coordinates of sound sources
    float[] sPos1 = {-8f, 0f, -8f};  // left
    float[] sPos2 = { 0f, 0f, -8f};  // middle
    float[] sPos3 = { 8f, 0f, -8f};  // right
    float[] sPos4 = { 5f, 0f, 8f};   // close to camera

	/**
	 * Start the application.  demo.run() calls setup(), handles mouse and keyboard input,
	 * and calls draw() in a loop.
	 */
    public static void main(String args[]) {
        GLApp_DemoSound demo = new GLApp_DemoSound();
        demo.window_title = "GLApp Sound Demo";
        demo.displayWidth = 800;
        demo.displayHeight = 600;
        demo.run();
    }

    /**
     * Initialize the scene.  Called by GLApp.run()
     */
    public void setup()
    {
        // setup perspective
        setPerspective();

        // Create a light (diffuse color, ambient color, specular color, position)
        float faDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        float faAmbient[] = { 0.2f, 0.2f, 0.3f, 1.0f };
        setLight( GL11.GL_LIGHT1, faDiffuse, faAmbient, faDiffuse, lightPosition );

        // set material properties
        setMaterial(  new float[] {.9f,.9f,1f,1f}, .1f);

        // enable lighting and texturing
        GL11.glEnable(GL11.GL_LIGHTING);
        GL11.glEnable(GL11.GL_TEXTURE_2D);

        // Create texture and mipmap for frog sphere
        sphereTextureHandle = makeTexture("images/tree_frog_1_512.jpg");

        // Create texture and mipmap for marble sphere
        marbleTextureHandle = makeTexture("images/marble.jpg");

        // Create texture with anisotropic filtering and mipmap for ground plane
        groundTextureHandle = makeTexture("images/grass_1_512.jpg", true, true);

        //---------------------------------------------------------------
        // Make sphere display list (for performance)
        //---------------------------------------------------------------

        sphereListID = beginDisplayList();
        renderSphere();     // draw the sphere into the display list
        endDisplayList();

        //---------------------------------------------------------------
        // Setup SoundScape
        //---------------------------------------------------------------

        // initialize sound environment
        SoundScape.create();

        // Over what distance should sounds drop off.  This is not exact, as there
        // are many variables that affect how loud a sound is (some wav samples are
        // louder than others).  Use the reference distance to control how "big"
        // the sounds are.  The space in this scene is 20 units wide, and I use a
        // default reference distance of .5 to keep the sounds fairly isolated.
        SoundScape.setReferenceDistance(.5f);

        // load 2 sound samples (these hold the actual sound data)
        int frogSample = SoundScape.loadSoundData("sounds/frog.wav");
        int cyberSample = SoundScape.loadSoundData("sounds/steam_loop.wav");  //jet_cruising_loop_1.wav"); //

        // make 4 sound sources (these are sound emitters that have a location in space)
        s1 = SoundScape.makeSoundSource( frogSample );
        s2 = SoundScape.makeSoundSource( frogSample );
        s3 = SoundScape.makeSoundSource( frogSample );
        s4 = SoundScape.makeSoundSource( cyberSample );

        // tweak pitch and volume on the sources
        SoundScape.setPitch(s1, .7f);    // lower pitch frog
        SoundScape.setPitch(s2, .9f);    // medium pitch frog
        SoundScape.setPitch(s3, 1.2f);   // high pitch frog
        SoundScape.setGain(s4, 4.0f);    // make this sound louder
        SoundScape.setReferenceDistance(s4,1);  // and make it spread out more

        // set the sound emitter locations
        SoundScape.setSoundPosition(s1, sPos1);
        SoundScape.setSoundPosition(s2, sPos2);
        SoundScape.setSoundPosition(s3, sPos3);
        SoundScape.setSoundPosition(s4, sPos4);

        // make them repeat
        SoundScape.setLoop(s1,true);
        SoundScape.setLoop(s2,true);
        SoundScape.setLoop(s3,true);
        SoundScape.setLoop(s4,true);

        // turn them on
        SoundScape.play(s1);
        SoundScape.play(s2);
        SoundScape.play(s3);
        SoundScape.play(s4);
    }

    /**
     * Render one frame.  Called by GLApp.run().
     */
    public void draw() {
        // clear depth buffer and color
        GL11.glClear(GL11.GL_COLOR_BUFFER_BIT | GL11.GL_DEPTH_BUFFER_BIT);

        // adjust camera position according to arrow key events
        setCameraPosition();

        // SOUND: set listener at camera position
        float[] d = getDirection(cameraRotation);
        SoundScape.setListenerPosition(cameraPos[0],cameraPos[1],cameraPos[2]);
        SoundScape.setListenerOrientation(d[0], d[1], d[2],   // we're facing this way
                                          0f, 1f, 0f);        // Y is "up"

        // select model view for subsequent transforms
        GL11.glMatrixMode(GL11.GL_MODELVIEW);
        GL11.glLoadIdentity();

    	// Place the viewpoint
		GLU.gluLookAt(	// camera position
						cameraPos[0], cameraPos[1], cameraPos[2],
						// look at a point directly in front of camera
						cameraPos[0]- (float) Math.sin(cameraRotation* piover180), cameraPos[1], cameraPos[2]- (float) Math.cos(cameraRotation* piover180),
						// Y axis is up
						0f, 1f, 0f);

        // draw the ground plane
        GL11.glBindTexture(GL11.GL_TEXTURE_2D, groundTextureHandle);
        GL11.glPushMatrix();
        {
            GL11.glTranslatef(0f, -2.1f, 0f); // down a bit
            GL11.glScalef(10f, .01f, 10f);  // squash it flat
            renderCube(2,20);
        }
        GL11.glPopMatrix();

        // draw sphere at sound1
        GL11.glBindTexture(GL11.GL_TEXTURE_2D, sphereTextureHandle);
        GL11.glPushMatrix();
        {
            GL11.glTranslatef(sPos1[0], sPos1[1], sPos1[2]);
            callDisplayList(sphereListID);
        }
        GL11.glPopMatrix();

        // draw sphere at sound 2
        GL11.glBindTexture(GL11.GL_TEXTURE_2D, sphereTextureHandle);
        GL11.glPushMatrix();
        {
            GL11.glTranslatef(sPos2[0], sPos2[1], sPos2[2]);
            callDisplayList(sphereListID);
        }
        GL11.glPopMatrix();

        // draw sphere at sound 3
        GL11.glBindTexture(GL11.GL_TEXTURE_2D, sphereTextureHandle);
        GL11.glPushMatrix();
        {
            GL11.glTranslatef(sPos3[0], sPos3[1], sPos3[2]);
            callDisplayList(sphereListID);
        }
        GL11.glPopMatrix();

        // draw sphere at sound 4 (marble texture)
        GL11.glBindTexture(GL11.GL_TEXTURE_2D, marbleTextureHandle);
        GL11.glPushMatrix();
        {
            GL11.glTranslatef(sPos4[0], sPos4[1], sPos4[2]);
            callDisplayList(sphereListID);
        }
        GL11.glPopMatrix();

        // Place the light.  Light will move with the rest of the scene
        GL11.glLight(GL11.GL_LIGHT1, GL11.GL_POSITION, allocFloats(lightPosition));

        // draw text
        print( 50, 550, "Use arrow keys to navigate");
        print( 50, 525, "Click to move marble ball (and sound)");
    }


    /**
     * set the field of view and view depth.
     */
    public static void setPerspective()
    {
        // select projection matrix (controls perspective)
        GL11.glMatrixMode(GL11.GL_PROJECTION);
        GL11.glLoadIdentity();
        GLU.gluPerspective(40f,         // field of view angle
                           aspectRatio, // proportions of viewport rectangle
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
            cameraRotation += 1f;
        }
        // Turn right
        if (Keyboard.isKeyDown(Keyboard.KEY_RIGHT)) {
            cameraRotation -= 1f;
        }
        // move forward in current direction
        if (Keyboard.isKeyDown(Keyboard.KEY_UP)) {
            cameraPos[0] -= (float) Math.sin(cameraRotation * piover180) * .1f;
            cameraPos[2] -= (float) Math.cos(cameraRotation * piover180) * .1f;
        }
        // move backward in current direction
        if (Keyboard.isKeyDown(Keyboard.KEY_DOWN)) {
            cameraPos[0] += (float) Math.sin(cameraRotation * piover180) * .1f;
            cameraPos[2] += (float) Math.cos(cameraRotation * piover180) * .1f;
        }
        // move camera down
        if (Keyboard.isKeyDown(Keyboard.KEY_PRIOR)) {
            cameraPos[1] +=  .1f;
        }
        // move camera up
        if (Keyboard.isKeyDown(Keyboard.KEY_NEXT)) {
            cameraPos[1] -=  .1f;
        }
    }


    /**
     * Given camera rotation in degrees, make a vector that points in
     * direction camera is looking.  Will use this to set direction of Sound Listener.
     * @return xyz direction as float[3]
     */
    public float[] getDirection(float camera_rotation)
    {
        float[] direction = {
            (float) -Math.sin(camera_rotation * piover180),
            0f,
            (float) -Math.cos(camera_rotation * piover180),
        };
        return direction;
    }


    /**
     * Move sphere and sound to mouse click position.  Have to pick a Z depth to
     * place the sphere at, so check the depth buffer at the cursor x,y to see
     * if user clicked on an object.  If yes, put the sphere at that Z depth.  If
     * user clicked empty space, put the sphere at Z=0
     */
    public void mouseDown(int x, int y) {
        float z = getZDepth(x,y);    // get the Z depth at the cursor
        if (z == 0 || z == 1) {      // if clicking on empty space
            z = getZDepthAtOrigin();     // get the Z depth at origin
        }
        sPos4 = getWorldCoordsAtScreen(x, y, z);  // convert mouse xyz to world
        SoundScape.setSoundPosition(s4, sPos4);   // put sound there
    }


    public void mouseMove(int x, int y) {
    }


    public void mouseUp(int x, int y) {
    }


    public void keyDown(int keycode) {
    }


    public void keyUp(int keycode) {
    }

    /**
     * Shutdown the OpenAL environment before exiting app.  Cleanup() is called
     * by GLApp when the main loop exits.
     */
    public void cleanup() {
        super.cleanup();
        SoundScape.destroy();
    }

}