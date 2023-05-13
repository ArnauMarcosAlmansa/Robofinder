#include <opencv2/opencv.hpp>

class Video
{
    private:
        cv::VideoCapture capture;

    public:
        Video(const char* pipeline);

        ~Video() { capture.release(); };
        auto read(cv::Mat&) -> bool;
};
