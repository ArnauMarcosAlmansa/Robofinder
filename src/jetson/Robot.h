#include <opencv2/core/core.hpp>

class Robot
{
public:
    Robot();
    void turn(float radians);
    void move(float meters);

    const cv::Mat& get_position() const;
    const cv::Mat& get_rotation() const;
private:
    cv::Mat position;
    cv::Mat orientation;
};
