#ifndef CONFIG_H
#define CONFIG_H

#define MPU9250_ENABLED false  // Zmień na false aby wyłączyć obsługę MPU9250

// Bluetooth credentials
#define BLUETOOTH_NAME "ESP32_PS5_Controller"
//#define BLUETOOTH_PIN "1234"

// Pin definitions
#define MOTOR_PIN 23  // Pin for motor control
#define SERVO_PIN 22  // Pin for servo control

// MPU9250 pins - używane dla I2C
#define SDA_PIN 18
#define SCL_PIN 19

// Usunięte zbędne parametry
// #define MOTOR_MIN_SPEED 0
// #define MOTOR_MAX_SPEED 255

// Servo parameters
#define SERVO_CENTER 90      // Środkowa pozycja serwa
#define SERVO_MIN_ANGLE 45   // Maksymalny skręt w lewo
#define SERVO_MAX_ANGLE 135  // Maksymalny skręt w prawo
#define SERVO_DEADZONE 10    // Strefa martwa dla drążka

// Motor servo parameters (for DC brushed motor)
#define MOTOR_CENTER 90     // Pozycja neutralna (zatrzymanie)
#define MOTOR_MAX_FWD 140   // Maksymalna prędkość do przodu
#define MOTOR_MAX_REV 40    // Maksymalna prędkość do tyłu
#define MOTOR_MIN_FWD 100   // Minimalna prędkość do przodu (aby pokonać tarcie)
#define MOTOR_MIN_REV 80    // Minimalna prędkość do tyłu (aby pokonać tarcie)
#define MOTOR_DEADZONE 10   // Strefa martwa dla triggerów

// Other configuration constants
#define MAX_CONNECTION_ATTEMPTS 5
#define CONNECTION_TIMEOUT 10000 // in milliseconds

// Debug settings
#define DEBUG_INTERVAL 100    // Interwał debugowania (ms)
#define LOOP_DELAY 20        // Opóźnienie głównej pętli (ms)

// Usunięte zbędne parametry
// #define MOTOR_ACCELERATION 5  // Przyspieszenie silnika
// #define MOTOR_DECELERATION 3  // Opóźnienie silnika

#endif // CONFIG_H