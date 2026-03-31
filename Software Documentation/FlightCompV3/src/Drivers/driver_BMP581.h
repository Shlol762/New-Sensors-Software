#ifndef DRIVER_BMP581_H
#define DRIVER_BMP581_H

// #include <Adafruit_BMP280.h>
#include <Adafruit_BMP5xx.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "../Config/flightConfig.h"

class Driver_BMP581 {
    public:
    
        Driver_BMP581();

        bool init();
        void update();

        float getAlt() const;
        float getTemp() const;
        float getPres() const;
    

    private:

        Adafruit_BMP5xx bmp;

        float grndRefPres;

        float currentAlt;
        float currentTemp;
        float currentPres;

};

#endif