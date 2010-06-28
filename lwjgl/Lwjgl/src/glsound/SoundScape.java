package glsound;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.nio.*;
import org.lwjgl.openal.*;

/**
 * SoundScape wraps OpenAL functions to create a sound environment.
 * Can load and play several sounds, set properties of sounds (pitch,
 * position, volume, dropoff), pause all sounds or one sound, control
 * global volume.
 *
 * To run a demo of SoundScape just call the static demo() function:
 * <PRE>
 *     SoundScape.demo();
 * </PRE>
 *
 * The class is static so can be used without instantiation, similar to
 * LWJGL Display, Mouse and Keyboard classes:
 * <PRE>
 *     SoundScape.create();      // init sound environment
 *     ...
 *     int soundData sd = SoundScape.loadSoundData("a_noise.wav");
 *     int soundSource ss = SoundScape.makeSoundSource(soundData);
 *     SoundScape.setSoundPosition(ss, 10, 2, 0);
 *     SoundScape.play(ss);
 *     ...
 *     SoundScape.destroy();
 * </PRE>
 *
 * Requires the WaveData class to load wav files.
 * <P>
 * Borrows heavily from Brian Matzon's PlayTest demo (at http://lwjgl.org)
 * <P>
 */
public class SoundScape {
    static final int NUM_SOUNDS = 100;
    static int[] soundDataBuffers = new int[NUM_SOUNDS];
    static int[] soundSources = new int[NUM_SOUNDS];
    static int soundDataBufferC = 0;
    static int soundSourceC = 0;
    static boolean ALcreated = false;
    static boolean haveVorbis = false;
    static float globalGain = 1f;
    static float referenceDistance = 5f;
    static int lastError;

    /**
     * Creates an instance of SoundScape BUT SoundScape is meant to be used
     * as static class.  Use SoundScape.create() to init the class.
     */
    public SoundScape() {
        create();  // static singleton class?
    }

    /**
     * Prepare the SoundScape class for use
     */
    public static void create() {
    	if (!ALcreated) {
    		try {
    			AL.create();
    		}
    		catch (Exception e) {
    			System.out.println("Unable to create OpenAL.\nPlease make sure that OpenAL is available on this system. Exception: " + e);
    			return;
    		}
    		
    		// Set these here?
            //AL10.alDistanceModel(AL10.AL_INVERSE_DISTANCE);
            //AL10.alListenerf(AL10.AL_ROLLOFF_FACTOR, .001f);
            //AL10.alListenerf(AL10.AL_REFERENCE_DISTANCE, 100f);

    		// Check if VORBIS sound format is supported
    		System.out.print("SoundScape.create(): Checking for Vorbis extension...");
    		if (AL10.alIsExtensionPresent("AL_EXT_vorbis")) {
    			System.out.println("Vorbis extension found.");
    			haveVorbis = true;
    		}
    		else {
    			System.out.println("Vorbis extension not supported.");
    		}
    		
    		ALcreated = true;
    	}
    }
    
    /**
     * Free up allocated sounds and shutdown audio environment.
     */
    public static void destroy()
    {
        // stop all sounds
        for (int i=0; i < soundSourceC; i++) {
            stop(soundSources[i]);
            deleteSoundSource(soundSources[i]);
        }

        // delete buffers and sources
        for (int i=0; i < soundDataBufferC; i++) {
            deleteSoundData(soundDataBuffers[i]);
        }

        // reset counters and flags
        soundSourceC = soundDataBufferC = 0;
        ALcreated = false;

        // shutdown
        alExit();
    }

    static protected void alExit() {
    	if(AL.isCreated()) {
    		AL.destroy();
    	}
    }
    
    public static void exit(int arg) {
    	System.out.println("SoundScape: got error # " + arg);
    	//System.exit(arg);
    }

    /**
     * Run a bare-bones demo of SoundScape.
     */
    public static void demo() {
        int soundDataA, soundDataB;   // handles to sound data buffers
        int source1, source2;         // handles to sound sources

        // start up OpenAL env
        create();

        // Load sound data into buffers
        soundDataA = loadSoundData("sounds/cardinal3.wav");
        soundDataB = loadSoundData("sounds/bomb.wav");

        // Create two sound sources bound to the sound data
        source1 = makeSoundSource(soundDataA);
        source2 = makeSoundSource(soundDataB);

        // Tweak the pitch and volume of each sound
        setGain(source2, .3f);
        setPitch(source2, .5f);
        setPitch(source1, 1.2f);

        // Play the sound sources (looping)
        setLoop(source1,true);
        setLoop(source2,true);
        play(source1);
        play(source2);

        // clean up: call destroy when done
        //destroy();
    }

