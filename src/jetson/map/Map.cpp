#include "Map.h"
#include <iostream>

Map::Map(float resolution) : resolution(resolution), tree(resolution) {

};

void Map::SaveMapToFile(std::string filename){
    if (this->tree.writeBinary(filename)) {
   	    std::cout << "Árbol octal guardado en: " << filename << std::endl;
    } else {
   	std::cout << "Error al guardar el árbol octal en: " << filename << std::endl;
   }
};

void Map::InsertPointInTree(cv::Point3f point){
    octomap::point3d pointOctomap(point.x, point.y, point.z);
    this->tree.updateNode(pointOctomap, true);  // Insertar el punto en el árbol
};

void Map::InsertPointsInTree(std::vector<cv::Point3f> points){
    for (auto& p : points) {
        std::cout << p.x << "\t" << p.y << "\t" << p.z << std::endl;
        InsertPointInTree(p);
	}
};

EnvironmentPerception Map::ComputeRayCasts(cv::Mat posit, cv::Mat Rrot)
{
    EnvironmentPerception perception;

	std::vector<std::pair<bool, octomap::point3d>> puntos;
    puntos.reserve(360);

    const float pi = 3.14159265358;
    const float step = pi / 180.0;  // Convertir 5 grados a radianes

	float x = posit.at<float>(0);
    float y = posit.at<float>(1);
    for (float z = posit.at<float>(2) + resolution; z < 15.0; z = z + resolution * 2)
    {
        octomap::point3d roboPos(x, y, z);
        for (float angle = 0.0; angle <= 2.0 * pi; angle += step) {
            cv::Mat matRay = (cv::Mat_<float>(3, 3) << 
                std::cos(angle), -std::sin(angle), 0,
                std::sin(angle), std::cos(angle), 0,
                0, 0, 1
            );

            octomap::point3d hitpoint = this->calculateRay(roboPos, matRay, Rrot);
            bool hit = hitpoint.x() != 0 || hitpoint.y() != 0 || hitpoint.z() != 0;
            puntos.push_back(std::make_pair(hit, hitpoint));
            if (hit) std::cout << "HIT" << std::endl;
        }

        perception.push_disc(puntos);
        puntos.clear();
    }
	
	return perception;
}

octomap::point3d Map::calculateRay(octomap::point3d robotPosition,cv::Mat Rray, cv::Mat Rrot){
        cv::Mat Ident = (cv::Mat_<float>(3, 1) << 1.0,
                                                  0,
						  0);
	cv::Mat direct = Rray * Rrot * Ident;
	octomap::point3d direction(direct.at<float>(0), direct.at<float>(1), direct.at<float>(2));
	octomap::point3d end;
	double maxRange = 1;
	bool hit = tree.castRay(robotPosition, direction, end, true, maxRange);
	if (hit) {
		return end;
	} else {
		return octomap::point3d(0,0,0);
	}
}
bool Map::DetectObjectInFront(cv::Mat posit){
	//double x = posit.at<double>(0);
	//double y = posit.at<double>(1);  // Extrae la coordenada y del punto
	//double z = posit.at<double>(2);  // Extrae la coordenada z del punto
	//octomap::point3d robotPosition(x, y, z);
	octomap::point3d robotPosition(0.0, 0.0, 0.0);
	octomap::point3d direction(1.0,0.05,-0.05);
	double maxRange = 1;
	octomap::point3d end;
	bool hit =  tree.castRay(robotPosition,direction,end,true,maxRange);
	if (hit) {
		std::cout << "El rayo alcanzó un punto ocupado en (" << end.x() << ", " << end.y() << ", " << end.z() << ")" << std::endl;
	} else {
		std::cout << "El rayo no alcanzó ningún punto ocupado" << std::endl;
	}
	return hit;
}


void EnvironmentPerception::push_disc(std::vector<std::pair<bool, octomap::point3d>> disc)
{
    if (cylinder.empty()) {
        cylinder = disc;
        return;
    }

    if (cylinder.size() != disc.size())
        throw std::runtime_error("Tamaño de disco incorrecto");

    for (int i = 0; i < cylinder.size(); i++)
    {
        if (disc[i].first)
        {
            if (!cylinder[i].first)
            {
                cylinder[i] = disc[i]; // std::cout << "FILL" << std::endl;
            }
            else if (disc[i].second.norm() < cylinder[i].second.norm())
            {
                cylinder[i] = disc[i]; // std::cout << "REPLACE" << std::endl;
            }
        }
    }
}

std::vector<std::pair<bool, octomap::point3d>>& EnvironmentPerception::get_cylinder()
{
    return cylinder;
}