/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __WIFI_WRAPPER_HPP__
#define __WIFI_WRAPPER_HPP__

#include <ESP8266WiFi.h>

#include "app_common_constants.h"

class WiFiWrapper {

    private:
        // constructor
        WiFiWrapper() {
            connectionStartTimestamp = 0;
            connectionTimeOut = 15 * 1000;
            connectedTime = 0;
        }

        // singleton (C++ 03) - unimplemented methods
        WiFiWrapper(WiFiWrapper const&);
        void operator=(WiFiWrapper const&);

        // singleton (C++ 11) - deleted methods
        // public:
        // WiFiWrapper(WiFiWrapper const&) = delete;
        // void operator=(WiFiWrapper const&) = delete;

        const char *_wifi_ssid;
        const char *_wifi_password;

        uint64_t connectionStartTimestamp;
        uint64_t connectionTimeOut;
        uint64_t connectedTime;

        WiFiEventHandler stationModeOnConnectedHandler;
        WiFiEventHandler stationModeOnDisconnectedHandler;

        void (*onConnected)();
        void (*onDisconnected)();
        
        static void onConnectedCallback(const WiFiEventStationModeConnected& event) {
            char station_ssid[32];
            event.ssid.toCharArray(station_ssid, event.ssid.length());
            Serial.printf("%s: connected to %s\n\r", FILE_NAME_WIFI_WRAPPER, station_ssid);

            getInstance().onConnected();
        }

        static void onDisconnectedCallback(const WiFiEventStationModeDisconnected& event) {
            char station_ssid[32];
            event.ssid.toCharArray(station_ssid, event.ssid.length());
            Serial.printf("%s: disconnected from %s\n\r", FILE_NAME_WIFI_WRAPPER, station_ssid);

            getInstance().onDisconnected();
        }
    
    public:
        // singleton
        static WiFiWrapper& getInstance() {
            static WiFiWrapper instance;
            return instance;
        }

        void initialize(const char *wifi_ssid, const char *wifi_password) {
            Serial.printf("%s: initialize...\n\r", FILE_NAME_WIFI_WRAPPER);

            _wifi_ssid = wifi_ssid;
            _wifi_password = wifi_password;
        }

        void onConnectCallback(void (*callback)()) {
            stationModeOnConnectedHandler = WiFi.onStationModeConnected(&onConnectedCallback);
            onConnected = callback;
        }

        void onDisonnectCallback(void (*callback)()) {
            stationModeOnDisconnectedHandler = WiFi.onStationModeDisconnected(&onDisconnectedCallback);
            onDisconnected = callback;
        }

        void setSSID(const char *wifi_ssid) {
            _wifi_ssid = wifi_ssid;
        }

        void setPassword(const char *wifi_password) {
            _wifi_password = wifi_password;
        }

        const char *getSSID() {
            return _wifi_ssid;
        }

        const char *getPassword() {
            return _wifi_password;
        }

        void connect() {
            if(WiFi.status() == isConnected()) {
                Serial.printf("%s: connected to %s\n\r", FILE_NAME_WIFI_WRAPPER, _wifi_ssid);
                getInstance().onConnected();
            } else {
                Serial.printf("%s: connecting to %s...\n\r", FILE_NAME_WIFI_WRAPPER, _wifi_ssid);
                WiFi.begin(_wifi_ssid, _wifi_password);
            }
        }

        void disconnect() {
            if(WiFi.status() == !isConnected()) {
                Serial.printf("%s: disconnected\n\r", FILE_NAME_WIFI_WRAPPER);
                getInstance().onDisconnected();
            } else {
                Serial.printf("%s: disconnecting from %s...\n\r", FILE_NAME_WIFI_WRAPPER, _wifi_ssid);
                WiFi.disconnect();
            }
        }

        bool isConnected() {
            if(WiFi.status() == WL_CONNECTED) {
                return true;
            } else {
                return false;
            }
        }

        void printWiFiStatus() {
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

        void threadLoop() {
            yield();
        }

        static void staticTickerCallback() {
            getInstance().tickerCallbackFunction();
        }

        void tickerCallbackFunction() {
            // feature function
        }
};

/* Define to prevent recursive inclusion ------------------------------------ */
#endif //__WIFI_WRAPPER_HPP__