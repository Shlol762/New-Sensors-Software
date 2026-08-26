# FlightCompV3_5 - Rocketry Flight Computer

## Overview

FlightCompV3_5 is the test version of the rocketry flight computer running on the **Arduino Portenta H7 (STM32H747XI - Cortex-M7 Core)**. It features high-speed **SPI communication** for all onboard flight sensors: the **MPU6500** 6-DOF IMU and the **BMP280** Barometric Pressure / Altitude Sensor.

### Key Features:

* **Active Stabilization:** Uses a dual-axis PID control loop to actuate servos for aerodynamic flight correction.
* **Sensor Fusion:** Employs complementary and Kalman filters to process raw IMU data from the MPU6500 over high-speed SPI.
* **Altitude Sensing:** Utilizes a BMP280 barometer over SPI for real-time altitude tracking and ground-reference relative apogee calculations.
* **Automated Recovery:** Detects apogee based on altitude drop and motor burnout acceleration thresholds to trigger parachute deployment via servo.
* **Modular Architecture:** Clean separation between configuration, hardware drivers, filtering, and control logic.

### Hardware & Pinout Mapping (Portenta H7)

| Function / Peripheral | Portenta Pin | Description |
| :--- | :--- | :--- |
| **SPI SCK** | `D9` / `PI1` | Shared Hardware SPI Clock |
| **SPI MOSI** | `D8` / `PC3` | Shared Hardware SPI Master Out |
| **SPI MISO** | `D10` / `PC2` | Shared Hardware SPI Master In |
| **MPU6500 CS** | `D7` | Chip Select for MPU6500 IMU |
| **BMP280 CS** | `D6` | Chip Select for BMP280 Barometer |
| **Servo X** | `D0` | X-Axis TVC / Fin Actuation Servo |
| **Servo Y** | `D1` | Y-Axis TVC / Fin Actuation Servo |
| **Chute Servo** | `D2` | Parachute Deployment Mechanism |

*Note: All pin numbers are configurable in `src/Config/flightConfig.h`.*

---

### Code Structure:

```text
FlightCompV3_5/
├── platformio.ini                    <- PlatformIO target & dependency config
├── README.md                         <- Documentation
└── src/
    ├── FlightCompV3_5.cpp            <- Main setup() and loop() entry point
    ├── Config/
    │   └── flightConfig.h            <- Pin definitions, PID gains, thresholds
    ├── Control/
    │   ├── flightController.cpp      <- Servo control with PID
    │   ├── flightController.h
    │   ├── kalman.cpp                <- 1D Kalman state estimator
    │   └── kalman.h
    ├── Drivers/
    │   ├── driver_BMP280.cpp         <- SPI Barometer driver (Adafruit BMP280)
    │   ├── driver_BMP280.h
    │   ├── driver_MPU6500.cpp        <- SPI IMU driver wrapper
    │   └── driver_MPU6500.h
    └── libs/
        ├── MPU6500_SPI.cpp           <- Custom high-speed SPI MPU6500 driver
        ├── MPU6500_SPI.h
        ├── PID_v1.cpp                <- Standard PID library
        └── PID_v1.h
```

---

## Development Environment Setup

### 1. Visual Studio Code & PlatformIO (Recommended)

The project is configured for **PlatformIO** targeting the Portenta H7 M7 core:

1. **Install VS Code**: Download and install [Visual Studio Code](https://code.visualstudio.com/).
2. **Install PlatformIO**: Go to the Extensions view in VS Code and install "PlatformIO IDE".
3. **Open Project**: Select "Open Project" and choose the `FlightCompV3_5` directory.
4. **Build & Upload**:
   - Click the checkmark icon (`✓`) in the PlatformIO bottom bar to build firmware.
   - Click the arrow icon (`→`) to upload to the Portenta H7 connected via USB-C.

### 2. Arduino IDE (Alternative)

To compile using Arduino IDE (version 2.x or later):

1. **Install Board Core**: In Boards Manager, search for **Arduino Mbed OS Portenta Boards** and click Install.
2. **Select Board**: Under `Tools -> Board`, choose **Arduino Portenta H7 (M7 core)**.
3. **Install Libraries**: Open Library Manager (`Sketch -> Include Library -> Manage Libraries...`) and install:
   - `Adafruit BMP280 Library` (and its dependencies: `Adafruit Unified Sensor`, `Adafruit BusIO`)
   - `Servo`
4. **Rename Main File**: In Arduino IDE, the sketch folder and main file must match. Rename `src/FlightCompV3_5.cpp` to `src/FlightCompV3_5.ino`.