    //========================================================================
    // functions that control entire sound environment
    //========================================================================

    /**
     * Pause or resume all sounds playing in the SoundScape
     */
    public static void pause(boolean bool) {
        for (int i=0; i < soundSourceC; i++) {
            pause(soundSources[i], bool);
        }
    }

    /**
     * Stop all sounds playing in the SoundScape
     */
    public static void stop() {
        for (int i=0; i < soundSourceC; i++) {
            stop(soundSources[i]);
        }
    }

    /**
     * Set the volume for all sounds.  Use this to adjust ambient volume
     * level for entire sound scape.
     * @param gain  float: 0 - 1
     */
    public static void setGlobalGain(float gain) {
        if (gain != globalGain) {
            if (gain > 1) {
                gain = 1;
            }
            else if (gain < 0) {
                gain = 0;
            }
            globalGain = gain;
            for (int i = 0; i < soundSourceC; i++) {
                setGain(soundSources[i], getGain(soundSources[i]));
            }
        }
    }

    /**
     * Set the reference distance, used by OpenAL to calculate how fast sound
     * drops off.  This distance should be proportional to the size of your
     * scene, ie. a scene that is hundreds of units wide might have a
     * reference distance of 10 for a "small" sound, or 100 for a "big" sound
     * that fills the whole space.
     * <P> 
     * @see makeSoundSource().
     */
    public static void setReferenceDistance(float d) {
        referenceDistance = d;
    }

    /**
     *  Set world coordinates of listener.
     */
    public static void setListenerPosition(float x, float y, float z) {
        AL10.alListener3f(AL10.AL_POSITION, x,y,z);
    }

    /**
     *  Set world coordinates of listener.
     */
    public static void setListenerVelocity(float x, float y, float z) {
        AL10.alListener3f(AL10.AL_VELOCITY, x,y,z);
    }

    /**
     *  Set orientation of the listener.
     *  first 3 elements are "at", second 3 are "up"
     */
    public static void setListenerOrientation(float lookatx, float lookaty, float lookatz, float upx, float upy, float upz) {
        float[] fvals = {lookatx, lookaty, lookatz, upx, upy, upz};
        AL10.alListener(AL10.AL_ORIENTATION, allocFloats(fvals));
    }

    //========================================================================
    // functions to control a sound source
    //========================================================================

    /**
     * Pause or resume one sound
     */
    public static void pause(int soundSourceHandle, boolean pause) {
        if (pause) {
            AL10.alSourcePause(soundSourceHandle);
        }
        else {
            AL10.alSourcePlay(soundSourceHandle);
        }
        if((lastError = AL10.alGetError()) != AL10.AL_NO_ERROR) {
            exit(lastError);
        }
    }

    /**
     * Play a sound source
     */
    public static void play(int soundSourceHandle) {
        AL10.alSourcePlay(soundSourceHandle);
        if((lastError = AL10.alGetError()) != AL10.AL_NO_ERROR) {
            exit(lastError);
        }
    }

    /**
     * Stop playing one sound.
     */
    public static void stop(int soundSourceHandle) {
        AL10.alSourceStop(soundSourceHandle);
        if((lastError = AL10.alGetError()) != AL10.AL_NO_ERROR) {
            exit(lastError);
        }
    }

    public static float getGain(int soundSourceHandle) {
        return AL10.alGetSourcef(soundSourceHandle, AL10.AL_GAIN);
    }

    /**
     * Set velocity of sound 0 - 1
     */
    public static void setVelocity(int soundSourceHandle, float velocity) {
        AL10.alSourcef(soundSourceHandle, AL10.AL_VELOCITY, velocity);
    }

    /**
     * @param soundSourceHandle  set pitch of this sound source
     * @param pitch
     */
    public static void setPitch(int soundSourceHandle, float pitch) {
        AL10.alSourcef(soundSourceHandle, AL10.AL_PITCH, pitch);
    }

