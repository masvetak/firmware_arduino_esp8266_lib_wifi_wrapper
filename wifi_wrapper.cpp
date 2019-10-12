#include "wifi_wrapper.hpp"

#define FILE_NAME_WIFI_WRAPPER "\e[34m[WiFi]\e[0m"

WiFiWrapper::WiFiWrapper() {
    connectionStartTimestamp = 0;
    connectionTimeOut = 15 * 1000;
    connectedTime = 0;
}

void WiFiWrapper::onConnectedCallback(const WiFiEventStationModeConnected& event) {
    char station_ssid[32];
    event.ssid.toCharArray(station_ssid, event.ssid.length());
    Serial.printf("%s: connected to %s\n\r", FILE_NAME_WIFI_WRAPPER, station_ssid);

    getInstance().onConnected();
}

void WiFiWrapper::onDisconnectedCallback(const WiFiEventStationModeDisconnected& event) {
    char station_ssid[32];
    event.ssid.toCharArray(station_ssid, event.ssid.length());
    Serial.printf("%s: disconnected from %s\n\r", FILE_NAME_WIFI_WRAPPER, station_ssid);

    getInstance().onDisconnected();
}

void WiFiWrapper::initialize(const char *wifi_ssid, const char *wifi_password) {
    Serial.printf("%s: initialize...\n\r", FILE_NAME_WIFI_WRAPPER);

    _wifi_ssid = wifi_ssid;
    _wifi_password = wifi_password;
}

void WiFiWrapper::registerOnConnectCallback(void (*callback)()) {
    stationModeOnConnectedHandler = WiFi.onStationModeConnected(&onConnectedCallback);
    onConnected = callback;
}

void WiFiWrapper::registerOnDisonnectCallback(void (*callback)()) {
    stationModeOnDisconnectedHandler = WiFi.onStationModeDisconnected(&onDisconnectedCallback);
    onDisconnected = callback;
}

void WiFiWrapper::setSSID(const char *wifi_ssid) {
    _wifi_ssid = wifi_ssid;
}

void WiFiWrapper::setPassword(const char *wifi_password) {
    _wifi_password = wifi_password;
}

const char *WiFiWrapper::getSSID() {
    return _wifi_ssid;
}

const char *WiFiWrapper::getPassword() {
    return _wifi_password;
}

void WiFiWrapper::connect() {
    if(WiFi.status() == isConnected()) {
        Serial.printf("%s: connected to %s\n\r", FILE_NAME_WIFI_WRAPPER, _wifi_ssid);
        WiFiWrapper::getInstance().onConnected();
    } else {
        Serial.printf("%s: connecting to %s...\n\r", FILE_NAME_WIFI_WRAPPER, _wifi_ssid);
        WiFi.begin(_wifi_ssid, _wifi_password);
    }
}

void WiFiWrapper::disconnect() {
    if(WiFi.status() == !isConnected()) {
        Serial.printf("%s: disconnected\n\r", FILE_NAME_WIFI_WRAPPER);
        WiFiWrapper::getInstance().onDisconnected();
    } else {
        Serial.printf("%s: disconnecting from %s...\n\r", FILE_NAME_WIFI_WRAPPER, _wifi_ssid);
        WiFi.disconnect();
    }
}

bool WiFiWrapper::isConnected() {
    if(WiFi.status() == WL_CONNECTED) {
        return true;
    } else {
        return false;
    }
}

void WiFiWrapper::printWiFiStatus() {
    switch(WiFi.status()) {
        case WL_IDLE_STATUS: {
            Serial.printf("%s: Wi-Fi is in process of changing between statuses\n\r", FILE_NAME_WIFI_WRAPPER);
            break;
        }
        case WL_NO_SSID_AVAIL: {
            Serial.printf("%s: %s cannot be reached\n\r", FILE_NAME_WIFI_WRAPPER, _wifi_ssid);
            break;
        }
        case WL_CONNECTED: {
            Serial.printf("%s: device connected to %s\n\r", FILE_NAME_WIFI_WRAPPER, _wifi_ssid);
            break;
        }
        case WL_CONNECT_FAILED: {
            Serial.printf("%s: connecton failed, wrong password\n\r", FILE_NAME_WIFI_WRAPPER);
            break;
        }
        case WL_DISCONNECTED: {
            Serial.printf("%s: device disconnected\n\r", FILE_NAME_WIFI_WRAPPER);
            break;
        }
        default: {
        
        }
    }
}

void WiFiWrapper::threadLoop() {
    yield();
}

void WiFiWrapper::staticTickerCallback() {
    WiFiWrapper::getInstance().tickerCallbackFunction();
}

void WiFiWrapper::tickerCallbackFunction() {
    // feature function
}