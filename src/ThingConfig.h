#ifndef __THING_CONFIG_H__
#define __THING_CONFIG_H__

typedef struct WifiConfig {
  char ssid[32];
  char password[20];
} WifiConfig;

typedef struct MqttConfig {
  char mqtt_server[30];
  char inbound_topic[30];
  char outbound_topic[30];
} MqttConfig;

class ThingConfig {
private:
  WifiConfig wifi_config;
  MqttConfig mqtt_config;

public:
  // Save to flash(SPI) somewhere in the setup mode
  void set_wifi_config(WifiConfig conf);
  void set_mqtt_config(MqttConfig conf);
  // Read from the flash(SPI) where it has been saved
  WifiConfig get_wifi_config(void);
  MqttConfig get_mqtt_config(void);
};

//TODO: Implement the ThingConfig
#endif
