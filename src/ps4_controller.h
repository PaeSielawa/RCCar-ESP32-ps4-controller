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
    bool steeringServoInitialized;
    bool motorServoInitialized;
    unsigned long lastDebugTime;
    const unsigned long DEBUG_INTERVAL = 100; // ms między debugowaniem
    Servo steeringServo;
    Servo motorServo;      // Drugie serwo symulujące silnik
    int currentMotorAngle; // Aktualna pozycja serwa silnika
    void controlMotor(int speed);
    void controlSteering(int angle);
    void updateMotor();    // Nowa metoda do aktualizacji pozycji silnika
};

#endif // PS4_CONTROLLER_H