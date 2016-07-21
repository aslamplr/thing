#include <cstring>
#include "ThingConfig.h"

void ThingConfig::set_wifi_config(WifiConfig conf){
  strcpy(wifi_config.ssid, conf.ssid);
  strcpy(wifi_config.password, conf.password);
}

WifiConfig ThingConfig::get_wifi_config(void){
  return wifi_config;
}

void ThingConfig::set_mqtt_config(MqttConfig conf){
  strcpy(mqtt_config.mqtt_server, conf.mqtt_server);
  strcpy(mqtt_config.inbound_topic, conf.inbound_topic);
  strcpy(mqtt_config.outbound_topic, conf.outbound_topic);
}

MqttConfig ThingConfig::get_mqtt_config(void){
  return mqtt_config;
}
