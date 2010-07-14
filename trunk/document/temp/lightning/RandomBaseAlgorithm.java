import java.util.Random; 
  
 /** 
  *  
  * @author Belicza Andras 
  */ 
 public abstract class RandomBaseAlgorithm extends BaseAlgorithm { 
          
         protected static final String PROPERTY_RANDOM_SEED = "randomSeed"; 
          
         private   static final String ALWAYS_NEW_RANDOM_SEED_VALUE = "<ALWAYS_NEW>"; 
          
         protected final Random random = new Random(); 
          
         public RandomBaseAlgorithm( final String name, final String author, final String version, final String description ) { 
                 super( name, author, version, description ); 
                  
                 //defaultProperties.setProperty( PROPERTY_RANDOM_SEED, Long.toString( new Random().nextLong() ) ); 
                 defaultProperties.setProperty( PROPERTY_RANDOM_SEED, ALWAYS_NEW_RANDOM_SEED_VALUE ); 
         } 
          
         @Override 
         protected void validateProperties() throws IllegalArgumentException { 
                 try { 
                         final String randomSeedString = properties.getProperty( PROPERTY_RANDOM_SEED ); 
                          
                         // If random seed property is not set, it will be like we set a new random (we "continue" where we left) 
                         if ( !randomSeedString.equals( ALWAYS_NEW_RANDOM_SEED_VALUE ) ) 
                                 random.setSeed( Long.parseLong( randomSeedString ) ); 
                 } 
                 catch ( final Exception e ) { 
                         throw new IllegalArgumentException( e.getMessage(), e ); 
                 } 
         } 
          
 } 
 