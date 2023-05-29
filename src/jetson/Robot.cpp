#include "Robot.h"
#include <cmath>

Robot::Robot()
{
    position = (cv::Mat_<float>(3, 1) <<
        0,
        0,
        0
    );

    orientation = (cv::Mat_<float>(3, 3) <<
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    );
}

void Robot::turn(float radians)
{
    cv::Mat rotation = (cv::Mat_<float>(3, 3) <<
        std::cos(radians), -std::sin(radians), 0,
        std::sin(radians), std::cos(radians), 0,
        0, 0, 1
    );

    orientation = rotation * orientation;
}

void Robot::move(float meters)
{
    cv::Mat dumb_translation = (cv::Mat_<float>(3, 1) <<
        meters,
        0,
        0
    );

    cv::Mat movement = orientation * dumb_translation;

    position = position + movement;
}


cv::Mat& Robot::get_position()
{
    return position;
}


cv::Mat& Robot::get_orientation()
{
    return orientation;
}



