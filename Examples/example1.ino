#include <CryptoWifiClient.h>

CryptoWifiClient esp8266(2, 3); 
String query_parameters;
String str;

void setup() 
{
  pinMode(6, OUTPUT);
  Serial.begin(9600);     
  Serial.println();
  esp8266.initialize();
  esp8266.sendWiFiCredentials("OnePlus 7", "123456789"); 
  esp8266.setServerDetails("http://blr1.blynk.cloud/external/api/update", HTTP); 
  esp8266.setKey(esp8266.generateKey(10));

  Serial.println("\nApproximate your card to the reader...");
  str = "token=N2WqLylm5Dlc4NzyIoQghr5Zyz-cdhDd";
}

void loop() 
{
  query_parameters = "v3=";
  String uidString = getData();
  query_parameters += uidString;
  esp8266.getQuery(str,query_parameters);
}

String getData() 
{
  //Logic for reading sensor data
}
