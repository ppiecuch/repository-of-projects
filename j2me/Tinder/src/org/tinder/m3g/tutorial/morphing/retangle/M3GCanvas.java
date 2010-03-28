package org.tinder.m3g.tutorial.morphing.retangle;

import javax.microedition.lcdui.*;
import javax.microedition.lcdui.game.*;
import javax.microedition.m3g.*;
public class M3GCanvas extends GameCanvas implements Runnable {
    private Graphics3D g3d ;
    private MorphingMesh morphingMesh;

    private Camera camera;
    private int CurrentFrame;


    private float[] weights;
    private Graphics g;
    private Background back = new Background();

    public M3GCanvas(){
        super(true);
        CurrentFrame = 0;

        weights = new float[2];
        VertexArray normals = new VertexArray(6, 3, 1);
        normals.set(0,6,new byte[] {0,0,-127,
                                    0,0,-127,
                                    0,0,-127,
                                    0,0,-127,
                                    0,0,-127,
                                    0,0,-127});

        VertexArray basePositions = new VertexArray(6, 3, 2);
        basePositions.set(0,6,new short[] {-100, -100, 0,
                                          100,  -100, 0,
                                          100,   0, 0,
                                          100,   100, 0,
                                          -100,  100, 0,
                                          -100,  0, 0});

       
        VertexArray[] targetPos = new VertexArray[2];
        for (int i = 0; i < targetPos.length; i++)
        {
            targetPos[i] = new VertexArray(6,3,2);
            targetPos[i].set(0,6,new short[] {-100,              -100, 0,
                                             100,               -100, 0,
                                             (short)(50*(1-i)+100),   0, 0,
                                             100,                100, 0,
                                             -100,               100, 0,
                                             (short)(-50*i-100), 0, 0});
        }

        TriangleStripArray strip = new TriangleStripArray(new int[] {1, 0, 2, 5, 3, 4},
                                                          new int[] {6});

        VertexBuffer base = new VertexBuffer();
        base.setPositions(basePositions, .1f, null);
        base.setNormals(normals);
        base.setDefaultColor(0xFF0B7FCC);

        VertexBuffer[] targets = new VertexBuffer[2];
        for (int i = 0; i < targets.length; i++)
        {
            targets[i] = new VertexBuffer();
            targets[i].setPositions(targetPos[i], 1.0f, null);
            if (i == 0)
            {
                targets[i].setDefaultColor(0xFFFF0000);
            }
            else
            {
                targets[i].setDefaultColor(0xFF0B7FCC);
            }
        }

        Material material = new Material();
        material.setVertexColorTrackingEnable(true);
        PolygonMode polyMode = new PolygonMode();
        polyMode.setCulling(PolygonMode.CULL_NONE);
        polyMode.setTwoSidedLightingEnable(true);
        Appearance appearance = new Appearance();
    //    appearance.setMaterial(material);
        appearance.setPolygonMode(polyMode);

        morphingMesh = new MorphingMesh(base, targets, strip, appearance);

 

        camera = new Camera();
        camera.setPerspective(90.f, 1.f, 1.f, 150.f);
        Transform cameraTrans = new Transform();
        cameraTrans.postTranslate(0.f, 0.f, 26.f);
        Graphics3D.getInstance().setCamera(camera, cameraTrans);
    
       // Background back = new Background();
        back.setColor(0xffF0F00F);
        back.setDepthClearEnable(true);
        Thread t = new Thread(this);
        t.start();
    }
    public void run() {
        Graphics g = getGraphics();
        while(true) {

        g.setColor( 255, 255, 255 );
       g.fillRect( 0, 0, getWidth(), getHeight() ); 
       g3d = Graphics3D.getInstance();
  

        
        try{
       g3d.bindTarget(g);
       g3d.clear(back);

        for (int i = 0; i < weights.length; i++)
        {
            weights[i] = (float) Math.sin(((float) (CurrentFrame % (60*(i+1)))) / ((float) (60*(i+1))) * Math.PI);
        }
        CurrentFrame++;
        morphingMesh.setWeights(weights);
        g3d.render(morphingMesh, null);

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
