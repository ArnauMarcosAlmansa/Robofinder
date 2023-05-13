#include "FeatureDetector.h"


FeatureDetectorCUDA::FeatureDetectorCUDA()
{
    detector = cv::cuda::ORB();
}