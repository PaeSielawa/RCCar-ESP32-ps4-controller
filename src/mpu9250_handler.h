#ifndef MPU9250_HANDLER_H
#define MPU9250_HANDLER_H

#include <Arduino.h>
#include <Wire.h>
#include <MPU9250.h>

class MPU9250Handler {
public:
    MPU9250Handler();
    ~MPU9250Handler(); // Destruktor do zwolnienia pamięci
    bool begin(TwoWire& wirePort = Wire, uint8_t addr = 0x68);
    void update();
    void calibrate();
    
    // Gettery dla pomiarów
    float getAccX() { return accelX; }
    float getAccY() { return accelY; }
    float getAccZ() { return accelZ; }
    float getGyroX() { return gyroX; }
    float getGyroY() { return gyroY; }
    float getGyroZ() { return gyroZ; }
    float getMagX() { return magX; }
    float getMagY() { return magY; }
    float getMagZ() { return magZ; }
    float getTemperature() { return temperature; }
    
    void printData();

private:
    MPU9250* imu;
    bool initialized;
    
    float accelX, accelY, accelZ;
    float gyroX, gyroY, gyroZ;
    float magX, magY, magZ;
    float temperature;
};

#endif