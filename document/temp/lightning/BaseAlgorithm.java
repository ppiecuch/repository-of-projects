import java.awt.Graphics; 
 import java.util.Properties; 
  
 /** 
  * Abstract base for {@link Algorithm} subclasses.<br> 
  * Handles the properties of the algorithm. 
  *  
  * @author Belicza Andras 
  */ 
 public abstract class BaseAlgorithm implements Algorithm { 
          
         private final String name; 
         private final String author; 
         private final String version; 
         private final String description; 
          
         protected final Properties defaultProperties = new Properties(); 
         protected final Properties properties        = new Properties( defaultProperties ); 
         protected Graphics         graphics; 
          
         public BaseAlgorithm( final String name, final String author, final String version, final String description ) { 
                 this.name        = name; 
                 this.author      = author; 
                 this.version     = version; 
                 this.description = description; 
         } 
          
         @Override 
         public String getName() { 
                 return name == null ? getClass().getName() : name; 
         } 
          
         @Override 
         public String getAuthor() { 
                 return author; 
         } 
          
         @Override 
         public String getVersion() { 
                 return version; 
         } 
          
         @Override 
         public String getDescription() { 
                 return description; 
         } 
          
         @Override 
         public Properties getProperties() { 
                 return properties; 
         } 
          
         @Override 
         public void restoreDefaultProperties() { 
                 properties.clear(); 
         } 
          
         /** 
          * Validates and converts properties. 
          * @throws IllegalArgumentException thrown if illegal property value is specified 
          */ 
         protected abstract void validateProperties() throws IllegalArgumentException; 
          
         @Override 
         public void paint( final Graphics graphics, final int width, final int height ) throws IllegalArgumentException { 
                 validateProperties(); 
                  
                 this.graphics = graphics; 
         } 
          
         @Override 
         public String toString() { 
                 return name + ( version == null ? "" : " ver. " + version ) + ( author == null ? "" : " by " + author );  
         } 
          
 } 
 