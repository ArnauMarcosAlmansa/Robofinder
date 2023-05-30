#include "Map.h"

Map::Map(float resolution) : tree(resolution) {

};

void Map::SaveMapToFile(std::string filename){
    if (this->tree.writeBinary(filename)) {
   	    std::cout << "Árbol octal guardado en: " << filename << std::endl;
    } else {
   	std::cout << "Error al guardar el árbol octal en: " << filename << std::endl;
   }
};

void Map::InsertPointInTree(cv::Point3f point){
    octomap::point3d pointOctomap(-point.z, point.x, point.y);
    this->tree.updateNode(pointOctomap, true);  // Insertar el punto en el árbol
};

void Map::InsertPointsInTree(std::vector<cv::Point3f> points){
    for (auto& p : points) {
        octomap::point3d pointOctomap(-p.z, p.x, p.y);
        tree.updateNode(pointOctomap, true);  // Insertar el punto en el árbol
	}
};