    /**
     * Set the soundsource volume.  Gain range is 0.0 or above.
     * A value of 1.0 means unattenuated/unchanged.
     * Each division by 2 equals an attenuation of -6dB.
     * Each multiplication by 2 equals an amplification of +6dB.
     * A value of 0.0 turns sound off.
     * @param soundSourceHandle
     * @param gain
     */
    public static void setGain(int soundSourceHandle, float gain) {
        AL10.alSourcef(soundSourceHandle, AL10.AL_GAIN, gain*globalGain);
    }

    public static void setSoundPosition(int soundSourceHandle, float x, float y, float z) {
        AL10.alSource3f(soundSourceHandle, AL10.AL_POSITION, x, y, z);
    }

    public static void setSoundPosition(int soundSourceHandle, float[] xyz) {
        AL10.alSource(soundSourceHandle, AL10.AL_POSITION, allocFloats(xyz));
    }

    /**
     * set the dropoff distance for this sound.  this number controls how far the 
     * sound travels.  The distance parameter should be proportional to the units of
     * distance used in the scene.
     * <P>
     * @param soundSourceHandle
     * @param distance
     */
    public static void setReferenceDistance(int soundSourceHandle, float distance) {
        AL10.alSourcef(soundSourceHandle, AL10.AL_REFERENCE_DISTANCE, distance);
    }

    public static void setLoop(int soundSourceHandle, boolean on) {
        AL10.alSourcei(soundSourceHandle, AL10.AL_LOOPING, (on? AL10.AL_TRUE : AL10.AL_FALSE));
        if((lastError = AL10.alGetError()) != AL10.AL_NO_ERROR) {
            exit(lastError);
        }
    }

    /**
     * return true if this sound is playing
     * @param soundSourceHandle 
     * @return true if sound is playing
     */
    public static boolean isPlaying(int soundSourceHandle) {
        IntBuffer ib = allocInts(1);
        AL10.alGetInteger(AL10.AL_PLAYING, ib);
        return (ib.get(0) == AL10.AL_TRUE);
    }

    //========================================================================
    // functions to load sound samples and create sources
    //========================================================================

    /**
     * Reads the file into a ByteBuffer
     *
     * @param filename Name of file to load
     * @return ByteBuffer containing file data
     */
    static protected ByteBuffer getData(String filename) {
      ByteBuffer buffer = null;

      System.out.println("Attempting to load: " + filename);

      try {
        BufferedInputStream bis = new BufferedInputStream(WaveData.class.getClassLoader().getResourceAsStream(filename));
        ByteArrayOutputStream baos = new ByteArrayOutputStream();

        int bufferLength = 4096;
        byte[] readBuffer = new byte[bufferLength];
        int read = -1;

        while((read = bis.read(readBuffer, 0, bufferLength)) != -1) {
          baos.write(readBuffer, 0, read);
        }

        //done reading, close
        bis.close();

        // if ogg vorbis data, we need to pass it unmodified to alBufferData
        buffer = ByteBuffer.allocateDirect(baos.size());
        buffer.order(ByteOrder.nativeOrder());
        buffer.put(baos.toByteArray());
        buffer.rewind();
      } catch (Exception ioe) {
        ioe.printStackTrace();
      }
      return buffer;
    }

    /**
     * Return integer handle to allocated sound data buffer.
     * @return
     */
    static int allocateSoundData() {
        // check that create() was run first
        if (ALcreated == false) {
            System.out.println("SoundScape.allocateSoundData() error: OpenAL has not been initialized -- call SoundScape.create() first!");
        }
        IntBuffer soundDataHandle = allocInts(1);
        // al generate buffers and sources
        AL10.alGenBuffers(soundDataHandle);
        if((lastError = AL10.alGetError()) != AL10.AL_NO_ERROR) {
            exit(lastError);
        }
        // hold onto sound Data handle
        if (soundDataBufferC < NUM_SOUNDS) {
            soundDataBuffers[soundDataBufferC++] = soundDataHandle.get(0);
        }
        return soundDataHandle.get(0);
    }


