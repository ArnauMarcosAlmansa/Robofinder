///////////////////////////////////////////////////////////////////////////////////
// This code sample demonstrates the use of DUO SDK in your own applications
// For updates and file downloads go to: http://duo3d.com/
// Copyright 2014-2016 (c) Code Laboratories, Inc.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////////
#include "DUO3D.h"
#include <opencv2/core/core.hpp>

#define WIDTH	320
#define HEIGHT	240
#define FPS		30

int main(int argc, char* argv[])
{
	printf("DUOLib Version:       v%s\n", GetDUOLibVersion());

	// Open DUO camera and start capturing
	if(!OpenDUOCamera(WIDTH, HEIGHT, FPS))
	{
		printf("Could not open DUO camera\n");
		return 0;
	}
	printf("\nHit <ESC> to exit.\n");
	
	// Create OpenCV windows
	cv::namedWindow("Left");
	cv::namedWindow("Right");


	// Set exposure and LED brightness
	SetGain(0);
	SetExposure(50);
	SetLed(25);

	// Run capture loop until <Esc> key is pressed
	while((cv::waitKey(1) & 0xff) != 27)
	{
		// Capture DUO frame
		PDUOFrame pFrameData = GetDUOFrame();
		if(pFrameData == NULL) continue;


        cv::Mat left { cv::Size(WIDTH, HEIGHT), cv::CV_8UC1, pFrameData->leftData };
        cv::Mat right { cv::Size(WIDTH, HEIGHT), cv::CV_8UC1, pFrameData->rightData };

		// Display images
		cv::imshow("Left", left);
		cv::imshow("Right", right);
	}

	cv::destroyAllWindows();

	// Close DUO camera
	CloseDUOCamera();
	return 0;
}
