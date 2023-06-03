#include <opencv2/core/core.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudafeatures2d.hpp>
#include <vector>
#include "../Robot.h"

struct CameraParams
{
    int width = 752;
    int height = 480;
    int fps = 15;

    int gain = 0;
    int exposure = 100;
    int led = 25;
};


class Vision
{
public:
    Vision(struct CameraParams params);

    std::vector<cv::Point3f> detect_points(cv::Mat position, cv::Mat orientation);

    ~Vision();
    
    int get_exposure();
    void set_exposure(int exposure);

    void auto_exposure();

private:
    struct CameraParams params;

    cv::Mat intrinsics_left;
    cv::Mat intrinsics_right;

    cv::Mat extrinsics;
    cv::Mat extrinsic_rotation;
    cv::Mat extrinsic_translation;

    cv::Mat projection_left;
    cv::Mat projection_right;

    cv::cuda::GpuMat gpu_previous_left_frame;

    cv::Ptr<cv::cuda::ORB> feature_detector;
    cv::Ptr<cv::cuda::DescriptorMatcher> matcher;


    cv::Mat triangulate(const cv::Mat& projection_left, const cv::Mat& projection_right, const std::vector<cv::Point2f>& points_left, const std::vector<cv::Point2f>& points_right);
    cv::Mat compute_projection_matrix(cv::Mat camera_matrix, cv::Mat rotation, cv::Mat translation);
};
