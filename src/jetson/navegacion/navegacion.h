#pragma once
#include "../i2c/I2C.h"
#include "../Robot.h"
#ifndef NAVEGACION_H
#define NAVEGACION_H
class Navegacion {
    private:
        I2C& i2c;

    public:

        Navegacion(I2C& i2c);

        int forward();
        int turn_left90();
        int turn_right90();
        int turn_right135();
    	void decide_movement(Robot* robot,bool object,bool wall);
        int turn_backward();
	int turn_right_pulses(int pulses);
	int turn_left_pulses(int pulses); 
        void wait_finish();
        int get_encoder_value();

};

#endif
