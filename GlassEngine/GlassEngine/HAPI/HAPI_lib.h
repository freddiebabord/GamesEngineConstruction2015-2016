/*
HAPI_lib.h
All the includes required to use the Hardware API (HAPI).

Your application must link with one of the HAPI static libraries:

For PC Builds:
HAPI.lib		- 32 bit release version
HAPI_debug.lib	- 32 bit debug version

HAPI64.lib			- 64 bit release version
HAPI_debug64.lib	- 64 bit debug version

There are also a number of DLLs required for the PC version:

FreeImage.dll - image loading library
SDL2.dll - SDL graphics library
SDL2_mixer.dll - SDL sound library
smpeg2.dll - for music streaming of mpeg files

For other platforms please see the ReadMe.txt document

For full details on using the library please see the online documentation (CTRL-Click to open):
https://scm-intranet.tees.ac.uk/users/u0018197/HAPI/HAPI-Reference/hapi-reference.html

Last modified by Keith Ditchburn: 2nd November 2015

Version 1.56 23/10/15
- Rebuilt with latest SDL 2 build (still beta!) due to official release breaking the rumble
- Changes so HAPI now supports Steam overlay (when launched from Steam)
- Also pressing XBox logo button on controller brings up XBox panel
- Changed internal performance query to use language agnostic lookup
(Basically this could mean HAPI might fail on a non-English OS)

Version 1.55 21/10/15
- Fixed a bug causing a crash when HAPI->Close was called
- Improved error checking for internal performance counters

Version 1.54 06/10/15
- Accidently left some debug code enabled in 1.53

Version 1.53 05/10/15
- Changed the GetFont call to allow the specifying of antialiasing and turned it off by default
- Added a function GetFontDirect to load a font from a filename rather than using one installed in the OS
- Font is now released correctly once the texture atlas is created

Version 1.52 17/09/15
- Minor modifications to text rendering

Version 1.51 11/08/15
- Added a function: SaveImage
- Removed some debug output text no longer needed

Version 1.50 Beta 30/07/15
- Moved to use SDL, completely replacing DirectX
- Added SaveImage function to allow screenshots to be saved etc.
- For further changes see the readme

Older version changes are listed in the ReadMe.txt file
*/
#pragma once

// String header needed to use std::string type, required by HAPI
#include <string>

// Key and controller codes header
#include "HAPI_InputCodes.h"

// an alias for a type used to hold a byte (1 byte or 8 bits) of data
using BYTE = unsigned char;

// an alias for a type used to hold a WORD (2 bytes or 16 bits) of data
using WORD = unsigned short;

// an alias for a type used to hold two WORDS (4 bytes or 32 bits) of data
using DWORD = unsigned long;

// HAPI Renderer subsystem types
enum HAPI_Renderer
{
	H_RDIRECT3D=0,		// Microsoft Direct3D
	H_RGAPI,			// Microsoft PocketPC GAPI library
	H_RSDL,				// SDL Library
	H_RDEFAULT,			// Use the default system for the platform
	H_NUM_HAPI_RENDERERS
};

// HAPI Sound subsystem types
enum HAPI_Sound
{
	H_SDIRECTSOUND=0, // Microsoft DirectSound
	H_SWINAPI,		  // Microsoft Windows API
	H_SSDL,			  // SDL Library
	H_SSDL_MIXER,	  // SDL Mixer (now the default for PC)
	H_SDEFAULT,		  // Use the default system for the platform
	H_NUM_HAPI_SOUNDS
};

// HAPI Input subsystem types
enum HAPI_Input
{
	H_IOS=0,		// Input handled by OS
	H_IGAPI,		// Microsoft PocketPC GAPI library
	H_IGP2X,		// GP2X input under Linux
	H_IDEFAULT,  	// Use the default system for the platform
	H_NUM_HAPI_INPUTS
};

// HAPI Error codes
enum HAPI_ErrorCodes
{
	H_OK=0,
	H_INITIALISATION_ERROR,
	H_INVALID_PARAMETER,
	H_OUT_OF_MEMORY,
	H_FILE_NOT_FOUND,
	H_INTERNAL_ERROR,
	H_UNSUPPORTED,
	H_DEVICE_NOT_CONNECTED,
	H_NUM_HAPI_ERROR_CODES
};

// HAPI systems
enum HAPI_System
{
	H_CORE_SYSTEM=0,
	H_OS_SYSTEM,
	H_RENDERER_SYSTEM,
	H_SOUND_SYSTEM,
	H_INPUT_SYSTEM,
	H_INTERNAL_SYSTEM,
	H_NUM_HAPI_SYSTEMS
};

