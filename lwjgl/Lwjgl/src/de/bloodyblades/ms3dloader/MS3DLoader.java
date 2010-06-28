package de.bloodyblades.ms3dloader;

import java.util.Date;

import org.lwjgl.LWJGLException;
import org.lwjgl.Sys;
import org.lwjgl.input.Keyboard;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;
import org.lwjgl.opengl.GL11;
import org.lwjgl.util.glu.GLU;

import de.bloodyblades.ms3dloader.data.ResourceLoader;

/**
 * A little program to test the MS3D class.
 * 
 * @author xindon
 */
public class MS3DLoader {

	ResourceLoader resourceLoader = new ResourceLoader();

	boolean closeRequested = false;

	Font font;

	Timer timer;
	double FPS = 60.0;

	MS3DModel g36c;
	float angle;

	/***************************************************************************************************************************************************************************************************
	 * Initialization stuff comes in here...
	 **************************************************************************************************************************************************************************************************/
	private void init() {

		try {
			Display.setDisplayMode(new DisplayMode(300, 200));
			Display.setVSyncEnabled(true);
			Display.setTitle("MS3D Loader [G36C]");
			Display.create();

			Keyboard.create();

		} catch (LWJGLException e) {
			Sys.alert("Error", "Initialization failed!\n\n" + e.getMessage());
			System.exit(0);
		}

		/* OpenGL */
		int width = Display.getDisplayMode().getWidth();
		int height = Display.getDisplayMode().getHeight();

		GL11.glViewport(0, 0, width, height); // Reset The Current Viewport
		GL11.glMatrixMode(GL11.GL_PROJECTION); // Select The Projection Matrix
		GL11.glLoadIdentity(); // Reset The Projection Matrix
		GLU.gluPerspective(45.0f, ((float) width / (float) height), 0.1f, 1000.0f); // Calculate The Aspect Ratio Of The Window
		GL11.glMatrixMode(GL11.GL_MODELVIEW); // Select The Modelview Matrix
		GL11.glLoadIdentity(); // Reset The Modelview Matrix

		GL11.glEnable(GL11.GL_TEXTURE_2D);
		GL11.glShadeModel(GL11.GL_SMOOTH);
		GL11.glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // Background color
		GL11.glClearDepth(1.0f);
		GL11.glEnable(GL11.GL_DEPTH_TEST);
		GL11.glDepthFunc(GL11.GL_LEQUAL);
		GL11.glHint(GL11.GL_PERSPECTIVE_CORRECTION_HINT, GL11.GL_NICEST);

		// Load model
		g36c = new MS3DModel(resourceLoader.loadResourceAsStream("models/gsg9.ms3d"));
		
		// Load font
		//font = new Font(resourceLoader.loadResourceAsStream("textures/font.bmp"), 12, width, height);

		// Init timer
		timer = new Timer();
	}

	/***************************************************************************************************************************************************************************************************
	 * All rendering happens here...
	 **************************************************************************************************************************************************************************************************/
	private void render() {

		GL11.glClear(GL11.GL_COLOR_BUFFER_BIT | GL11.GL_DEPTH_BUFFER_BIT);
		GL11.glLoadIdentity();

		// Model anzeigen
		GL11.glTranslatef(0.0f, -40.0f, -150.0f);
		GL11.glRotatef(angle, 0.0f, 1.0f, 0.0f);
		g36c.updateModel(new Date().getTime());
//		g36c.render();

		GL11.glColor3f(1.0f, 1.0f, 1.0f);
		//font.print("" + FPS, 2, font.getSize() + 4, 0);

		Display.update(); // Update The Display
	}

	/***************************************************************************************************************************************************************************************************
	 * Updates variables and such stuff
	 **************************************************************************************************************************************************************************************************/
	private void updateLogic() {

		// Update angle
		angle += 20.0 / FPS;
		if (angle > 360.0f)
			angle -= 360.0f;

		// Update FPS
		FPS = 1000.0 / timer.getElapsedMillis();
		timer.resetTimer();

	}

	/***************************************************************************************************************************************************************************************************
	 * Checks if key is pressed or mouse is moved etc.
	 **************************************************************************************************************************************************************************************************/

	private void checkInput() {

		if (Keyboard.next() && Keyboard.getEventKeyState()) {
			if (Keyboard.getEventKey() == Keyboard.KEY_ESCAPE)
				closeRequested = true;
		}

		if (Display.isCloseRequested()) {
			closeRequested = true;
		}
	}

	/***************************************************************************************************************************************************************************************************
	 * Deinitialization and clean up.
	 **************************************************************************************************************************************************************************************************/

	private void deInit() {

		Keyboard.destroy();
		Display.destroy();
	}

	/***************************************************************************************************************************************************************************************************
	 * Constructor (Mainloop)
	 **************************************************************************************************************************************************************************************************/
	public MS3DLoader() {
		init();
		while (!closeRequested) {

			checkInput();
			updateLogic();
			render();

		}
		deInit();
	}

	/***************************************************************************************************************************************************************************************************
	 * The main entry point of this app
	 **************************************************************************************************************************************************************************************************/
	public static void main(String[] args) {
		new MS3DLoader();

	}

}