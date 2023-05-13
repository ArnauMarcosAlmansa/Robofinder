#define USE_CUDA

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#ifndef USE_CUDA
#include <opencv2/features2d.hpp>
#else
#include <opencv2/xfeatures2d/cuda.hpp>
#endif

#include <opencv2/cudafeatures2d.hpp>

#include <opencv2/core/mat.hpp>

#include <octomap/octomap.h>
#include <octomap/OcTree.h>

//#include "vision/FeatureDetector.h"

#include "Video.h"


using namespace std;
using namespace octomap;


std::string gstreamer_pipeline (int sensor_id, int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {

    return "nvarguscamerasrc sensor-id=" + std::to_string(sensor_id) + " ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
           std::to_string(capture_height) + 
           " ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! videoconvert ! video/x-raw, format=(string)BGR ! appsink max-buffers=1 drop=true";
}

std::string usb_pipeline (int sensor_id, int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {

    return "v4l2src device=" + std::to_string(sensor_id) + " ! video/x-raw, width=(int)" + std::to_string(capture_width) + ", height=(int)" +
           std::to_string(capture_height) + ", framerate=(fraction)" + std::to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)" +
           std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}




auto main() -> int
{
    cv::Ptr<cv::cuda::ORB> orb = cv::cuda::ORB::create();

    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("FlannBased");


    int capture_width = 256;
    int capture_height = 256;
    int display_width = 256;
    int display_height = 256;
    int framerate = 60;
    int flip_method = 2;

    int sensor_id = 0;

    std::string pipeline1 = gstreamer_pipeline(0, capture_width,
                                              capture_height,
                                              display_width,
                                              display_height,
                                              framerate,
                                              flip_method);

    std::string pipeline2 = gstreamer_pipeline(1, capture_width,
                                               capture_height,
                                               display_width,
                                               display_height,
                                               framerate,
                                               flip_method);


    std::cout << "gst-launch-1.0 " << pipeline1 << std::endl;

    Video v1((char*) pipeline1.data()), v2((char*) pipeline2.data());

    cv::Mat host_frame1, host_frame2;
    cv::cuda::GpuMat gpu_frame1, gpu_frame2;

    if (!v1.read(host_frame1) || !v1.read(host_frame2))
    {
        std::cerr << "HA PETADO LA PRIMERA LECTURA." << std::endl;
        return -1;
    }

    gpu_frame1.upload(host_frame1);
    gpu_frame2.upload(host_frame2);

    long int counter = 0;

    while (v1.read(host_frame1) && v2.read(host_frame2))
    {
        /*
        gpu_frame1.upload(host_frame1);
        gpu_frame2.upload(host_frame2);


        std::vector<cv::KeyPoint> keypoints1, keypoints2;

        cv::Mat descriptors1, descriptors2;
        orb->detectAndComputeAsync(prev_frame, cv::cuda::GpuMat(), keypoints1, descriptors1);
        orb->detectAndComputeAsync(frame, cv::cuda::GpuMat(), keypoints2, descriptors2);

        std::vector<std::vector<cv::DMatch> > rawMatches;
        std::vector<cv::Point2f> p1, p2;

        std::vector<float> distances;
        matcher->knnMatch(descriptors1, descriptors2, rawMatches, 2);
        for (size_t i = 0; i < rawMatches.size(); i++)
        {
            const std::vector<cv::DMatch>& m = rawMatches[i];
            if (m.size() == 2 && m[0].distance < m[1].distance * 0.75)
            {
                p1.push_back(keypoints1[m[0].queryIdx].pt);
                p2.push_back(keypoints2[m[0].trainIdx].pt);
                distances.push_back(m[0].distance);
            }
        }
	*/
        cv::Mat output;

        cv::hconcat(host_frame1, host_frame2, output);

        imshow("Live", output);
        std::cout << counter << std::endl;

        char c=(char)cv::waitKey(1);
        if(c==27)
            break;

        counter++;
    }
}

/*
 void print_query_info(point3d query, OcTreeNode* node) {
   if (node != NULL) {
     cout << "occupancy probability at " << query << ":\t " << node->getOccupancy() << endl;
   }
   else 
     cout << "occupancy probability at " << query << ":\t is unknown" << endl;    
 }
  
 int main() {
  
   cout << endl;
   cout << "generating example map" << endl;
  
   OcTree tree (0.1);  // create empty tree with resolution 0.1
  
  
   // insert some measurements of occupied cells
  
   for (int x=-20; x<20; x++) {
     for (int y=-20; y<20; y++) {
       for (int z=-20; z<20; z++) {
         point3d endpoint ((float) x*0.05f, (float) y*0.05f, (float) z*0.05f);
         tree.updateNode(endpoint, true); // integrate 'occupied' measurement
       }
     }
   }
  
   // insert some measurements of free cells
  
   for (int x=-30; x<30; x++) {
     for (int y=-30; y<30; y++) {
       for (int z=-30; z<30; z++) {
         point3d endpoint ((float) x*0.02f-1.0f, (float) y*0.02f-1.0f, (float) z*0.02f-1.0f);
         tree.updateNode(endpoint, false);  // integrate 'free' measurement
       }
     }
   }
  
   cout << endl;
   cout << "performing some queries:" << endl;
   
   point3d query (0., 0., 0.);
   OcTreeNode* result = tree.search (query);
   print_query_info(query, result);
  
   query = point3d(-1.,-1.,-1.);
   result = tree.search (query);
   print_query_info(query, result);
  
   query = point3d(1.,1.,1.);
   result = tree.search (query);
   print_query_info(query, result);
  
  
   cout << endl;
   tree.writeBinary("simple_tree.bt");
   cout << "wrote example file simple_tree.bt" << endl << endl;
   cout << "now you can use octovis to visualize: octovis simple_tree.bt"  << endl;
   cout << "Hint: hit 'F'-key in viewer to see the freespace" << endl  << endl;  
  
 }
*/

/*
auto main() -> int
{
    cv::Mat im = cv::imread("harold.jpg");
    cv::threshold(im, im, 100, 255, cv::THRESH_BINARY);
    cv::imwrite("harold_bin.jpg", im);
}
*/
