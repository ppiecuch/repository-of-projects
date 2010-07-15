import java.awt.Dimension;

import javax.swing.JFrame;

public class OpenGLJPanelTest extends JFrame
{
//    public static Logger logger = Logger.getLogger("OpenGLJPanelTest");

    public OpenGLJPanelTest()
    {
        OpenGLTool panel = new OpenGLTool();

        getContentPane().add( panel );
        panel.setPreferredSize( new Dimension( 640, 480 ) );
    }

    public static final void main(String[] args )
    {
        BasicConfigurator.configure();

        try
        {
            OpenGLJPanelTest test = new OpenGLJPanelTest();

            test.pack();
            test.setVisible( true );

            test.setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );
        }
        catch( Exception e )
        {
            logger.error(e);
        }

    }
}