#include <cryptoModule.h>

cryptoModule esp8266(2, 3); 

void setup()  
{
    Serial.begin(9600);
    esp8266.initialize();
    esp8266.sendWiFiCredentials("OnePlus 7", "123456789");
    esp8266.setServerDetails("http://blr1.blynk.cloud/external/api/update", HTTP);
    String query_parameters = "v2=11";
    String str = "token=N2WqLylm5Dlc4NzyIoQghr5Zyz-cdhDd";
    esp8266.GET(str,query_parameters);
}

void loop() 
{

}
