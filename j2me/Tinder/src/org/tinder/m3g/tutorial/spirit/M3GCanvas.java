package org.tinder.m3g.tutorial.spirit;

import javax.microedition.lcdui.*;
import javax.microedition.lcdui.game.*;
import javax.microedition.m3g.*;
public class M3GCanvas extends GameCanvas implements Runnable {
    private Graphics3D g3d ;

    private Camera camera;
    private World world;
    private Light light;
    private Sprite3D sprite;
    private Graphics g;
    private Background background ;
    private Image2D image2D;
    public M3GCanvas(){
        super(true);

        world = new World();
        camera = new Camera();
        camera.setPerspective(70.0f, 1.0f, 1.0f, 10000.0f);
        camera.setTranslation(0.0f, 0.0f, 40.0f);
        world.addChild(camera);
        world.setActiveCamera(camera);
        light = new Light();
        light.setTranslation(0.0f, 0.0f, 40.0f);
        world.addChild(light);
        background = new Background();
        background.setColor(0xffF0F00F);
        background.setDepthClearEnable(true);
        world.setBackground(background);
        try {
          image2D = (Image2D)Loader.load("/fish.png")[0];
        }catch (Exception e)
        { System.out.println("Cannot make image"); }

        sprite = new Sprite3D(true, image2D, new Appearance());

        sprite.setScale(20.0f, 20.0f, 20.0f);
        world.addChild(sprite);
        Thread t = new Thread(this);
        t.start();
    }
    public void run() {
        Graphics g = getGraphics();
        while(true) {

        g.setColor( 255, 255, 255 );
        g.fillRect( 0, 0, getWidth(), getHeight() ); 
        g3d = Graphics3D.getInstance();
  
        double temp = Math.tan((1.0f - (float)Math.cos(System.currentTimeMillis()))*0.5f);
        sprite.setTranslation(0.0f, 0.0f, (float)(temp*temp*temp) * -100.0f + 30.0f);
        
        try{
          g3d.bindTarget(g);

          g3d.render(world);    
   

        }catch(Exception e){
            System.err.println(e);
        }finally{
            g3d.releaseTarget();
        }
        flushGraphics();
            try {
                Thread.sleep(200); 
            }
            catch (InterruptedException e) {
                
            }
        }
    }
  
}
