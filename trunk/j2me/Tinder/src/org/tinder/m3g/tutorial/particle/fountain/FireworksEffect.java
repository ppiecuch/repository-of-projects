package org.tinder.m3g.tutorial.particle.fountain;
import java.util.Random;
import javax.microedition.m3g.*;
import javax.microedition.lcdui.Image;

public class FireworksEffect 
{
    // The angle of particle emission
    private int angle = 90;
    
    // The sine and cosine of the current angle
    private float[] trig = {1.0f, 0.0f};
    
    // The emitting origin
    private float[] pos = {0.0f, 0.0f, 0.0f};
    
    // The randomizer
    Random rand = null;
       // The mesh
    Mesh mesh = null;
    
    // The transformation matrix
    Transform trans = new Transform();
    
    // The scale
    float scale = 1.0f;
    /**
     * @param angle The angle of particle emission
     */
    public FireworksEffect(int angle)
    {
        // Init the bitmap
     //   super(, 0.05f);
        
        // Set the angle
        setAngle(angle);
        // Load the plane with the wanted texture
        mesh = createAlphaPlane("/particle.png");
        
        // Make sure we set the scale
        this.scale = 0.1f;        
        // Get randomizer
        rand = new Random();
    }

    /**
     * @see ParticleEffect#init(Particle)
     */
    public void init(Particle p)
    {
        // Set the particle's life
        p.setLife(1.0f);
        
        // Set the particle's position
        p.setPos(pos);
        
        // Create the particle's velocties
        float[] vel = new float[3];
        
        // We want velocities from 0.2f to 1.0f
        float xyvel = rand.nextFloat() * 0.8f + 0.2f;
        
        // We want the particle to die slowly
        p.setDegradation(xyvel / 18);
        
        // Set velocities according to trigonometry with a small deviation
        vel[0] = xyvel * trig[1] + rand.nextFloat() * 0.125f - 0.0625f;
        vel[1] = xyvel * trig[0] + rand.nextFloat() * 0.125f - 0.0625f;
        
        // No movement in depth
        vel[2] = 0.0f;
        
        // Set the velocity
        p.setVel(vel);
        
        // Set the random color
        int r = (int)(120 * rand.nextFloat()) + 120;
        int g = (int)(120 * rand.nextFloat()) + 120;
        int b = (int)(120 * rand.nextFloat()) + 120;
        int col = (r << 16) | (g << 8) | b;
        p.setColor(col);
    }

    /**
     * @see ParticleEffect#update(Particle)
     */
    public void update(Particle p)
    {
        // Simply update position
        float[] ppos = p.getPos();
        float[] vel = p.getVel();
        ppos[0] += vel[0];
        ppos[1] += vel[1];
        ppos[2] += vel[2];
        
        // Update life
        p.setLife(p.getLife() - p.getDegradation());
        
        // Check life. If it is dead, we just reinit it
        if(p.getLife() < -0.001f)
        {
            init(p);
        }
    }

    /**
     * @param angle The angle to set.
     */
    public void setAngle(int angle) {
        this.angle = angle;
        trig[0] = (float)Math.sin(Math.toRadians(angle));
        trig[1] = (float)Math.cos(Math.toRadians(angle));
    }

    /**
     * @return Returns the angle.
     */
    public int getAngle() {
        return angle;
    }



    /**
     * @see ParticleEffect#render(Particle, Graphics3D)
     */
    public void render(Particle p, Graphics3D g3d)
    {
        // Calculate the alpha
        int alpha = (int)(255 * p.getLife());
        
        // Create the color
        int color = p.getColor() | (alpha << 24);
        
        // Set alpha

        mesh.getVertexBuffer().setDefaultColor(color);
        // Transform
        trans.setIdentity();
        trans.postScale(scale, scale, scale);
        float[] pos = p.getPos();
        trans.postTranslate(pos[0], pos[1], pos[2]);
        
        // Render
        g3d.render(mesh, trans);
    }
       private Mesh createAlphaPlane(String texFilename)
    {
        // Create a normal mesh
                // The POINTS of the plane
        short POINTS[] = new short[] {-1, -1, 0,
                                       1, -1, 0,
                                       1, 1, 0,
                                       -1, 1, 0};
        // Texture coords of the plane
        short TEXCOORDS[] = new short[] {0, 255,
                                         255, 255,
                                         255, 0,
                                         0, 0};
        // Create the model's POINTS
        VertexArray POSITION_ARRAY = new VertexArray(POINTS.length/3, 3, 2);
        POSITION_ARRAY.set(0, POINTS.length/3, POINTS);
        // Create the model's texture coords
        VertexArray TEXCOORD_ARRAY = new VertexArray(TEXCOORDS.length / 2, 2, 2);
        TEXCOORD_ARRAY.set(0, TEXCOORDS.length / 2, TEXCOORDS);
        // Compose a VertexBuffer out of the previous POINTS and texture coordinates
        VertexBuffer vertexBuffer = new VertexBuffer();
        vertexBuffer.setPositions(POSITION_ARRAY, 1.0f, null);
        vertexBuffer.setTexCoords(0, TEXCOORD_ARRAY, 1.0f/255.0f, null);
        vertexBuffer.setDefaultColor(0xffffffff);
        // Create INDICES and face lengths
        int INDICES[] = new int[] {0, 1, 3, 2};
        int[] LENGTHS = new int[] {4};
        
        // Create the model's indexBuffer
        IndexBuffer indexBuffer = new TriangleStripArray(INDICES, LENGTHS);

        // Create the appearance
        Appearance appearance = new Appearance();
        PolygonMode polygonmode = new PolygonMode();
        polygonmode.setCulling(PolygonMode.CULL_BACK);
        appearance.setPolygonMode(polygonmode);
        CompositingMode compositingmode = new CompositingMode();
        compositingmode.setBlending(CompositingMode.ALPHA);
        appearance.setCompositingMode(compositingmode);
      
        // Create and set the texture
        try
        {
            // Open image
            Image texImage = Image.createImage(texFilename);
            Texture2D texture = new Texture2D(new Image2D(Image2D.RGBA, texImage));
            
            // Replace the mesh's original colors (no blending)
            texture.setBlending(Texture2D.FUNC_REPLACE);
            
            // Set wrapping and filtering
            texture.setWrapping(Texture2D.WRAP_CLAMP, Texture2D.WRAP_CLAMP);
            texture.setFiltering(Texture2D.FILTER_BASE_LEVEL, Texture2D.FILTER_NEAREST);
            texture.setBlending(Texture2D.FUNC_BLEND);
            // Add texture to the appearance
            appearance.setTexture(0, texture);

        }
        catch(Exception e)
        {
            // Something went wrong
            System.out.println("Failed to create texture");
            System.out.println(e);
        }
        
        // Finally create the Mesh
        Mesh mesh = new Mesh(vertexBuffer, indexBuffer, appearance);

        return mesh;
    }
}
