// This Arduino sketch reads DS18B20 "1-Wire" digital
// temperature sensors.
// Tutorial:
// http://www.hacktronics.com/Tutorials/arduino-1-wire-tutorial.html

#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Data wire is plugged into pin 3 on the Arduino
#define ONE_WIRE_BUS 5
#define MQTT_VERSION MQTT_VERSION_3_1_1

// wifi settings
const char* ssid     = "7_Gladstone";
const char* wifi_password = "augustpoe";

// mqtt server settings
const char* mqtt_server   = "m14.cloudmqtt.com";
const int mqtt_port       = 16261;
const char* mqtt_username = "temp_01";
const char* mqtt_password = "yph8m3s5";

// MQTT: topic
const char* mqtt_client_id    = "temp01";
const char* mqtt_topic = "temp/01";

// seconds between readings
const int WAIT_SEC = 10;

// Init WiFi and MQTT clients
WiFiClient wifiClient;
PubSubClient client(wifiClient);


// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// Assign the addresses of your 1-Wire temp sensors.
// See the tutorial on how to obtain these addresses:
// http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html

DeviceAddress tempProbe = { 0x28, 0xFF, 0x97, 0x89, 0xC2, 0x16, 0x03, 0x5C };


// Publish the temperature and the humidity
void publishData(float temperature_c) {
  // create a JSON object
  // doc : https://github.com/bblanchon/ArduinoJson/wiki/API%20Reference
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  // INFO: the data must be converted into a string; a problem occurs when using floats...
  root["tempC"] = (String)temperature_c;
  root.prettyPrintTo(Serial);
  Serial.println("");
  /*
     {
        "temperature": "23.20" ,
        "humidity": "43.70"
     }
  */
  char data[200];
  root.printTo(data, root.measureLength() + 1);
  client.publish(mqtt_topic, data, true);
}

// function called when a MQTT message arrived
void callback(char* p_topic, byte* p_payload, unsigned int p_length) {
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("INFO: Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqtt_client_id, mqtt_username, mqtt_password)) {
      Serial.println("INFO: connected");
    } else {
      Serial.print("ERROR: failed, rc=");
      Serial.print(client.state());
      Serial.println("DEBUG: try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//-----------------------

void setup(void)
{
  // start serial port
  Serial.begin(115200);
  // Start up the library
  sensors.begin();
  // set the resolution to 10 bit (good enough?)
  sensors.setResolution(tempProbe, 10);

  // init the WiFi connection
  Serial.println();
  Serial.println();
  Serial.print("INFO: Connecting to ");
  WiFi.mode(WIFI_STA);
  Serial.println(ssid);
  WiFi.begin(ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("INFO: WiFi connected");
  Serial.println("INFO: IP address: ");
  Serial.println(WiFi.localIP());

  // init the MQTT connection
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

float getTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == -127.00) {
    Serial.print("Error getting temperature");
  } else {
    Serial.print("C: ");
    Serial.print(tempC);
    Serial.print(" F: ");
    Serial.print(DallasTemperature::toFahrenheit(tempC));
  }
  return tempC;
}

void loop(void)
{ 
  // Stay connected to MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  Serial.print("Getting temperatures...\n\r");
  sensors.requestTemperatures();
  Serial.print("Temperature is: ");
  float tempC = sensors.getTempC(tempProbe);
  Serial.print("\n\r");

  if (tempC != -127.00) {
    publishData(tempC);
  }
  delay(WAIT_SEC*1000);
}

