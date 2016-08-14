#include <Arduino.h>
#include <Esp.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <coap.h>

#include "debug.h"

#define LED_PIN 2
#define SW_PIN 0

const char *ssid = "OnePlus2";
const char *password = "global@123";

void setup_wifi(void);
void callback_response(CoapPacket &packet, IPAddress ip, int port);
void callback_wellknown_core(CoapPacket &packet, IPAddress ip, int port);
void callback_light(CoapPacket &packet, IPAddress ip, int port);
void handle_toggle_switch_interrupt(void);

void setup(void);
void loop(void);


WiFiUDP udp;
Coap coap(udp);

volatile bool ledstate = true; //Active low relay switch
volatile bool interrupted = false;

void callback_wellknown_core(CoapPacket &packet, IPAddress ip, int port){
  Serial_println("[Coap .well-known/core]");

  const char *content = "</led>;ep=\"led\"";
  coap.sendResponse(ip, port, packet.messageid, (char *)content, strlen(content),
                      COAP_CONTENT, COAP_APPLICATION_LINK_FORMAT, NULL, 0);
}

void callback_response(CoapPacket &packet, IPAddress ip, int port) {
  Serial_println("[Coap Response got]");

  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = '\0';

  Serial_println(p);
}

void callback_light(CoapPacket &packet, IPAddress ip, int port){
  Serial_print("Coap [/led] ");

  // send response
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = '\0';

  String message(p);

  if (message.equals("0"))
    ledstate = true;
  else if(message.equals("1"))
    ledstate = false;

  Serial_println(ledstate ? "OFF" : "ON");
  digitalWrite(LED_PIN, ledstate);
  coap.sendResponse(ip, port, packet.messageid, (char *)(ledstate ? "0" : "1"));
}

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

//Interrupt function
void handle_toggle_switch_interrupt(void){
  ledstate = !ledstate;
  interrupted = true;
  digitalWrite(LED_PIN, ledstate);
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  pinMode(LED_PIN, OUTPUT);
  pinMode(SW_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SW_PIN), handle_toggle_switch_interrupt, FALLING);

  coap.server(callback_wellknown_core, ".well-known/core");
  coap.server(callback_light, "led");
  coap.response(callback_response);
  coap.start();
}

void loop() {
  coap.loop();
  delay(90);
}
