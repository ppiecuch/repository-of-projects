package org.tinder.m3g.tutorial.morphing.penguin;


import javax.microedition.lcdui.*;
import javax.microedition.m3g.*;


public class MorphModel 
{



  private MorphingMesh morphModel;


  public MorphModel() 
  { 
    makeMorph();
    setUpAnimation(morphModel);
  } 

  public MorphingMesh getModel()
  {  return morphModel; }

  // ------------------- morphing mesh creation -----------------------------

  private void makeMorph()
  /* The model base is PenguinBase, and its two targets 
     are PenguinUp (flippers up) and PenguinDown (flippers down). 
  */ 
  {
    VertexBuffer base = (new PenguinBase()).getVertexBuffer();

    VertexBuffer[] targets = new VertexBuffer[2];  // 2 targets
    targets[0] = (new PenguinUp()).getVertexBuffer();  // PenguinUp
    targets[1] = (new PenguinDown()).getVertexBuffer();  // PenguinDown
    int[] LENGTH = {235, 23, 32};
    IndexBuffer indexBuffer  = new TriangleStripArray(0, LENGTH);
    Appearance appearance = new Appearance();
    Material material = new Material();

    material.setColor(Material.AMBIENT, 0x0003C1C1);
    material.setColor(Material.EMISSIVE, 0x00000000);
    material.setColor(Material.DIFFUSE, 0xFFE9B1E8);
    material.setColor(Material.SPECULAR, 0x00000000);
    material.setShininess(128.0f);
    appearance.setMaterial(material);

    Image image = null;
    try {
      image = Image.createImage("/texture_penguin.png");
    }
    catch (Exception e)
    { System.out.println("Cannot make image " ); }
    if (image != null) {
      // create an Image2D for the Texture2D
      Image2D image2D = new Image2D(Image2D.RGB, image);

      // create the Texture2D and enable mipmapping
      Texture2D texture = new Texture2D(image2D);

      texture.setFiltering(Texture2D.FILTER_NEAREST, Texture2D.FILTER_NEAREST);
      texture.setWrapping(Texture2D.WRAP_CLAMP, Texture2D.WRAP_CLAMP);
      texture.setBlending(Texture2D.FUNC_MODULATE);  // mix material and texture

      appearance.setTexture(0, texture);
    }

    morphModel = new MorphingMesh(base, targets, indexBuffer , appearance);
    morphModel.setTranslation(0.15f, 0.15f, 0.1f);  // centered at the origin
    morphModel.scale(0.5f, 0.5f, 0.5f);

  }  // end of makeMorph()

  // ---------------- morphing animation ----------------

  private void setUpAnimation(MorphingMesh morphModel)
  /* The animation adjusts the morph weights to switch between the
     base and the targets. */
  {
    // creation animation controller 
    AnimationController animCtrl = new AnimationController();
    animCtrl.setActiveInterval(0, 1500);
    animCtrl.setSpeed(4.0f, 0);	// flap quickly

    // creation morph weights animation track 
    KeyframeSequence ks = new KeyframeSequence(8, 2, KeyframeSequence.LINEAR);

    /* Morphing weights make the penguin's flippers flap up, then down,
       then back to the starting base position. First weight is for 
       the up target, the second for the down target.
    */
    ks.setKeyframe(0, 0, new float[]  { 0.0f, 0.0f });  // at base
    ks.setKeyframe(1, 10, new float[] { 0.5f, 0.0f });
    ks.setKeyframe(2, 20, new float[] { 1.0f, 0.0f });  // totally up
    ks.setKeyframe(3, 30, new float[] { 0.5f, 0.0f });
    ks.setKeyframe(4, 40, new float[] { 0.0f, 0.0f });  // back at base
    ks.setKeyframe(5, 50, new float[] { 0.0f, 0.5f });
    ks.setKeyframe(6, 60, new float[] { 0.0f, 1.0f });  // totally down
    ks.setKeyframe(7, 70, new float[] { 0.0f, 0.5f });

    ks.setDuration(80);
    ks.setValidRange(0, 7);
    ks.setRepeatMode(KeyframeSequence.LOOP);
    
    AnimationTrack animTrack = new AnimationTrack(ks, AnimationTrack.MORPH_WEIGHTS); 
    animTrack.setController(animCtrl);
    morphModel.addAnimationTrack(animTrack);  // will affect morphModel
  }  // end of setUpAnimation()



}  // end of MorphModel class
