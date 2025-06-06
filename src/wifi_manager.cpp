#include "wifi_manager.h"

WiFiManager::WiFiManager() {
    _lastSendTime = 0;
}

bool WiFiManager::begin() {
    #if WIFI_ENABLED
    WiFi.disconnect(true);  // Rozłącz ewentualne poprzednie połączenia
    delay(1000);
    
    #if USE_WIFI_AP_MODE
    return setupAccessPoint();
    #else
    return setupClient();
    #endif
    
    #else
    return false;
    #endif
}

bool WiFiManager::setupClient() {
    Serial.println("Connecting to WiFi network...");
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(CLIENT_SSID, CLIENT_PASSWORD);
    
    // Czekaj na połączenie maksymalnie 20 sekund
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 20000) {
        delay(500);
        Serial.print(".");
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("\nConnected to WiFi with IP: ");
        Serial.println(WiFi.localIP());
        return true;
    } else {
        Serial.println("\nWiFi connection failed!");
        return false;
    }
}

bool WiFiManager::sendSpeedData(float speed, float distance) {
    unsigned long currentTime = millis();
    
    // Wysyłaj dane w określonych odstępach czasu
    if (currentTime - _lastSendTime < DATA_UPDATE_INTERVAL) {
        return true;  // Jeszcze nie czas
    }
    
    _lastSendTime = currentTime;
    
    #if WIFI_ENABLED
    // Sprawdź status połączenia WiFi
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected. Cannot send data.");
        
        // Próba ponownego połączenia
        setupClient();
        return false;
    }
    
    HTTPClient http;
    
    // Formatuj URL z parametrami prędkości i dystansu
    String url = "http://" + String(ESP32CAM_IP) + ":" + String(ESP32CAM_PORT) + "/speed_data";
    url += "?speed=" + String(speed, 2);  // Precyzja 2 miejsca po przecinku
    url += "&distance=" + String(distance, 2);
    
    Serial.print("Sending data: ");
    Serial.println(url);
    
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode > 0) {
        Serial.print("HTTP Response: ");
        Serial.println(httpCode);
        http.end();
        return true;
    } else {
        Serial.print("HTTP Error: ");
        Serial.println(http.errorToString(httpCode));
        http.end();
        return false;
    }
    #else
    return false;
    #endif
}