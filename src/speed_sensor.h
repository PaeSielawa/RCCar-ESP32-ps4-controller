#ifndef SPEED_SENSOR_H
#define SPEED_SENSOR_H

#include <Arduino.h>

class SpeedSensor {
public:
    SpeedSensor(int pin, float wheelDiameter = 0.065, int pulsesPerRevolution = 20);
    void begin();
    void update();
    float getSpeedMps();
    float getSpeedKmh();
    float getDistanceMeters();
    unsigned long getLastPulseTime();
    unsigned long getPulseCount();

private:
    static void IRAM_ATTR pulseCounter();
    
    int _pin;
    float _wheelDiameter;  // średnica koła w metrach
    int _pulsesPerRevolution;  // ilość otworów/impulsów na jeden obrót koła
    float _wheelCircumference;  // obwód koła w metrach
    
    static volatile unsigned long _pulseCount;
    static volatile unsigned long _lastPulseTime;
    unsigned long _lastMeasurementTime;
    
    float _speedKmh;
    float _speedMps;
    float _totalDistance;  // całkowity dystans w metrach
};

#endif