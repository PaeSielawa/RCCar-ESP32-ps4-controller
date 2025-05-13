#include <Arduino.h>
#include "ps5_controller.h"
#include "config.h"

CarController car;

void setup() {
    Serial.begin(115200);
    car.setupPins();
    car.begin();
    Serial.println("ESP32 initialized, waiting for PS5 controller");
}

void loop() {
    static unsigned long lastLoopTime = 0;
    unsigned long currentTime = millis();
    
    if (currentTime - lastLoopTime >= LOOP_DELAY) {
        car.update();
        lastLoopTime = currentTime;
    }
}