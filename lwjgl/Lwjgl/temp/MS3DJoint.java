package org.java3dgamesdk.graphics.MS3D;

import javax.vecmath.*;

public class MS3DJoint {

    public byte                             flags;
    public String                           name;
    public String                           parentName;
    public float                            x;
    public float                            y;
    public float                            z;
    public float                            rotX;
    public float                            rotY;
    public float                            rotZ;
    public int                              numKeyFramesRot;
    public int                              numKeyFramesTrans;
    
    public MS3DKeyFrameRotation             keyRotations[];
    public MS3DKeyFrameTranslation          keyTranslations[];
    
    public Matrix4f                         absoluteMatrix;
    public Matrix4f                         relativeMatrix;
    public Matrix4f                         finalMatrix;
    
    public short                            currentRotationKeyframe;
    public short                            currentTranslationKeyframe;
        
}
