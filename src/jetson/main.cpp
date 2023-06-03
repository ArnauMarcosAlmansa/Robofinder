#include "Robot.h"
#include "vision/Vision.h"
#include "i2c/I2C.h"
#include "map/Map.h"
#include <iostream>
#include <chrono>
#include <utility>
#include <thread>
#include "navegacion/navegacion.h"

using namespace std::chrono_literals;


const double WHEEL_CIRCUMFERENCE = 0.132;
const double pi = 3.14159265359;
const int LIMIT_ULTRASENSOR = 18;


struct CameraParams cam_params;

auto main() -> int
{
    cam_params.width = 752;
    cam_params.height = 480;
    cam_params.fps = 15;
    cam_params.exposure = 50;
    cam_params.gain = 0;
    cam_params.led = 25;


    Robot robot;
    /*
    std::cout << "starting position: " << robot.get_position() << std::endl;
    std::cout << "starting orientation: " << robot.get_orientation() << std::endl;

    robot.move_from_last_known(1);
    robot.turn_from_last_known(PI);
    robot.commit();

    std::cout << "middle position: " << robot.get_position() << std::endl;
    std::cout << "middle orientation: " << robot.get_orientation() << std::endl;

    robot.move_from_last_known(1);
    robot.turn_from_last_known(PI);
    robot.commit();

    std::cout << "end position: " << robot.get_position() << std::endl;
    std::cout << "end orientation: " << robot.get_orientation() << std::endl;
    */
    Vision vision(cam_params);
    I2C i2c("/dev/i2c-1", ARDUINO_ADDRESS);
    Map map(0.01);

    cv::Mat origin = robot.get_position();

    auto start = std::chrono::steady_clock::now();

    //i2c.forward(65, 50, 24);
    // robot.move_from_last_known(0.5);
    
    Navegacion nav(i2c);
    bool object = false;
    bool wall = false;

    for (int i = 0; i < 1; i++) 
    {
        wall = i2c.getMinimumUltraSoundValue().second < LIMIT_ULTRASENSOR;
        //object = TODO.
        if (wall && !object)
            wall = i2c.getMinimumUltraSoundValue().second < LIMIT_ULTRASENSOR;	

        std::cout << "CAMERA POSITION: " << robot.compute_camera_position() << std::endl;

        std::vector<cv::Point3f> points = vision.detect_points(robot.compute_camera_position(), robot.get_orientation());
        if (points.size() != 0) {
            map.InsertPointsInTree(points);
        }

        std::vector<octomap::point3d> hit_points = map.ComputeRayCasts(
            robot.get_position(),
            robot.get_orientation()
        );

        std::cout << "HIT POINTS:" << std::endl;
        for (octomap::point3d& p : hit_points)
            std::cout << p << std::endl;

        std::cout << std::endl;

        // nav.decide_movement(&robot,object,wall);
        std::cout << "Posición robot : " << robot.get_position() << std::endl;
        for (int z =0; z < 5; z++){
            std::vector<cv::Point3f> points = vision.detect_points(robot.get_position(), robot.get_orientation());
            if (points.size() != 0) {
                map.InsertPointsInTree(points);
            }
        }

        robot.commit();
    }

    map.SaveMapToFile("vista.bt");
    return 0;
}
