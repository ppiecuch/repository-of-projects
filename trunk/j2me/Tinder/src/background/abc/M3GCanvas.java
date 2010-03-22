package background.abc;


import javax.microedition.lcdui.*;
import javax.microedition.m3g.*;


public class M3GCanvas extends Canvas implements Runnable
{
  

  private Graphics3D g3d;


  private World world;

  // world elements
  private MobileBackground mobileBackGnd;
  private MobileCamera mobCam;
  private Group cameraGroup;




  public M3GCanvas() 
  { 
  


    g3d = Graphics3D.getInstance();

    world = new World();   // create world graph
    buildScene();


    Thread t = new Thread(this);
    t.start();
  }  // end of SkinCanvas()



  private void buildScene()
  /* Add a background, camera, trees, the bending model,
     and a floor to the world. There are no lights needed,
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
    world.setBackground( mobileBackGnd.getBackground() );
  }


  private void addCamera()
  {
    mobCam = new MobileCamera( getWidth(), getHeight(), mobileBackGnd);  
     /* The camera needs a reference to the MobileBackground
        object, so it can tell it to move. */

    cameraGroup = mobCam.getCameraGroup();
    world.addChild( cameraGroup );
    world.setActiveCamera( mobCam.getCamera() ); 
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

  public void run()
  // called by the timer task in SkinM3G
  {
  	while(true){
      mobCam.update();
      repaint();
    }
  }  // end of update()


  protected void paint(Graphics g)
  {
    g3d.bindTarget(g);
    try {
      g3d.render(world);
    }
    catch(Exception e)
    { e.printStackTrace();  }
    finally {
      g3d.releaseTarget();
    }

    // show the camera's details
    g.drawString( mobCam.getKeyMode(), 5,5, Graphics.TOP|Graphics.LEFT);
    g.drawString( mobCam.getPosition(), 5,18, Graphics.TOP|Graphics.LEFT);
    g.drawString( mobCam.getRotation(), 5,31, Graphics.TOP|Graphics.LEFT);
  } // end of paint()


}  // end of SkinCanvas class
