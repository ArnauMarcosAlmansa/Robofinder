#include <opencv2/opencv.hpp>

class Video
{
    private:
        cv::VideoCapture capture;

    public:
        Video();

        ~Video() { capture.release(); };
        auto read(cv::Mat&) -> bool;
};
