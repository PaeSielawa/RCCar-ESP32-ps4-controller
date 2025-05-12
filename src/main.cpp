#include <Arduino.h>
#include "ps4_controller.h"
#include "config.h"

CarController car;

void setup() {
    Serial.begin(115200);
    car.setupPins();
    car.connect();
    Serial.println("ESP32 initialized, waiting for PS4 controller");
}

void loop() {
    static unsigned long lastLoopTime = 0;
    unsigned long currentTime = millis();
    
    if (currentTime - lastLoopTime >= LOOP_DELAY) {
        if (car.isConnected()) {
            car.readInput();
        }
        lastLoopTime = currentTime;
    }
}