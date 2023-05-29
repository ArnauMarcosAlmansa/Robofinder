#include "Robot.h"
#include "vision/Vision.h"
#include "i2c/I2C.h"
#include <iostream>


struct CameraParams camera_params;

auto main() -> int
{
    Robot robot;
    Vision vision(camera_params);
    I2C i2c;
    Map map(0.05);

    while (true)
    {
        std::vector<cv::Point3f> points = vision.detect_points(robot.get_position(), robot.get_orientation());
        map.InsertPointsInTree(points);
    }
}