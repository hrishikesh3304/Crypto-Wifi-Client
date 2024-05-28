#include "CryptoWifiClient.h"

CryptoWifiClient::CryptoWifiClient(uint8_t rxPin, uint8_t txPin): _wifiSerial(rxPin, txPin) 
{
}

void CryptoWifiClient::initialize() 
{
    _wifiSerial.begin(9600);
    Serial.println("");
    Serial.println("");
    Serial.println("");
Serial.println("*****************************************************");
Serial.println("*               Cryptographic WiFi Module           *");
Serial.println("*****************************************************");

}

bool CryptoWifiClient::sendWiFiCredentials(const char* ssid, const char* pass) 
{
    while (_wifiSerial.available() > 0) 
    {
      _wifiSerial.read();
    }

    _wifiSerial.print("WIFI:");
    _wifiSerial.print(ssid);
    _wifiSerial.print(",");
    _wifiSerial.println(pass); 

    Serial.println("");
    Serial.print("\n[INFO] Sending WiFi credentials....");
    int i = 0;
    delay(1000);
    while(_wifiSerial.available() <= 0)
    {
      if (i%2==0)
      {
        Serial.print("____");
      }
      else
      {
        Serial.print("....");
      }
      i++;
      delay(1000);
    }

    String str = _wifiSerial.readStringUntil('\n');
    if(str.startsWith("OK"))
    {
    String ssidString = String(ssid); 
    ssidString.trim();
    Serial.println("\n[INFO] Connected to WiFi:");
    Serial.print("       SSID: ");
    Serial.println(ssidString);
    Serial.print("       IP Address: ");
    String ip = str.substring(2);
    Serial.println(ip);
    return true;
    }
    else
    {
      Serial.println("\n\n[ERROR] Failed to connect to WiFi");
      return false;
    }
}

void CryptoWifiClient::getQuery(String query_parameters, String query_parameters_encrypt)
{
  while (_wifiSerial.available() > 0) 
  {
    _wifiSerial.read();
  }
  _wifiSerial.print("GET:");
  _wifiSerial.print(query_parameters);
  _wifiSerial.print(",");
  _wifiSerial.println(query_parameters_encrypt);
  delay(2000);
  
  while(_wifiSerial.available() <= 0 ){}
  String str = _wifiSerial.readStringUntil('\n');
  Serial.println("\n\n[INFO] HTTP GET (URL Encoded) Request Sent");
  Serial.println("       ");
  Serial.println(str);
  
}

void CryptoWifiClient::postQuery(String query_parameters, String query_parameters_encrypt)
{
  while (_wifiSerial.available() > 0) 
  {
    _wifiSerial.read();
  }
  _wifiSerial.print("POST_QUERY:");
  _wifiSerial.print(query_parameters);
  _wifiSerial.print(",");
  _wifiSerial.println(query_parameters_encrypt);
  delay(2000);

  while(_wifiSerial.available() <= 0)
  {}
    String str = _wifiSerial.readStringUntil('\n');
    Serial.println("\n\n[INFO] HTTP POST (URL Encoded) Request Sent");
    Serial.print("       ");
    Serial.println(str);
  
}

void CryptoWifiClient::postJSON(String JSONobj)
{
  while (_wifiSerial.available() > 0) 
  {
    _wifiSerial.read();
  }

  _wifiSerial.print("POST_JSON:");
  _wifiSerial.println(JSONobj);
  delay(2000);

  while(_wifiSerial.available() <= 0)
  {}
    String str = _wifiSerial.readStringUntil('\n');
    Serial.println("\n\n[INFO] HTTP POST (JSON) Request Sent");
    Serial.print("       ");
    Serial.println(str);
}

const char* CryptoWifiClient::generateKey(int length)
{
    const char charset[] = " !#$()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[^_`abcdefghijklmnopqrstuvwxyz{|}~";
    int charset_size = sizeof(charset) - 1;
    char *key = (char *)malloc(length + 1); 

    if (key) 
    {
        srand((unsigned int)time(NULL)); 

        for (int i = 0; i < length; i++) 
        {
            int key_index = rand() % charset_size;
            key[i] = charset[key_index];
        }

        key[length] = '\0'; 
    }

    return key; 
}


void CryptoWifiClient::setKey(const char* key)
{
  _wifiSerial.print("KEY:");
  _wifiSerial.println(key);
  delay(2000);
  
  while(_wifiSerial.available() <= 0){}
  String str1 = _wifiSerial.readStringUntil('\n');
  if(str1.startsWith("OK"))
  {
    Serial.println("\n\n[INFO] Encryption Key Set Successfully");
    Serial.print("       Key: ");
    Serial.println(key);
  }
  else
  {
    Serial.println("\n\n[ERROR] Failed to Set Encryption Key");
  }
}

void CryptoWifiClient::setServerDetails(const char* server, const char* protocol)
{
  while (_wifiSerial.available() > 0) 
  {
    _wifiSerial.read();
  }
  _serverName = server;
  _protocol = protocol;
  _wifiSerial.print("SET:");
  _wifiSerial.print(server);
  _wifiSerial.print(",");
  _wifiSerial.println(protocol);
  delay(2000);

  while(_wifiSerial.available() <= 0){}
  String str = _wifiSerial.readStringUntil('\n');
  if(str.startsWith("OK"))
  {
    Serial.println("\n\n[INFO] Server Details Set Successfully");
    Serial.print("       Server: ");
    Serial.println(server);
    Serial.print("       Protocol: ");
    Serial.println(protocol);
  }
  else
  {
    Serial.println("\n\n[ERROR] Failed to Set Server Details");
  }
}

String CryptoWifiClient::getServerName()
{
  return _serverName;
}

String CryptoWifiClient::getProtocol()
{
  return _protocol;
}

String CryptoWifiClient::getKey()
{
  return _key;
}

