#include "ps5_controller.h"
#include "config.h"

CarController::CarController() {
    steeringServoInitialized = false;
    motorControllerInitialized = false;
    currentMotorValue = MOTOR_CENTER;
    lastDebugTime = 0;
}

void CarController::setupPins() {
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    
    steeringServo.setPeriodHertz(50);
    if (steeringServo.attach(SERVO_PIN, 500, 2400)) {
        steeringServoInitialized = true;
        steeringServo.write(SERVO_CENTER);
        Serial.println("Steering servo initialized");
    }
    
    motorController.setPeriodHertz(50);
    if (motorController.attach(MOTOR_PIN, 500, 2400)) {
        motorControllerInitialized = true;
        motorController.write(MOTOR_CENTER);
        Serial.println("Motor servo initialized");
    }
}

bool CarController::isConnected() {
    return ps5.isConnected();
}

void CarController::update() {
    if (!steeringServoInitialized || !motorControllerInitialized) return;
    
    if (ps5.isConnected()) {
        handleSteeringInput();
        handleMotorInput();
        debugOutput();
    }
}

void CarController::handleSteeringInput() {
    int leftStickX = ps5.LStickX();
    int steeringAngle = map(leftStickX, -127, 127, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
    
    if (abs(leftStickX) < SERVO_DEADZONE) {
        steeringAngle = SERVO_CENTER;
    }
    controlSteering(steeringAngle);
}

void CarController::handleMotorInput() {
    int motorPower = MOTOR_CENTER;
    
    // Jazda do przodu
    if (ps5.R2()) {
        int r2Value = ps5.R2Value();
        if (r2Value < MOTOR_DEADZONE) {
            // Ignorujemy delikatne naciśnięcia triggera
            motorPower = MOTOR_CENTER;
        } else {
            // Mapujemy wartość triggera na zakres prędkości
            motorPower = map(r2Value, MOTOR_DEADZONE, 255, MOTOR_MIN_FWD, MOTOR_MAX_FWD);
        }
    }
    // Jazda do tyłu
    else if (ps5.L2()) {
        int l2Value = ps5.L2Value();
        if (l2Value < MOTOR_DEADZONE) {
            // Ignorujemy delikatne naciśnięcia triggera
            motorPower = MOTOR_CENTER;
        } else {
            // Mapujemy wartość triggera na zakres prędkości
            motorPower = map(l2Value, MOTOR_DEADZONE, 255, MOTOR_MIN_REV, MOTOR_MAX_REV);
        }
    }
    
    controlMotor(motorPower);
}

void CarController::controlMotor(int value) {
    value = constrain(value, MOTOR_MAX_REV, MOTOR_MAX_FWD);
    currentMotorValue = value;
    motorController.write(value);
}

void CarController::controlSteering(int angle) {
    angle = constrain(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
    steeringServo.write(angle);
}

void CarController::debugOutput() {
    unsigned long currentTime = millis();
    if (currentTime - lastDebugTime >= DEBUG_INTERVAL) {
        String direction = "STOP";
        if (currentMotorValue > MOTOR_CENTER) direction = "FORWARD";
        if (currentMotorValue < MOTOR_CENTER) direction = "REVERSE";
        
        int motorSpeed = abs(currentMotorValue - MOTOR_CENTER) * 100 / (MOTOR_MAX_FWD - MOTOR_CENTER);
        
        Serial.printf("Steering: %d | Motor: %d (%s %d%%) | R2: %d | L2: %d\n",
            steeringServo.read(), currentMotorValue, 
            direction.c_str(), motorSpeed,
            ps5.R2Value(), ps5.L2Value());
        lastDebugTime = currentTime;
    }
}
