#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi Credentials:
const char* ssid = "Realme 3 Pro";
const char* password = "pappu@1234";
const char* mqtt_server = "test.mosquitto.org";
#define soil_moisture_pin 34 // Sensor input Pin

WiFiClient espClient;
PubSubClient client(espClient);

// Connecting to WiFi
void setupWiFi() {
  delay(1000);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to WiFi");
}

// Reconnect to MQTT server
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Attempt to connect
    if (client.connect("espClient")) {
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

void setup() {
  Serial.begin(9600);
  setupWiFi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int soil_moisture = analogRead(soil_moisture_pin); 

  Serial.print("Soil Moisture: "); 
  Serial.println(soil_moisture); 

  if (soil_moisture <= 1800) {
    Serial.println("Soil is wet :)");
  } 
  else {
    Serial.println("Soil is DRY :(");
  }

  // Publish soil moisture reading to MQTT topic
  char message[20]; // Buffer for message
  snprintf(message, sizeof(message), "%d", soil_moisture); // Convert integer to string
  client.publish("device/mano/data/soilsensor1", message);

  delay(1000);
}
