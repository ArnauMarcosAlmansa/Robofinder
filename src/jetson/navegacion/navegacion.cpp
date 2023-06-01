#include "navegacion.h"


Navegacion::Navegacion() : i2c("/dev/i2c-1", ARDUINO_ADDRESS) { };

void Navegacion::forward(){
    i2c.forward(100, 80, 34);
    wait_finish();
}

void Navegacion::turn_left90() {
	i2c.turn_left(100,80,15);
    wait_finish();
}

void Navegacion::turn_right90() {
	i2c.turn_right(100,80,15);
    wait_finish();
}

void Navegacion::turn_backward() {
    i2c.turn_left(100,80,30);
	wait_finish();
}

void Navegacion::wait_finish() {
    while(!i2c.hasStoped())
        usleep(1000000);
}