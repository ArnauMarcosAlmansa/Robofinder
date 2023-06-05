#include "Robot.h"
#include "vision/Vision.h"
#include "i2c/I2C.h"
#include "map/Map.h"
#include <iostream>
#include <chrono>
#include <utility>
#include <thread>
#include "navegacion/navegacion.h"
#include "localization/Localization.h"
#include <cmath>
using namespace std::chrono_literals;


const double WHEEL_CIRCUMFERENCE = 0.132;
const double pi = 3.14159265359;
const int LIMIT_ULTRASENSOR = 18;


struct CameraParams cam_params;

void doComputeStart(Robot* robot, Map* map, Vision* vision){
    for (int prec = 0; prec < 2; prec++){
	std::cout <<"Trying Start Compute " << std::endl;
    	cv::Mat camera_points = (*vision).detect_points();
    	std::vector<cv::Point3f> cam_points = camera_points;
    	std::vector<cv::Point3f> good_points = (*vision).filter_out_faraway_points(cam_points);
    	std::vector<cv::Point3f> points = (*vision).camera_points_to_world(good_points, (*robot).compute_own_camera_position(), (*robot).get_orientation());
    	map->InsertPointsInTree(points);
        map->SaveMapToFile("mapa");
    }

};

void doRecognition(Robot* robot, Map* map, Vision* vision, Navegacion* nav){
	for (int i = 0; i < 3; i++){
        	doComputeStart(robot, map, vision);
        	int pulsos = (*nav).turn_right90();
        	(*robot).turn_from_last_known_with_pulses(false, pulsos);
        	(*robot).commit();
    	}
	doComputeStart(robot, map, vision);
   	int pulsos135 = (*nav).turn_right135();
	(*robot).turn_from_last_known_with_pulses(false, pulsos135);
	(*robot).commit();
	for (int i = 0; i < 3; i++){
        	doComputeStart(robot, map, vision);
        	int pulsos = (*nav).turn_right90();
        	(*robot).turn_from_last_known_with_pulses(false, pulsos);
        	(*robot).commit();
    	}
}

int findMiddleZeroIndex(std::vector<std::pair<bool, octomap::point3d>>& points) {

    // int initial_size = points.size();
    // for (int i = 0; i < initial_size; i++)
    //     points.push_back(points[i]);


    int maxConsecutiveZeros = 0;  // Contador para el número máximo de ceros consecutivos
    int maxConsecutiveZerosIndex = -1;  // Índice del inicio del tramo más largo de ceros consecutivos

    int currentConsecutiveZeros = 0;  // Contador para el número actual de ceros consecutivos
    int currentConsecutiveZerosIndex = -1;  // Índice actual del inicio del tramo de ceros consecutivos

    for (auto i = points.begin(); i != points.end(); ++i) {
        if ((i->second.x() == 0.0 && i->second.y() == 0.0 && i->second.z() == 0.0)) {
            if (currentConsecutiveZeros == 0) {
                currentConsecutiveZerosIndex = std::distance(points.begin(), i);
            }
            currentConsecutiveZeros++;
        } else {
            if (currentConsecutiveZeros > maxConsecutiveZeros) {
                maxConsecutiveZeros = currentConsecutiveZeros;
                maxConsecutiveZerosIndex = currentConsecutiveZerosIndex;
            }
            currentConsecutiveZeros = 0;
        }
    }

    if (currentConsecutiveZeros > maxConsecutiveZeros) {
        // Si la última secuencia de ceros es la más larga
        maxConsecutiveZeros = currentConsecutiveZeros;
        maxConsecutiveZerosIndex = currentConsecutiveZerosIndex;
    }

    // Calculamos el índice del punto del medio
    int middleZeroIndex = maxConsecutiveZerosIndex + maxConsecutiveZeros / 2;

    return middleZeroIndex;
};

auto main() -> int
{
    cam_params.width = 752;
    cam_params.height = 480;
    cam_params.fps = 15;
    cam_params.exposure = 100;
    cam_params.gain = 0;
    cam_params.led = 25;


    Robot robot;
    Vision vision(cam_params);
    I2C i2c("/dev/i2c-1", ARDUINO_ADDRESS);
    Navegacion nav(i2c);
    Map map(0.01);
    MonteCarloLocalization localization(20);


    auto start_time = std::chrono::steady_clock::now();
    auto current_time = std::chrono::steady_clock::now();

    bool object = false;
    bool wall = false;

    doRecognition(&robot, &map, &vision, &nav);

    start_time = std::chrono::steady_clock::now();
    current_time = std::chrono::steady_clock::now();

    while (current_time - start_time < std::chrono::seconds(120))
    {
        wall = i2c.getMinimumUltraSoundValue().second < LIMIT_ULTRASENSOR;
        if (wall && !object)
            wall = i2c.getMinimumUltraSoundValue().second < LIMIT_ULTRASENSOR;


        cv::Mat camera_points = vision.detect_points();
        std::vector<cv::Point3f> cam_points = camera_points;
        std::vector<cv::Point3f> good_points = vision.filter_out_faraway_points(cam_points);
        // localization.localize(robot, cam_points, map);
        std::vector<cv::Point3f> points = vision.camera_points_to_world(good_points, robot.compute_own_camera_position(), robot.get_orientation());
        map.InsertPointsInTree(points);
        map.SaveMapToFile("mapa");

        EnvironmentPerception perception = map.ComputeRayCasts(
            robot.get_position(),
            robot.get_orientation()
        );

        std::vector<std::pair<bool, octomap::point3d>> tochamen = perception.get_cylinder();
        std::cout << "Longitud del vector: " << tochamen.size() << std::endl;
        for(auto& pareja : tochamen){
            std::cout <<"Pareja de cylindro: " << pareja.second << std::endl;
	    }
            
        int index = findMiddleZeroIndex(tochamen);
        std::cout << "Index Value: " << index << std::endl;
        float radians = index * (M_PI / 180.0);
        std::cout <<"Radians: " << radians << std::endl;
        std::pair<bool, int> escape = robot.rad_to_pulses(radians);
        std::cout <<"Me muevorrr:" << escape.first << std::endl;
        std::cout <<"Pulses needed:" << escape.second << std::endl;

        int idk = nav.turn_right_pulses(escape.second);
        robot.turn_from_last_known_with_pulses(true, idk);


        robot.commit();
        int pulsos = nav.forward();
        std::cout << "pulsos: " << pulsos << std::endl;
        robot.move_from_last_known(robot.pulses_to_meters(pulsos));
        robot.commit();

        current_time = std::chrono::steady_clock::now();
    }
    /*
    for (int prec = 0; prec < 2; prec++)
    {
        cv::Mat camera_points = vision.detect_points();
        std::vector<cv::Point3f> cam_points = camera_points;
        std::vector<cv::Point3f> good_points = vision.filter_out_faraway_points(cam_points);
        localization.localize(robot, cam_points, map);
        std::vector<cv::Point3f> points = vision.camera_points_to_world(good_points, robot.compute_own_camera_position(), robot.get_orientation());
        map.InsertPointsInTree(points);
    }
   */
    map.SaveMapToFile("mapa");
    return 0;
}
