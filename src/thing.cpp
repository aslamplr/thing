#include <Arduino.h>
#include <Esp.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "debug.h"

#define LED_PIN 2
#define SW_PIN 0

const char *ssid = "OnePlus2";
const char *password = "global@123";
const char *mqtt_server = "192.168.43.162";
const char *control_topic = "esp8266/control/led";
const char *status_topic = "esp8266/status/led";
const char *override_topic = "esp8266/override/led";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
volatile bool ledstate = true; //Active low relay switch
volatile bool interrupted = false;

void setup_wifi(void);
void callback(char *topic, byte *payload, unsigned int length);
void reconnect(void);
void handle_switching(void);
void handle_toggle_switch_interrupt(void);

void setup(void);
void loop(void);

void setup_wifi() {
  delay(10);
  Serial_println();
  Serial_print("Connecting to ");
  Serial_println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(10);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial_println("Connection failed! rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial_println("WiFi connected");
  Serial_println("IP address: ");
  Serial_println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial_print("Message arrived [");
  Serial_print(topic);
  Serial_print("] ");
  for (int i = 0; i < length; i++) {
    Serial_print((char)payload[i]);
  }
  Serial_println();

  if(strcmp(control_topic,topic)==0){
    if ((char)payload[0] == '1') {
      ledstate = false;
      digitalWrite(LED_PIN, LOW);
    } else {
      ledstate = true;
      digitalWrite(LED_PIN, HIGH);
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial_print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial_println("connected");
      client.publish(status_topic, "system ready");
      client.subscribe(control_topic);
    } else {
      handle_switching();
      Serial_print("failed, rc=");
      Serial_print(client.state());
      Serial_println(" try again in 5 seconds");
      for(int msec=0; msec < 5000; msec+= 500){
        delay(500);
        handle_switching();
      }
    }
  }
}

//Interrupt function
void handle_toggle_switch_interrupt(void){
  ledstate = !ledstate;
  interrupted = true;
}

void handle_switching(void){
    if(interrupted){
      digitalWrite(LED_PIN, ledstate);
      if(client.connected()){
        snprintf (msg, 75, "%ld", ledstate ? 0 : 1);
        Serial_print("Publish override message: ledstate ");
        Serial_println(msg);
        client.publish(override_topic, msg);
        interrupted = false;
      }
    }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  pinMode(LED_PIN, OUTPUT);
  pinMode(SW_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SW_PIN), handle_toggle_switch_interrupt, FALLING);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  handle_switching();

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  long now = millis();

  if (now - lastMsg > 5000) {
    lastMsg = now;
    snprintf (msg, 75, "%ld", ledstate ? 0 : 1);
    Serial_print("Publish status message: ledstate ");
    Serial_println(msg);
    client.publish(status_topic, msg);
  }
  delay(90);
}
