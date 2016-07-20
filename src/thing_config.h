#ifndef __THING_H__
#define __THING_H__

typedef struct Wifi_config {
  char ssid[32];
  char password[20];
} wifi_config_t;

typedef struct Mqtt_config {
  char mqtt_server[30];
  char inbound_topic[30];
  char outbound_topic[30];
} mqtt_config_t;

class ThingConfig {
private:
  wifi_config_t wifi_config;
  mqtt_config_t mqtt_config;

public:
  // Save to flash(SPI) somewhere in the setup mode
  void set_wifi_config(wifi_config_t wifi_config);
  void set_mqtt_config(mqtt_config_t mqtt_config);
  // Read from the flash(SPI) where it has been saved
  wifi_config_t get_wifi_config(void);
  mqtt_config_t get_mqtt_config(void);
};

//TODO: Implement the ThingConfig
#endif
