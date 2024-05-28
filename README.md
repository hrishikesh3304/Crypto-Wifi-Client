# Cryptographic WiFi Module Library

The Cryptographic WiFi Module Library provides a comprehensive solution for integrating cryptographic capabilities into WiFi-enabled devices, particularly targeting the ESP8266 platform. It simplifies the process of connecting to WiFi networks securely and facilitates easy integration with cloud services for beginners. This library is designed to offer straightforward cryptographic functionalities alongside standard WiFi connectivity.
## Getting Started

Download or clone this repository into your Arduino libraries directory.

```bash
git clone https://github.com/yourusername/Cryptographic-WiFi-Module-Library.git
```

## 
#### Include Library
In your Arduino sketch, include the CryptoWifiClient.h header file.
```cpp
#include <CryptoWifiClient.h>
```

#### Initialization
Initialize the library in your setup function.
```cpp
CryptoWifiClient wifiClient(rxPin, txPin);
wifiClient.initialize();
```


#### Usage
Utilize the library's functions for various tasks such as sending WiFi credentials, setting server details, and performing HTTP requests.
```cpp
// Example: Sending WiFi credentials
wifiClient.sendWiFiCredentials("SSID", "password");

// Example: Setting server details
wifiClient.setServerDetails("example.com", HTTPS);

// Example: Getting server name and protocol
String server = wifiClient.getServerName();
String protocol = wifiClient.getProtocol();

// Example: Sending HTTP GET request
wifiClient.getQuery("endpoint", "encrypted_params");

// Example: Sending HTTP POST request with URL encoded parameters
wifiClient.postQuery("data", "encrypted_data");

// Example: Sending HTTP POST request with JSON data
wifiClient.postJSON("{\"key\":\"value\"}");

// Example: Generating encryption key
const char* key = wifiClient.generateKey(16);
wifiClient.setKey(key);
```

