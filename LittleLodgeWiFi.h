#include <WiFi.h>
#include <WiFiClientSecure.h>


// Function Prototypes
typedef void (*Callback)();
void wifiInit();

WiFiClient espClient;
WiFiClientSecure espClientSecure;

// WiFi -----------------------------------------------------------------------


void wifiConnect(bool isSecure) {

  WiFi.disconnect();

  WiFi.mode(WIFI_STA);
  WiFi.begin(SECRET_SSID, SECRET_PASS);  // Connect to WiFi network
  Serial.println("Connecting to WiFi.");
  while (WiFi.status() != WL_CONNECTED) {  // Wait for connection
    Serial.println(".");
    delay(1000);
  }

  Serial.print("Connected IP Address: ");
  Serial.println(WiFi.localIP());  // Print the ESP's IP address

  if (isSecure) {
    Serial.println("Securing wifi; Setting CA Cert...");
    espClientSecure.setCACert(MQTT_CERT);
  }
}




