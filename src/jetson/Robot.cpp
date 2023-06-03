#include "Robot.h"
#include <cmath>

Robot::Robot()
{
    last_known_position = (cv::Mat_<float>(3, 1) <<
        0,
        0,
        0
    );

    last_known_orientation = (cv::Mat_<float>(3, 3) <<
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    );

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

void Robot::turn_from_last_known(float radians)
{
    cv::Mat rotation = (cv::Mat_<float>(3, 3) <<
        std::cos(radians), -std::sin(radians), 0,
        std::sin(radians), std::cos(radians), 0,
        0, 0, 1
    );

    orientation = rotation * last_known_orientation;
}

void Robot::move_from_last_known(float meters)
{
    cv::Mat dumb_translation = (cv::Mat_<float>(3, 1) <<
        meters,
        0,
        0
    );

    cv::Mat movement = orientation * dumb_translation;

    position = last_known_position + movement;
}

void Robot::move_from_last_known_with_pulses(int pulses)
{
    float meters = pulses_to_meters(pulses);

    cv::Mat dumb_translation = (cv::Mat_<float>(3, 1) <<
        meters,
        0,
        0
    );    

    cv::Mat movement = orientation * dumb_translation;

    position = last_known_position + movement;
}

cv::Mat Robot::compute_camera_position()
{
    cv::Mat camera_translation = (cv::Mat_<float>(3, 1) <<
        0.15,
        0,
        0.11
    );

    return position + orientation * camera_translation;
}

void Robot::commit()
{
    last_known_position = position;
    last_known_orientation = orientation;
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
    float d = PI * WHEEL_DISTANCE * (std::abs(rad) / (2 * PI) );
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

