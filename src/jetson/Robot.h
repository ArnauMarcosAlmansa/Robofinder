#include <opencv2/core/core.hpp>

class Robot
{
public:
    Robot();
    void turn(float radians);
    void move(float meters);

    cv::Mat& get_position();
    cv::Mat& get_orientation();
private:
    cv::Mat position;
    cv::Mat orientation;
};