// Message box button types for use in the HAPI_MessageBox call
enum HAPI_ButtonType
{
	eButtonTypeOk,				// Just an OK button
	eButtonTypeOkCancel,		// OK and CANCEL buttons
	eButtonTypeYesNo,			// YES and NO buttons
	eButtonTypeYesNoCancel		// YES, NO and CANCEL buttons
};

// Message box return types from the HAPI_UserMessage call
enum HAPI_UserResponse
{
	eUserResponseUnknown=-1,
	eUserResponseOk,		// OK button clicked by user
	eUserResponseCancel,	// CANCEL button clicked
	eUserResponseYes,		// YES button clicked
	eUserResponseNo			// NO button clicked
};

/**
* \struct HAPI_TMouseData
* \brief Contains information about the current mouse state
*/
struct HAPI_TMouseData
{
	int x{ 0 };
	int y{ 0 };
	int wheelMovement{ 0 };
	bool leftButtonDown{ false };
	bool rightButtonDown{ false };
	bool middleButtonDown{ false };
	bool xButton1Down{ false };
	bool xButton2Down{ false };
};

/**
* \struct HAPI_TKeyboardData
* \brief Contains information about the current keyboard state
* Note: you can use the defines in HAPI_InputCodes.h as indices into the array
*/
struct HAPI_TKeyboardData
{
	bool scanCode[256];
};

// The maximum number of buttons a controller may have
// Note: use this define rather than hard coding the number as it may change in the future
#define HAPI_MAX_BUTTON_ARRAY_SIZE	16

/**
* \struct HAPI_TControllerData
* \brief Contains information about a controller state
* Note that not all controllers have HAPI_MAX_BUTTON_ARRAY_SIZE of each type of button
* Note: you can use the defines in HAPI_InputCodes.h as indices into the arrays
*/
struct HAPI_TControllerData
{
	bool digitalButtons[HAPI_MAX_BUTTON_ARRAY_SIZE];
	int analogueButtons[HAPI_MAX_BUTTON_ARRAY_SIZE];	// Value range is -32768 to +32767
};

/**
* \struct HAPI_TColour
* \brief Used by HAPI to represent a colour. Values range from 0 to 255.
*/
struct HAPI_TColour
{
	// public data
	BYTE blue{ 255 };
	BYTE green{ 255 };
	BYTE red{ 255 };
	BYTE alpha{ 255 };

	// Constructor 1 - Colour defaults to white	
	HAPI_TColour() = default;

	// Constructor 2 - Colours passed in, alpha defaults to 255 (opaque) if not provided
	HAPI_TColour(BYTE r,BYTE g,BYTE b,BYTE a=255): blue(b),green(g),red(r),alpha(a){}

	// Constructor 3 - grey
	HAPI_TColour(BYTE gray,BYTE a=255) : blue(gray), green(gray), red(gray), alpha(a){}
};

// Flag values that can be used during initialise
#define HAPI_INIT_DEFAULTS		  0x00000000
#define HAPI_INIT_FULLSCREEN      0x00000001
#define HAPI_INIT_PERFOVERLAY     0x00000002
#define HAPI_INIT_RESERVED2		  0x00000004

// Public HAPI interface
class IHapi
{
public:
	// Initialise HAPI, pass in the required screen size and any flags then
	// specify what types of renderer,sound and input subsystems to use (or leave blank for defaults)
	// Note: width and height values may be altered to a legal size by HAPI
	virtual bool Initialise(int *width,int *height,DWORD flags=HAPI_INIT_DEFAULTS,
		HAPI_Renderer rendererType = H_RDEFAULT,
		HAPI_Sound soundType=H_SDEFAULT,
		HAPI_Input inputType=H_IDEFAULT)=0;

	// Causes HAPI to close the window and return false in the next update
	virtual bool Close()=0;

	// Tells HAPI to display (or not) the frames per second at the optional position
	virtual bool SetShowFPS(bool set,int x=0,int y=0)=0;

	// Tells HAPI to display or not the cursor (mouse pointer)
	virtual bool SetShowCursor(bool set)=0;

	// Update HAPI, must be called each game loop. Returns false when user closes the window or an error occurs
	virtual bool Update()=0;

	// Returns a pointer to the screen data. There are 4 bytes per pixel, in XRGB byte order
	// On error returns nullptr, use HAPI_GetLastError to determine reason or look in output pane
	virtual BYTE* GetScreenPointer()=0;

