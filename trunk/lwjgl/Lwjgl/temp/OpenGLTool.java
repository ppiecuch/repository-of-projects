import org.lwjgl.Sys;
import org.lwjgl.LWJGLException;
import org.lwjgl.opengl.*;

import javax.swing.Timer;
import javax.swing.JPanel;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferInt;
import java.awt.Graphics;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.nio.ByteOrder;

import com.sojournermobile.studio.component.itempanel.ItemPanelWidgetToolIF;


public class OpenGLTool extends JPanel implements ActionListener, ItemPanelWidgetToolIF
{
//    public static Logger logger = Logger.getLogger("OpenGLTool");

    static
    {
        Sys.initialize();
    }

    private Timer                       animationTimer = new Timer(10, this); // the timer that controls animation

    private Pbuffer                     pBuffer;  // the native opengl pBuffer that we render the scene into

    private BufferedImage               renderedImage;    // the image that will be rendered to the swing component
    private int[]                       imageData; // the int[] that represents the backing store of our BufferedImage

    private IntBuffer                   intBuffer; // the int buffer that will host the captured opengl data
    private boolean                     isInitialized = false;


    private int                         width; // stored width for our component
    private int                         height; // stored height for our component
    private int                         bitDepth = 32;
    private int                         refreshRate = 60;
    private int                         alphaBits;
    private int                         stencilBits;
    private int                         depthBufferBits;
    private int                         sampleBits;

    private float                       rotation = 0.1f;
    private int                         frameCount = 0;
    private long                        startTime;


    /**
     * Creates a JPanel that can be rendered to by LWJGL. It sets itself to opaque
     * by default so that Swing knows that we will render our entire bounds and therefore
     * doesn't need to do any checks
     */
    public OpenGLTool()
    {
        // this component is responsible for painting its entire bounds
        //
        setOpaque( true );
    }

    public int getBitDepth()
    {
        return bitDepth;
    }

    public void setBitDepth(int bitDepth)
    {
        this.bitDepth = bitDepth;
    }

    public int getAlphaBits()
    {
        return alphaBits;
    }

    public void setAlphaBits(int alphaBits)
    {
        this.alphaBits = alphaBits;
    }

    public int getStencilBits()
    {
        return stencilBits;
    }

    public void setStencilBits(int stencilBits)
    {
        this.stencilBits = stencilBits;
    }

    public int getDepthBufferBits()
    {
        return depthBufferBits;
    }

    public void setDepthBufferBits(int depthBufferBits)
    {
        this.depthBufferBits = depthBufferBits;
    }

    public int getSampleBits()
    {
        return sampleBits;
    }

    public void setSampleBits(int sampleBits)
    {
        this.sampleBits = sampleBits;
    }

    /**

     */
    public void initComponent( int bpp, int freq ) throws Exception
    {
        logger.debug("Creating headless window " + width +"/" + height + "/" + bitDepth);

        DisplayMode[] modes = Display.getAvailableDisplayModes();
        DisplayMode mode = null;
        
        // Make sure that we find the mode that uses our current monitor freq.

        for (int i = 0; i < modes.length; i++) 
        {
            if (modes[i].getWidth() == width && modes[i].getHeight() == height
                    && modes[i].getBitsPerPixel() == bpp
                    && (freq == 0 || modes[i].getFrequency() == freq)) 
            {
                mode = modes[i];
            }
        }
        
        PixelFormat format = new PixelFormat(bitDepth, alphaBits, depthBufferBits, stencilBits, sampleBits);

        try
        {
            Display.setDisplayMode(mode);

            // create a PbUffer that will store the contents of our renderings
            //
            pBuffer = new Pbuffer(width, height, format, null, null);
            pBuffer.makeCurrent();
        }
        catch( Exception e )
        {
            logger.error("Unable to create headless display", e );
        }
    }

    /**
     * override update to avoid clearing
     */
    public void update(Graphics g)
    {
        paint(g);
    }

    public void start()
    {
        logger.debug("Starting animation timer");
        animationTimer.start();
        startTime = System.currentTimeMillis();
    }

    public void stop()
    {
        logger.debug("Stopping animation timer");
        animationTimer.stop();
    }

