ESP32 PS4 Controller Project
This project implements a remote control system using ESP32-WROOM-32 microcontroller and PS4 controller for servo motor control.

Hardware Requirements
ESP32-WROOM-32 development board
PS4 controller
Servo motor
USB cable for programming
Power supply for ESP32
Pin Configuration
Servo control: GPIO22
Motor control: GPIO23
Software Requirements
PlatformIO IDE
Required libraries:
ESP32Servo
PS4-esp32
Installation
Clone this repository:
Open the project in PlatformIO IDE

Install dependencies:

The required libraries will be automatically installed through PlatformIO
Configuration
Hardware Setup
Connect servo to GPIO22
Connect ESP32 to your computer via USB
Make sure to use the correct USB port (COM3 by default)
Software Setup
Update platformio.ini if needed
Compile and upload the code
Open Serial Monitor to view debug information
Usage
Power up the ESP32

Pair the PS4 controller:

Press and hold SHARE button
While holding SHARE, press and hold PS button
Wait for the light bar to start blinking rapidly
Release both buttons
Controls:

Left stick X-axis: Servo control
Center position: 90°
Left maximum: 45°
Right maximum: 135°
Deadzone implemented for stability
Debugging
Serial output provides debugging information:
Stick position
Servo angle
Deadzone status
Baud rate: 115200
Project Structure
License
This project is open-source and available under the MIT License.

Contributing
Feel free to submit issues and pull requests.

Authors
Your Name
Acknowledgments
Thanks to the ESP32Servo library developers
Thanks to the PS4-esp32 library developers
