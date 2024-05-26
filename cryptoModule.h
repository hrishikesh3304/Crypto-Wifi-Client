#ifndef cryptoModule_h
#define cryptoModule_h

#define HTTP "http"
#define HTTPS "https"

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <String.h>
#include <time.h>

class cryptoModule {
public:
    cryptoModule(uint8_t rxPin, uint8_t txPin); 
    void initialize();
    bool sendWiFiCredentials(const char* ssid, const char* pass);
    void setServerDetails(const char* server, const char* protocol);
    String get_serverName();
    String get_protocol();
    void GET(String query_parameters, String query_parameters_encrypt);
    int POST();
    void generate_key(int length);
    void get_key();
    void set_key();
private:
    SoftwareSerial _espSerial;
    String _serverName;
    String _protocol;
    String _key;
};

#endif
