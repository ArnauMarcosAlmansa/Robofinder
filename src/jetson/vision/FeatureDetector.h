#include <opencv2/features2d.hpp>


class FeatureDetectorCUDA
{
public:
    FeatureDetectorCUDA();

private:
    Ptr<cv::cuda::ORB> detector;
};
