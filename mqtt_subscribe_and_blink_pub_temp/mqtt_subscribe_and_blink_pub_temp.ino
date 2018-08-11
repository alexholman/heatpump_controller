#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 3 on the Arduino
const int ONE_WIRE_BUS = 5;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
DeviceAddress tempProbe = { 0x28, 0xFF, 0x97, 0x89, 0xC2, 0x16, 0x03, 0x5C };


const char* wifi_ssid = "7_Gladstone";
const char* wifi_password = "augustpoe";

const char* mqtt_server = "m14.cloudmqtt.com";
const int mqtt_port = 15034;
const char* mqtt_user = "esp_01";
const char* mqtt_password = "fmpud22v";

const char* topic = "home-assistant/hello_mqtt";

char message_buff[128];   // initialise storage buffer (i haven't tested to this capacity.)

// function prototypes
void callback(char* topic, byte* payload, unsigned int length);
// end function prototypes

WiFiClient espClient;
PubSubClient client(mqtt_server, mqtt_port, callback, espClient);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

  Serial.begin(115200);
  delay(2000);

  // Start up the sensor library
  sensors.begin();
  sensors.setResolution(tempProbe, 10);

  setup_wifi();

  client.setCallback(callback); // callback executed when message received

  reconnect();
  //if (client.connect("arduinoClient")) {
    //client.publish(topic,"hello world");
    //client.subscribe("#");   
  //}
  
}

//=================


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  client.publish(topic,"hello world");
  client.subscribe("#");   
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Here is what i have been using to handle subscriptions. I took it as a snippet from elsewhere but i cannot credit author as i dont have reference!
  int i = 0;

  Serial.println("Message arrived:  topic: " + String(topic));
  Serial.println("Length: " + String(length,DEC));
  
  // create character buffer with ending null terminator (string)
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  
  String msgString = String(message_buff);
  
  Serial.println("Payload: " + msgString);
  int state = digitalRead(LED_BUILTIN);  // get the current state of GPIO1 pin
  if (msgString == "1"){    // if there is a "1" published to any topic (#) on the broker then:
    digitalWrite(LED_BUILTIN, !state);     // set pin to the opposite state 
    Serial.println("Switching LED"); 
  }
}

void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == -127.00) {
    Serial.print("Error getting temperature");
  } else {
    float tempF = DallasTemperature::toFahrenheit(tempC)
    Serial.print("C: ");
    Serial.print(tempC);
    Serial.print(" F: ");
    Serial.print(DallasTemperature::toFahrenheit(tempC));
  }
  // return tempF
}



//-------------------

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  delay(1000);
  client.loop();
  printTemperature(tempProbe);
//  char* str_temp;
//  sprintf(str_temp, "%s", sensors.getTempC(tempProbe));
//  client.publish(topic, str_temp);
}


