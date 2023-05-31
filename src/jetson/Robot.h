#include <opencv2/core/core.hpp>

class Robot
{
public:
    Robot();
    void turn_from_last_known(float radians);
    void move_from_last_known(float meters);

    cv::Mat& get_position();
    cv::Mat& get_orientation();
private:
    cv::Mat last_known_position;
    cv::Mat last_known_orientation;

    cv::Mat position;
    cv::Mat orientation;
};
