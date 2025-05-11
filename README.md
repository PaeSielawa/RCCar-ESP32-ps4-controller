# README.md

# ESP32 PS4 Controller Project

This project allows you to control an ESP32 using a PS4 controller. It provides functionalities to connect to the controller, read input data, and handle button presses.

## Project Structure

```
esp32-ps4-controller
├── src
│   ├── main.cpp            # Entry point of the application
│   ├── ps4_controller.cpp   # Implementation of PS4 controller functionalities
│   ├── ps4_controller.h     # Header file for PS4Controller class
│   └── config.h            # Configuration settings
├── platformio.ini          # PlatformIO configuration file
├── lib
│   └── README.md           # Documentation for libraries used
└── README.md               # Project documentation
```

## Setup Instructions

1. Clone the repository:
   ```
   git clone <repository-url>
   ```

2. Navigate to the project directory:
   ```
   cd esp32-ps4-controller
   ```

3. Open the project in your preferred IDE.

4. Install the required libraries specified in `platformio.ini`.

5. Connect your ESP32 board to your computer.

6. Upload the code to the ESP32 using PlatformIO.

## Usage Guidelines

- Ensure your PS4 controller is in pairing mode.
- The ESP32 will attempt to connect to the controller upon startup.
- Use the defined methods in the `PS4Controller` class to interact with the controller.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.