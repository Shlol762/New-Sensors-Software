# FlightCompV3 - Rocketry Flight Computer

## Overview

FlightCompV3 is a modular, PID-controlled flight computer designed for model rockets. It is built for the **Teensy 4.1** microcontroller and integrates sensor fusion and active stabilization to maintain flight orientation and manage recovery systems.

### Key Features:

* **Active Stabilization:** Uses a PID control loop to actuate servos for flight correction.
* **Sensor Fusion:** Employs Kalman filters to process raw IMU data (MPU6050) for stable orientation estimates.
* **Altitude Sensing:** Utilizes a BMP280 barometer for accurate real-time altitude tracking.
* **Automated Recovery:** Detects apogee based on altitude and acceleration triggers to deploy a parachute via servo.
* **Modular Architecture:** Clean separation between configuration, hardware drivers, and control logic.

### Code Structure:

```text
FlightCompV3/
├── FlightCompV3.cpp                        <- Main entry point
└── src/
    ├── Config/
    │   └── flightConfig.h                  <- One place to change all constants
    ├── Control/
    │   ├── flightController.cpp            <- Servo control with PID
    │   ├── flightController.h
    │   ├── kalman.cpp
    │   └── kalman.h
    ├── Drivers/
    │   ├── driver_BMP280.cpp
    │   ├── driver_BMP280.h
    │   ├── driver_MPU6050.cpp
    │   └── driver_MPU6050.h
    └── libs/
        ├── MPU6050_light.cpp
        ├── MPU6050_light.h
        ├── PID_v1.cpp
        └── PID_v1.h
```

---

## Development Environment Setup

### 1. Visual Studio Code (Recommended)

The project is natively configured for **PlatformIO**, which provides the best developer experience for this codebase.

1.  **Install VS Code**: Download and install [Visual Studio Code](https://code.visualstudio.com/).
2.  **Install PlatformIO**: Go to the Extensions view in VS Code and search for "PlatformIO IDE". Install it.
3.  **Open Project**:
    * Click on the PlatformIO icon in the sidebar.
    * Select "Open Project" and navigate to the `FlightCompV2_5` directory.
4.  **Build & Upload**: Use the PlatformIO toolbar at the bottom of the window (Checkmark for Build, Arrow for Upload). PlatformIO will automatically download the necessary toolchains and libraries (Adafruit BMP280, etc.) based on the `platformio.ini` file.

### 2. Arduino IDE

While PlatformIO is recommended, you can still develop using the Arduino IDE (version 2.x or later) with some manual configuration.

1.  **Install Arduino IDE**: Download and install the latest version from [arduino.cc](https://www.arduino.cc/en/software).
2.  **Add Teensy Board Support**: Add the board directly via the IDE's built-in Boards Manager:
    * Go to `File -> Preferences`.
    * In the "Additional boards manager URLs" field, paste this link:
        `https://www.pjrc.com/teensy/package_teensy_index.json`
3.  **Install the Teensy Core**: Open the Boards Manager (the board icon in the left sidebar), search for **Teensy**, and click "Install" on the package by Paul Stoffregen.
4.  **Install Libraries**: You must manually install the external dependencies listed in `platformio.ini` via the Library Manager (`Sketch -> Include Library -> Manage Libraries...`)
5.  **Handle Project Structure**: The Arduino IDE strictly requires the main sketch file to share the exact same name as its parent folder. To compile this version:
    * Create a new folder named `FlightCompV2_5`.
    * Rename `FlightCompV2_5.cpp` to `FlightCompV2_5.ino` and place it inside that new folder.
    * Move the entire `src/` directory into that same folder alongside the `.ino` file.
6.  **Configure and Upload**: 
    * Go to `Tools -> Board -> Teensy` and select **Teensy 4.1**.
    * Select your board's connection under `Tools -> Port`.
    * Click the Upload button (right arrow icon) to compile and flash the flight computer.