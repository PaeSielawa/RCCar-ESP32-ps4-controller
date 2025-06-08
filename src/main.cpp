#include <Arduino.h>
#include "ps5_controller.h"
#include "mpu9250_handler.h"
#include "config.h"
#include "speed_sensor.h"
#include "wifi_manager.h"
#include <driver/i2c.h>
#include <Wire.h>

// Inicjalizacja głównych komponentów
CarController car;
MPU9250Handler mpu;
SpeedSensor speedSensor(SPEED_SENSOR_PIN, WHEEL_DIAMETER, PULSES_PER_REVOLUTION);
WiFiManager wifiManager;

// Zmienne dla pomiarów prędkości i WiFi
unsigned long lastSpeedUpdateTime = 0;
unsigned long lastMpuDebugTime = 0;
unsigned long lastDebugTime = 0;

// Zmienne dla diagnostyki czujnika
unsigned long lastPulseCount = 0;

// Zamiana układu bajtów na string w formacie MAC
const char* mac = "84:30:95:D8:A6:7B";

// Uchwyty dla zadań na różnych rdzeniach
TaskHandle_t WiFiTask;
TaskHandle_t ControlTask;

// Funkcja zadania WiFi - działa na rdzeniu 0
void wifiTaskFunction(void * parameter) {
    for(;;) {
        if (millis() - lastSpeedUpdateTime >= DATA_UPDATE_INTERVAL) {
            // Pobierz prędkość i zastosuj dodatkowe filtrowanie fałszywych odczytów
            float speed = speedSensor.getSpeedKmh();
            float distance = speedSensor.getDistanceMeters();
            
            // Sprawdź czy pojazd jest w ruchu na podstawie kontrolera
            bool isMoving = false;
            if (ps5.isConnected()) {
                isMoving = (ps5.R2Value() > MOTOR_DEADZONE || ps5.L2Value() > MOTOR_DEADZONE);
            }
            
            // Jeśli pojazd nie jest w ruchu wg kontrolera, ale prędkość jest niska,
            // uznaj to za fałszywy odczyt
            if (!isMoving && speed < 2.0) {
                speed = 0.0;
            }
            
            // Wyślij dane do ESP32-CAM
            wifiManager.sendSpeedData(speed, distance);
            
            lastSpeedUpdateTime = millis();
        }
        delay(50); // Krótkie opóźnienie, aby dać czas innym zadaniom
    }
}

// Funkcja zadania sterowania - działa na rdzeniu 1
void controlTaskFunction(void * parameter) {
    for(;;) {
        // Aktualizacja czujnika prędkości
        speedSensor.update();
        
        // Obsługa kontrolera PS5 i sterowanie pojazdem
        if (ps5.isConnected()) {
            car.update();
        }
        
        // Dodaj diagnostykę czujnika co 500ms
        unsigned long currentTime = millis();
        if (currentTime - lastDebugTime >= 500) {
            // Informacje o czujniku
            extern volatile unsigned long _pulseCount;
            extern volatile unsigned long _lastPulseTime;
            
            float speed = speedSensor.getSpeedKmh();
            unsigned long pulsesDelta = speedSensor.getPulseCount() - lastPulseCount;
            lastPulseCount = speedSensor.getPulseCount();
            
            unsigned long timeSinceLastPulse = currentTime - speedSensor.getLastPulseTime();
            
            // Wyświetl w konsoli
            Serial.println("--- HC-020K DEBUG ---");
            Serial.printf("Speed: %.2f km/h | Pulses: %lu | Time since pulse: %lu ms\n", 
                          speed, pulsesDelta, timeSinceLastPulse);
                          
            if (ps5.isConnected()) {
                bool isMoving = (ps5.R2Value() > MOTOR_DEADZONE || ps5.L2Value() > MOTOR_DEADZONE);
                Serial.printf("Vehicle moving: %s | R2: %d | L2: %d\n", 
                             isMoving ? "YES" : "NO", ps5.R2Value(), ps5.L2Value());
            }
            
            Serial.println("-------------------");
            
            lastDebugTime = currentTime;
        }
        
        delay(LOOP_DELAY);
    }
}

void setup() {
    // Inicjalizacja portu szeregowego
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n\n===========================================");
    Serial.println("  ESP32 PS5 Controller z HC-020K");
    Serial.println("===========================================");
    
    // Wzmocnione rezystory podciągające dla I2C
    pinMode(SDA_PIN, OUTPUT);
    pinMode(SCL_PIN, OUTPUT);
    digitalWrite(SDA_PIN, HIGH);
    digitalWrite(SCL_PIN, HIGH);
    delay(100);
    
    pinMode(SDA_PIN, INPUT_PULLUP);
    pinMode(SCL_PIN, INPUT_PULLUP);
    delay(100);
    
    // Konfiguracja I2C
    Wire.begin(SDA_PIN, SCL_PIN);
    Wire.setClock(50000);  // 50kHz dla stabilności
    delay(200);
    
    // Inicjalizacja MPU9250 jeśli włączona
    #if MPU9250_ENABLED
    if (mpu.begin()) {
        Serial.println("MPU9250 initialized");
    } else {
        Serial.println("Failed to initialize MPU9250");
    }
    #else
    Serial.println("MPU9250 support disabled in config");
    #endif
    
    // Inicjalizacja serw i silnika
    car.setupPins();
    
    // Inicjalizacja kontrolera PS5
    ps5.begin(mac);        
    Serial.println("ESP32 initialized, waiting for PS5 controller");
    
    // Czekamy na połączenie z kontrolerem
    unsigned long startTime = millis();
    while (!ps5.isConnected() && (millis() - startTime < CONNECTION_TIMEOUT)) {
        delay(100);
    }
    
    if (ps5.isConnected()) {
        Serial.println("PS5 controller connected!");
    } else {
        Serial.println("Failed to connect PS5 controller!");
    }
    
    // Inicjalizacja czujnika prędkości
    speedSensor.begin();
    Serial.println("Speed sensor initialized");
    
    // Inicjalizacja WiFi
    #if WIFI_ENABLED
    if (wifiManager.begin()) {
        Serial.println("WiFi initialized successfully");
    } else {
        Serial.println("WiFi initialization failed");
    }
    #else
    Serial.println("WiFi disabled in config");
    #endif
    
    // Inicjalizacja zmiennych diagnostycznych
    lastDebugTime = millis();
    lastPulseCount = 0;
    
    // Tworzenie zadań dla dwóch rdzeni
    xTaskCreatePinnedToCore(
        wifiTaskFunction,   /* Funkcja zadania */
        "WiFiTask",         /* Nazwa zadania */
        10000,              /* Rozmiar stosu */
        NULL,               /* Parametry */
        1,                  /* Priorytet */
        &WiFiTask,          /* Uchwyt zadania */
        0);                 /* Rdzeń (0) */
        
    xTaskCreatePinnedToCore(
        controlTaskFunction, /* Funkcja zadania */
        "ControlTask",       /* Nazwa zadania */
        10000,               /* Rozmiar stosu */
        NULL,                /* Parametry */
        1,                   /* Priorytet */
        &ControlTask,        /* Uchwyt zadania */
        1);                  /* Rdzeń (1) */
        
    Serial.println("System started. Running tasks on both cores.");
    Serial.println("===========================================");
}

// Bardzo uproszczona pętla główna - główne zadania są wykonywane w osobnych zadaniach
void loop() {
    // Minimalna funkcjonalność, główne zadania są w wątkach na różnych rdzeniach
    delay(100); 
}