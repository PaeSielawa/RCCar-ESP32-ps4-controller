#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "config.h"

class WiFiManager {
public:
    WiFiManager();
    bool begin();
    bool sendSpeedData(float speed, float distance);
    
private:
    bool setupClient();
    unsigned long _lastSendTime;
};

#endif