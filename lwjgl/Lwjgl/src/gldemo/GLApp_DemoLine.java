package gldemo;

import org.lwjgl.opengl.*;
import org.lwjgl.util.glu.*;
import org.lwjgl.input.Keyboard;
import glapp.*;

/**
 * Demo the GLLine class.  Creates a line by building a quad strip from point positions.
 * Unlike default OpenGL LINE_STRIP behavior, GLLine smoothly joins line segments and
 * can make arbitrarily wide lines.  
 * <P>
 * napier at potatoland dot org
 */
public class GLApp_DemoLine extends GLApp {
	GLLine L;
    int marble = 0;
    boolean showTexture = false;
    int prevMouseX=0, prevMouseY=0;
    
	/**
	 * Start the application.  run() calls setup(), handles mouse and keyboard input,
	 * calls render() in a loop.
	 */
    public static void main(String args[]) {
    	// create the app
    	GLApp_DemoLine demo = new GLApp_DemoLine();

    	// set title, window size
    	demo.window_title = "Line Drawing";
    	demo.displayWidth = 800;
    	demo.displayHeight = 600;

    	// start running: will call init(), setup(), draw(), mouse functions
    	demo.run();
    }

    /**
     * Initialize the scene.  Called by GLApp.run().  For now the default
     * settings will be fine, so no code here.
     */
    public void setup()
    {
    	L = new GLLine(10f);
    	L.setUVfactor(200);
    	
    	marble = makeTexture("images/marble.jpg");

    	// turn on 2D drawing
    	setOrtho();    	
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
            0f, 0f, 25f,   // eye position (10 units in front of the origin)
            0f, 0f, 0f,    // target to look at (the origin)
            0f, 1f, 0f);   // which way is up (Y axis)

        // activate/de-activate texture
        activateTexture(showTexture? marble : 0);
        
        // draw the line
		GL11.glColor3f(1, 1, 0);

		L.draw();


		// message
		print(20,50,"Click and drag to draw, SPACE to clear, 1-9 to change width");
		print(20,20,"T to toggle texture, C to close shape, R to draw rectangle");

		// in side box: draw line in perspective
		drawRect(549,99,201,201);
		setViewport(550,100,200,200);
		setPerspective();
		GLU.gluLookAt(-200,300,200, 200,300,0, 0,1,0);
		GL11.glEnable(GL11.GL_LIGHTING);
		setLight(GL11.GL_LIGHT1, colorWhite,colorGray,colorWhite, new float[] {0f,0f,1f,0});
		setMaterial(colorWhite,.2f);
		//GL11.glTranslatef(400,0,0);
		//GL11.glRotatef(rotation, 0,1,0);
		//GL11.glTranslatef(-400,0,0);
		//rotation += .6f;
		if (showTexture) {
			L.draw3D(100);
		}
		else {
			L.draw();
		}
		resetViewport();
    	setOrtho();
    	GL11.glDisable(GL11.GL_LIGHTING);
    }
    
    public void keyDown(int keycode) {
    	if (keycode == Keyboard.KEY_SPACE) {
    		L.clear();
    	}
    	else if (keycode == Keyboard.KEY_1) {
    		L.setWidth(1);
    	}
    	else if (keycode == Keyboard.KEY_2) {
    		L.setWidth(2);
    	}
    	else if (keycode == Keyboard.KEY_3) {
    		L.setWidth(5);
    	}
    	else if (keycode == Keyboard.KEY_4) {
    		L.setWidth(10);
    	}
    	else if (keycode == Keyboard.KEY_5) {
    		L.setWidth(20);
    	}
    	else if (keycode == Keyboard.KEY_6) {
    		L.setWidth(40);
    	}
    	else if (keycode == Keyboard.KEY_7) {
    		L.setWidth(70);
    	}
    	else if (keycode == Keyboard.KEY_8) {
    		L.setWidth(100);
    	}
    	else if (keycode == Keyboard.KEY_9) {
    		L.setWidth(150);
    	}
    	else if (keycode == Keyboard.KEY_C) {
    		L.close();
    	}
    	else if (keycode == Keyboard.KEY_T) {
    		showTexture = !showTexture;
    	}
    	else if (keycode == Keyboard.KEY_R) {
    		L = GLLine.makeRectangle(100,200,300,200,30);
    	}
    	L.rebuild();
    }
    
    /**
     * Add last mouse motion to the line, only if left mouse button is down.
     */
    public void mouseMove(int x, int y) {
    	// add mouse motion to line if left button is down, and mouse has moved more than 10 pixels 
    	if (mouseButtonDown(0) && distance(prevMouseX,prevMouseY,x,y) > 10f) {
    		// add a segment to the line
    		L.point(x,y);
        	// regenerate the line
        	L.rebuild();
        	// save mouse position
        	prevMouseX = x; 
        	prevMouseY = y;
    	}
    }

    /**
     * start a new line
     */
    public void mouseDown(int x, int y) {
    	L.clear();
    }
    
    public float distance(int preX, int preY, int x, int y) {
    	int diffX = x - preX;
    	int diffY = y - preY;
    	return (float)Math.sqrt((double)(diffX*diffX + diffY*diffY));
    }
}
