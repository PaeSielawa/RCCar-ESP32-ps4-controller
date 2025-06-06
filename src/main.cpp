#include <Arduino.h>
#include "ps5_controller.h"
#include "mpu9250_handler.h"
#include "config.h"

CarController car;
MPU9250Handler mpu;

// Zamiana układu bajtów na string w formacie MAC
const char* mac = "84:30:95:D8:A6:7B";

void setup() {
    Serial.begin(115200);
    
    // Inicjalizacja MPU9250
    if (mpu.begin()) {
        Serial.println("MPU9250 found");
        
        // Opcjonalna kalibracja - można odkomentować w razie potrzeby
        // mpu.calibrate();
    }
    
    car.setupPins();       
    ps5.begin(mac);        
    Serial.println("ESP32 initialized, waiting for PS5 controller");
    
    // Czekamy na połączenie z kontrolerem
    unsigned long startTime = millis();
    while (!ps5.isConnected() && (millis() - startTime < CONNECTION_TIMEOUT)) {
        delay(100);
    }
    
    if (ps5.isConnected()) {
        Serial.println("PS5 controller connected!");
        Serial.println("Ready.");
    } else {
        Serial.println("Failed to connect PS5 controller!");
    }
}

void loop() {
    static unsigned long lastLoopTime = 0;
    static unsigned long lastMpuDebugTime = 0;
    unsigned long currentTime = millis();
    
    // Aktualizacja danych z MPU9250
    mpu.update();
    
    // Wyświetlanie danych co sekundę
    if (currentTime - lastMpuDebugTime >= 1000) {
        mpu.printData();
        lastMpuDebugTime = currentTime;
    }
    
    // Aktualizacja kontrolera samochodu
    if (currentTime - lastLoopTime >= LOOP_DELAY) {
        car.update();
        lastLoopTime = currentTime;
    }
}