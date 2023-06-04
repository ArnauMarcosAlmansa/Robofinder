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
    	(*map).InsertPointsInTree(points);
    }

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

    for (int i = 0; i < 3; i++)
    {
	doComputeStart(&robot, &map, &vision);
        int pulsos = nav.turn_right90();
        robot.turn_from_last_known_with_pulses(false, pulsos);
        robot.commit();
    }

    doComputeStart(&robot, &map, &vision);
    int pulsos135 = nav.turn_right135();
    robot.turn_from_last_known_with_pulses(false, pulsos135);
    robot.commit();

    for (int i = 0; i < 3; i++)
    {
        doComputeStart(&robot, &map, &vision);
        int pulsos = nav.turn_right90();
        robot.turn_from_last_known_with_pulses(false, pulsos);
        robot.commit();
    }

    start_time = std::chrono::steady_clock::now();
    current_time = std::chrono::steady_clock::now();
    while (current_time - start_time < std::chrono::seconds(30))
    {
        wall = i2c.getMinimumUltraSoundValue().second < LIMIT_ULTRASENSOR;
        if (wall && !object)
            wall = i2c.getMinimumUltraSoundValue().second < LIMIT_ULTRASENSOR;	


        cv::Mat camera_points = vision.detect_points();
        std::vector<cv::Point3f> cam_points = camera_points;
        std::vector<cv::Point3f> good_points = vision.filter_out_faraway_points(cam_points);
        localization.localize(robot, cam_points, map);
        std::vector<cv::Point3f> points = vision.camera_points_to_world(good_points, robot.compute_own_camera_position(), robot.get_orientation());
        map.InsertPointsInTree(points);

        EnvironmentPerception perception = map.ComputeRayCasts(
            robot.get_position(),
            robot.get_orientation()
        );

        int pulsos = nav.turn_right90();
        std::cout << "pulsos: " << pulsos << std::endl;
        robot.turn_from_last_known_with_pulses(false, pulsos);
        robot.commit();
	current_time = std::chrono::steady_clock::now();
    }

    for (int prec = 0; prec < 2; prec++)
    {
        cv::Mat camera_points = vision.detect_points();
        std::vector<cv::Point3f> cam_points = camera_points;
        std::vector<cv::Point3f> good_points = vision.filter_out_faraway_points(cam_points);
        localization.localize(robot, cam_points, map);
        std::vector<cv::Point3f> points = vision.camera_points_to_world(good_points, robot.compute_own_camera_position(), robot.get_orientation());
        map.InsertPointsInTree(points);
    }

    map.SaveMapToFile("vista.bt");
    return 0;
}
