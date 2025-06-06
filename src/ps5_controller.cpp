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
    
    // Odwrócenie kierunku - teraz ujemne wartości (lewo) będą prawym skrętem
    // a dodatnie wartości (prawo) będą lewym skrętem
    int steeringAngle = map(leftStickX, -127, 127, SERVO_MAX_ANGLE, SERVO_MIN_ANGLE);
    
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
            motorPower = MOTOR_CENTER;
        } else {
            // Początkowy skok mocy (20% maksymalnej mocy) już na starcie
            float initialBoost = 0.5 * (MOTOR_MAX_FWD - MOTOR_CENTER);
            float remainingPower = 0.5 * (MOTOR_MAX_FWD - MOTOR_CENTER);
            
            // Normalizacja wartości triggera
            float normalizedInput = (float)(r2Value - MOTOR_DEADZONE) / (255.0f - MOTOR_DEADZONE);
            
            // Skok na początku + liniowy przyrost
            motorPower = MOTOR_CENTER + initialBoost + (normalizedInput * remainingPower);
            
            motorPower = constrain(motorPower, MOTOR_CENTER, MOTOR_MAX_FWD);
        }
    }
    // Jazda do tyłu (podobna logika)
    else if (ps5.L2()) {
        int l2Value = ps5.L2Value();
        if (l2Value < MOTOR_DEADZONE) {
            motorPower = MOTOR_CENTER;
        } else {
            // Początkowy skok mocy (20% maksymalnej mocy) już na starcie
            float initialBoost = 0.2 * (MOTOR_CENTER - MOTOR_MAX_REV);
            float remainingPower = 0.8 * (MOTOR_CENTER - MOTOR_MAX_REV);
            
            float normalizedInput = (float)(l2Value - MOTOR_DEADZONE) / (255.0f - MOTOR_DEADZONE);
            
            // Skok na początku + liniowy przyrost
            motorPower = MOTOR_CENTER - initialBoost - (normalizedInput * remainingPower);
            
            motorPower = constrain(motorPower, MOTOR_MAX_REV, MOTOR_CENTER);
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
