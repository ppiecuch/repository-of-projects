package org.tinder.m3g.tutorial.skeleton.tail;

import javax.microedition.lcdui.*;
import javax.microedition.lcdui.game.*;
import javax.microedition.m3g.*;
public class M3GCanvas extends GameCanvas implements Runnable {
    private Graphics3D g3d ;
    private SkinnedMesh skinnedMesh;
    private Light light;
    private Camera camera;
    private int CurrentFrame;
    private Transform trans;
    private Transform lightTrans;
    private Transform bone1Trans;
    private Transform bone2Trans;
    private Group skeleton;
    private Group bone1;
    private Group bone2;
    private int iStartTime;
    private Graphics g;
    private Background back = new Background();

    public M3GCanvas(){
        super(true);
        CurrentFrame = 0;
        trans = new Transform();
        makeSkinned();
        back.setColor(0xFFFFFFFF);

        back.setDepthClearEnable(true);
        

        light = new Light();
        light.setMode(Light.OMNI);
        light.setAttenuation(0.001f, 0.1f, 0.0f);
        light.setIntensity(1.0f);
        light.setColor(0xFFFFFF);
        lightTrans = new Transform();
        lightTrans.postTranslate(-5.f, -5.f, 16.f);
        Graphics3D.getInstance().addLight(light, lightTrans);

        camera = new Camera();
        camera.setPerspective(90.f, 1.f, 1.f, 150.f);
        Transform cameraTrans = new Transform();
        cameraTrans.postTranslate(0.f, 0.f, 26.f);
        Graphics3D.getInstance().setCamera(camera, cameraTrans);
        Thread t = new Thread(this);
        t.start();
    }
    private void makeSkinned(){
        bone2Trans = new Transform();
        bone1Trans = new Transform();        
        int sides = 12;
        short[] POINTS = new short[sides*8*3*3];
        short[] NORMALS = new short[sides*8*3*3];
        for (int i = 0; i < 8*3; i++)
        {
            for (int j = 0; j < sides; j++)
            {
                POINTS[3*(i*sides + j)] = (short) (100*Math.sin(2*Math.PI*((float)j / (float)sides)));
                POINTS[3*(i*sides + j) + 1] = (short) (-50*i + 4*3*50); // Normalize to origin.
                POINTS[3*(i*sides + j) + 2] = (short) (100*Math.cos(2*Math.PI*((float)j / (float)sides)));
                NORMALS[3*(i*sides + j)] = (short) (-100*Math.sin(2*Math.PI*((float)j / (float)sides)));
                NORMALS[3*(i*sides + j) + 1] = (short) 0;
                NORMALS[3*(i*sides + j) + 2] = (short) (-100*Math.cos(2*Math.PI*((float)j / (float)sides)));
            }
        }
        VertexArray NORMAL_ARRAY = new VertexArray(3*8*sides, 3, 2);
        NORMAL_ARRAY.set(0,3*8*sides, NORMALS);
        VertexArray POSITION_ARRAY = new VertexArray(3*8*sides, 3, 2);
        POSITION_ARRAY.set(0,3*8*sides, POINTS);

        int[] INDICES = new int[(8*3-1)*(2*sides+2)];
        int[] LENGTH = new int[8*3-1];
        for (int i = 0; i < 8*3-1; i++)
        {
            INDICES[i*(2*sides+2)] = i*sides;
            for (int j = 1; j < sides+1; j++)
            {
                INDICES[i*(2*sides+2)+j*2-1] = (j%sides) + i*sides;
                INDICES[i*(2*sides+2)+j*2] = (j%sides) + (i+1)*sides;
            }
            INDICES[i*(2*sides+2)+sides*2+1] = (i+1)*sides + 1;
            LENGTH[i] = 2*sides+2;
        }

        TriangleStripArray indexBuffer = new TriangleStripArray(INDICES, LENGTH);

        VertexBuffer vertexBuffer = new VertexBuffer();
        vertexBuffer.setPositions(POSITION_ARRAY, .03f, null);
        vertexBuffer.setNormals(NORMAL_ARRAY);
        vertexBuffer.setDefaultColor(0xFFFF0000);


        Material material = new Material();
        material.setColor(Material.AMBIENT, 0xFFFF0097); 
        material.setColor(Material.DIFFUSE, 0xff0B7FCC); 
        PolygonMode polyMode = new PolygonMode();
        polyMode.setCulling(PolygonMode.CULL_NONE);
        polyMode.setTwoSidedLightingEnable(true);
        Appearance appearance = new Appearance();
        appearance.setMaterial(material);
        appearance.setPolygonMode(polyMode);

        skeleton = new Group();
        bone1 = new Group();
        bone2 = new Group();
        skeleton.addChild(bone1);
        bone1.addChild(bone2);
        
        skinnedMesh = new SkinnedMesh(vertexBuffer, indexBuffer, appearance, skeleton);
        skinnedMesh.addTransform(bone1, 200, 8*sides, 10*sides);
        skinnedMesh.addTransform(bone2, 1, 2*8*sides, 8*sides);
    
    }
    public void run() {
        Graphics g = getGraphics();
        while(true) {
        g3d = Graphics3D.getInstance();
        g3d.bindTarget(g);
        try{
        g3d.clear(back);
        bone1Trans.setIdentity();
        bone1Trans.postTranslate(0.0f, (float) (4*3*25)*0.03f, 0.0f);
        bone1Trans.postRotate((float)Math.sin(((double)CurrentFrame/30)*Math.PI*2)*40, 0.0f, 0.0f, 1.0f);
        bone1Trans.postTranslate(0.0f, (float) (-4*3*25)*0.03f, 0.0f);
        bone1.setTransform(bone1Trans);

        bone2Trans.setIdentity();
        bone2Trans.postTranslate(0.0f, (float) (-4*3*25)*0.03f, 0.0f);
        bone2Trans.postRotate((float)Math.sin(((double)CurrentFrame/60)*Math.PI*2)*40, 0.0f, 0.0f, 1.0f);
        bone2Trans.postTranslate(0.0f, (float) (4*3*25)*0.03f, 0.0f);
        bone2.setTransform(bone2Trans);

        trans.postRotate(2.0f, 0.0f, 1.0f, 0.0f);

        g3d.render(skinnedMesh, trans);

        if ((CurrentFrame + 15) % 30 == 0)
        {
            skinnedMesh = (SkinnedMesh) skinnedMesh.duplicate();
            bone1 = (Group) skinnedMesh.getSkeleton().getChild(0);
            bone2 = (Group) bone1.getChild(0);
        }

        CurrentFrame++;
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
