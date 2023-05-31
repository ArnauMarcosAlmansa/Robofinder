#include "Vision.h"
#include "DUO3D.h"
#include <stdexcept>
#include <cmath>
#include <iostream>
#include "../map/Map.h"
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>

//Map map(0.01);
//octomap::OcTree tree(0.01);
Vision::Vision(struct CameraParams params)
{
    this->params = params;

    intrinsics_left = (cv::Mat_<float>(3, 3) <<
        381.426243, 0, 394.676891,
        0, 381.586377, 248.563332,
        0, 0, 1
    );

    intrinsics_right = (cv::Mat_<float>(3, 3) <<
        381.426243, 0, 394.676891,
        0, 381.586377, 248.563332,
        0, 0, 1
    );

    extrinsic_rotation = (cv::Mat_<float>(3, 3) <<
        0.999987, 0.004473, 0.002619,
        -0.004478, 0.999988, 0.001871,
        -0.002611, -0.001883, 0.999995
    );

    extrinsic_translation = (cv::Mat_<float>(3, 1) <<
        -28.5599996,
        0.14802,
        -0.491068
    );

    extrinsic_translation = extrinsic_translation / 1000;


    projection_left = (cv::Mat_<float>(3, 4) <<
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0
    );
    
    projection_right = (cv::Mat_<float>(3, 4) <<
        0.999987, 0.004473, 0.002619, -28.5599996,
        -0.004478, 0.999988, 0.001871, 0.14802,
        -0.002611, -0.001883, 0.999995, -0.491068
    );

    if(!OpenDUOCamera(params.width, params.height, params.fps))
        throw std:: runtime_error("Failed to open DUO3D camera.");


    SetGain(params.gain);
	SetExposure(params.exposure);
	SetLed(params.led);

    feature_detector = cv::cuda::ORB::create();
    matcher = cv::cuda::DescriptorMatcher::createBFMatcher(cv::NORM_HAMMING);
}


std::vector<cv::Point3f> Vision::detect_points(cv::Mat position, cv::Mat orientation)
{
    //for (int iter = 0; iter < 100; iter++)
    //{
    //    std::cout << "Iter = "<< iter << std::endl;
    PDUOFrame pFrameData = GetDUOFrame();
	if (pFrameData == NULL)
        throw std:: runtime_error("Failed to read frame from DUO3D camera.");

    cv::Mat left { cv::Size(params.width, params.height), CV_8UC1, pFrameData->leftData };
    cv::Mat right { cv::Size(params.width, params.height), CV_8UC1, pFrameData->rightData };


    double min, max;
    cv::minMaxLoc(left, &min, &max);
    std::cout << "left (min, max) = (" << min << ", " << max << ")" << std::endl;

    cv::cuda::GpuMat gpu_left, gpu_right;
    gpu_left.upload(left);
    gpu_right.upload(right);

    if (!gpu_previous_left_frame.empty())
    {
        cv::cuda::GpuMat gpu_diff;
        cv::cuda::absdiff(gpu_left, gpu_previous_left_frame, gpu_diff);

        double sum = cv::cuda::sum(gpu_diff)[0];
        if (sum > params.height * params.width * 255 * 1)
            throw std:: runtime_error("Frames are too different.");
    }

    std::vector<cv::KeyPoint> keypoints_left, keypoints_right;
    cv::cuda::GpuMat gpu_descriptors_left, gpu_descriptors_right;

    feature_detector->detectAndCompute(gpu_left,  cv::noArray(), keypoints_left, gpu_descriptors_left);
    feature_detector->detectAndCompute(gpu_right,  cv::noArray(), keypoints_right, gpu_descriptors_right);

    cv::Mat output1, output2;
    cv::drawKeypoints(left, keypoints_left, output1);
    cv::drawKeypoints(right, keypoints_right, output2);

    cv::Mat output;

    cv::hconcat(output1, output2, output);
    std::cout << std::endl;

    // Display images
    //cv::destroyAllWindows();
    //cv::imshow("Output", output);
    //char c=(char)cv::waitKey(1);
    //if(c==27)
        //throw std:: runtime_error("Pressed out");


    std::vector<std::vector<cv::DMatch> > rawMatches;
    std::vector<cv::Point2f> p1, p2;

    std::vector<float> distances;
    matcher->knnMatch(gpu_descriptors_left, gpu_descriptors_right, rawMatches, 2);

    for (size_t i = 0; i < rawMatches.size(); i++)
    {
        const std::vector<cv::DMatch>& m = rawMatches[i];
        if (m.size() == 2 && m[0].distance < m[1].distance * 0.75)
        {
            cv::KeyPoint& pt1 = keypoints_left[m[0].queryIdx];
            cv::KeyPoint& pt2 = keypoints_right[m[0].trainIdx];
            if (abs(pt1.pt.y - pt2.pt.y) < 3 && pt1.pt.x > pt2.pt.x)
            {
                p1.push_back(keypoints_left[m[0].queryIdx].pt);
                p2.push_back(keypoints_right[m[0].trainIdx].pt);
                distances.push_back(m[0].distance);
            }
        }
    }

    std::cout << "p1.size() = " << p1.size() << std::endl;

    if (p1.size() == 0)
        throw std:: runtime_error("No valid matches.");
        //continue;

    cv::Mat rotation_left = extrinsic_rotation * orientation, translation_left = extrinsic_translation + position;
    cv::Mat rotation_right = orientation, translation_right = position;

    projection_left = compute_projection_matrix(intrinsics_left, rotation_left, translation_left);
    projection_right = compute_projection_matrix(intrinsics_right, rotation_right, translation_right);

    cv::Mat homogeneous_points = triangulate(projection_left, projection_right, p1, p2);

    cv::transpose(homogeneous_points, homogeneous_points);

    std::vector<cv::Point3f> points3d;
    cv::convertPointsFromHomogeneous(homogeneous_points, points3d);
    //map.InsertPointsInTree(points3d);
    //}
    //std::string filename = "octree.bt";
    //map.SaveMapToFile("octree.bt");
    return points3d;
}


cv::Mat Vision::compute_projection_matrix(cv::Mat camera_matrix, cv::Mat rotation, cv::Mat translation)
{
    cv::Mat RTMat(3, 4, CV_64F);
    cv::hconcat(rotation, translation, RTMat);
    return (camera_matrix * RTMat);
}


cv::Mat Vision::triangulate(
    const cv::Mat& projection_left,
    const cv::Mat& projection_right,
    const std::vector<cv::Point2f>& points_left,
    const std::vector<cv::Point2f>& points_right
)
{
    cv::Mat homogeneous_points;
    cv::triangulatePoints(projection_left, projection_right, points_left, points_right, homogeneous_points);
    return homogeneous_points;
}

Vision::~Vision()
{
	CloseDUOCamera();
}
