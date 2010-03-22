package org.tinder.m3g.tutorial.pick;




import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;



public class M3GMIDlet extends MIDlet implements CommandListener
{


   

  

  private Command exitCommand = new Command("Exit", Command.EXIT, 1);
  
  /**
   * Starts application and shows the list of samples. The user can 
   * select a sample for execution and return to the selection screen.
   */
  public void startApp()
  {
   
    M3GCanvas mCanvas = new M3GCanvas();
    mCanvas.addCommand(exitCommand);
    mCanvas.setCommandListener(this);
    Display.getDisplay(this).setCurrent(mCanvas);
  }
  
  /**
   * Not used.
   */
  public void pauseApp()
  {
  }
  
  /**
   * Not used.
   */
  public void destroyApp(boolean unconditional)
  {
  }
  
  /**
   * Receives the command actions from the selection screen as well as
   * the sample screens.
   *
   * @param command command
   * @param displayable source of the command
   */
  public void commandAction(Command command, Displayable displayable)
  {
    if (command == exitCommand)
    {
    	
      destroyApp(true);
      notifyDestroyed();
    }
   
  }
}
