#pragma once
#ifndef MAP_H
#define MAP_H
#include <iostream>
#include <string>
#include <octomap/octomap.h>
#include <opencv2/core/types.hpp>
#include <opencv2/core/core.hpp>
#include <cmath>
class Map {
private:
    octomap::OcTree tree;
public:
    Map(float resolution);  // Constructor
    void SaveMapToFile(std::string filename);
    void InsertPointInTree(cv::Point3f point);
    void InsertPointsInTree(std::vector<cv::Point3f> points);
    bool DetectObjectInFront(cv::Mat robotPosition);
    std::vector<octomap::point3d> ComputeRayCasts(cv::Mat posit, cv::Mat Rrot);
    octomap::point3d calculateRay(octomap::point3d robotPosition, cv::Mat Rray, cv::Mat Rrot);
};

#endif  // Map_H
