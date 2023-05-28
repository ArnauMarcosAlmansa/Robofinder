#include "Robot.h"
#include "vision/Vision.h"
#include <iostream>


struct CameraParams camera_params;

auto main() -> int
{
    Robot robot;
    Vision vision(camera_params);
}