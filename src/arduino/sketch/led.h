#include <Adafruit_NeoPixel.h>

class Led {
  private:
    int pin;
    int numLeds;
    int brightness;
    int led_width;
    Adafruit_NeoPixel pixels;
  
  public:
    Led(int pin, int numLeds) : pixels(numLeds, pin, NEO_GRB + NEO_KHZ800) {
      this->pin = pin;
      this->numLeds = numLeds;
      this->brightness = 10; // Brillo máximo (0-255)
      this->led_width = 8;
    }
    
    void begin() {
      pixels.begin();
      pixels.setBrightness(brightness); // Establecer el brillo al iniciar
      pixels.show();
    }
    
    void stop() {
      uint32_t redColor = pixels.Color(255, 0, 0); // Rojo (R, G, B)
      for (int i = 0; i < numLeds; i++) {
        pixels.setPixelColor(i, redColor);
      }
      pixels.show();
    }
    
    void forward() {
      uint32_t greenColor = pixels.Color(0, 255, 0); // Verde (R, G, B)
      for (int i = 0; i < numLeds; i++) {
        pixels.setPixelColor(i, greenColor);
      }
      pixels.show();
    }
    

    void backward() {
      uint32_t greenColor = pixels.Color(255, 255, 255); // Verde (R, G, B)
      for (int i = 0; i < numLeds; i++) {
        pixels.setPixelColor(i, greenColor);
      }
      pixels.show();
    }

    void setBrightness(int brightness) {
      this->brightness = brightness;
      pixels.setBrightness(brightness);
      pixels.show();
    }

    void off() {
      pixels.clear(); // Apagar todos los LEDs
      pixels.show(); // Mostrar los LEDs apagado
    }



    void Led::setPixelColor(int x, int y, uint32_t color) {
      int index = y * led_width + x;
      pixels.setPixelColor(index, color);
    }


    void Led::arrowLeft() {
      pixels.clear(); // Apagar todos los LEDs
      pixels.show(); // Mostrar los LEDs apagado

      // Pixeles que forman la cabeza de la flecha.
      int arrowLeft[] = {10,13,17,18,21,22,24,25,30,31,32,39,3,4,11,12,19,20,27,28,35,36,43,44,51,52,59,60,67,68,75,76};
      int numValues = sizeof(arrowLeft) / sizeof(arrowLeft[0]);
      for (int i = 0; i < numValues; i++) {
        int value = arrowLeft[i];
        pixels.setPixelColor(value, pixels.Color(252, 72, 0));
      }
      pixels.show();


      // Pixeles que forman la cola de la flecha.
      // Encender.
      for (int i=32; i>9; i--) {
        pixels.setPixelColor((i*8)+3, pixels.Color(252, 72, 0));
        pixels.setPixelColor((i*8)+4, pixels.Color(252, 72, 0));
        pixels.show();
        delay(30);
      }
      pixels.show();
    
      // Apagar
      for (int i=32; i>9; i--) {
        pixels.setPixelColor((i*8)+3, pixels.Color(0, 0, 0));
        pixels.setPixelColor((i*8)+4, pixels.Color(0, 0, 0));
      }
      pixels.show();

      delay(100);
    }

    void Led::arrowRight() {
      pixels.clear(); // Apagar todos los LEDs
      pixels.show(); // Mostrar los LEDs apagado

      // Pixeles que forman la cabeza de la flecha.
      int arrowRight[] = {252, 251, 242, 243, 244, 245, 238, 237, 236, 235, 234,233,231,230,228,227,225,224,223,220,219,216, 212,211, 204,203,196, 195,188,187};
      int numValues = sizeof(arrowRight) / sizeof(arrowRight[0]);
      for (int i = 0; i < numValues; i++) {
        int value = arrowRight[i];
        pixels.setPixelColor(value, pixels.Color(252, 72, 0));
      }
      pixels.show();


      // Pixeles que forman la cola de la flecha.
      // Encender.
      for (int i=0; i<23; i++) {
        pixels.setPixelColor((i*8)+3, pixels.Color(252, 72, 0));
        pixels.setPixelColor((i*8)+4, pixels.Color(252, 72, 0));
        pixels.show();
        delay(30);
      }
      pixels.show();
    
      // Apagar
      for (int i=0; i<23; i++) {
        pixels.setPixelColor((i*8)+3, pixels.Color(0, 0, 0));
        pixels.setPixelColor((i*8)+4, pixels.Color(0, 0, 0));
      }
      pixels.show();

      delay(100);
    }


    void Led::startup(){
        rainbowCycle(0);
        this->ide();
    }

    void Led::ide() {
      
      
      for(int i=0;i<numLeds; i++) {
        pixels.setPixelColor(i, pixels.Color(0,0,100));
      }
      pixels.show();
      //while (true) {
      /*
        colorWipe(pixels.Color(255, 0, 0), 50); // Rojo
        colorWipe(pixels.Color(0, 255, 0), 50); // Verde
        colorWipe(pixels.Color(0, 0, 255), 50); // Azul
        theaterChase(pixels.Color(127, 127, 127), 50); // Blanco
        theaterChase(pixels.Color(127, 0, 0), 50); // Rojo
        theaterChase(pixels.Color(0, 0, 127), 50); // Azul
      */
        //rainbow(0);
        
          //rainbowCycle(0);
        //theaterChaseRainbow(20);
      //}
    }

    void Led::colorWipe(uint32_t c, uint8_t wait) {
      for (uint16_t i = 0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, c);
      }
      pixels.show();
      delay(wait);
    }

    void Led::rainbow(uint8_t wait) {
      for (uint16_t j = 0; j < 256; j++) {
        for (uint16_t i = 0; i < pixels.numPixels(); i++) {
          pixels.setPixelColor(i, Wheel((i + j) & 255));
        }
        pixels.show();
        delay(wait);
      }
    }

    void Led::rainbowCycle(uint8_t wait) {
      for (uint16_t j = 0; j < 256; j++) {
        for (uint16_t i = 0; i < pixels.numPixels(); i++) {
          pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
        }
        pixels.show();
        //delay(wait);
      }
    }

    void Led::theaterChase(uint32_t c, uint8_t wait) {
      for (int q = 0; q < 3; q++) {
        for (uint16_t i = q; i < pixels.numPixels(); i += 3) {
          pixels.setPixelColor(i, c);
        }
        pixels.show();
        delay(wait);
        for (uint16_t i = q; i < pixels.numPixels(); i += 3) {
          pixels.setPixelColor(i, 0);
        }
      }
    }

    void Led::theaterChaseRainbow(uint8_t wait) {
      for (int j = 0; j < 256; j++) {
        for (int q = 0; q < 3; q++) {
          for (uint16_t i = q; i < pixels.numPixels(); i += 3) {
            pixels.setPixelColor(i, Wheel((i + j) % 255));
          }
          pixels.show();
          delay(wait);
          for (uint16_t i = q; i < pixels.numPixels(); i += 3) {
            pixels.setPixelColor(i, 0);
          }
        }
      }
    }

    uint32_t Led::Wheel(byte WheelPos) {
      if (WheelPos < 85) {
        return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
      } else if (WheelPos < 170) {
        WheelPos -= 85;
        return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
      } else {
        WheelPos -= 170;
        return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
      }
    }

};
