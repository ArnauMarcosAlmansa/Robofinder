#include "navegacion.h"


Navegacion::Navegacion(I2C& i2c) : i2c(i2c) { };

int Navegacion::forward(){
    i2c.forward(100, 80, 12);
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

int Navegacion::turn_right135() {
	i2c.turn_right(100, 80, 23);
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
    std::pair<int,int> leftValue = i2c.getEncoderLeft();
    return (std::abs(rightValue.second) + std::abs(leftValue.second)) / 2;
}

void Navegacion::decide_movement(Robot* robot,bool object,bool wall){
	if (object == true){
		(*robot).move_from_last_known_with_pulses(this->turn_backward());
	}else{
		if(wall == true){
            (*robot).move_from_last_known_with_pulses(this->turn_backward());
		}else{
			(*robot).move_from_last_known_with_pulses(this->forward());
		}
	}
}