	// Fills the passed structure with the current mouse state
	virtual bool GetMouseData(HAPI_TMouseData* data) const=0;

	// Fills the passed structure with the current keyboard state
	virtual bool GetKeyboardData(HAPI_TKeyboardData *data)=0;

	// Returns the maximum number of controller devices that could be installed
	virtual int GetMaxControllers() const =0;

	// Fills the passed structure with the specified controller state
	virtual bool GetControllerData(int controller, HAPI_TControllerData *data)=0;

	// Sets the rumble motor speeds for the specified controller
	virtual bool SetControllerRumble(int controller, WORD leftMotor,WORD rightMotor) const=0;

	// Playback a sound. Use the id retrieved from LoadSoundFile. Volume is 0-1000
	virtual bool PlayASound(int id,bool loop=false,int volume=1000,int pan=500)=0;

	// Stop a sound early. Use the id retrieved from LoadSoundFile
	virtual bool StopSound(int id)=0;

	// Returns the time since HAPI was initialised in milliseconds
	virtual DWORD GetTime()=0;

	// Stops streaming media started via PlayStreamedMedia
	virtual void StopStreamedMedia()=0;

	// If a HAPI call fails this can be used to retrieve the error code and system the error occurred in (optional)
	// (see HAPI_ErrorCodes enum above and online documentation for more details on errors)
	virtual HAPI_ErrorCodes GetLastError(HAPI_System *system=nullptr) const=0;

#if !defined(_UNICODE)
	// Plays streamed media. PC supported formats are .avi, .asf, .mpeg, .mp3 and .wav. If video you can choose to play
	// it in the main window or in a separate pop up window via the useNewWindow flag
	// V1.5: added volume
	virtual bool PlayStreamedMedia(const std::string &filename,int volume=1000,bool useNewWindow=false)=0;

	// Allows the font, its height and intensity to be changed. Height is in pnts. Intensity is 0 to 1000 with 500 default
	// Note: if antiAliased is false then intensity has no effect
	virtual bool ChangeFont(const std::string &fontName, int height, int intensity = 500, bool antiAliased=false) = 0;

	// As above but from a file rather than from a windows registered font. Added in 1.53
	// Note: if antiAliased is false then intensity has no effect
	virtual bool ChangeFontDirect(const std::string &fontFilename, int height, int intensity = 500, bool antiAliased = false) = 0;

	// Allows debug text to be displayed in the output pane of Visual Studio
	virtual void DebugText(const std::string &str)=0;

	// Allows text to be drawn on the screen at a position and with a specified colour
	// The text is drawn with a 12 point Arial font
	virtual bool RenderText(int x,int y,const HAPI_TColour &colour,const std::string& text)=0;

	// Loads a texture, supported most PC formats.
	// There are 4 bytes per texel in ARGB order
	// Caller is responsible for deleting the allocated data (use delete[])
	virtual bool LoadTexture(const std::string &filename, BYTE** data,int *width=0,int *height=0)=0;

	// Saves the data in rawImageData to a file (currently a png). 
	// Width and height must be correct and assumes 4 bytes per pixel.
	// If no path information is in the filename it is saved to the workign directory
	virtual bool SaveImage(BYTE *rawImageData, int width, int height, const std::string &filename) = 0;

	// Load a sound file, supported formats are: .wav
	virtual bool LoadSound(const std::string &filename, int *id)=0;

	// Display a message box to the user
	virtual bool UserMessage(const std::string &text,const std::string& title,HAPI_ButtonType buttonType=eButtonTypeOk,HAPI_UserResponse *userResponse=0)=0;
#else
	// Unicode versions of above using std::wstring (wide character string)
	// Note: if using UNICODE you also need to link with the UNICODE version of the HAPI lib file
	virtual void DebugText(const std::wstring &str)=0;
	virtual bool RenderText(int x,int y,const HAPI_TColour &colour,const std::wstring& text)=0;
	virtual bool LoadTexture(const std::wstring &filename, BYTE** data,int *width=0,int *height=0)=0;
	virtual bool LoadSound(const std::wstring &filename, int *id)=0;
	virtual bool PlayStreamedMedia(const std::wstring &filename,int volume=1000,bool useNewWindow=false)=0;
	virtual bool UserMessage(const std::wstring &text,const std::wstring& title,HAPI_ButtonType buttonType=eButtonTypeOk,HAPI_UserResponse *userResponse=0)=0;
#endif

};

// Returns the HAPI instance pointer
IHapi* GetHAPI();

// Shortcut to above
#define HAPI GetHAPI()
