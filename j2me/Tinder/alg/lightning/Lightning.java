import hu.belicza.andras.recursion.model.RandomBaseAlgorithm; 
  
 import java.awt.Graphics; 
  
 /** 
  * A simple recursive algorithm to generate lightnings. 
  *  
  * @author Belicza Andras 
  */ 
 public class Lightning extends RandomBaseAlgorithm { 
          
         private static final String PROPERTY_GRANULARITY = "granularity"; 
         private static final String PROPERTY_DISPERSION  = "dispersion"; 
          
         private int   granularity; 
         private float dispersion; 
          
         public Lightning() { 
                 super( "Lightning", "Andr¨¢s Belicza", "1.0", 
                            "A simple recursive algorithm to generate lightnings.\n" 
                          + "You can see this algorithm in action in the Qpac project:\n" 
                          + "http://code.google.com/p/qpac" ); 
                  
                 defaultProperties.setProperty( PROPERTY_GRANULARITY, "20"   ); 
                 defaultProperties.setProperty( PROPERTY_DISPERSION , "0.3f" ); 
         } 
          
         @Override 
         protected void validateProperties() throws IllegalArgumentException { 
                 super.validateProperties(); 
                  
                 try { 
                         granularity = Integer.parseInt  ( properties.getProperty( PROPERTY_GRANULARITY ) ); 
                         dispersion  = Float  .parseFloat( properties.getProperty( PROPERTY_DISPERSION  ) ); 
                 } 
                 catch ( final Exception e ) { 
                         throw new IllegalArgumentException( e.getMessage(), e ); 
                 } 
         } 
          
         @Override 
         public void paint( final Graphics graphics, final int width, final int height ) throws IllegalArgumentException { 
                 super.paint( graphics, width, height ); 
                  
                 paint( 0, 0, width - 1, height - 1 ); 
         } 
          
         public void paint( final int x1, final int y1, final int x2, final int y2 ) { 
                 final int internalX = x1 + random.nextInt( (int) ( ( x2 - x1 ) * dispersion ) + 1 ); 
                 final int internalY = y1 + random.nextInt( (int) ( ( y2 - y1 ) * dispersion ) + 1 ); 
                  
                 if ( x2 - x1 + y2 - y1 < granularity ) { 
                         graphics.drawLine( x1, y1, internalX, internalY ); 
                         graphics.drawLine( internalX, internalY, x2, y2 ); 
                 } 
                 else { 
                         paint( x1, y1, internalX, internalY ); 
                         paint( internalX, internalY, x2, y2 ); 
                 } 
         } 
          
 } 
 