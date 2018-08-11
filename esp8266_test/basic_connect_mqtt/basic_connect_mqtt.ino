#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define wifi_ssid "7_Gladstone"
#define wifi_password "augustpoe"

#define mqtt_server "m14.cloudmqtt.com"
#define mqtt_user "esp_01"
#define mqtt_password "fmpud22v"

#define topic "home-assistant/hello_mqtt"

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  delay(2000);

  setup_wifi();
  client.setServer(mqtt_server, 15034);
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
}


//-------------------

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  client.publish(topic, "saturday potato", true);
  delay(2000);
}


