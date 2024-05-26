#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <string>
SoftwareSerial Master(D2, D3);
String serverName;
String protocol;
String updated_query;
String key = "123";
int encryption_mode;

void setup() {
  Serial.begin(9600);
  Master.begin(9600);
}

void loop() 
{
  if (Master.available() > 0) 
  {
    String mssg = Master.readStringUntil('\n');
    if(mssg.startsWith("WIFI:"))
    { 
      receive_WiFi_cred(mssg);
    }
    else if(mssg.startsWith("SET:"))
    {
      set_server(mssg);
    }
    else if(mssg.startsWith("GET:"))
    {
      http_get(mssg);
      //Serial.println(send_HTTP_request("https://blr1.blynk.cloud/external/api/update?token=N2WqLylm5Dlc4NzyIoQghr5Zyz-cdhDd&", queryparameters))
    }
    else if(mssg.startsWith("KEY:"))
    {
      set_key(mssg);
    }
  }  
  else
  {
    Master.read();
  }
  
}

void set_key(String mssg)
{
  key = str.substring(4);
  key.trim();
}

String encrypt(String val)
{
  int len = val.length();
  int key_len = key.length();
  Serial.println("Hello");
  Serial.println(len);
  Serial.println(key_len);

  String cipher = "";

  for(int i =0 ; i < len; i++)
  {
    cipher += (val[i]^key[i % key_len]);
    Serial.println("in");
  }
  return cipher;
}

void set_server(String str)
{
  int delimiter = str.indexOf(',');
  serverName = str.substring(4, delimiter);
  serverName.trim();
  protocol = str.substring(delimiter+1);
  protocol.trim();
  Master.println("OK");
}

// void http_get(String mssg)
// {
//   int i = 0;
//   updated_query = "?";
//   String key;
//   String value;
//   int posEquals;
//   int nextAnd;

//   while(i<mssg.length() || i==0)
//     {
//     if(i==0)
//     {
//         posEquals = mssg.indexOf('=');
//         nextAnd = mssg.indexOf('&');
//         key = mssg.substring(1, (posEquals)-1);
//         value = mssg.substring(posEquals+1, (nextAnd-1)-(posEquals));
//         i = nextAnd+1;
//     }
//     else
//     {
//         posEquals = mssg.indexOf('=', i);
//         nextAnd = mssg.indexOf('&', i);
//         if (posEquals == -1 || nextAnd == -1)
//         {
//             break;
//         }
//         key = mssg.substring(i, (posEquals) - i);
//         value = mssg.substring(posEquals + 1, (nextAnd - 1)-(posEquals));
//         i = nextAnd+1;
//     }
   
//     updated_query += key;
//     updated_query += "=";
//     //value = encrypt_AES(value);
//     updated_query += value;
//     }

//   Serial.println(updated_query);
//   send_HTTP_request(serverName , "?token=N2WqLylm5Dlc4NzyIoQghr5Zyz-cdhDd&v1=123123123");
// }


void http_get(String mssg) {
  int delimiter = mssg.indexOf(',');
  String query = mssg.substring(4, delimiter);
  query.trim();
  String encrypt_query = mssg.substring(delimiter+1);
  encrypt_query.trim();

  int i = 0;  
  String updated_query = "?";
  updated_query += query;
  String key;
  String value;
  int posEquals;
  int nextAnd;

  if(!encrypt_query.isEmpty())
  {
    updated_query += "&";
    while (i < encrypt_query.length()) {
    posEquals = encrypt_query.indexOf('=', i);
    nextAnd = encrypt_query.indexOf('&', i);

    
    if (nextAnd == -1) {
      key = encrypt_query.substring(i, posEquals);
      value = encrypt_query.substring(posEquals + 1);
      i = encrypt_query.length();  
    } else {
      key = encrypt_query.substring(i, posEquals);
      value = encrypt_query.substring(posEquals + 1, nextAnd);
      i = nextAnd + 1;  
    }

    updated_query += key;
    updated_query += "=";
    updated_query += encrypt(value);

    if (i < encrypt_query.length()) {
      updated_query += "&"; 
    }
  }

  }
  
  //Serial.println("")
  Serial.println(updated_query);
  send_HTTP_request(serverName, updated_query);
  Serial.println("done");
}
 
int send_HTTP_request(String serverName, String queryParameters) {
  WiFiClient client;
  HTTPClient http;

 
  String serverPath = serverName + queryParameters;

  http.begin(client, serverPath.c_str());
  int httpResponseCode = http.GET();


  if (httpResponseCode > 0) {

    String payload = http.getString();
    Master.print("HTTP Response code: ");
    Master.println(httpResponseCode);
    Master.println(payload);
  } else {
 
    Master.print("Error code: ");
    Master.println(httpResponseCode);
  }

  
  http.end();

  return httpResponseCode;
}


void receive_WiFi_cred(String str)
{
  int delimiter = str.indexOf(',');
  String ssid = str.substring(5, delimiter);
  ssid.trim();
  String pass = str.substring(delimiter+1);
  pass.trim(); 

  WiFi.begin(ssid.c_str(), pass.c_str());

    // Serial.print("Connecting to ");
    // Serial.println(ssid);

    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      //Serial.print(".");
    }

    // Serial.println("");
    // Serial.println("WiFi connected");
    // Serial.println("IP address: ");
    // Serial.println(WiFi.localIP());
    Master.print("OK");
    Master.print(WiFi.localIP());
    Master.println("");
  }