    /**
     * Instruct the component to paint its entire bounds
     *
     * @param e
     */
    public void actionPerformed(ActionEvent e)
    {
        paintImmediately(0, 0, getWidth(), getHeight());
    }

    public void paintComponent( Graphics g )
    {
        if ( ! isInitialized )
        {
            width = getWidth();
            height = getHeight();

            try
            {
                initComponent( bitDepth, refreshRate );
            }
            catch (Exception e)
            {
                e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
            }

            // create a buffered image that is of the size of this component
            // we will render directly into this buffered image in Swing
            // fashion so we can render our interface in Swing properly and get the
            // image data that is backing this buffered image so we can write directly
            // to the internal data structure with no unnecessary copy
            //
            renderedImage = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
            imageData = ((DataBufferInt)renderedImage.getRaster().getDataBuffer()).getData();

            // create an int buffer to store the captured data from OpenGL
            //
            intBuffer = ByteBuffer.allocateDirect(width * height * 4).order(ByteOrder.LITTLE_ENDIAN).asIntBuffer();

            // we're initialized
            //
            isInitialized = true;

            //start the animation thread
            //
            start();
        }
        else
        {
            // do render in opengl
            //
            render();

            // make sure we're done drawing before capturing the frame
            //
            GL11.glFlush();

            // capture the result
            //
            grabGLFrame();

            // draw the result to this component
            //
            g.drawImage(renderedImage, 0, 0, null);

        }

        frameCount ++;

        if ( frameCount % 100 == 0 )
        {
            long currentTime = System.currentTimeMillis();
            long secondTime = ( currentTime - startTime ) / 1000;

            float frameRate = frameCount / secondTime;

            logger.debug("Another 100 frames " + frameRate );

        }
    }


    /**
     * Captures the currently rendered frame to a buffered image so it can be rendered
     * to a Swing component without any repaint issues. Most assuredly will be slower
     * than direct rendering, but will be compliant with Swing
     */
    private void grabGLFrame()
    {
        GL11.glReadPixels(0, 0, width, height, GL12.GL_BGRA, GL11.GL_UNSIGNED_BYTE, intBuffer);

        intBuffer.clear();

        // dump the captured data into the buffered image
        //
        for (int x = height; --x >= 0; )
        {
           intBuffer.get(imageData, x * width, width);
        }

        intBuffer.flip();
    }

        /**
     * <code>getValidDisplayMode</code> returns a <code>DisplayMode</code>
     * object that has the requested width, height and color depth. If there is
     * no mode that supports a requested resolution, null is returned.
     */
    private DisplayMode getValidDisplayMode(int width, int height, int bpp, int freq)
    {
        // get all the modes, and find one that matches our width, height, bpp.
        DisplayMode[] modes;
        try
        {
            modes = Display.getAvailableDisplayModes();
        } catch (LWJGLException e) {
            e.printStackTrace();
            return null;
        }
        // Make sure that we find the mode that uses our current monitor freq.

        for (int i = 0; i < modes.length; i++) {
            if (modes[i].getWidth() == width && modes[i].getHeight() == height
                    && modes[i].getBitsPerPixel() == bpp
                    && (freq == 0 || modes[i].getFrequency() == freq)) {

                return modes[i];
            }
        }

        // none found
        return null;
    }

    private void render()
    {
        GL11.glClear(GL11.GL_COLOR_BUFFER_BIT);

        GL11.glRotatef(rotation,0.0f,1.0f,0.0f);				// Rotate The Triangle On The Y axis ( NEW )
        GL11.glBegin(GL11.GL_TRIANGLES);						// Drawing Using Triangles
        GL11.glVertex3f( 0.0f, 1.0f, 0.0f);				// Top
        GL11.glVertex3f(-1.0f,-1.0f, 0.0f);				// Bottom Left
        GL11.glVertex3f( 1.0f,-1.0f, 0.0f);				// Bottom Right
        GL11.glEnd();

        rotation += 0.2f;
    }

    public void setData(Element metadata) throws Exception
    {
        //To change body of implemented methods use File | Settings | File Templates.
    }
}
