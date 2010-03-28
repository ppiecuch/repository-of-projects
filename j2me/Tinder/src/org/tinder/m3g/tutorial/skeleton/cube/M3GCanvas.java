package org.tinder.m3g.tutorial.skeleton.cube;


import javax.microedition.lcdui.*;
import javax.microedition.m3g.*;
import javax.microedition.lcdui.game.*;

public class M3GCanvas  extends GameCanvas implements Runnable
{
  private Command exitCmd; 

  private Graphics3D g3d;


  private World world;



    private Background bg = new Background();
  // timing information
  private int appTime = 0;
  private int nextTimeToAnimate;
  private Camera camera;
  private Light light = new Light();
  public M3GCanvas() 
  { 
    super(false);
    setFullScreenMode(false);
    g3d = Graphics3D.getInstance();
    world = new World();   // create world graph
    camera = new Camera();
    float aspect = (float) getWidth() / (float) getHeight();
    camera.setPerspective(45.0f, aspect, 1.0f, 1000.0f);
    camera.postRotate(-10.0f, 1.0f, 0, 0); 
    camera.postRotate(-10.0f, 0, 1.0f, 0); 
    camera.translate(-0.5f, 0.5f, 2.0f);

   world.addChild(camera);
    
    world.setActiveCamera(camera);
    light.setColor(0xffffff);  // white light
    light.setIntensity(1.25f); // over bright
    world.addChild(light);
    SkinModel sm = new SkinModel();
    world.addChild( sm.getModel() );  // add the model
    bg.setColor(0x9EFE00); // light blue 
    world.setBackground(bg);
    nextTimeToAnimate = world.animate(appTime);
    Thread t = new Thread(this);
    t.start();
  }  // end of SkinCanvas()

  
     public void run() {
        Graphics g2d = getGraphics();
        while(true){
        try{
            g3d.bindTarget(g2d); // Binds the given Graphics or mutable Image2D as the rendering target of this Graphics3D
            g3d.clear(null);
            appTime++;
            if (appTime >= nextTimeToAnimate)nextTimeToAnimate = world.animate(appTime) + appTime;
            g3d.render(world);
          }finally{
            g3d.releaseTarget();
        }
            flushGraphics();
            try {
                Thread.sleep(50); 
            }catch (InterruptedException e) {}
        }

    }





}  
