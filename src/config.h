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
#define SERVO_MIN_ANGLE 20   // Maksymalny skręt w lewo
#define SERVO_MAX_ANGLE 160  // Maksymalny skręt w prawo
#define SERVO_DEADZONE 10    // Strefa martwa dla drążka

// Motor servo parameters (for DC brushed motor)
#define MOTOR_CENTER 90     // Pozycja neutralna (zatrzymanie) - nie zmieniamy
#define MOTOR_MAX_FWD 115    // Maksymalna prędkość do przodu (około 30% oryginalnej mocy)
#define MOTOR_MAX_REV 70    // Maksymalna prędkość do tyłu (około 30% oryginalnej mocy)
#define MOTOR_MIN_FWD 93    // Minimalna prędkość do przodu (jeszcze mniejsza)
#define MOTOR_MIN_REV 87    // Minimalna prędkość do tyłu (jeszcze mniejsza)
#define MOTOR_DEADZONE 10    // Zmniejszona strefa martwa dla większej czułości

// Other configuration constants
#define MAX_CONNECTION_ATTEMPTS 5
#define CONNECTION_TIMEOUT 10000 // in milliseconds

// Debug settings
#define DEBUG_INTERVAL 100    // Interwał debugowania (ms)
#define LOOP_DELAY 20        // Opóźnienie głównej pętli (ms)

// WiFi settings for speed data transmission
#define WIFI_ENABLED 1  // Włącz/wyłącz WiFi (1 = true, 0 = false)
#define USE_WIFI_AP_MODE 0  // 1 = Access Point, 0 = Client

// Access Point settings (ESP32 tworzy własną sieć)
#define AP_SSID "ESP32-Car"
#define AP_PASSWORD "12345678"
#define AP_CHANNEL 1
#define AP_HIDDEN false
#define AP_MAX_CONNECTIONS 1

// Client settings (ESP32 łączy się z istniejącą siecią)
#define CLIENT_SSID "S23"
#define CLIENT_PASSWORD "1234567890"

// ESP32-CAM settings
#define ESP32CAM_IP "192.168.251.156"  // Domyślny adres IP ESP32-CAM gdy działa jako AP
#define ESP32CAM_PORT 80
#define DATA_UPDATE_INTERVAL 500  // Jak często wysyłać dane (ms)

// HC-020K settings
#define SPEED_SENSOR_PIN 21  // Pin do którego podłączamy HC-020K
#define WHEEL_DIAMETER 0.065  // Średnica koła w metrach
#define PULSES_PER_REVOLUTION 20  // Ilość impulsów na jeden obrót koła

#endif // CONFIG_H