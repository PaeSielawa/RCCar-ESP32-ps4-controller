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
    
    // Aktualizuj co 10ms - dobry kompromis
    if (timeElapsed >= 10) {
        // Pobierz liczbę impulsów
        noInterrupts();
        unsigned long currentPulseCount = _pulseCount;
        _pulseCount = 0;
        interrupts();
        
        // Oblicz prędkość
        float revolutions = (float)currentPulseCount / _pulsesPerRevolution;
        float distance = revolutions * _wheelCircumference;
        
        // Przelicz na jednostkę czasu i normalizuj
        float newSpeedMps = distance / (timeElapsed / 1000.0);
        float newSpeedKmh = newSpeedMps * 3.6;
        
        // Wygładź dane (70% poprzedniej wartości, 30% nowej)
        _speedMps = (_speedMps * 0.7) + (newSpeedMps * 0.3);
        _speedKmh = (_speedKmh * 0.7) + (newSpeedKmh * 0.3);
        
        // Dodaj dystans przy znaczącej prędkości
        if (_speedKmh >= 0.5) {
            _totalDistance += distance;
        }
        
        // Resetuj prędkość jeśli nie było impulsów przez 500ms
        if (currentTime - _lastPulseTime > 500 && _lastPulseTime > 0) {
            _speedMps = 0;
            _speedKmh = 0;
        }
        
        _lastMeasurementTime = currentTime;
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

unsigned long SpeedSensor::getPulseCount() {
    // Ponieważ _pulseCount jest zmienną volatile używaną w przerwaniu,
    // pobieramy jej wartość w bezpieczny sposób
    noInterrupts();
    unsigned long count = _pulseCount;
    interrupts();
    return count;
}

unsigned long SpeedSensor::getLastPulseTime() {
    // Ponieważ _lastPulseTime jest zmienną volatile używaną w przerwaniu,
    // pobieramy jej wartość w bezpieczny sposób
    noInterrupts();
    unsigned long time = _lastPulseTime;
    interrupts();
    return time;
}