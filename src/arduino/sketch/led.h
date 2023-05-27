#include <Adafruit_NeoPixel.h>

class Led {
  private:
    int pin;
    int numLeds;
    int brightness;
    int led_width;
    Adafruit_NeoPixel pixels;
  
  public:
    Led(int pin, int numLeds);
    void begin();
    void stop();
    void forward();
    void backward();
    void setBrightness(int brightness);
    void off();
    void setPixelColor(int x, int y, uint32_t color);
    void arrowLeft();
    void arrowRight();
    void startup();
    void ide();
    void rainbowCycle(uint8_t wait);
    uint32_t Wheel(byte WheelPos);
    
    /*
    void colorWipe(uint32_t c, uint8_t wait);
    void rainbow(uint8_t wait);
    void theaterChase(uint32_t c, uint8_t wait);
    void theaterChaseRainbow(uint8_t wait);
    */
};
