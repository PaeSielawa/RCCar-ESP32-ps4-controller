#include "ps5_controller.h"
#include "config.h"

CarController::CarController() {
    steeringServoInitialized = false;
    motorServoInitialized = false;
    currentMotorAngle = MOTOR_CENTER;
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
    
    motorServo.setPeriodHertz(50);
    if (motorServo.attach(MOTOR_PIN, 500, 2400)) {
        motorServoInitialized = true;
        motorServo.write(MOTOR_CENTER);
        Serial.println("Motor servo initialized");
    }
}

void CarController::begin() {
    ps5.begin();
    Serial.println("PS5 Controller initialized");
}

bool CarController::isConnected() {
    return ps5.isConnected();
}

void CarController::update() {
    if (!steeringServoInitialized || !motorServoInitialized) return;
    
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
    
    if (ps5.R2()) {
        motorPower = map(ps5.R2Value(), 0, 255, MOTOR_CENTER, MOTOR_MAX_FWD);
    } else if (ps5.L2()) {
        motorPower = map(ps5.L2Value(), 0, 255, MOTOR_CENTER, MOTOR_MAX_REV);
    } else {
        if (currentMotorAngle > MOTOR_CENTER) {
            motorPower = currentMotorAngle - MOTOR_RETURN_SPEED;
        } else if (currentMotorAngle < MOTOR_CENTER) {
            motorPower = currentMotorAngle + MOTOR_RETURN_SPEED;
        }
    }
    
    controlMotor(motorPower);
}

void CarController::controlMotor(int angle) {
    angle = constrain(angle, MOTOR_MAX_REV, MOTOR_MAX_FWD);
    currentMotorAngle = angle;
    motorServo.write(angle);
}

void CarController::controlSteering(int angle) {
    angle = constrain(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
    steeringServo.write(angle);
}

void CarController::debugOutput() {
    unsigned long currentTime = millis();
    if (currentTime - lastDebugTime >= DEBUG_INTERVAL) {
        Serial.printf("Steering: %d | Motor: %d | R2: %d | L2: %d\n",
            steeringServo.read(), currentMotorAngle, ps5.R2Value(), ps5.L2Value());
        lastDebugTime = currentTime;
    }
}