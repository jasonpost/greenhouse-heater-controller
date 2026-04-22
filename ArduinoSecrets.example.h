#ifndef ARDUINO_SECRETS_EXAMPLE_H
#define ARDUINO_SECRETS_EXAMPLE_H

#define SECRET_SSID "your-wifi-ssid"
#define SECRET_PASS "your-wifi-password"
#define MQTT_BROKER "192.168.1.100"
#define MQTT_PORT 1883
#define MQTT_PORT_SSL 8883
#define MQTT_WEB_SOCKET 8084
#define MQTT_KEEPALIVE 60
#define MQTT_USER "mqtt-username"
#define MQTT_PASS "mqtt-password"
#define MQTT_CLIENT_ID "esp32-greenhouse-diesel-heater"
#define MQTT_STATE_TOPIC "your/site/diesel_heater"
#define MQTT_STATUS_TOPIC "your/site/diesel_heater/status"
#define MQTT_COMMAND_ON "your/site/diesel_heater/on"
#define MQTT_COMMAND_OFF "your/site/diesel_heater/off"
#define MQTT_COMMAND_UP "your/site/diesel_heater/up"
#define MQTT_COMMAND_DOWN "your/site/diesel_heater/down"
#define MQTT_CERT "-----BEGIN CERTIFICATE-----\n" \
"YOUR_CA_CERTIFICATE_CONTENTS_GO_HERE\n" \
"-----END CERTIFICATE-----\n"

#define DIESEL_HEATER_PROTOCOL 1
#define DIESEL_HEATER_PULSE 668
#define DIESEL_HEATER_BIT 32
#define DIESEL_HEATER_ON "01001001011011110001000110110000"
#define DIESEL_HEATER_OFF "01001001011011110000010100001000"
#define DIESEL_HEATER_UP "01001001011011110000100011110000"
#define DIESEL_HEATER_DOWN "01001001011011110000001010001000"

#endif
