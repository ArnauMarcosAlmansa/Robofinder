#pragma once
#ifndef MAP_H
#define MAP_H
#include <iostream>
#include <string>
#include <octomap/octomap.h>
#include <opencv2/core/types.hpp>
#include <opencv2/core/core.hpp>
#include <cmath>

class EnvironmentPerception;


class Map {
private:
    octomap::OcTree tree;
    float resolution;

    int version;
public:
    Map(float resolution);  // Constructor
    void SaveMapToFile(std::string filename);
    void InsertPointInTree(cv::Point3f point);
    void InsertPointsInTree(std::vector<cv::Point3f> points);
    bool DetectObjectInFront(cv::Mat robotPosition);
    EnvironmentPerception ComputeRayCasts(cv::Mat posit, cv::Mat Rrot);
    octomap::point3d calculateRay(octomap::point3d robotPosition, cv::Mat Rray, cv::Mat Rrot);

    std::vector<cv::Point3f> detect_points_with_virtual_camera(cv::Mat& camera_position, cv::Mat& camera_orientation);
};


class EnvironmentPerception
{
private:
    float resolution_in_radians;
    std::vector<std::pair<bool, octomap::point3d>> cylinder;

public:
    void push_disc(std::vector<std::pair<bool, octomap::point3d>> disc);
    std::vector<std::pair<bool, octomap::point3d>>& get_cylinder();
};


#endif  // Map_H
