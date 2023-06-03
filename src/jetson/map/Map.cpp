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

bool Map::DetectObjectInFront(cv::Mat posit){
	//double x = posit.at<double>(0);
	//double y = posit.at<double>(1);  // Extrae la coordenada y del punto
	//double z = posit.at<double>(2);  // Extrae la coordenada z del punto
	//octomap::point3d robotPosition(x, y, z);
	octomap::point3d robotPosition(0.0, 0.0, 0.0);
	octomap::point3d direction(robotPosition.x()+1.0,0.0,-0.05);
	double maxRange = 0.5;
	octomap::point3d end(robotPosition.x() +0.1,robotPosition.y(), robotPosition.z() -0.05);
	bool hit =  tree.castRay(robotPosition,direction,end,true,maxRange);
	if (hit) {
		std::cout << "El rayo alcanzó un punto ocupado en (" << end.x() << ", " << end.y() << ", " << end.z() << ")" << std::endl;
	} else {
		std::cout << "El rayo no alcanzó ningún punto ocupado" << std::endl;
	}
	return hit;
}
