#include <WiFi.h>
#include <PubSubClient.h>
#include <M5Core2.h>

// WiFi Credentials
const char *ssid = "Scytale";
const char *password = "Chaumurky26";

// MQTT Broker Setting
const char *mqtt_broker = "broker.emqx.io";
const char *mqtt_topic = "uat/dylanmaxwell";
const char *mqtt_username = "emqt";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

// Global counter for messages recieved
int messageCount = 0;

// Function Declations
void connectToWiFi();

void connectToMQTT();

void mqttCallback(char *mqtt_topic, byte *payload, unsigned int length);

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
      }
      Serial.println("\nConnected to WiFi");
}

void connectToMQTT () {
  while (!mqtt_client.connected()) {
    String client_id = "esp32-client-" + String(WiFi.macAddress());
    Serial.printf("Connecting to MQTT Broker as......\n", client_id.c_str());
    if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT broker");
      mqtt_client.subscribe(mqtt_topic);
      mqtt_client.publish(mqtt_topic, "Hi EMQX I'm ESP32");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqttCallback(char *mqtt_topic, byte *payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(mqtt_topic);

  String message = "";
  for (unsigned int i=0; i < length; i++) {
    message += (char)payload[i];
  }


  messageCount++;

  //Print to serial monitor
  Serial.print("Message: ");
  Serial.println(message);
  Serial.print("Total messages received: ");
  Serial.println(messageCount);
  Serial.println("-----------------------");

  //Display on M5Core2 Screen
  M5.Lcd.clear();
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.println("MQTT Message:");
  M5.Lcd.println(message);
  M5.Lcd.println();
  M5.Lcd.print("Count: ");
  M5.Lcd.println(messageCount);
}

void setup() {
  M5.begin();
  Serial.begin(115200);
  connectToWiFi();
  mqtt_client.setServer(mqtt_broker, mqtt_port);
  mqtt_client.setKeepAlive(60);
  mqtt_client.setCallback(mqttCallback);
  connectToMQTT();

}

void loop() {
  if (!mqtt_client.connected()) {
    connectToMQTT();
  }
  mqtt_client.loop();

}
