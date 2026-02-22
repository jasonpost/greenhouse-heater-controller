#include <WiFi.h>
#include <PubSubClient.h>
#include <math.h>
// #include <CooperativeMultitasking.h>
#include <ArduinoJson.h>
#include <Eventually.h>
#include <ArduinoSecrets.h>
//#include <EventuallyCommand.h>
//#include <EventuallyStateMachine.h>
#include <output.h>
#include <RCSwitch.h>
#include <LittleLodgeWiFi.h>
#include <LittleLodgeMQTT.h>


// Defs --------------------------------------------------------
#define RF_RECEIVER_PIN 23
#define RF_TRANSMITTER_PIN 21


// Function Prototype Definitions -----------------------------------------------

void rfReceiverSetup();
void rfTransmitterSetup();
void rfReceive();
void mqttInit();
void mqttProcessMessages(char* topic, byte* payload, unsigned int length);
void mqttPublishState();
void mqttSetSubscriptions();
void heaterOn();
void heaterOff();
void heaterUp();
void heaterDown();

// Globals -----------------------------------------------------

EvtManager mgr(true);  // true to manage memory
EvtPinListener rfReceiverOuputListener(RF_RECEIVER_PIN, (EvtAction)rfReceive);
RCSwitch mySwitch = RCSwitch();

// CooperativeMultitasking tasks;
// Continuation mqttCheckMessages;

int heaterTempC = 8;
int heaterTempF = 46;
char* heaterSwitch = "OFF";



// Arduino callbacks -------------------------------------------

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting setup...");

  // WiFi
  wifiConnect(false);

  // MQTT
  mqttSetup(espClient, mqttProcessMessages);
  mqttConnect();
  mqttSetSubscriptions();

  // RF Receiver Setup
  rfReceiverSetup();
  // RF Transmitter Setup
  rfTransmitterSetup();

  mySwitch.send(DIESEL_HEATER_OFF);

  Serial.println("Finishing setup...");
}

void loop() {
  //mgr.loopIteration();
  mqttClient.loop();

  rfReceive();
}

int convertCToF(int& C) {
  return round((C * 1.8) + 32);
}

// RF --------------------------------------------------------------------------------------------------------------------------------------------------------------

void rfReceiverSetup() {
  // setup pin modes
  mySwitch.enableReceive(RF_RECEIVER_PIN);
}

void rfTransmitterSetup() {
  pinMode(RF_TRANSMITTER_PIN, OUTPUT);
  mySwitch.enableTransmit(RF_TRANSMITTER_PIN);
  mySwitch.setPulseLength(DIESEL_HEATER_PULSE);
  //mySwitch.setProtocol(DIESEL_HEATER_PROTOCOL);
}

void rfReceive() {
  //Serial.println("RF has recieved something...");
  if (mySwitch.available()) {
    output(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(), mySwitch.getReceivedProtocol());
    mySwitch.resetAvailable();
  }
}

// MQTT ------------------------------------------------------------------------------------------------------------------------------------------------------------

void mqttSetSubscriptions() {

  mqttClient.subscribe(MQTT_COMMAND_ON);
  mqttClient.subscribe(MQTT_COMMAND_OFF);
  mqttClient.subscribe(MQTT_COMMAND_UP);
  mqttClient.subscribe(MQTT_COMMAND_DOWN);
}


void mqttProcessMessages(char* topic, byte* payload, unsigned int length) {

  //JsonDocument doc;
  char message[length - 1];
  std::string ss;

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  // deserialize message
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    message[i] = (char)payload[i];
    ss += (char)payload[i];
  }

  // deserializeJson(doc, ss);
  // serializeJson(doc, Serial);
  // const char* mode = doc["mode"];

  Serial.printf("Topic is %s", topic);
  Serial.println("");

  // turn heater on
  if (strcmp(MQTT_COMMAND_ON, topic) == 0) {
    heaterOn();
  }
  // turn heater off
  if (strcmp(MQTT_COMMAND_OFF, topic) == 0) {
    heaterOff();
  }
  // turn heater up
  if (strcmp(MQTT_COMMAND_UP, topic) == 0) {
    heaterUp();
  }
  // turn heater down
  if (strcmp(MQTT_COMMAND_DOWN, topic) == 0) {
    heaterDown();
  }
}

void mqttPublishState() {

  JsonDocument doc;
  doc["heaterTempC"] = heaterTempC;
  doc["heaterTempF"] = heaterTempF;
  doc["heaterSwitch"] = heaterSwitch;

  char pay[256];
  serializeJson(doc, pay);

  // Serial.println("......................................");
  // //serializeJson(doc, Serial);
  // Serial.print(pay);
  // Serial.println("......................................");

  // char value[10];
  // dtostrf(sonarDistanceIN, 10, 8, value);

  mqttClient.publish(MQTT_STATE_TOPIC, pay, true);

  // counter++;
  // char display_value[4];
  // const char temp = counter + '0';
  // strncat(display_value, &temp, 1);
  // mqttClient.publish("sh/wtf", display_value, true);

  mqttClient.publish(MQTT_STATUS_TOPIC, "online", true);
}

// Diesel Heater Functions -----------------------------------------------------------------------

void heaterOn() {
  Serial.println("Diesel Heater ON Received.");
  //mySwitch.setRepeatTransmit(5);
  mySwitch.send(DIESEL_HEATER_ON);
  //mySwitch.setRepeatTransmit(0);
}
void heaterOff() {
  Serial.println("Diesel Heater OFF Received.");
  mySwitch.setRepeatTransmit(5);
  mySwitch.send(DIESEL_HEATER_OFF);
  //digitalWrite(21, HIGH);
  //delay(500);
  //digitalWrite(21, LOW);
  
  // Wait another second
  //delay(500);


  mySwitch.setRepeatTransmit(0);
}
void heaterUp() {
  Serial.println("Diesel Heater UP Received.");
  mySwitch.send(DIESEL_HEATER_UP);
  rfReceive();
}
void heaterDown() {
  Serial.println("Diesel Heater DOWN Received.");
  mySwitch.send(DIESEL_HEATER_DOWN);
}
