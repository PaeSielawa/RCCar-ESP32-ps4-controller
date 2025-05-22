#ifndef PS5_CONTROLLER_H
#define PS5_CONTROLLER_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include <PS5Controller.h>

class CarController {
public:
    CarController();
    void setupPins();
    bool isConnected();
    void update();

private:
    bool steeringServoInitialized;
    bool motorServoInitialized;
    unsigned long lastDebugTime;
    const unsigned long DEBUG_INTERVAL = 100;
    
    Servo steeringServo;
    Servo motorServo;
    int currentMotorAngle;
    
    void controlMotor(int angle);
    void controlSteering(int angle);
    void handleSteeringInput();
    void handleMotorInput();
    void debugOutput();
};

#endif