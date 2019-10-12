/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __WIFI_WRAPPER_HPP__
#define __WIFI_WRAPPER_HPP__

#include <ESP8266WiFi.h>

class WiFiWrapper {

    private:
        // constructor
        WiFiWrapper();

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
        
        static void onConnectedCallback(const WiFiEventStationModeConnected& event);
        static void onDisconnectedCallback(const WiFiEventStationModeDisconnected& event);

        void tickerCallbackFunction();

    public:
        // singleton
        static WiFiWrapper& getInstance() {
            static WiFiWrapper instance;
            return instance;
        }

        void initialize(const char *wifi_ssid, const char *wifi_password);
        void registerOnConnectCallback(void (*callback)());
        void registerOnDisonnectCallback(void (*callback)());
        void setSSID(const char *wifi_ssid);
        void setPassword(const char *wifi_password);
        const char *getSSID();
        const char *getPassword();
        void connect();
        void disconnect();
        bool isConnected();
        void printWiFiStatus();
        void threadLoop();
        static void staticTickerCallback();

};

/* Define to prevent recursive inclusion ------------------------------------ */
#endif //__WIFI_WRAPPER_HPP__