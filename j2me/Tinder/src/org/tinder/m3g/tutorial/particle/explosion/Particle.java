package org.tinder.m3g.tutorial.particle.explosion;

public class Particle
{
    // The life of the particle. Goes from 1.0f to 0.0f
    private float life = 1.0f;
    
    // The degradation of the particle
    private float degradation = 0.1f;
    
    // The velocities of the particle
    private float[] vel = {0.0f, 0.0f, 0.0f};
    
    // The position of the particle
    private float[] pos = {0.0f, 0.0f, 0.0f};
    
    // The color of the particle (RGB format 0xRRGGBB)
    private int color = 0xffffff;
    
    private int color_index;
    /** Empty initialization */
    public Particle()
    {
        
    }
    
    /**
     * Initializes the particle
     * @param velocity Sets the velocity
     * @param position Sets the position
     * @param color Sets the color (no alpha)
     */
    public Particle(float[] velocity, float[] position, int color)
    {
        setVel(velocity);
        setPos(position);
        this.setColor(color);
    }

    /**
     * @param life The life to set.
     */
    void setLife(float life) {
        this.life = life;
    }
    void setColorIndex(int index){
    	this.color_index = index;
    }
    int getColorIndex(){
      return color_index;
    }

    /**
     * @return Returns the life.
     */
    float getLife() {
        return life;
    }

    /**
     * @param vel The vel to set.
     */
    void setVel(float[] tvel) {
        System.arraycopy(tvel, 0, vel, 0, vel.length);
    }

    /**
     * @return Returns the vel.
     */
    float[] getVel() {
        return vel;
    }

    /**
     * @param pos The pos to set.
     */
    void setPos(float[] tpos) {
        System.arraycopy(tpos, 0, pos, 0, pos.length);
    }

    /**
     * @return Returns the pos.
     */
    float[] getPos() {
        return pos;
    }

    /**
     * @param color The color to set.
     */
    void setColor(int color) {
        this.color = color;
    }

    /**
     * @return Returns the color.
     */
    int getColor() {
        return color;
    }

    /**
     * @param degradation The degradation to set.
     */
    public void setDegradation(float degradation) {
        this.degradation = degradation;
    }

    /**
     * @return Returns the degradation.
     */
    public float getDegradation() {
        return degradation;
    }
}
