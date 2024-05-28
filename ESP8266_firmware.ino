#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <string>
#include <Base64.h>

//SoftwareSerial Master(D2, D3);
String serverName;
String protocol;
String updated_query;
String key = "mysecretkey";
int encryption_mode;

void setup() {
  Serial.begin(9600);
}

void loop() 
{
  if (Serial.available() > 0) 
  {
    String mssg = Serial.readStringUntil('\n');
    mssg.trim();
    if(mssg.startsWith("WIFI:"))
    {
      //Serial.println("1");
      receive_WiFi_cred(mssg);
      //delay(1000);
    }
    else if(mssg.startsWith("SET:"))
    {
      //Serial.println("2");
      set_server(mssg);
      //delay(1000);
    }
    else if(mssg.startsWith("GET:"))
    {
      //Serial.println("3");
      http_get(mssg);
      //delay(2000);
    }
    else if(mssg.startsWith("POST_QUERY:"))
    {
      //Serial.println("4");
      http_post_query(mssg);
      //delay(2000);
    }
    else if(mssg.startsWith("KEY:"))
    {
      //Serial.println("5");
      set_key(mssg);
      //delay(2000);
    }
    else if(mssg.startsWith("POST_JSON:"))
    {
      //Serial.println("6");
      http_post_JSON(mssg);
      //delay(2000);
    }
    delay(2000);
  }  
}

void set_key(String mssg)
{
  key = mssg.substring(4);
  key.trim();
  Serial.println("OK");
}

String encrypt(String val) {
  int len = val.length();
  int key_len = key.length();

  String cipher = "";

  for(int i = 0; i < len; i++) {
    char encryptedChar = val.charAt(i) ^ key.charAt(i % key_len);
    cipher += encryptedChar;
    //Serial.println((int)cipher.charAt(i));
  }

  String base64Encoded = base64::encode(cipher);
  return base64Encoded;
}

void set_server(String str)
{
  int delimiter = str.indexOf(',');
  serverName = str.substring(4, delimiter);
  serverName.trim();
  protocol = str.substring(delimiter+1);
  protocol.trim();
  Serial.println("OK");
}

void http_post_JSON(String mssg)
{
  int delimiter = mssg.indexOf(',');
  String obj = mssg.substring(10, delimiter);
  obj.trim();

  WiFiClient client;
  HTTPClient http;

  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(obj);

  if (httpResponseCode > 0) {

    String payload = http.getString();
    Serial.print("[INFO] HTTP Response code: ");
    Serial.print(httpResponseCode);
    Serial.print(payload);
  } else {
 
    Serial.print("[INFO] Error code: ");
    Serial.print(httpResponseCode);
  }

  
  http.end();
}

void http_post_query(String mssg)
{
  String updatedPOSTdata = "";
  
  int delimiter = mssg.indexOf(',');
  String query = mssg.substring(11, delimiter);
  query.trim();
  String encrypt_query = mssg.substring(delimiter+1);
  encrypt_query.trim();

  int i = 0;  
  
  updatedPOSTdata += query;
  String key;
  String value;
  int posEquals;
  int nextAnd;

  if(!encrypt_query.isEmpty())
  {
    updatedPOSTdata += "&";
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

    updatedPOSTdata += key;
    updatedPOSTdata += "=";
    updatedPOSTdata += encrypt(value);

    if (i < encrypt_query.length()) {
      updatedPOSTdata += "&"; 
    }
  }

  }

  WiFiClient client;
  HTTPClient http;

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpResponseCode = http.POST(updatedPOSTdata);

  if (httpResponseCode > 0) {

    String payload = http.getString();
    Serial.print("[INFO] HTTP Response code: ");
    Serial.print(httpResponseCode);
    Serial.print(payload);
  } else {
 
    Serial.print("[INFO] Error code: ");
    Serial.print(httpResponseCode);
  }

  
  http.end();
}

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
  //Serial.println(updated_query);
  send_get_HTTP_request(serverName, updated_query);
}
 
int send_get_HTTP_request(String serverName, String queryParameters) {
  WiFiClient client;
  HTTPClient http;

 
  String serverPath = serverName + queryParameters;

  http.begin(client, serverPath.c_str());
  int httpResponseCode = http.GET();


  if (httpResponseCode > 0) {

    String payload = http.getString();
    Serial.print("[INFO] HTTP Response code: ");
    Serial.print(httpResponseCode);
    Serial.print(payload);
  } else {
 
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
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
    Serial.print("OK");
    Serial.print(WiFi.localIP());
    Serial.println("");
  }
