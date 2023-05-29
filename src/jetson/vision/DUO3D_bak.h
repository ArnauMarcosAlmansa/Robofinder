#pragma once
#include <DUOLib.h>

// Some global variables
static DUOInstance _duo = NULL;
static PDUOFrame _pFrameData = NULL;


// One and only duo callback function
// It sets the current frame data and signals that the new frame data is ready
static void CALLBACK DUOCallback(const PDUOFrame pFrameData, void *pUserData)
{
	_pFrameData = pFrameData;
	SetEvent(_evFrame);
}

// Opens, sets current image format, fps and start capturing
static bool OpenDUOCamera(int width, int height, float fps)
{
	if(_duo != NULL)
	{
		// Stop capture
		StopDUO(_duo);
		// Close DUO
		CloseDUO(_duo);
		_duo = NULL;
	}

	// Find optimal binning parameters for given (width, height)
	// This maximizes sensor imaging area for given resolution
	int binning = DUO_BIN_NONE;
	if(width <= 752/2) 
		binning += DUO_BIN_HORIZONTAL2;
	else if(width <= 752/4) 
		binning += DUO_BIN_HORIZONTAL4;
	if(height <= 480/4) 
		binning += DUO_BIN_VERTICAL4;
	else if(height <= 480/2) 
		binning += DUO_BIN_VERTICAL2;

	// Check if we support given resolution (width, height, binning, fps)
	DUOResolutionInfo ri;
	if(!EnumerateDUOResolutions(&ri, 1, width, height, binning, fps))
		return false;

	if(!OpenDUO(&_duo))
		return false;

	char tmp[260];
	// Get and print some DUO parameter values
	GetDUODeviceName(_duo,tmp);
	printf("DUO Device Name:      '%s'\n", tmp);
	GetDUOSerialNumber(_duo, tmp);
	printf("DUO Serial Number:    %s\n", tmp);
	GetDUOFirmwareVersion(_duo, tmp);
	printf("DUO Firmware Version: v%s\n", tmp);
	GetDUOFirmwareBuild(_duo, tmp);
	printf("DUO Firmware Build:   %s\n", tmp);

	// Set selected resolution
	SetDUOResolutionInfo(_duo, ri);

	// Start capture
	if(!StartDUO(_duo, DUOCallback, NULL))
		return false;

	return true;
}

// Waits until the new DUO frame is ready and returns it
static PDUOFrame GetDUOFrame()
{
	if(_duo == NULL) 
		return NULL;

	if(WaitForSingleObject(_evFrame, 1000) == WAIT_OBJECT_0)
		return _pFrameData;
	else
		return NULL;
}

// Stops capture and closes the camera
static void CloseDUOCamera()
{
	if(_duo == NULL)
		return;

	// Stop capture
	StopDUO(_duo);
	// Close DUO
	CloseDUO(_duo);
	_duo = NULL;
}

static void SetExposure(float value)
{
	if(_duo == NULL)
		return;
	SetDUOExposure(_duo, value);
}

static void SetGain(float value)
{
	if(_duo == NULL)
		return;
	SetDUOGain(_duo, value);
}

static void SetLed(float value)
{
	if(_duo == NULL)
		return;
	SetDUOLedPWM(_duo, value);
}
