///////////////////////////////////////////////////////////////////////////////////
// File		: main.cpp
// Purpose	: Main entry point
// Author	: Lee Winder
///////////////////////////////////////////////////////////////////////////////////

// Includes ///////////////////////////////////////////////////////////////////////
// OpenAL includes
#include <al.h>
#include <alc.h>
#include <alu.h>
#include <alut.h>

#include <stdio.h>

// Defines ////////////////////////////////////////////////////////////////////////
#define NULL			0
#define NUM_BUFFERS		2
#define NUM_SOURCES		1

// Externs ////////////////////////////////////////////////////////////////////////

// Globals ////////////////////////////////////////////////////////////////////////
ALuint buffers[NUM_BUFFERS];
ALuint source[NUM_SOURCES];

// Declarations ///////////////////////////////////////////////////////////////////
void	DisplayOpenALError(char* _string, ALenum error);	// Displays the error that has occured
int		LoadAndAssignWAV(char* _fileName, ALuint _buffer);	// Fill a buffer with sound

int		Init(void);											// Program init
int		Update(void);										// Main update
int		Shutdown(void);										// Program shutdown
int		main(void);											// Main entry

// Functions //////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------------
Function	: DisplayOpenALError
Purpose		: Prints out the error that we have encountered
Info		:
Author		: Lee Winder
*/
void DisplayOpenALError(char* _string, ALenum error)
{
	switch (error)
	{
	case AL_INVALID_NAME:
		printf("%s Invalid Name", _string);
		break;
	case AL_INVALID_ENUM:
		printf("%s Invalid Enum", _string);
		break;
	case AL_INVALID_VALUE:
		printf("%s Invalid Value", _string);
		break;
	case AL_INVALID_OPERATION:
		printf("%s Invalid Operation", _string);
		break;
	case AL_OUT_OF_MEMORY:
		printf("%s Out Of Memory", _string);
		break;
	default:
		printf("*** ERROR *** Unknown error case in DisplayOpenALError\n");
		break;
	};
}
/*---------------------------------------------------------------------------------
Function	: LoadAndAssignWAV
Purpose		: Fills up a buffer with a new WAV sound
Info		: Loads in the file, then fills up the buffer
Author		: Lee Winder
*/
int LoadAndAssignWAV(char* _fileName, ALuint _buffer)
{
	int			error;
	ALenum		format; 
	ALsizei		size;
	ALsizei		freq; 
	ALboolean	loop;
	ALvoid*		data;

	// Load in the WAV file from disk
	alutLoadWAVFile(_fileName, &format, &data, &size, &freq, &loop); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		DisplayOpenALError("alutLoadWAVFile : ", error);	
		return 0; 
	}

	// Copy the new WAV data into the buffer
	alBufferData(_buffer,format,data,size,freq); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		DisplayOpenALError("alBufferData :", error); 
		return 0; 
	}

	// Unload the WAV file
	alutUnloadWAV(format,data,size,freq); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		DisplayOpenALError("alutUnloadWAV :", error);
		return 0;
	}

	return 1;
}
/*---------------------------------------------------------------------------------
Function	: Init
Purpose		: Initilises the program
Info		:
Author		: Lee Winder
*/
int Init(void)
{
	int			error;

	ALfloat		listenerPos[]={0.0,0.0,0.0};				// At the origin
	ALfloat		listenerVel[]={0.0,0.0,0.0};				// The velocity (no doppler here)
	ALfloat		listenerOri[]={0.0,0.0,-1.0, 0.0,1.0,0.0};	// LookAt then Up

	// Init openAL
	alutInit(0, NULL);	
	// Clear Error Code
	alGetError(); 

	// Create the buffers
	alGenBuffers(NUM_BUFFERS, buffers);
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		DisplayOpenALError("alutUnloadWAV :", error);
		return 0;
	}

	// Load in the WAV and store it in a buffer
	if (!LoadAndAssignWAV("test1.wav", buffers[0]))
	{
		// Error loading in the WAV so quit
		alDeleteBuffers(NUM_BUFFERS, buffers); 
		return 0;
	}
	// Load in the WAV and store it in a buffer
	if (!LoadAndAssignWAV("test2.wav", buffers[1]))
	{
		// Error loading in the WAV so quit
		alDeleteBuffers(NUM_BUFFERS, buffers); 
		return 0;
	}

	// Generate Sources 
	alGenSources(NUM_SOURCES, source); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		DisplayOpenALError("alutUnloadWAV :", error); 
		return 0; 
	}

	// Attach buffer 0 to our source
	alSourcei(source[0], AL_BUFFER, buffers[0]); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		DisplayOpenALError("alSourcei :", error); 
		return 0; 
	}

	// Set Listener attributes
	// Position ... 
	alListenerfv(AL_POSITION,listenerPos); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		DisplayOpenALError("alListenerfv :", error); 
		return 0;
	}
	// Velocity ... 
	alListenerfv(AL_VELOCITY,listenerVel); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		DisplayOpenALError("alListenerfv :", error); 
		return 0;
	}
	// Orientation ... 
	alListenerfv(AL_ORIENTATION,listenerOri); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		DisplayOpenALError("alListenerfv :", error); 
		return 0;
	}

	// Alll done...
	return 1;
}

