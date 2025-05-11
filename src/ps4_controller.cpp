#include "ps4_controller.h"
#include "config.h"

CarController::CarController() {
    connected = false;
}

void CarController::setupPins() {
    ESP32PWM::allocateTimer(0); // Alokuj timer dla serwa
    steeringServo.setPeriodHertz(50); // Standardowa częstotliwość dla serwa
    steeringServo.attach(SERVO_PIN, 500, 2400); // Min i max pulsy dla typowego serwa
    steeringServo.write(SERVO_CENTER); // Centruj serwo na start
}

void CarController::connect() {
    PS4.begin(BLUETOOTH_NAME);
    connected = true;
}

void CarController::controlMotor(int speed) {
    speed = constrain(speed, MOTOR_MIN_SPEED, MOTOR_MAX_SPEED);
    analogWrite(MOTOR_PIN, speed);
}

void CarController::controlSteering(int angle) {
    angle = constrain(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
    steeringServo.write(angle);
}

void CarController::readInput() {
    if (PS4.isConnected()) {
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
        
        // Szczegółowe debugowanie
        Serial.printf("Stick X: %d | Angle: %d | Deadzone: %s\n", 
            leftStickX, 
            steeringAngle, 
            (abs(leftStickX) < SERVO_DEADZONE) ? "Active" : "Inactive"
        );
    } else {
        Serial.println("Controller disconnected!");
    }
}

bool CarController::isConnected() {
    return PS4.isConnected();
}