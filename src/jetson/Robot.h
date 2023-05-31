#include <opencv2/core/core.hpp>

#define WHEEL_PERIMETER 0.132       // Permimetro de la rueda.
#define PULSES_FOR_REVOLUTION 12    // Pulsos del encoder
#define WHEEL_DISTANCE 0.22         // Ancho entre ruedas.
#define PI 3.14159265359

class Robot
{
public:
    Robot();
    void turn(float radians);
    void move(float meters);

    float pulses_to_meters(int pulses);
    int meters_to_pulses(float meters);

    std::pair<bool, int> rad_to_pulses(float rad);

    cv::Mat& get_position();
    cv::Mat& get_orientation();
private:
    cv::Mat position;
    cv::Mat orientation;
};
