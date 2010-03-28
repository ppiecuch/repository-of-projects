package org.tinder.m3g.tutorial.particle.explosion;
import javax.microedition.m3g.Graphics3D;

public class ParticleSystem
{
    // The effect
    private ExplosionEffect effect = null;
    
    // The particles
    Particle[] parts = null;

    
    /**
     * Creates a particle system that emits particles according to a defined effect.
     * @param effect The effect that controls the behaviour of the particles
     * @param num The number of particles to emit
     */
    public ParticleSystem(ExplosionEffect effect, int num)
    {
        // Copy the effect
        setEffect(effect);

        // Init the particles
        parts = new Particle[num];
        for(int i = 0; i < num; i++)
        {
            parts[i] = new Particle();
            effect.init(parts[i],360*i/num);
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
    public void setEffect(ExplosionEffect effect) {
        this.effect = effect;
    }

    /**
     * @return Returns the effect.
     */
    public ExplosionEffect getEffect() {
        return effect;
    }
}
