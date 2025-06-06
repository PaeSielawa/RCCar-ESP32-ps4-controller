#include <Arduino.h>
#include "ps5_controller.h"
#include "mpu9250_handler.h"
#include "config.h"
#include <driver/i2c.h>
#include <Wire.h>

CarController car;
MPU9250Handler mpu;

// Zamiana układu bajtów na string w formacie MAC
const char* mac = "84:30:95:D8:A6:7B";

// Funkcja do konfiguracji I2C dla lepszej stabilności
void configureI2C(uint32_t freq) {
    // Zastąpmy próbę bezpośredniej modyfikacji rejestrów bardziej kompatybilną funkcją
    #ifdef ESP32
    // Najpierw zatrzymujemy i usuwamy istniejącą instancję I2C
    Wire.end();
    
    // Konfigurujemy i startujemy I2C na nowo z innymi parametrami
    // Użyjemy niskiej częstotliwości i dłuższego timeoutu
    Wire.begin(SDA_PIN, SCL_PIN, freq);
    
    // Alternatywnie, możemy użyć natywnego API ESP32 (ale będziemy musieli uwzględnić numery pinów GPIO)
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = SDA_PIN;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = SCL_PIN;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = freq;
    conf.clk_flags = 0; // Opcjonalne flagi zegara
    
    i2c_param_config(I2C_NUM_0, &conf);
    i2c_set_timeout(I2C_NUM_0, 50000); // 50ms timeout
    #endif
}

// Funkcja do skanowania urządzeń I2C
void scanI2CDevices() {
    Serial.println("Scanning I2C bus for devices...");
    byte error, address;
    int deviceCount = 0;
    
    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        
        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16) {
                Serial.print("0");
            }
            Serial.print(address, HEX);
            Serial.println();
            deviceCount++;
        }
    }
    
    if (deviceCount == 0) {
        Serial.println("No I2C devices found!");
    } else {
        Serial.print("Found ");
        Serial.print(deviceCount);
        Serial.println(" device(s).");
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000); // Give time for serial connection to establish
    
    // Wzmocnione rezystory podciągające
    // 1. Najpierw ustawiamy GPIO jako OUTPUT i ustawiamy stan wysoki
    pinMode(SDA_PIN, OUTPUT);
    pinMode(SCL_PIN, OUTPUT);
    digitalWrite(SDA_PIN, HIGH);
    digitalWrite(SCL_PIN, HIGH);
    delay(100);
    
    // 2. Następnie przełączamy na INPUT_PULLUP aby utrzymać podciągnięcie
    pinMode(SDA_PIN, INPUT_PULLUP);
    pinMode(SCL_PIN, INPUT_PULLUP);
    delay(100);
    
    Wire.begin(SDA_PIN, SCL_PIN);
    Wire.setClock(50000);  // 50kHz
    delay(2000);
    
    // Reszta kodu jak dotychczas
    // MPU9250 initialization
    #if MPU9250_ENABLED
    scanI2CDevices();
    
    if (mpu.begin()) {
        Serial.println("MPU9250 found");
        
        // Opcjonalnie możemy dodać krótkie opóźnienie po inicjalizacji
        delay(100);
        
        // Sprawdź, czy MPU9250 rzeczywiście wysyła dane
        mpu.update();
        if (mpu.getAccX() == 0 && mpu.getAccY() == 0 && mpu.getAccZ() == 0) {
            Serial.println("Warning: MPU9250 initialized but not sending data");
        } else {
            Serial.println("MPU9250 is working correctly");
        }
    } else {
        Serial.println("MPU9250 initialization failed!");
    }
    #else
    Serial.println("MPU9250 support disabled in config");
    #endif
    
    car.setupPins();       
    ps5.begin(mac);        
    Serial.println("ESP32 initialized, waiting for PS5 controller");
    
    // Czekamy na połączenie z kontrolerem
    unsigned long startTime = millis();
    while (!ps5.isConnected() && (millis() - startTime < CONNECTION_TIMEOUT)) {
        delay(100);
    }
    
    if (ps5.isConnected()) {
        Serial.println("PS5 controller connected!");
        Serial.println("Ready.");
    } else {
        Serial.println("Failed to connect PS5 controller!");
    }
}

void loop() {
    static unsigned long lastLoopTime = 0;
    static unsigned long lastMpuDebugTime = 0;
    unsigned long currentTime = millis();
    
    #if MPU9250_ENABLED
    // Aktualizacja danych z MPU9250
    mpu.update();
    
    // Wyświetlanie danych co sekundę
    if (currentTime - lastMpuDebugTime >= 1000) {
        mpu.printData();
        lastMpuDebugTime = currentTime;
    }
    #endif
    
    // Aktualizacja kontrolera samochodu
    if (currentTime - lastLoopTime >= LOOP_DELAY) {
        car.update();
        lastLoopTime = currentTime;
    }
}