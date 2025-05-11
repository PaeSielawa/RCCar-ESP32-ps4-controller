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
    if (car.isConnected()) {
        car.readInput();
    }
    delay(20); // Small delay for stability
}