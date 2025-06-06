#include "speed_sensor.h"

volatile unsigned long SpeedSensor::_pulseCount = 0;
volatile unsigned long SpeedSensor::_lastPulseTime = 0;

SpeedSensor::SpeedSensor(int pin, float wheelDiameter, int pulsesPerRevolution) {
    _pin = pin;
    _wheelDiameter = wheelDiameter;
    _pulsesPerRevolution = pulsesPerRevolution;
    _wheelCircumference = PI * wheelDiameter;
    _lastMeasurementTime = 0;
    _speedKmh = 0;
    _speedMps = 0;
    _totalDistance = 0;
}

void SpeedSensor::begin() {
    pinMode(_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(_pin), pulseCounter, FALLING);
    _lastMeasurementTime = millis();
}

void IRAM_ATTR SpeedSensor::pulseCounter() {
    _pulseCount++;
    _lastPulseTime = millis();
}

void SpeedSensor::update() {
    unsigned long currentTime = millis();
    unsigned long timeElapsed = currentTime - _lastMeasurementTime;
    
    // Jeśli minęło więcej niż 1 sekunda, przelicz prędkość
    if (timeElapsed >= 1000) {
        // Oblicz prędkość na podstawie ilości impulsów
        float revolutions = (float)_pulseCount / _pulsesPerRevolution;
        float distance = revolutions * _wheelCircumference;  // metry
        
        _speedMps = distance / (timeElapsed / 1000.0);  // m/s
        _speedKmh = _speedMps * 3.6;  // km/h
        
        // Dodaj dystans
        _totalDistance += distance;
        
        // Resetuj licznik impulsów i czas pomiaru
        _pulseCount = 0;
        _lastMeasurementTime = currentTime;
        
        // Jeśli nie było impulsów przez 2 sekundy, ustaw prędkość na 0
        if (currentTime - _lastPulseTime > 2000) {
            _speedMps = 0;
            _speedKmh = 0;
        }
    }
}

float SpeedSensor::getSpeedKmh() {
    return _speedKmh;
}

float SpeedSensor::getSpeedMps() {
    return _speedMps;
}

float SpeedSensor::getDistanceMeters() {
    return _totalDistance;
}