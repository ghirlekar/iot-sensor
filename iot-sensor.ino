#include <DHT.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

// Replace ... with appropriate ssid/password
const char* ssid = "...";
const char* password = "...";

const char* server = "broker.mqtt-dashboard.com";
const int port = 1883;

const char* deviceID = "0xff12";
char* topic = "gauravESP8266";
int payload = 0;

WiFiClient wificlient;
PubSubClient client(server, port, callback, wificlient);
DHT dht(13, DHT11);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println(topic);
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Connecting to ");
  Serial.print(server);
  Serial.print(" as ");
  Serial.println(deviceID);
}

void loop() {
  if (!client.connected())
    reconnect();
  char payload[27];
  String msg = String("temperature:" + String((int)dht.readTemperature()) + ",humidity:" + String((int)dht.readHumidity()));
  msg.toCharArray(payload, 27);
  if (client.publish(topic, payload)) {
    Serial.println("Publish ok");
  }
  else {
    Serial.println("Publish failed");
  }
  delay(2000);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(deviceID)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