    /**
     * Return integer handle to new sound source.  Expects a handle to a
     * loaded sound data buffer (see loadSoundData()).  Multiple sound sources
     * can share the same data buffer.  Each sound source can have a position,
     * pitch, velocity that will affect the way the sound plays.
     * @return
     */
    public static int makeSoundSource(int soundDataHandle) {
        IntBuffer soundSourceHandle = allocInts(1);
        // al generate buffers and sources
        AL10.alGenSources(soundSourceHandle);
        if((lastError = AL10.alGetError()) != AL10.AL_NO_ERROR) {
            exit(lastError);
        }
        // bind the sound source to a sound data buffer
        AL10.alSourcei(soundSourceHandle.get(0), AL10.AL_BUFFER, soundDataHandle);
        // set the distance for sound droppoff calculations (a typical distance at which we should hear this sound)
        AL10.alSourcef(soundSourceHandle.get(0), AL10.AL_REFERENCE_DISTANCE, referenceDistance);
        // set volume to current environment volume
        AL10.alSourcef(soundSourceHandle.get(0), AL10.AL_GAIN, globalGain);
        // hold onto the new sound source
        if (soundSourceC < NUM_SOUNDS) {
            soundSources[soundSourceC++] = soundSourceHandle.get(0);
        }
        return soundSourceHandle.get(0);
    }

    /**
     * Delete the sound data buffer with the given handle
     * @param whichSoundHandle
     */
    static void deleteSoundData(int soundDataHandle)
    {
        IntBuffer ib = allocInts(1);
        ib.put(soundDataHandle);
        ib.position(0).limit(1);
        AL10.alDeleteBuffers(ib);
        if((lastError = AL10.alGetError()) != AL10.AL_NO_ERROR) {
            exit(lastError);
        }
    }

    /**
     * Delete the sound source with the given handle
     * @param whichSoundHandle
     */
    static void deleteSoundSource(int soundSourceHandle)
    {
        IntBuffer ib = allocInts(1);
        ib.put(soundSourceHandle);
        ib.position(0).limit(1);
        AL10.alDeleteSources(ib);
        if((lastError = AL10.alGetError()) != AL10.AL_NO_ERROR) {
            exit(lastError);
        }
    }

    /**
     * Load sound data from a file into a buffer.  Returns a handle that is
     * passed to makeSoundSource() to create a sound in the environment.
     * Multiple sound sources can be bound to one data buffer.
     * <P>
     * @param soundFilename  name of file to load (.wav)
     * @return numeric handle to the sound data buffer
     */
    public static int loadSoundData(String soundFilename) //, int soundDataBufferHandle)
    {
        int soundDataBufferHandle = allocateSoundData();
        if(soundFilename.endsWith(".ogg")) {
            if (haveVorbis) {
                ByteBuffer filebuffer = getData(soundFilename);
                AL10.alBufferData(soundDataBufferHandle, AL10.AL_FORMAT_VORBIS_EXT, filebuffer, -1);
                filebuffer.clear();
            }
        } else {
          // load wave data into buffer
          WaveData wavefile = WaveData.create(soundFilename);
          AL10.alBufferData(soundDataBufferHandle, wavefile.format, wavefile.data, wavefile.samplerate);
          wavefile.dispose();
        }
        if((lastError = AL10.alGetError()) != AL10.AL_NO_ERROR) {
            exit(lastError);
        }
        return soundDataBufferHandle;
    }

    /**
     * 
     * <P>
     * @param soundFilename  name of file to load (.wav)
     * @return numeric handle to the sound data buffer
     */
    public static int makeSound(String soundFilename) 
    {
        int soundData;   // handle to sound data buffers
        int source;      // handle to sound sources

        // start up OpenAL env
        if (!ALcreated) create();

        // Load sound data into buffer
        soundData = loadSoundData(soundFilename);

        // Create sound source bound to the sound data
        source = makeSoundSource(soundData);
        
        // place it at origin
        setSoundPosition(source,0,0,0);

        // return the sound (play it with SoundScape.play(soundHandle);)
        return source;
    }

    //=========================================================================
    // These are taken verbatim from GLApp
    //=========================================================================
    public static final int SIZE_FLOAT = 4;
    public static final int SIZE_INT = 4;

    public static IntBuffer allocInts(int howmany) {
        return ByteBuffer.allocateDirect(howmany * SIZE_INT).order(ByteOrder.nativeOrder()).asIntBuffer();
    }

    public static FloatBuffer allocFloats(float[] floatarray) {
        FloatBuffer fb = ByteBuffer.allocateDirect(floatarray.length * SIZE_FLOAT).order(ByteOrder.nativeOrder()).asFloatBuffer();
        fb.put(floatarray).flip();
        return fb;
    }
}
