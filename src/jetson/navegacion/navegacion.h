#include "../i2c/I2C.h"


class Navegacion {
    private:
        I2C i2c;

    public:

        Navegacion();

        void forward();
        void turn_left90();
        void turn_right90();

        void turn_backward();

        void wait_finish();

};