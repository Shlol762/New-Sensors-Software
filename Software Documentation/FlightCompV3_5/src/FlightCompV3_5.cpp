#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>

#include "Config/flightConfig.h"
#include "Drivers/driver_MPU6500.h"
#include "Drivers/driver_BMP280.h"
#include "Drivers/driver_GPS.h"
#include "Control/flightController.h"
#include "Comms/Telemetry.h"


Driver_MPU6500 imu;
Driver_BMP280 baro;
Driver_GPS gps;

FlightController controller;
Servo parachuteServo;

Telemetry radio;

double maxAltitude = 0.0;
double altitude = 0.0;
bool parachuteDeployed = false;


void setup() {
    Serial.begin(9600);

    // Drive all SPI chip select pins HIGH BEFORE calling SPI.begin().
    // This prevents any device from being accidentally selected during
    // SPI bus initialisation, which could corrupt bus state.
    pinMode(PIN_CS_MPU,  OUTPUT);
    digitalWrite(PIN_CS_MPU,  HIGH);
    pinMode(PIN_CS_BMP,  OUTPUT);
    digitalWrite(PIN_CS_BMP,  HIGH);
    pinMode(PIN_CS_LORA, OUTPUT);
    digitalWrite(PIN_CS_LORA, HIGH);

    SPI.begin();
    delay(2000);
    Serial.println("Flight Computer V3.5 Booting...");

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW); // Turn on LED builtin to indicate board is alive

    baro.init();
    baro.update(); // Perform initial reading to initialize internal altitude state
    maxAltitude = baro.getAlt(); // Set initial max altitude to current altitude for accurate apogee detection

    imu.init();
    controller.init();

    // Setup the parachute servo
    parachuteServo.attach(PIN_CHUTE_SERVO);
    parachuteServo.write(90); // Locked position

    // Initialize GPS on Serial1 (D13=RX, D14=TX).
    // Sends UBX commands to configure 115200 baud and 10 Hz update rate.
    gps.init();

    // Initialize LoRa SX1278 on SPI1.
    // Pins: NSS=D4, RST=D2, DIO0=D3. Frequency: 433 MHz.
    radio.init();
}

void loop() {
    imu.update();
    baro.update();
    gps.update(); // Non-blocking: drains Serial1 bytes into TinyGPS++ parser

    altitude = baro.getAlt();
    float accelMag = imu.getAccelMag();

    // Track Max Altitude
    if (altitude > maxAltitude) maxAltitude = altitude;

    // Apogee Detection Logic
    if (!parachuteDeployed && (maxAltitude > MIN_ARM_ALTITUDE)) { // First check: must be above min alt. and parachute not deployed.

        if (accelMag < 0.5) { // Second check: Condition for thrust cutoff detection - can be tuned based on testing

            // Final check: Confirm decreasing altitude, This detection logic remains untested. Uncertain if it remains a reliable way to detect apogee.
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

    // Transmit telemetry to Ground Station via LoRa SX1278.
    // Rate-limited internally to once per 500ms.
    // Packet: "alt,maxAlt,accelMag,lat,lng,chute"
    radio.transmitData(
        (float)altitude,
        (float)maxAltitude,
        accelMag,
        gps.getLat(),
        gps.getLng(),
        parachuteDeployed
    );
}
