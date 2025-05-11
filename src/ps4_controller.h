#ifndef PS4_CONTROLLER_H
#define PS4_CONTROLLER_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include <PS4Controller.h>

class CarController {
public:
    CarController();
    void connect();
    void readInput();
    bool isConnected();
    void setupPins();

private:
    bool connected;
    Servo steeringServo;
    void controlMotor(int speed);
    void controlSteering(int angle);
};

#endif // PS4_CONTROLLER_H