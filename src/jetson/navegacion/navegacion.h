#include "../i2c/I2C.h"


class Navegacion {
    private:
        I2C i2c;

    public:

        Navegacion();

        int forward();
        int turn_left90();
        int turn_right90();
        
        int turn_backward();

        void wait_finish();
        int get_encoder_value();

};