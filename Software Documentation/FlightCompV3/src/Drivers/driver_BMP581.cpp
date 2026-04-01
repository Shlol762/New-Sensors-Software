#include "driver_BMP581.h"


Driver_BMP581::Driver_BMP581() : bmp() {
  // Constructor can be empty or used for initialization if needed
}


// Initialize the BMP581 sensor
bool Driver_BMP581::init() {


    if (!bmp.begin(BMP5XX_DEFAULT_ADDRESS, &Wire)) {        // Try the default I2C address
        if (!bmp.begin(BMP5XX_ALTERNATIVE_ADDRESS, &Wire)) {      // Try the alternate I2C address
            Serial.println(F("Could not find a valid BMP581 sensor, check wiring!"));

            while (1); // Halt if no sensor is found
        
        }
    }
    
  
    bmp.setTemperatureOversampling(bmp5xx_oversampling_t::BMP5XX_OVERSAMPLING_1X);   // Temp. oversampling
    bmp.setPressureOversampling(bmp5xx_oversampling_t::BMP5XX_OVERSAMPLING_4X);      // Pressure oversampling    
    bmp.setIIRFilterCoeff(bmp5xx_iir_filter_t::BMP5XX_IIR_FILTER_COEFF_3);           // Filtering coefficient - 3 in BMP581 might be similar to 4 in BMP280, but must compare datasheets for exact behavior
    bmp.setOutputDataRate(bmp5xx_odr_t::BMP5XX_ODR_240_HZ);                           // Standby time - 240 Hz means ~4 ms bw measurements. Need to find setting to bring down to 1ms
    bmp.setPowerMode(bmp5xx_powermode_t::BMP5XX_POWERMODE_NORMAL);                   // Operating mode - normal mode for continuous measurements
    
    bmp.enablePressure(true); 
    
    // Calculate ground reference pressure by averaging multiple readings
    grndRefPres = 0;
    int successCount = 0;
    for(int i=0; i<10; i++) {
        if (bmp.performReading()) {
            grndRefPres += bmp.pressure; // bmp pressure is already in hPa
            successCount++;
        }
        delay(10);
    }

    if (successCount > 0) {
        grndRefPres /= static_cast<float>(successCount); // Average the successful readings
    } else {
        // Fallback to standard sea-level pressure if no readings succeeded
        grndRefPres = 1013.25f;
    }
    Serial.print("Ground Pressure: ");
    Serial.print(grndRefPres);
    Serial.println(" hPa");

    return true; // Initialization successful
}

void Driver_BMP581::update() {
    // Read the latest sensor data and store it in the member variables

    if (!bmp.dataReady()) return; // If data is not ready, skip the update

    if (!bmp.performReading()) return; // If reading fails, skip the update

    currentTemp = bmp.temperature;
    currentPres = bmp.pressure;
    currentAlt = bmp.readAltitude(grndRefPres); // Use ground reference pressure for altitude calculation
}

float Driver_BMP581::getAlt() const {
    return currentAlt;
}

float Driver_BMP581::getTemp() const {
    return currentTemp;
}

float Driver_BMP581::getPres() const {
    return currentPres;
}

