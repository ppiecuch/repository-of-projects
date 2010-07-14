import java.awt.Graphics; 
 import java.util.Properties; 
  
 /** 
  * Represents a configurable self-visualizable algorithm.<br> 
  * Configuration is possible via the <code>Properties</code> object returned by the {@link #getProperties()}. 
  * s 
  * @author Belicza Andras 
  */ 
 public interface Algorithm { 
          
         /** 
          * Returns the name of the algorithm. 
          * @return the name of the algorithm 
          */ 
         public String getName(); 
          
         /** 
          * Returns the name of the author of the algorithm. 
          * @return the name of the author of the algorithm 
          */ 
         public String getAuthor(); 
          
         /** 
          * Returns the version of the algorithm. 
          * @return the version of the algorithm 
          */ 
         public String getVersion(); 
          
         /** 
          * Returns the description of the algorithm. 
          * @return the description of the algorithm 
          */ 
         public String getDescription(); 
          
         /** 
          * Returns the properties used to control the algorithm.<br> 
          * The returned instance can be used to modify the properties. 
          *  
          * @return the properties used to control the algorithm 
          */ 
         public Properties getProperties(); 
          
         /** 
          * Restores the default properties. 
          */ 
         public void restoreDefaultProperties(); 
          
         /** 
          * Paints the result of the algorithm or returns an error if the supplied properties are not valid. 
          *  
          * @param graphics graphics context to be used for painting 
          * @param width    width of the desired image 
          * @param height   height of the desired image 
          * @throws IllegalArgumentException thrown if illegal property value is specified 
          */ 
         public void paint( final Graphics graphics, final int width, final int height ) throws IllegalArgumentException; 
          
 } 
 