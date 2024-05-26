#include "cryptoModule.h"

cryptoModule::cryptoModule(uint8_t rxPin, uint8_t txPin): _espSerial(rxPin, txPin) 
{
}

void cryptoModule::initialize() 
{
    _espSerial.begin(9600);
    Serial.println("");
    Serial.println("");
    Serial.println("");

    Serial.println("******************************************");
    Serial.println("*        Cryptographic WiFi Module       *");
    Serial.println("******************************************");
    delay(2000); 
}

bool cryptoModule::sendWiFiCredentials(const char* ssid, const char* pass) 
  {
    _espSerial.print("WIFI:");
    _espSerial.print(ssid);
    _espSerial.print(",");
    _espSerial.print(pass);
    _espSerial.println(); 
    _espSerial.flush();

    while(_espSerial.available() <= 0)
    {
    }
    String str = _espSerial.readStringUntil('\n');
    if(str.startsWith("OK"))
    {
    String ssidString = String(ssid); 
    ssidString.trim();
    Serial.println("");
    Serial.println("Connected to " +  ssidString);
    Serial.print("IP ADDRESS: ");
    String ip = str.substring(2);
    Serial.print(ip);
    Serial.println("");
    return true;
    }
    else
    {
      Serial.println("Failed to connect");
      return false;
    }
}

void cryptoModule::GET(String query_parameters, String query_parameters_encrypt)
{
  _espSerial.print("GET:");
  _espSerial.print(query_parameters);
  _espSerial.print(",");
  _espSerial.print(query_parameters_encrypt);
  _espSerial.println();
  _espSerial.flush();
  
  while(_espSerial.available() <= 0){
  String str = _espSerial.readStringUntil('\n');
  Serial.println("");
  Serial.println(str);
  Serial.println("");
  }
}

const char* cryptoModule::generate_key(int length)
  {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_=+[]{}|;:,.<>?";
    int charset_size = sizeof(charset) - 1;
    char *key = (char *)malloc(length + 1); 

  
    if (key) 
    {
      srand((unsigned int)time(NULL)); 

      for (int i = 0; i < length; i++) 
      {
        int key_index = rand() % charset_size;
        key[i] = charset[key_index];
        //printf("%c",key[i]);

      }
    
      key[length] = '\0'; 
    }

    return key; 
  }

void cryptoModule::set_key(const char* key)
{
  _espSerial.print("KEY:");
  _espSerial.print(key);
  _espSerial.println();
  
  while(_espSerial.available() <= 0){}
  String str = _espSerial.readStringUntil('\n');
  if(str.startsWith("OK"))
  {
    Serial.println("");
    Serial.println("New key :");
    Serial.println(key);
    Serial.println("");
  }
  else
  {
    Serial.println("Failed to set");
  }
}

void cryptoModule::setServerDetails(const char* server, const char* protocol)
{
  _serverName = server;
  _protocol = protocol;
  _espSerial.print("SET:");
  _espSerial.print(server);
  _espSerial.print(",");
  _espSerial.print(protocol);
  _espSerial.println();

  while(_espSerial.available() <= 0){}
  String str = _espSerial.readStringUntil('\n');
  if(str.startsWith("OK"))
  {
    Serial.println("");
    Serial.println("Server name :");
    Serial.print(server);
    Serial.println("Protocol :");
    Serial.print(protocol);
    Serial.println("");
  }
  else
  {
    Serial.println("Failed to set");
  }

  while(_espSerial.available() <= 0){}
  String str = _espSerial.readStringUntil('\n');
  if(str.startsWith("OK"))
  {
    Serial.println("");
    Serial.println("Server name :");
    Serial.print(server);
    Serial.println("Protocol :");
    Serial.print(protocol);
    Serial.println("");
  }
  else
  {
    Serial.println("Failed to set");
  }
}

String cryptoModule::get_serverName()
{
  return _serverName;
}

String cryptoModule::get_protocol()
{
  return _protocol;
}

String cryptoModule::get_key()
{
  return _key;
}


// void cryptoModule::sendData(const char* server_name, const char* query_parameters)
// {
//   _espSerial.print("SEND:");
//   _espSerial.print(query_parameters);
//   _espSerial.print(",");
//   _espSerial.print(server_name);
//   _espSerial.println();

//   delay(2000);
//   while(_espSerial.available() <= 0){}
//   String str = _espSerial.readStringUntil('\n');
//   Serial.println(str);
// }
