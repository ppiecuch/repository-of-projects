package org.tinder.m3g.tutorial.morphing.penguin;

import javax.microedition.lcdui.*;
import javax.microedition.m3g.*;
import javax.microedition.lcdui.game.*;

public class M3GCanvas extends GameCanvas implements Runnable
{
  private Graphics3D g3d;
  private World world;
  // timing information
  private int appTime = 0;
  private int nextTimeToAnimate;
  public M3GCanvas() 
  { 
    super(false);
    setFullScreenMode(false);
    g3d = Graphics3D.getInstance();
    world = new World();   // create world graph
    Camera camera = new Camera();
    float aspect = (float) getWidth() / (float) getHeight();
    camera.setPerspective(45.0f, aspect, 0.1f, 100.0f);
    camera.translate(0.0f, 0.25f, 1.0f);
    world.addChild(camera);
    world.setActiveCamera(camera);
    Light light = new Light();  // default white, directional light
    light.setIntensity(1.25f);  // make it a bit brighter
    light.setOrientation(-45.0f, 1.0f, 0, 0); // pointing down and into world
    world.addChild(light);
    Background backGnd = new Background();
    backGnd.setColor(0x00bbff); // a light blue background
    world.setBackground(backGnd);
    MorphModel penguin = new MorphModel();
    world.addChild( penguin.getModel() );  // add the model
    // start the animation
    nextTimeToAnimate = world.animate(appTime);
    Thread t = new Thread(this);
    t.start();
  }  // end of MorphCanvas()

  public void run() {
        Graphics g2d = getGraphics();
        while(true){
        try{
            g3d.bindTarget(g2d); // Binds the given Graphics or mutable Image2D as the rendering target of this Graphics3D
          //  g3d.clear(bg);

    appTime++;
    if (appTime >= nextTimeToAnimate)
      nextTimeToAnimate = world.animate(appTime) + appTime;

            g3d.render(world);
  
        }finally{
            g3d.releaseTarget();
        }
            flushGraphics();
                     try {
                Thread.sleep(50); 
            }
            catch (InterruptedException e) {
                
            }
        }

    }

}  
