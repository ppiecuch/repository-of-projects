package background.abc;


import javax.microedition.lcdui.*;
import javax.microedition.midlet.*;
import java.util.*;


public class SkinM3G extends MIDlet
{
  private static final int PERIOD = 50;   // in ms

  private Timer timer;
  private SkinCanvas skinCanvas = null;

  public SkinM3G()
  { timer = new Timer();  
    skinCanvas = new SkinCanvas(this); 
  }


  public void startApp() 
  { Display.getDisplay(this).setCurrent( skinCanvas );
    timer.schedule( new SkinTimer(), 0, PERIOD);   // tick every PERIOD ms
  }
  
  public void pauseApp() {}

  public void destroyApp(boolean unconditional) {}

  public void finishGame()
  { timer.cancel();    // stop the timer
    notifyDestroyed();  
  }


  // -------------------------------------------

  class SkinTimer extends TimerTask 
  {
    public void run() 
    { if(skinCanvas != null)
        skinCanvas.update();   // update the scene
    }
  }

} // end of SkinM3G class
