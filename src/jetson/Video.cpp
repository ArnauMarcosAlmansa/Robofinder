#include "Video.h"

Video::Video(const char* pipeline)
{
    capture.open(pipeline, cv::CAP_GSTREAMER);
    if (!capture.isOpened())
    {
        throw "Error: cap is not open";
    }
}

auto Video::read(cv::Mat& frame) -> bool
{
    capture.read(frame);
    return true;
    // return !frame.empty();
}
