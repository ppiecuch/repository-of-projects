package background.abc;



import javax.microedition.lcdui.*;
import javax.microedition.m3g.*;


public class SkinCanvas extends Canvas implements CommandListener 
{
  private Command exitCmd; 

  private Graphics3D iG3D;

  private SkinM3G top;
  private World scene;

  // scene elements
  private MobileBackground mobileBackGnd;
  private MobileCamera mobCam;
  private Group cameraGroup;


  // timing information
  private int appTime = 0;
  private int nextTimeToAnimate;


  public SkinCanvas(SkinM3G top) 
  { 
    this.top = top;

    exitCmd = new Command("Exit", Command.EXIT, 0);
    addCommand(exitCmd);
    setCommandListener(this);

    iG3D = Graphics3D.getInstance();
    scene = new World();   // create scene graph
    buildScene();

    // start the animation
    nextTimeToAnimate = scene.animate(appTime);
  }  // end of SkinCanvas()


  public void commandAction(Command command, Displayable disp)
  { 
    if (command == exitCmd)    // finish the morphing
      top.finishGame();
    else   // execution should not get here
      System.out.println("Command Action Error: " + command);
  }


  private void buildScene()
  /* Add a background, camera, trees, the bending model,
     and a floor to the scene. There are no lights needed,
     since nothing uses a Material component.
  */
  { /* The ordering of the first three methods is important.
       The Billboard trees need a reference to the camera, and
       the camera needs a reference to the background. */
    addBackground(); 
    addCamera();


  } // end of buildScene()


  private void addBackground()
  // the background is a MobileBackground object
  {    
    Image2D backIm = loadImage("/clouds.png");
    mobileBackGnd = new MobileBackground(backIm, getWidth(), getHeight());
    scene.setBackground( mobileBackGnd.getBackground() );
  }


  private void addCamera()
  {
    mobCam = new MobileCamera( getWidth(), getHeight(), mobileBackGnd);  
     /* The camera needs a reference to the MobileBackground
        object, so it can tell it to move. */

    cameraGroup = mobCam.getCameraGroup();
    scene.addChild( cameraGroup );
    scene.setActiveCamera( mobCam.getCamera() ); 
  }  // end of addCamera()


 
  private Image2D loadImage(String fn)
  // load the image stored in fn
  { Image2D im = null;
    try {
       im = (Image2D)Loader.load(fn)[0];
    }
    catch (Exception e)
    { System.out.println("Cannot make image from " + fn); }
    return im;
  }  // end of loadImage()



  // ------------ process key presses and releases ------------


  protected void keyPressed(int keyCode)
  { int gameAction = getGameAction(keyCode);
    mobCam.pressedKey(gameAction);
  }

  protected void keyReleased(int keyCode)
  { int gameAction = getGameAction(keyCode);
    mobCam.releasedKey(gameAction);
  }

  // ------------------ update and paint the canvas ------------------

  public void update()
  // called by the timer task in SkinM3G
  {
    mobCam.update();


    appTime++;
    if (appTime >= nextTimeToAnimate)
      nextTimeToAnimate = scene.animate(appTime) + appTime;
    repaint();
  }  // end of update()


  protected void paint(Graphics g)
  {
    iG3D.bindTarget(g);
    try {
      iG3D.render(scene);
    }
    catch(Exception e)
    { e.printStackTrace();  }
    finally {
      iG3D.releaseTarget();
    }

    // show the camera's details
    g.drawString( mobCam.getKeyMode(), 5,5, Graphics.TOP|Graphics.LEFT);
    g.drawString( mobCam.getPosition(), 5,18, Graphics.TOP|Graphics.LEFT);
    g.drawString( mobCam.getRotation(), 5,31, Graphics.TOP|Graphics.LEFT);
  } // end of paint()


}  // end of SkinCanvas class
