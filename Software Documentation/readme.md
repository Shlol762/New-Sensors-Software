# Rocketry Flight Computer List

## Version 3

Current flight computer in use. Development in progress.

### Changes from V2.5:

*   **Primary change only:** Altitude/barometric sensor path was swapped from BMP280 to BMP581 (`Adafruit_BMP5xx`-based driver).
*   **No major control-logic improvement claims:** PID, Kalman, and recovery architecture are otherwise carried forward from V2.5.

#### Issues:

*   Telemetry/transmission logic is scaffolded but not yet implemented.
*   Data logging is still not included.
*   Some thresholds are still marked for tuning and need full flight validation.
*   Servo-axis frame mapping still requires final documentation to avoid integration mistakes.



## Version 2.5 

### Features:

*   **Flight Control:** Uses a PID controller with data from an MPU6050 IMU to stabilize the rocket.
*   **Altitude Sensing:** A BMP280 barometer is used for altitude determination.
*   **Parachute Deployment:** The software automatically deploys a parachute at apogee.
*   **Improved Sensor Fusion:** Implemented Kalman filters for smoother and more accurate orientation data from the IMU.
*   **Code Refactoring:** The codebase has been refactored for better modularity and readability.

### Improvements from V2.0:
*   The code has been made more modular and easier to maintain.
    
    * Improves code readability
    * Provides ease for integrating new hardware
    * Frees up main code body clutter -> code logic is clearer

#### Issues:
*   Does not include Data logging logic
*   Does not include transmission/comms logic
*   Confusion between relative orientation of Servos and axes of IMU, could lead to buggy code

## Version 2.0

This version is located in the `old contents/test` directory.

### Synopsis:

This version introduced active flight control to the flight computer.

### Features:

*   **PID Control:** A PID controller was implemented to stabilize the rocket's flight using servos.
*   **Basic IMU and Barometer Integration:** The flight computer could read data from the MPU6050 IMU and BMP280 barometer.
*   **Kalman Filter:** A basic Kalman filter was used to process the IMU data.

---
*Code for versions prior to 2.0 was not comprehensible enough to upload as documentation.*
