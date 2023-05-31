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


float Robot::pulses_to_meters(int pulses) {
    return pulses * (WHEEL_PERIMETER/PULSES_FOR_REVOLUTION);
}


int Robot::meters_to_pulses(float meters) {
    return meters * (PULSES_FOR_REVOLUTION / WHEEL_PERIMETER);
}

std::pair<bool, int> Robot::rad_to_pulses(float rad) {
    float d = PI * WHEEL_DISTANCE * (abs(rad) / (2 * PI) );
    bool direction_of_rotation = rad < 0;
    return std::make_pair(direction_of_rotation, meters_to_pulses(d));
}


float Robot::pulses_to_rad(bool direction_of_rotation, int pulses) {
    float dis = pulses_to_meters(pulses);
    float rad = (dis / (PI * WHEEL_DISTANCE )) * (2 * PI);

    if (direction_of_rotation == false) {
        rad = -rad;
    }

    return rad;
}

