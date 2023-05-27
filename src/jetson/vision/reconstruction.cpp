#include <iostream>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <octomap/octomap.h>
#include "DUO3D.h"

#define WIDTH	752
#define HEIGHT	480
#define FPS		15

/*
std::vector<cv::Point2d> Project(const std::vector<cv::Point3d>& points,
                                 const cv::Mat& intrinsic,
                                 const cv::Mat& distortion) {
  std::vector<cv::Point2d> result;
  if (!points.empty()) {
    cv::projectPoints(points, cv::Mat(3, 1, CV_64F, cv::Scalar(0.)),
                      cv::Mat(3, 1, CV_64F, cv::Scalar(0.)), intrinsic,
                      distortion, result);
  }
  return result;
}

std::vector<cv::Point3d> Unproject(const std::vector<cv::Point2d>& points,
                                   const std::vector<double>& Z,
                                   const cv::Mat& intrinsic,
                                   const cv::Mat& distortion) {
    double f_x = intrinsic.at<double>(0, 0);
    double f_y = intrinsic.at<double>(1, 1);
    double c_x = intrinsic.at<double>(0, 3);
    double c_y = intrinsic.at<double>(1, 3);
  // This was an error before:
  // double c_x = intrinsic.at<double>(0, 3);
  // double c_y = intrinsic.at<double>(1, 3);

  // Step 1. Undistort
  std::vector<cv::Point2d> points_undistorted;
  assert(Z.size() == 1 || Z.size() == points.size());
  if (!points.empty()) {
    cv::undistortPoints(points, points_undistorted, intrinsic,
                        distortion, cv::noArray(), intrinsic);
  }

  // Step 2. Reproject
  std::vector<cv::Point3d> result;
  result.reserve(points.size());
  for (size_t idx = 0; idx < points_undistorted.size(); ++idx) {
    const double z = Z.size() == 1 ? Z[0] : Z[idx];
    result.push_back(
        cv::Point3d((points_undistorted[idx].x - c_x) / f_x * z,
                    (points_undistorted[idx].y - c_y) / f_y * z, z));
  }
  return result;
}

*/


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

  cv::Ptr<cv::Feature2D> feature_detector = cv::SIFT::create();
  cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("FlannBased");

	// Run capture loop until <Esc> key is pressed
	// while((cv::waitKey(1) & 0xff) != 27)
  for (int iter = 0; iter < 100; iter++)
	{
		// Capture DUO frame
		PDUOFrame pFrameData = GetDUOFrame();
		if(pFrameData == NULL) continue;


    // LAS PUTAS CAMARAS ESTAN GIRADAS
    cv::Mat left { cv::Size(WIDTH, HEIGHT), CV_8UC1, pFrameData->rightData };
    cv::Mat right { cv::Size(WIDTH, HEIGHT), CV_8UC1, pFrameData->leftData };

    std::vector<cv::KeyPoint> keypoints_left, keypoints_right;
    cv::Mat descriptors_left, descriptors_right;

    feature_detector->detectAndCompute(left, cv::Mat(), keypoints_left, descriptors_left);
    feature_detector->detectAndCompute(right, cv::Mat(), keypoints_right, descriptors_right);


    cv::Mat output1, output2;
    cv::drawKeypoints(left, keypoints_left, output1);
    cv::drawKeypoints(right, keypoints_right, output2);


    cv::Mat output;

    cv::hconcat(output1, output2, output);

    char c=(char)cv::waitKey(25);
    if(c==27)
        break;


    std::vector<std::vector<cv::DMatch> > rawMatches;
    std::vector<cv::Point2f> p1, p2;

    std::vector<float> distances;
    matcher->knnMatch(descriptors_left, descriptors_right, rawMatches, 2);
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
		cv::imshow("Output", output);
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


/*
int maindos() {
    const double f_x = 2746.;
    const double f_y = 2748.;
    const double c_x = 991.;
    const double c_y = 619.;

  const cv::Mat intrinsic =
      (cv::Mat_<double>(3, 3) << f_x, 0.0, c_x, 0.0, f_y, c_y, 0.0, 0.0, 1.0);
  const cv::Mat distortion =
      (cv::Mat_<double>(5, 1) << 0.0, 0.0, 0.0, 0.0);  // This works!
      // (cv::Mat_<double>(5, 1) << -0.32, 1.24, 0.0013, 0.0013);  // This doesn't!

  // Single point test.
  const cv::Point3d point_single(10.0, -2.0, 30.0);
  const cv::Point2d point_single_projected = Project({point_single}, intrinsic,
                                                     distortion)[0];
  const cv::Point3d point_single_unprojected = Unproject({point_single_projected},
                                    {point_single.z}, intrinsic, distortion)[0];

  std::cout << "Expected Point: " << point_single.x;
  std::cout << " " << point_single.y;
  std::cout << " " << point_single.z << std::endl;
  std::cout << "Computed Point: " << point_single_unprojected.x;
  std::cout << " " << point_single_unprojected.y;
  std::cout << " " << point_single_unprojected.z << std::endl;
}

*/
