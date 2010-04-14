package immediate;

/* * MIDletMain.java * * Created on 2008年2月16日, 下午1:04 */
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import java.util.*;

/**
 * * *
 * 
 * @author Administrator *
 * @version
 */
public class MIDletMain extends MIDlet {
	static MIDletMain instance;

	private MyCanvas displayable = new MyCanvas();

	private Timer iTimer = new Timer();

	public MIDletMain() {
		instance = this;
	}

	public void startApp() {
		Display.getDisplay(this).setCurrent(displayable);
		iTimer.schedule(new MyTimerTask(), 0, 40);
	}

	public void pauseApp() {
	}

	public void destroyApp(boolean unconditional) {
	}

	public static void quitApp() {
		instance.destroyApp(true);
		instance.notifyDestroyed();
		instance = null;
	}

	class MyTimerTask extends TimerTask {
		public void run() {
			if (displayable != null) {
				displayable.repaint();
			}
		}
	}
}