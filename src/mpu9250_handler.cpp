#include "mpu9250_handler.h"
#include "config.h"

MPU9250Handler::MPU9250Handler() {
    imu = nullptr;
    initialized = false;
    accelX = accelY = accelZ = 0.0f;
    gyroX = gyroY = gyroZ = 0.0f;
    magX = magY = magZ = 0.0f;
    temperature = 0.0f;
}

MPU9250Handler::~MPU9250Handler() {
    if (imu != nullptr) {
        delete imu;
        imu = nullptr;
    }
}

bool MPU9250Handler::begin(TwoWire& wirePort, uint8_t addr) {
    // Tworzenie obiektu MPU9250
    imu = new MPU9250();
    if (imu == nullptr) {
        Serial.println("Failed to allocate memory for MPU9250");
        return false;
    }

    // Uproszczona inicjalizacja - dokładnie jak w przykładzie dokumentacji
    // Dodajemy 200 ms opóźnienia podobnie jak w przykładzie
    delay(200);
    
    // Najpierw próbujemy najprostszą inicjalizację z adresem 0x68
    Serial.println("Trying minimal MPU9250 initialization...");
    if (imu->setup(0x68)) {
        initialized = true;
        Serial.println("MPU9250 initialized with minimal configuration at address 0x68");
        return true;
    }
    
    // Jeśli prosta inicjalizacja nie zadziałała, próbujemy z pełną konfiguracją
    Serial.println("Trying with advanced settings...");
    MPU9250Setting setting;
    setting.accel_fs_sel = ACCEL_FS_SEL::A16G;
    setting.gyro_fs_sel = GYRO_FS_SEL::G2000DPS;
    setting.mag_output_bits = MAG_OUTPUT_BITS::M16BITS;
    setting.fifo_sample_rate = FIFO_SAMPLE_RATE::SMPL_200HZ;
    setting.gyro_fchoice = 0x03;
    setting.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_41HZ;
    setting.accel_fchoice = 0x01;
    setting.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_45HZ;
    
    // Próbujemy z pełną konfiguracją
    if (imu->setup(0x68, setting)) {
        initialized = true;
        Serial.println("MPU9250 initialized with advanced configuration at address 0x68");
        return true;
    }
    
    // Jeśli wszystkie próby zawiodły
    Serial.println("MPU9250 not found on any address!");
    delete imu;
    imu = nullptr;
    return false;
}

void MPU9250Handler::update() {
    if (!initialized || imu == nullptr) return;
    
    // Dodaj zabezpieczenie przed błędami I2C
    static uint8_t errorCount = 0;
    static unsigned long lastSuccessTime = 0;
    
    bool success = false;
    
    // Użyj sprawdzenia available() przed update()
    if (imu->available()) {
        // Aktualizacja danych z MPU9250
        if (imu->update()) {
            // Odczytaj dane z czujnika
            accelX = imu->getAccX();
            accelY = imu->getAccY();
            accelZ = imu->getAccZ();
            
            gyroX = imu->getGyroX();
            gyroY = imu->getGyroY();
            gyroZ = imu->getGyroZ();
            
            magX = imu->getMagX();
            magY = imu->getMagY();
            magZ = imu->getMagZ();
            
            temperature = imu->getTemperature();
            
            // Aktualizacja udana
            errorCount = 0;
            lastSuccessTime = millis();
            success = true;
        }
    }
    
    // Jeśli wystąpił błąd, zwiększ licznik
    if (!success) {
        errorCount++;
        
        // Po kilku błędach wyświetl ostrzeżenie
        if (errorCount == 10) {
            Serial.println("MPU9250 communication errors detected");
        }
        
        // Jeśli nie było aktualizacji przez dłuższy czas, spróbuj zresetować MPU
        if (errorCount > 50 || (millis() - lastSuccessTime > 10000 && lastSuccessTime > 0)) {
            Serial.println("Attempting to reset MPU9250...");
            
            // Zwolnij bieżący obiekt
            if (imu != nullptr) {
                delete imu;
                imu = nullptr;
            }
            
            // Próba reinicjalizacji
            initialized = false;
            begin();
            
            // Reset licznika błędów
            errorCount = 0;
        }
    }
}

void MPU9250Handler::calibrate() {
    if (!initialized || imu == nullptr) return;
    
    Serial.println("Calibrating gyroscope and accelerometer...");
    Serial.println("Keep the device still for a few seconds");
    delay(2000);
    
    imu->calibrateAccelGyro();
    Serial.println("Accel/Gyro calibration complete");
    
    Serial.println("Calibrating magnetometer...");
    Serial.println("Wave the device in a figure-8 pattern for 10 seconds");
    delay(2000);
    
    imu->calibrateMag();
    Serial.println("Magnetometer calibration complete");
    
    Serial.println("All calibrations complete");
}

void MPU9250Handler::printData() {
    if (!initialized || imu == nullptr) {
        Serial.println("MPU9250 not initialized - no data to print");
        return;
    }
    
    Serial.print("Accel (G): ");
    Serial.print(accelX, 2); Serial.print(", ");
    Serial.print(accelY, 2); Serial.print(", ");
    Serial.println(accelZ, 2);
    
    Serial.print("Gyro (degrees/s): ");
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