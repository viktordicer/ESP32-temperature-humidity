#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "ClosedCube_HDC1080.h" // HDC1080
#include "configuration.h"
#include "secrets.h"

RTC_DATA_ATTR int bootCount = 0;

long lastMsg = 0;
char msg[50];
int value = 0;
int led = 13;

// data variables
char tempValue[10];
char humValue[10];


WiFiClient espClient;
PubSubClient client(espClient);
ClosedCube_HDC1080 hdc1080; //temperature sensor


void espDeepSleep(){
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(led, LOW);   // Turn the LED on (Note that LOW is the voltage level
  } else  {
    digitalWrite(led, HIGH);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
    } else {
      Serial.println("failed, rc=");
      Serial.println(client.state());
      Serial.println(" try again in 5 seconds");
      //ESP.deepSleep(20 * 1000000, WAKE_RF_DEFAULT);
      delay(5000);
    }
  }
}

void setup() {
  pinMode(led, OUTPUT);  
  Serial.begin(57600);
  hdc1080.begin(0x40);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  delay(200);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  delay(100);
  client.loop();
  delay(200);
  dtostrf(hdc1080.readHumidity(),3,0,humValue); //read humidity
  delay(200);
  dtostrf(hdc1080.readTemperature(),5,1,tempValue); //read temperature
  delay(200);
  client.publish(tempTopic,tempValue);
  client.publish(humTopic,humValue);
  delay(200);
  Serial.println(hdc1080.readTemperature());
  Serial.println(hdc1080.readHumidity());
  delay(200);

  espDeepSleep();
}