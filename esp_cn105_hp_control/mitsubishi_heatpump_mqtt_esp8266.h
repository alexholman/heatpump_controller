
#define OTA

// wifi settings
const char* ssid     = "7_Gladstone";
const char* password = "augustpoe";

// mqtt server settings
const char* mqtt_server   = "m14.cloudmqtt.com";
const int mqtt_port       = 15034;
const char* mqtt_username = "esp_01";
const char* mqtt_password = "fmpud22v";

// mqtt client settings
// Note PubSubClient.h has a MQTT_MAX_PACKET_SIZE of 128 defined, so either raise it to 256 or use short topics
const char* client_id                   = "heatpump-controller-01_prime"; // Must be unique on the MQTT network
const char* heatpump_topic              = "heatpump/01";
const char* heatpump_set_topic          = "heatpump/01/set";
const char* heatpump_status_topic       = "heatpump/01/status";
const char* heatpump_timers_topic       = "heatpump/01/timers";

const char* heatpump_debug_topic        = "heatpump/01/debug";
const char* heatpump_debug_set_topic    = "heatpump/01/debug/set";

// pinouts
const int redLedPin  = 5; // Onboard LED = digital pin 0 (red LED on adafruit ESP8266 huzzah)
const int blueLedPin = 5; // Onboard LED = digital pin 0 (blue LED on adafruit ESP8266 huzzah)

// sketch settings
const unsigned int SEND_ROOM_TEMP_INTERVAL_MS = 60000;
