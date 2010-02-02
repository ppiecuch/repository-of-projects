package org.tinder.m3g.tutorial.abc01;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;


public class M3GDemo extends MIDlet implements CommandListener {
	
	private M3GCanvas canvas;

	public M3GDemo() {
		this.canvas=new M3GCanvas();
	}

	protected void destroyApp(boolean unconditional) throws MIDletStateChangeException {
		this.canvas.stop();
	}

	protected void pauseApp() {

	}

	protected void startApp() throws MIDletStateChangeException {
		Display.getDisplay(this).setCurrent(this.canvas);
		this.canvas.start();

	}

	public void commandAction(Command c, Displayable d) {
		notifyDestroyed();
	}

}
