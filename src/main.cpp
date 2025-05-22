#include <Arduino.h>
#include "ps5_controller.h"
#include "config.h"

CarController car;

// Zamiana układu bajtów na string w formacie MAC
const char* mac = "84:30:95:D8:A6:7B";

void setup() {
    Serial.begin(115200);  
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
    unsigned long currentTime = millis();
    
    if (currentTime - lastLoopTime >= LOOP_DELAY) {
        car.update();
        lastLoopTime = currentTime;
    }
}