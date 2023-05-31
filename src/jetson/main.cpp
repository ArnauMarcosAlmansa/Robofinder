#include "Robot.h"
#include "vision/Vision.h"
#include "i2c/I2C.h"
#include "map/Map.h"
#include <iostream>
#include <chrono>
#include <utility>

const double WHEEL_CIRCUMFERENCE = 0.132;


struct CameraParams camera_params;

auto main() -> int
{
    Robot robot;
    Vision vision(camera_params);
    I2C i2c("/dev/i2c-1", ARDUINO_ADDRESS);
    Map map(0.01);

    cv::Mat origin = robot.get_position();

    auto start = std::chrono::steady_clock::now();

    i2c.forward(50, 50, 24);
    unsigned char msg[4] = {11, 100, 100, 24};
    i2c.writeBytes(msg, 4);

    while (true)
    {
        //auto value_left = i2c.getEncoderLeft().second;
        //auto value_right = i2c.getEncoderRight().second;

        //auto value = ((double) value_left + (double) value_right) / 2.0;

        //robot.move(value / 12 * WHEEL_CIRCUMFERENCE);

        std::vector<cv::Point3f> points = vision.detect_points(robot.get_position(), robot.get_orientation());
	    map.InsertPointsInTree(points);

        if (std::chrono::steady_clock::now() - start > std::chrono::seconds(5))
            break;
        robot.get_position() = origin;
    }

    i2c.stop();

    map.SaveMapToFile("vista.bt");
    return 0;
}
