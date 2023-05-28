#pragma once
#ifndef MAP_H
#define MAP_H
#include <iostream>
#include <string>
#include <octomap/octomap.h>
#include <opencv2/core/types.hpp>

class Map {
private:
    octomap::OcTree tree;
public:
    Map(float resolution);  // Constructor
    void SaveMapToFile(std::string filename);
    void InsertPointInTree(cv::Point3f point);
    void InsertPointsInTree(std::vector<cv::Point3f> points);
};

#endif  // Map_H