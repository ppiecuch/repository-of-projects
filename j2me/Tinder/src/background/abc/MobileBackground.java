package background.abc;


import javax.microedition.lcdui.*;
import javax.microedition.m3g.*;


public class MobileBackground 
{
  private final static int STEP = 3;  
  // incremental change to the cropping rectangle's x- or y- top-left coordinate

  private Background backGnd;
  private boolean isMoveable = false;

  private int width, height;   // of the screen


  public MobileBackground(Image2D backIm, int w, int h)
  {
    width = w;  height = h;
    backGnd = new Background();

    if (backIm != null) {
      backGnd.setImage( backIm );
      backGnd.setImageMode(Background.REPEAT, Background.REPEAT);
      backGnd.setCrop(backGnd.getCropX(), backGnd.getCropY(), width, height);
      isMoveable = true;
    }
    else 
      backGnd.setColor(0x00bffe); // a light blue background
  } // end of MobileBackground()


  public Background getBackground()
  {  return backGnd;  }


  // ----------------- background movement methods ----------------

  /* A movement is controlled by the STEP constant and also a factor
     input argument to the method. The STEP and factor are used to adjust
     the crop rectangle's x- or y- top-left coordinate.

     A movement only occurs if a repeating background image was created
     by the constructor (isMoveable == true).

     These methods are called from the MobileCamera object when the 
     camera moves.
  */

  public void moveRight(int factor)
  { if (isMoveable)
      backGnd.setCrop(backGnd.getCropX()-(STEP*factor), 
                      backGnd.getCropY(), width, height);
  } 

  public void moveLeft(int factor)
  { if (isMoveable)
      backGnd.setCrop(backGnd.getCropX()+(STEP*factor), 
                      backGnd.getCropY(), width, height);
  }

  public void moveUp(int factor)
  { if (isMoveable)
      backGnd.setCrop(backGnd.getCropX(), 
                      backGnd.getCropY()+(STEP*factor), width, height);
  }

  public void moveDown(int factor)
  { if (isMoveable)
      backGnd.setCrop(backGnd.getCropX(), 
                      backGnd.getCropY()-(STEP*factor), width, height);
  }

}  // end of MobileBackground class
