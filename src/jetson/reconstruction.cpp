#include <iostream>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <opencv2/core/cuda.hpp>
#include <octomap/octomap.h>
#include "DUO3D.h"
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudafeatures2d.hpp>
#define WIDTH	752
#define HEIGHT	480
#define FPS		15

cv::Mat Triangulate(
    const cv::Mat& proj1,
    const cv::Mat& proj2,
    const std::vector<cv::Point2f>& points1,
    const std::vector<cv::Point2f>& points2
)
{
    cv::Mat homogeneous_points;
    cv::triangulatePoints(proj1, proj2, points1, points2, homogeneous_points);
    return homogeneous_points;
}

cv::Mat computeProjMat(cv::Mat camMat, cv::Mat rotation, cv::Mat translation)
{
    cv::Mat RTMat(3, 4, CV_64F);
    //2. Append translation vector to rotation matrix
    cv::hconcat(rotation, translation, RTMat);
    //3. Compute projection matrix by multiplying intrinsic parameter 
    //matrix (A) with 3 x 4 rotation and translation pose matrix (RT).
    return (camMat * RTMat);
}


cv::Mat intrinsics_left = (cv::Mat_<float>(3, 3) <<
    381.426243, 0, 394.676891,
    0, 381.586377, 248.563332,
    0, 0, 1);

cv::Mat intrinsics_right = (cv::Mat_<float>(3, 3) <<
    381.426243, 0, 394.676891,
    0, 381.586377, 248.563332,
    0, 0, 1
);


cv::Mat extrinsics = (cv::Mat_<float>(3, 4) <<
    0.999987, 0.004473, 0.002619, -0.285599996,
    -0.004478, 0.999988, 0.001871, 0.0014802,
    -0.002611, -0.001883, 0.999995, -0.00491068
);

cv::Mat extrinsic_rotation = (cv::Mat_<float>(3, 3) <<
    0.999987, 0.004473, 0.002619,
    -0.004478, 0.999988, 0.001871,
    -0.002611, -0.001883, 0.999995
);

cv::Mat extrinsic_translation = (cv::Mat_<float>(3, 1) <<
    -0.285599996,
    0.0014802,
    -0.00491068
);

cv::Mat rotation = (cv::Mat_<float>(3, 3) <<
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
);

cv::Mat translation = (cv::Mat_<float>(3, 1) <<
    0,
    0,
    0
);


cv::Mat proj1 = (cv::Mat_<float>(3, 4) <<
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0
);

cv::Mat proj2 = (cv::Mat_<float>(3, 4) <<
    0.999987, 0.004473, 0.002619, -28.5599996,
    -0.004478, 0.999988, 0.001871, 0.14802,
    -0.002611, -0.001883, 0.999995, -0.491068
);


auto main() -> int {
	printf("DUOLib Version:       v%s\n", GetDUOLibVersion());
  octomap::OcTree tree(0.05);
	// Open DUO camera and start capturing
	if(!OpenDUOCamera(WIDTH, HEIGHT, FPS))
	{
		printf("Could not open DUO camera\n");
		return 0;
	}
	printf("\nHit <ESC> to exit.\n");

  // Set exposure and LED brightness
	SetGain(0);
	SetExposure(50);
	SetLed(25);
    extrinsic_translation = extrinsic_translation * 100;

    cv::Ptr<cv::cuda::ORB> feature_detector = cv::cuda::ORB::create();
    cv::Ptr<cv::cuda::DescriptorMatcher> matcher = cv::cuda::DescriptorMatcher::createBFMatcher(cv::NORM_HAMMING);

    // Capture DUO frame
	PDUOFrame pFrameData = NULL;

    while (!pFrameData)
        pFrameData = GetDUOFrame();

    // LAS PUTAS CAMARAS ESTAN GIRADAS
    cv::Mat previous_left_frame { cv::Size(WIDTH, HEIGHT), CV_8UC1, pFrameData->leftData };
    cv::cuda::GpuMat gpu_previous_left_frame;
    gpu_previous_left_frame.upload(previous_left_frame);

	// Run capture loop until <Esc> key is pressed
	// while((cv::waitKey(1) & 0xff) != 27)

  for (int iter = 0; iter < 100; iter++){
		// Capture DUO frame
		PDUOFrame pFrameData = GetDUOFrame();
		if(pFrameData == NULL) continue;


    // LAS PUTAS CAMARAS ESTAN GIRADAS
    cv::Mat left { cv::Size(WIDTH, HEIGHT), CV_8UC1, pFrameData->leftData };
    cv::Mat right { cv::Size(WIDTH, HEIGHT), CV_8UC1, pFrameData->rightData };


    cv::cuda::GpuMat gpu_left, gpu_right;
    gpu_left.upload(left);
    gpu_right.upload(right);

    cv::cuda::GpuMat gpu_diff;
    cv::cuda::absdiff(gpu_left, gpu_previous_left_frame, gpu_diff);

    double sum = cv::cuda::sum(gpu_diff)[0];
    if (sum > HEIGHT * WIDTH * 255 * 1)
        continue;

    std::vector<cv::KeyPoint> keypoints_left, keypoints_right;
    cv::cuda::GpuMat gpu_descriptors_left, gpu_descriptors_right;

    feature_detector->detectAndCompute(gpu_left,  cv::noArray(), keypoints_left, gpu_descriptors_left);
    feature_detector->detectAndCompute(gpu_right,  cv::noArray(), keypoints_right, gpu_descriptors_right);

    cv::Mat descriptors_left, descriptors_right;

    cv::Mat output1, output2;
    cv::drawKeypoints(left, keypoints_left, output1);
    cv::drawKeypoints(right, keypoints_right, output2);

    cv::Mat output;

    cv::hconcat(output1, output2, output);
    cv::imshow("Output", output);


    char c=(char)cv::waitKey(1);
    if(c==27)
        break;


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
      continue;

    cv::Mat rotation_left = extrinsic_rotation * rotation, translation_left = extrinsic_translation + translation;
    cv::Mat rotation_right = rotation, translation_right = translation;

    proj1 = computeProjMat(intrinsics_left, rotation_left, translation_left);
    proj2 = computeProjMat(intrinsics_right, rotation_right, translation_right);

    std::cout << "proj1 = " << proj1 << std::endl;
    std::cout << "proj2 = " << proj2 << std::endl;

    cv::Mat homogeneous_points = Triangulate(proj1, proj2, p1, p2);

    cv::transpose(homogeneous_points, homogeneous_points);

    cv::Size s = homogeneous_points.size();
    int rows = s.height;
    int cols = s.width;

    std::cout << "rows = " <<  rows << std::endl;
    std::cout << "cols = " <<  cols << std::endl;
    std::cout << "total = " <<  homogeneous_points.total() << std::endl;
    std::cout << std::flush;

    std::vector<cv::Point3f> points3d;
    cv::convertPointsFromHomogeneous(homogeneous_points, points3d);
   
    for (auto& p : points3d) {
      octomap::point3d point(p.x, p.y, p.z);
      if (iter == 10)
        tree.updateNode(point, true);  // Insertar el punto en el árbol
      /*
      std::cout
        << p.x << ", "
        << p.y << ", "
        << p.z
        << std::endl;
      */
    }
    std::cout << std::endl;

		// Display images
	}
	cv::destroyAllWindows();
    
    std::string filename = "octree.bt";

   if (tree.writeBinary(filename)) {
   	std::cout << "Árbol octal guardado en: " << filename << std::endl;
   } else {
   	std::cout << "Error al guardar el árbol octal en: " << filename << std::endl;
   }
	// Close DUO camera
	CloseDUOCamera();
	return 0;
}
