#include <Arduino.h>
#include <Esp.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define LED_PIN 5

const char* ssid = "OnePlus2";
const char* password = "global@123";
const char* mqtt_server = "192.168.43.162";
const char* inbound_topic = "esp8266/control/led";
const char* outbound_topic = "esp8266/status/led";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
bool ledstate = false;

void setup_wifi(void);
void callback(char* topic, byte* payload, unsigned int length);
void reconnect(void);

void setup(void);
void loop(void);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(10);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection failed! rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if(strcmp(inbound_topic,topic)==0){
    if ((char)payload[0] == '1') {
      ledstate = true;
      digitalWrite(LED_PIN, HIGH);
    } else {
      ledstate = false;
      digitalWrite(LED_PIN, LOW);
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      client.publish(outbound_topic, "system ready");
      client.subscribe(inbound_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  pinMode(LED_PIN, OUTPUT);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  long now = millis();

  if (now - lastMsg > 5000) {
    lastMsg = now;
    snprintf (msg, 75, "%ld", ledstate ? 1:0);
    Serial.print("Publish message: ledstate ");
    Serial.println(msg);
    client.publish(outbound_topic, msg);
  }
  delay(1000);
}
