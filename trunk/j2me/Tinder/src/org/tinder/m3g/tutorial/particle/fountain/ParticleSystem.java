package org.tinder.m3g.tutorial.particle.fountain;
import javax.microedition.m3g.Graphics3D;

/**
 * Manages emission of particles in our 3D world
 */
public class ParticleSystem
{
    // The effect
    private FireworksEffect effect = null;
    
    // The particles
    Particle[] parts = null;
    

    public ParticleSystem(FireworksEffect effect, int num)
    {
        // Copy the effect
        setEffect(effect);
        
        // Init the particles
        parts = new Particle[num];
        for(int i = 0; i < num; i++)
        {
            parts[i] = new Particle();
            effect.init(parts[i]);
        }
    }
    
    /** The method that does it all. Needs to be called every tick of a game loop */
    public void emit(Graphics3D g3d)
    {
        for(int i = 0; i < parts.length; i++)
        {
            getEffect().update(parts[i]);
            getEffect().render(parts[i], g3d);
        }
    }

    /**
     * @param effect The effect to set.
     */
    public void setEffect(FireworksEffect effect) {
        this.effect = effect;
    }

    /**
     * @return Returns the effect.
     */
    public FireworksEffect getEffect() {
        return effect;
    }
}
