# firmware_esp8266_arduino_lib_wifi_wrapper
ESP8266 Adruino Framework WiFi Wrapper

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
