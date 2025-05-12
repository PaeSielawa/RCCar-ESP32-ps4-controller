#include "ps4_controller.h"
#include "config.h"

CarController::CarController() {
    connected = false;
    steeringServoInitialized = false;
    motorServoInitialized = false;
    currentMotorAngle = MOTOR_CENTER;
    lastDebugTime = 0;
}

void CarController::setupPins() {
    ESP32PWM::allocateTimer(0); // Alokuj timer dla serwa
    ESP32PWM::allocateTimer(1);
    
    // Inicjalizacja serwa kierownicy
    steeringServo.setPeriodHertz(50); // Standardowa częstotliwość dla serwa
    if (steeringServo.attach(SERVO_PIN, 500, 2400)) { // Min i max pulsy dla typowego serwa
        steeringServoInitialized = true;
        steeringServo.write(SERVO_CENTER); // Centruj serwo na start
        Serial.println("Steering servo initialized");
    } else {
        Serial.println("Failed to initialize steering servo");
        return;
    }
    
    // Inicjalizacja serwa silnika
    motorServo.setPeriodHertz(50);
    if (motorServo.attach(MOTOR_PIN, 500, 2400)) {
        motorServoInitialized = true;
        motorServo.write(MOTOR_CENTER);
        Serial.println("Motor servo initialized");
    } else {
        Serial.println("Failed to initialize motor servo");
        return;
    }
}

void CarController::connect() {
    if (PS4.begin(BLUETOOTH_NAME)) {
        connected = true;
        Serial.println("PS4 Controller connected successfully");
    } else {
        connected = false;
        Serial.println("Failed to connect PS4 controller");
    }
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

void CarController::readInput() {
    if (!steeringServoInitialized || !motorServoInitialized) {
        Serial.println("Servos not initialized!");
        return;
    }

    if (PS4.isConnected()) {
        // Obsługa kierownicy
        // Odczyt pozycji lewego drążka w osi X
        int leftStickX = PS4.LStickX();
        // Mapowanie wartości z zakresu drążka (-128 do 127) na kąt serwa
        int steeringAngle = map(leftStickX, -128, 127, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
        // Dodanie małej strefy martwej wokół środka (deadzone)
        if (abs(leftStickX) < SERVO_DEADZONE) {
            steeringAngle = SERVO_CENTER;
        }
        // Zastosowanie kąta skrętu
        controlSteering(steeringAngle);

        // Obsługa silnika
        int motorPower = 0;
        if (PS4.R2()) {
            // Do przodu - mapowanie wartości R2 (0-255) na kąt serwa
            motorPower = map(PS4.R2Value(), 0, 255, MOTOR_CENTER, MOTOR_MAX_FWD);
        } else if (PS4.L2()) {
            // Do tyłu - mapowanie wartości L2 (0-255) na kąt serwa
            motorPower = map(PS4.L2Value(), 0, 255, MOTOR_CENTER, MOTOR_MAX_REV);
        } else {
            // Powolny powrót do pozycji neutralnej
            if (currentMotorAngle > MOTOR_CENTER) {
                motorPower = currentMotorAngle - MOTOR_RETURN_SPEED;
            } else if (currentMotorAngle < MOTOR_CENTER) {
                motorPower = currentMotorAngle + MOTOR_RETURN_SPEED;
            } else {
                motorPower = MOTOR_CENTER;
            }
        }
        
        controlMotor(motorPower);
        
        // Debugowanie z limitem czasowym
        unsigned long currentTime = millis();
        if (currentTime - lastDebugTime >= DEBUG_INTERVAL) {
            Serial.printf("Stick X: %d | Steering: %d | Motor: %d | R2: %d | L2: %d\n",
                leftStickX, steeringAngle, motorPower, PS4.R2Value(), PS4.L2Value());
            lastDebugTime = currentTime;
        }
    } else {
        Serial.println("Controller disconnected!");
    }
}

bool CarController::isConnected() {
    return PS4.isConnected();
}