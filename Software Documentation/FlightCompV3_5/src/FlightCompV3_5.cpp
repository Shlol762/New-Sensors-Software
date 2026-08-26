#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>

#include "Config/flightConfig.h"
#include "Drivers/driver_MPU6500.h"
#include "Drivers/driver_BMP280.h"
#include "Control/flightController.h"
// #include "Comms/Telemetry.h"

Driver_MPU6500 imu;
Driver_BMP280 baro;

FlightController controller;
Servo parachuteServo;

// Telemetry radio;

double maxAltitude = 0.0;
double altitude = 0.0;
bool parachuteDeployed = false;

void setup() {
    Serial.begin(9600);

    // Initialize SPI and Chip Select pins
    SPI.begin();
    pinMode(PIN_CS_MPU, OUTPUT);
    digitalWrite(PIN_CS_MPU, HIGH);
    pinMode(PIN_CS_BMP, OUTPUT);
    digitalWrite(PIN_CS_BMP, HIGH);

    delay(2000);
    Serial.println("Flight Computer Booting...");

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH); // Turn on built-in LED to indicate that Portenta H7 is alive
    
    baro.init();
    baro.update(); // Perform initial reading to initialize internal altitude state
    maxAltitude = baro.getAlt(); // Set initial max altitude to current altitude for accurate apogee detection

    imu.init();
    controller.init(); 

    // Setup the parachute
    parachuteServo.attach(PIN_CHUTE_SERVO);
    parachuteServo.write(90); // Locked position
    
    // radio.init();
}

void loop() {
    imu.update();
    baro.update();
    
    altitude = baro.getAlt();
    double accelMag = imu.getAccelMag();

    // Track Max Altitude
    if (altitude > maxAltitude) {
        maxAltitude = altitude;
    }

    // A. Apogee Detection Logic
    if (!parachuteDeployed && maxAltitude > MIN_ARM_ALTITUDE) {
        if (accelMag < 0.5) { // Condition for thrust cutoff detection - can be tuned based on testing
            
            // This detection logic remains untested. Uncertain if it remains a reliable way to detect apogee.
            if ((maxAltitude - altitude) >= APOGEE_DROP_THRES) {
                parachuteServo.write(180); // Deploy parachute

                parachuteDeployed = true;

                Serial.println("\n!!! APOGEE DETECTED - PARACHUTE DEPLOYED !!!");
            }
        }
    }

    controller.update(imu.getRawDiffX(), imu.getRawDiffZ()); // Y is optional for now, can be added in future versions

    Serial.print("\nAltitude: ");
    Serial.print(altitude);
    Serial.print(" m | Max Altitude: ");
    Serial.print(maxAltitude);
    Serial.print(" m | Accel Mag: ");
    Serial.print(accelMag);
    Serial.println(" m/s^2");
    
    // F. Transmit Data to Ground Station
    // radio.transmitData(altitude, controller.getPitch(), controller.getRoll());
}
