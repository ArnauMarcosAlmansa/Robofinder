#pragma once
#include <DUOLib.h>
	#include <stdio.h>
	#include <termios.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <pthread.h>
	#include <stdlib.h>
	#define WAIT_OBJECT_0	0
	struct event_flag
	{
		pthread_mutex_t mutex;
		pthread_cond_t  condition;
		unsigned int flag;
	};

	event_flag *CreateEvent(void *lpEventAttributes, bool bManualReset, bool bInitialState, char *name)
	{
		struct event_flag* ev = (struct event_flag*) malloc(sizeof(struct event_flag));
		pthread_mutex_init(&ev->mutex, NULL);
		pthread_cond_init(&ev->condition, NULL);
		ev->flag = 0;
		return ev;
	}

	void SetEvent(struct event_flag* ev)
	{
		pthread_mutex_lock(&ev->mutex);
		ev->flag = 1;
		pthread_cond_signal(&ev->condition);
		pthread_mutex_unlock(&ev->mutex);
	}

	int WaitForSingleObject(struct event_flag* ev, int timeout)
	{
		pthread_mutex_lock(&ev->mutex);
		while (!ev->flag)
			pthread_cond_wait(&ev->condition, &ev->mutex);
		ev->flag = 0;
		pthread_mutex_unlock(&ev->mutex);
		return WAIT_OBJECT_0;
	}

	static event_flag *_evFrame = CreateEvent(NULL, 0, 0, NULL);


typedef struct {
	unsigned short w, h;
	double left[12];
	double right[12];
} INTRINSICS;

// Some global variables
static DUOInstance _duo = nullptr;
static PDUOFrame _pFrameData = nullptr;

static void PrintIntrinsics(DUO_INTR* intrinsics)
{
	printf("### INTRINSECOS ###\n");
	printf("w = %hu, h = %hu\n", intrinsics->width, intrinsics->height);

	printf("LEFT CAMERA\n");

	printf("Distorsión radial:\n");
	printf("k1 = %f, k2 = %f, k3 = %f\n", intrinsics->left.k1, intrinsics->left.k2, intrinsics->left.k3);
	printf("k4 = %f, k5 = %f, k6 = %f\n", intrinsics->left.k4, intrinsics->left.k5, intrinsics->left.k6);

	printf("Distorsión tangencial:\n");
	printf("p1 = %f, p2 = %f\n", intrinsics->left.p1, intrinsics->left.p2);

	printf("Longitud focal:\n");
	printf("fx = %f, fy = %f\n", intrinsics->left.fx, intrinsics->left.fy);

	printf("Punto principal:\n");
	printf("cx = %f, cy = %f\n", intrinsics->left.cx, intrinsics->left.cy);

	printf("RIGHT CAMERA\n");

	printf("Distorsión radial:\n");
	printf("k1 = %f, k2 = %f, k3 = %f\n", intrinsics->right.k1, intrinsics->right.k2, intrinsics->right.k3);
	printf("k4 = %f, k5 = %f, k6 = %f\n", intrinsics->right.k4, intrinsics->right.k5, intrinsics->right.k6);

	printf("Distorsión tangencial:\n");
	printf("p1 = %f, p2 = %f\n", intrinsics->right.p1, intrinsics->right.p2);

	printf("Longitud focal:\n");
	printf("fx = %f, fy = %f\n", intrinsics->right.fx, intrinsics->right.fy);

	printf("Punto principal:\n");
	printf("cx = %f, cy = %f\n", intrinsics->right.cx, intrinsics->right.cy);
}

static void PrintExtrinsics(DUO_EXTR* extrinsics)
{
	printf("### EXTRINSECOS ###\n");

	printf("Rotación:\n");
	printf("r1 = %f, r2 = %f, r3 = %f\n", extrinsics->rotation[0], extrinsics->rotation[1], extrinsics->rotation[2]);
	printf("r4 = %f, r5 = %f, r5 = %f\n", extrinsics->rotation[3], extrinsics->rotation[4], extrinsics->rotation[5]);
	printf("r6 = %f, r7 = %f, r8 = %f\n", extrinsics->rotation[6], extrinsics->rotation[7], extrinsics->rotation[8]);

	printf("Translación:\n");
	printf("t1 = %f, t2 = %f, t3 = %f\n", extrinsics->translation[0], extrinsics->translation[1], extrinsics->translation[2]);
}


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
	if(_duo != nullptr)
	{
		// Stop capture
		StopDUO(_duo);
		// Close DUO
		CloseDUO(_duo);
		_duo = nullptr;
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
        SetDUOHFlip(_duo,1);
        SetDUOVFlip(_duo, 1);
        SetDUOUndistort(_duo, 1);

	DUO_INTR intrinsics;
	GetDUOIntrinsics(_duo, &intrinsics);
	PrintIntrinsics(&intrinsics);

	DUO_EXTR extrinsics;
	GetDUOExtrinsics(_duo, &extrinsics);
	PrintExtrinsics(&extrinsics);

	// Start capture
	if(!StartDUO(_duo, DUOCallback, nullptr))
		return false;

	printf("STARTED CAPTURE\n");

	return true;
}

// Waits until the new DUO frame is ready and returns it
static PDUOFrame GetDUOFrame()
{
	if(_duo == nullptr) 
		return nullptr;

	if(WaitForSingleObject(_evFrame, 1000) == WAIT_OBJECT_0)
		return _pFrameData;
	else
		return nullptr;
}

// Stops capture and closes the camera
static void CloseDUOCamera()
{
	if(_duo == nullptr)
		return;

	// Stop capture
	StopDUO(_duo);
	// Close DUO
	CloseDUO(_duo);
	_duo = nullptr;
}

static void SetExposure(float value)
{
	if(_duo == nullptr)
		return;
	SetDUOExposure(_duo, value);
}

static void SetGain(float value)
{
	if(_duo == nullptr)
		return;
	SetDUOGain(_duo, value);
}

static void SetLed(float value)
{
	if(_duo == nullptr)
		return;
	SetDUOLedPWM(_duo, value);
}

