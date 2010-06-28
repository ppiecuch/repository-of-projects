package de.bloodyblades.ms3dloader;

public class MS3DAnimation {
    
    public short startFrame;
    public short endFrame;
    
    public MS3DAnimation() {
        this.startFrame = 0;
        this.endFrame = 0;
    }
    
    /** Creates a new instance of MS3DAnimation */
    public MS3DAnimation(short startFrame, short endFrame) {
        this.startFrame = startFrame;
        this.endFrame = endFrame;
    }

    public MS3DAnimation(int startFrame, int endFrame) {
        this.startFrame = (short)startFrame;
        this.endFrame = (short)endFrame;
    }
       
}
