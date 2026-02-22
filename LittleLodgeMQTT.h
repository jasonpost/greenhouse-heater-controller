// LittleLodgeMQTT.h

#include <PubSubClient.h>
#include <AsyncTimer.h>
#include <ArduinoSecrets.h>

// Defs --------------------------------------------------------

#define MQTT_PING_DELAY 5000
#define MQTT_PUBLISH_STATE_DELAY 1000
#define MQTT_SERIAL_LOG false

// Global -----------------------------------------------------
PubSubClient mqttClient;

// Function Prototype ------------------------------------------
typedef void (*ConnectCallback)(char*, uint8_t*, unsigned int);
typedef void (*PingCallback)();
void mqttConnect();
void mqttPing();
void mqttSetup(Client&, ConnectCallback);



void mqttConnect() {

  Serial.println("Initiating connection to MQTT broker.");
  while (!mqttClient.connected()) {
    Serial.println(".");
    if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS)) {
      Serial.printf("MQTT connected to broker: %s with client id: %s", MQTT_BROKER, MQTT_CLIENT_ID);
      Serial.println(".");
    } else {
      Serial.printf("MQTT - failed with state :  %s", mqttClient.state());
      Serial.print("");
      delay(2000);
    }
  }
}




void mqttSetup(Client& wifiClient, ConnectCallback callbackFunc) {

  Serial.printf("Setting up MQTT broker: %s with client id: %s", MQTT_BROKER, MQTT_CLIENT_ID);
  Serial.println("");
  //Serial.printf("Setting callback to %s ", callbackFunc);
  Serial.println("");
  mqttClient.setKeepAlive(90);
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(callbackFunc);
  mqttClient.setClient(wifiClient);

}

void mqttPing() {
  //Serial.println("MQTT Ping");
  // mqtt client
  if (!mqttClient.connected()) {
    mqttConnect();
  }

  // tick mqtt client
  mqttClient.loop();
}

// void mqttPing(PingCallback callbackFunc) {
//   // mqtt client
//   if (!mqttClient.connected()) {
//     mqttConnect();
//   }

//   // tick mqtt client
//   mqttClient.loop();

//   // make callback call
//   callbackFunc();
// }

// void messageReceived(String topic, String payload, char* bytes, unsigned int length) {
//   Serial.print("incoming: ");
//   Serial.print(topic);
//   Serial.print(" - ");
//   Serial.print(payload);
//   Serial.println();
// }