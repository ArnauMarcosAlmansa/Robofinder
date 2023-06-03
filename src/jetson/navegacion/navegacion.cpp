#include "navegacion.h"


Navegacion::Navegacion() : i2c("/dev/i2c-1", ARDUINO_ADDRESS) { };

int Navegacion::forward(){
    i2c.forward(100, 80, 34);
    wait_finish();
    return get_encoder_value();
}

int Navegacion::turn_left90() {
	i2c.turn_left(100,80,15);
    wait_finish();
    return get_encoder_value();
}

int Navegacion::turn_right90() {
	i2c.turn_right(100,80,15);
    wait_finish();
    return get_encoder_value();
}

int Navegacion::turn_backward() {
    i2c.turn_left(100,80,31);
	wait_finish();
    return get_encoder_value();
}

void Navegacion::wait_finish() {
    while(!i2c.hasStoped())
        usleep(1000000);
}

int Navegacion::get_encoder_value() {
    std::pair<int,int> rightValue = i2c.getEncoderRight();
    return rightValue.second;
}

void Navegacion::decide_movement(Robot* robot,bool object,bool wall){
	if (object == true){
		(*robot).move_from_last_known_with_pulses(this->turn_backward());
	}else{
		if(wall == true){
                	std::cout <<"TODO" << std::endl;
		}else{
			(*robot).move_from_last_known_with_pulses(this->forward());
		}
	}
}