/*---------------------------------------------------------------------------------
Function	: Update
Purpose		: Main update loop
Info		:
Author		: Lee Winder
*/
int Update(void)
{
	int		sourceState;
	ALfloat sourcePos[] = {0.0f, 0.0f, 0.0f};

	// Set the position of our source
	alSourcefv(source[0], AL_POSITION, sourcePos);
	// Play it
	alSourcePlay(source[0]);
	printf("Source played dead centre\n");

	// Wait till it has finished
	alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);
	while(sourceState == AL_PLAYING)
		alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);

	// Set position of our source
	sourcePos[0] = 5.0f;
	alSourcefv(source[0], AL_POSITION, sourcePos);
	// Play it
	alSourcePlay(source[0]);
	printf("Source played to the right\n");

	// Wait till it has finished
	alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);
	while(sourceState == AL_PLAYING)
		alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);

	// Set position of our source
	sourcePos[0] = -5.0f;
	alSourcefv(source[0], AL_POSITION, sourcePos);
	// Play it
	alSourcePlay(source[0]);
	printf("Source played to the left\n");

	// Wait till it has finished
	alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);
	while(sourceState == AL_PLAYING)
		alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);

	// Set position of our source
	sourcePos[0] = -10.0f;
	alSourcefv(source[0], AL_POSITION, sourcePos);
	// Play it
	alSourcePlay(source[0]);
	printf("Tracking source\n");

	// Wait till it has finished (As im not taking into account frame time, this will be different on your PC!)
	alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);
	while(sourceState == AL_PLAYING)
	{
		// Move the position
		sourcePos[0] += 0.0005f;
		alSourcefv(source[0], AL_POSITION, sourcePos);
		// Check the state
		alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);
	}

	// Assign a new buffer
	alSourcei(source[0], AL_BUFFER, buffers[1]); 
	// Play it
	alSourcePlay(source[0]);
	printf("Stereo sample.  Non-Positional\n");

	// Wait till it has finished (As im not taking into account frame time, this will be different on your PC!)
	//alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);
	//while(sourceState == AL_PLAYING)
	//	alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);

	// Finished
	return 1;
}

/*---------------------------------------------------------------------------------
Function	: Shutdown
Purpose		: Shutdowns the program
Info		:
Author		: Lee Winder
*/
int Shutdown(void)
{
	// Delete the sources and the buffers
	alDeleteSources(NUM_SOURCES, source);
	alDeleteBuffers(NUM_BUFFERS, buffers); 
	// Exit OpenAL
	alutExit();

	return 1;
}

/*---------------------------------------------------------------------------------
Function	: main
Purpose		: Main entry point for the program
Info		:
Author		: Lee Winder
*/
int main(void)
{
	Init();
	Update();
	Shutdown();

	getchar();

	return 0;
}