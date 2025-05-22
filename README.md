# ESP32 PS5 Controller Project

## Description
This project implements a remote control system using ESP32-WROOM-32 microcontroller and PS5 controller. It enables precise servo motor control through wireless communication, making it suitable for robotics and RC applications.

## Features
- Wireless control using PS5 controller
- Precise servo motor control
- Configurable control parameters
- Debug output via Serial
- Easy pairing process

## Hardware Requirements
- ESP32-WROOM-32 development board
- PS5 controller
- Servo motor
- USB cable for programming
- 5V power supply
- Jumper wires

## Software Requirements
- PlatformIO IDE
- ESP32 Arduino framework
- Required libraries:
  - ESP32Servo
  - ps5-esp32

## Installation
1. Clone the repository
```bash
git clone https://github.com/PaeSielawa/RCCar-ESP32-ps5-controller.git
```
2. Open project in PlatformIO
3. Configure pins in `config.h`:
```cpp
#define MOTOR_PIN 23  // Pin for motor control
#define SERVO_PIN 22  // Pin for servo control
```

## Wiring
Connect components to ESP32-WROOM-32:
|   Component     |   ESP32 Pin    |
|-----------------|----------------|
| Servo Signal    | GPIO22         |
| Motor Control   | GPIO23         |
| Power (5V)      | VIN            |
| Ground          | GND            |

## Project Configuration
Project settings in `platformio.ini`:
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
upload_port = COM3
upload_speed = 57600
monitor_speed = 115200
```

## Usage
1. Upload the code to ESP32-CAM
2. Pair PS5 controller:
    - Hold SHARE button
    - While holding SHARE, press PS button
    - Wait for rapid light bar flashing
3. Check serial monitor for IP address
    - Left stick X-axis: Servo control (45° - 135°)
    - Center: 90° position

## Control Parameters
Default configuration:
- Servo range: 45° - 135°
- Center position: 90°
- Motor speed range: 0-255
- Debug level: 5 (detailed)

## Troubleshooting
- **Controller won't pair**: Reset controller using reset button
- **Upload fails**: Check COM port and USB connection
- **Servo not responding**: Verify pin connections and power
- **Connection issues**: Ensure proper power supply

## Contributing
1. Fork the repository
2. Create feature branch (`git checkout -b feature/YourFeature`)
3. Commit changes (`git commit -m 'Add some feature'`)
4. Push to branch (`git push origin feature/YourFeature`)
5. Open Pull Request

## Acknowledgments
- ESP32Servo library developers
- ps5-esp32 library developers
- PlatformIO development platform
  
## License
This project was created by PS. All rights reserved.
