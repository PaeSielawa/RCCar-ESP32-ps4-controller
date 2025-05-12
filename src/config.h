#ifndef CONFIG_H
#define CONFIG_H

// Bluetooth credentials
#define BLUETOOTH_NAME "ESP32_PS4_Controller"
#define BLUETOOTH_PIN "1234"

// Pin definitions
#define MOTOR_PIN 23  // Pin for motor control
#define SERVO_PIN 22  // Pin for servo control

// Control parameters
#define MOTOR_MIN_SPEED 0
#define MOTOR_MAX_SPEED 255

// Servo parameters
#define SERVO_CENTER 90      // Środkowa pozycja serwa
#define SERVO_MIN_ANGLE 45   // Maksymalny skręt w lewo
#define SERVO_MAX_ANGLE 135  // Maksymalny skręt w prawo
#define SERVO_DEADZONE 10    // Strefa martwa dla drążka

// Motor servo parameters (temporary for testing)
#define MOTOR_CENTER 90     // Pozycja neutralna
#define MOTOR_MAX_FWD 135   // Maksymalne wychylenie do przodu
#define MOTOR_MAX_REV 45    // Maksymalne wychylenie do tyłu
#define MOTOR_RETURN_SPEED 2 // Prędkość powrotu do pozycji neutralnej

// Other configuration constants
#define MAX_CONNECTION_ATTEMPTS 5
#define CONNECTION_TIMEOUT 10000 // in milliseconds

// Debug settings
#define DEBUG_INTERVAL 100    // Interwał debugowania (ms)
#define LOOP_DELAY 20        // Opóźnienie głównej pętli (ms)

// Motor control parameters
#define MOTOR_ACCELERATION 5  // Przyspieszenie silnika
#define MOTOR_DECELERATION 3  // Opóźnienie silnika

#endif // CONFIG_H