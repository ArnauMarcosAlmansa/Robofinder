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


struct CameraParams cam_params;

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
    //I2C i2c("/dev/i2c-1", ARDUINO_ADDRESS);
    Map map(0.01);

    cv::Mat origin = robot.get_position();

    auto start = std::chrono::steady_clock::now();

    //i2c.forward(65, 50, 24);
    //robot.move_from_last_known(1.0);
    Navegacion nav;
    bool object = false;
    bool wall = false;
    for (int i = 0; i < 1; i++)
    {
	//nav.decide_movement(&robot,object,wall);
        std::cout << "Posición robot : " << robot.get_position() << std::endl;
	for (int z =0; z < 20; z++){
		std::vector<cv::Point3f> points = vision.detect_points(robot.get_position(), robot.get_orientation());
        	if (points.size() != 0){
            		map.InsertPointsInTree(points);
        	}
	}
	object = map.DetectObjectInFront(robot.get_position());
	std::cout <<"Object = " << object << std::endl;
        robot.commit();
    }
        //auto value_left = i2c.getEncoderLeft();
        //auto value_right = i2c.getEncoderRight();
    /*
        //std::cout << "ENCODER LEFT = (" << value_left.first << ", " << value_left.second << ")" << std::endl;
        //std::cout << "ENCODER RIGHT = (" << value_right.first << ", " << value_right.second << ")" << std::endl;

        //auto value = ((double) value_left + (double) value_right) / 2.0;

        //robot.move(value / 12 * WHEEL_CIRCUMFERENCE);

        std::vector<cv::Point3f> points = vision.detect_points(robot.get_position(), robot.get_orientation());
	if (points.size() != 0){
	    map.InsertPointsInTree(points);
	}

        if (std::chrono::steady_clock::now() - start > std::chrono::seconds(18))
            break;

        robot.get_position() = origin;
    }

    robot.commit();
    //i2c.stop();
   */
    map.SaveMapToFile("vista.bt");
    return 0;
}
