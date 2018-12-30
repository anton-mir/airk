#pragma once

//////////////////////////////////////////////////////

#define LED_PIN_BLUE 14
#define LED_PIN_RED 12
#define LED_PIN_GREEN 13
#define BTN 5
#define FULLRESETPIN 4
#define ONOFFPIN 16

//////////////////////////////////////////////////////
#define DEBUG
#ifdef DEBUG
#define DPRINTFUNC(...)    Serial.print(__VA_ARGS__)
#define DPRINTLNFUNC(...)  Serial.println(__VA_ARGS__)
#define PRINTVAR(var) Serial.print("VAR :: ");Serial.print(#var);Serial.print(" = [");Serial.print(var);Serial.println("]");
#else
#define PRINTVAR(...)
#define DPRINT(...)
#define DPRINTLN(...)
#endif
//////////////////////////////////////////////////////

char mqtt_topic[50];
char json[2000];
char json_config[1500];

int timezone = 2;
int dst, set_wifi_mode, PacketsSent, mqtt_value = 0;
int DeviceID = 3;

//char*client_topic_template="/airk_device/%d/%s";
char*client_topic_template="airKdevice_%d_%s";

//char*standart_mqtt_server_address="mqtt.meteostation.online";
//int standart_mqtt_server_port=1883;
//char*standart_mqtt_server_login="kyivis";
//char*standart_mqtt_server_password="smartcity";

char*standart_mqtt_server_address="46.101.166.167";
int standart_mqtt_server_port=1883;
char*standart_mqtt_server_login="";
char*standart_mqtt_server_password="";

char*standart_mqtt_service_server_address="192.168.88.230";
int standart_mqtt_service_server_port=1883;
char*standart_mqtt_service_server_login="";
char*standart_mqtt_service_server_password="";

byte loop_cntr, connect_attempt, stationsPrev = 0;
boolean prev_wifi_creditials_active, led, try_service_server_connect = false;

long timer_btn = 0;

struct {
  uint SetWifiMode;
  uint DeviceState;
  uint DeviceStatePrev;
  unsigned long MessageID;
  time_t TimeNow;
  char WifiSsid[20];
  char WifiPassword[20];
  char WifiSsidOld[20];
  char WifiPasswordOld[20];
  char Description[50];
  char DescriptiveLocation[50];
  char LatitudeSet[10];
  char LongtitudeSet[10];
  char AltitudeSet[10];
  char mqttServerAddress[40];
  char mqttServerUsername[40];
  char mqttServerPassword[40];
  long mqttServerPort;
  char mqttServiceServerAddress[40];
  char mqttServiceServerUsername[40];
  char mqttServiceServerPassword[40];
  long mqttServiceServerPort;  
  uint mqttServerUserSettingsActive;
  char mqttServerTopicTemplate[40];
} eepromData;

WiFiClient mqtt_espClient;
WiFiClient mqtt_espService;
PubSubClient mqtt_publish_client(mqtt_espClient);
PubSubClient mqtt_service_client(mqtt_espService);
StaticJsonDocument<1000> doc_in;
StaticJsonDocument<2000> doc_out;
WiFiServer server(80);

RBD::Timer BtnTimer;
RBD::Timer MqttConnectionTimer;
RBD::Button ButtonWifi(BTN);
