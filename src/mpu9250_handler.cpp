#include "mpu9250_handler.h"
#include "config.h"

MPU9250Handler::MPU9250Handler() {
    initialized = false;
    accelX = accelY = accelZ = 0.0f;
    gyroX = gyroY = gyroZ = 0.0f;
    magX = magY = magZ = 0.0f;
    temperature = 0.0f;
    imu = nullptr;
}

bool MPU9250Handler::begin(TwoWire &wirePort, uint8_t addr) {
    // Inicjalizacja I2C
    wirePort.begin(SDA_PIN, SCL_PIN);
    wirePort.setClock(400000); // 400 kHz
    
    // Tworzymy obiekt dopiero tutaj
    imu = new MPU9250(wirePort, addr);
    if (imu == nullptr) {
        Serial.println("Failed to allocate memory for MPU9250");
        return false;
    }
    
    // Konfiguracja MPU9250
    int status = imu->begin();
    
    if (status < 0) {
        Serial.println("MPU9250 initialization failed!");
        Serial.print("Status: ");
        Serial.println(status);
        delete imu;
        imu = nullptr;
        return false;
    }
    
    initialized = true;
    Serial.println("MPU9250 initialized successfully");
    
    // Ustaw zakres czujników - poprawione zgodnie z biblioteką bolderflight
    imu->setAccelRange(MPU9250::ACCEL_RANGE_4G);  // 4g
    imu->setGyroRange(MPU9250::GYRO_RANGE_500DPS);  // 500dps
    imu->setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);  // 20Hz
    imu->setSrd(19); // Częstotliwość odświeżania ~50 Hz
    
    return true;
}

void MPU9250Handler::update() {
    if (!initialized || imu == nullptr) return;
    
    if (imu->readSensor() == 1) {
        // Aktualizacja wartości pomiarów
        accelX = imu->getAccelX_mss();
        accelY = imu->getAccelY_mss();
        accelZ = imu->getAccelZ_mss();
        
        gyroX = imu->getGyroX_rads();
        gyroY = imu->getGyroY_rads();
        gyroZ = imu->getGyroZ_rads();
        
        magX = imu->getMagX_uT();
        magY = imu->getMagY_uT();
        magZ = imu->getMagZ_uT();
        
        temperature = imu->getTemperature_C();
    }
}

void MPU9250Handler::calibrate() {
    if (!initialized || imu == nullptr) return;
    
    Serial.println("Calibrating gyroscope, keep the device still...");
    delay(1000);
    imu->calibrateGyro();
    Serial.println("Gyro calibration complete");
    
    Serial.println("Calibrating accelerometer, keep the device still...");
    delay(1000);
    imu->calibrateAccel();
    Serial.println("Accelerometer calibration complete");
    
    Serial.println("All calibrations complete");
}

void MPU9250Handler::printData() {
    if (!initialized) return;
    
    Serial.print("Accel (m/s²): ");
    Serial.print(accelX, 2); Serial.print(", ");
    Serial.print(accelY, 2); Serial.print(", ");
    Serial.println(accelZ, 2);
    
    Serial.print("Gyro (rad/s): ");
    Serial.print(gyroX, 2); Serial.print(", ");
    Serial.print(gyroY, 2); Serial.print(", ");
    Serial.println(gyroZ, 2);
    
    Serial.print("Mag (uT): ");
    Serial.print(magX, 2); Serial.print(", ");
    Serial.print(magY, 2); Serial.print(", ");
    Serial.println(magZ, 2);
    
    Serial.print("Temp (°C): ");
    Serial.println(temperature, 2);
    Serial.println();
}