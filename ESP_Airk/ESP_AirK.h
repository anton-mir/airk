#pragma once

//////////////////////////////////////////////////////
#define SOFTWARE_VERSION "AIRK-2019-1"

#define LED_PIN_BLUE 14
#define LED_PIN_RED 12
#define LED_PIN_GREEN 13
#define BTN 5
#define FULLRESETPIN 4
#define ONOFFPIN 16
#define CALIBRATIONPIN 2
#define ESP_AUTORESET_INTERVAL 86400000 // one full day
#define TIME_FOR_WIFI_CONFIG 360000 // 5 min
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

String esp_chipid;
long last_page_load = millis();

char mqtt_topic[50];
char mqtt_topic_thingspeak[50];
char json[2000];
char json_config[1500];

int timezone = 1;
int dst, set_wifi_mode, PacketsSent, mqtt_value = 0;
int DeviceID = 2;
String server_name;

char*client_topic_template="/airk_device/%d/%s";
//char*client_topic_template="airKdevice_%d_%s";

char thingspeak_topic_template[]="channels/%d/publish/%s";
const char* mqtt_server_thingspeak = "mqtt.thingspeak.com"; 
const int port_thingspeak = 1883;
char thingspeak_mqttUserName[] = "AirK_dev";  // Use any name.
uint8_t thingspeak_publish_counter=0;

// dev#3 yellow btn
//char thingspeak_mqttPass[] = "DJ733U6CSPL6LOC7";  // Change this to your MQTT API Key from Account > MyProfile.
//char thingspeak_ch1_writeAPIKey[] = "FSZAZMF3MCR5L8YN";    // channel 1 write API key.
//char thingspeak_ch2_writeAPIKey[] = "RMTIGO47DQ79CWB9";    // channel 2 write API key.
//long thingspeak_channelID_1 = 813133; //PM and temp etc
//long thingspeak_channelID_2 = 813265; //Gases
// dev#2 green btn
char thingspeak_mqttPass[] = "UDN1NGN3ECCP5NPJ";  // Change this to your MQTT API Key from Account > MyProfile.
char thingspeak_ch1_writeAPIKey[] = "MASOOO9BI15KUFRX";    // channel 1 write API key.
char thingspeak_ch2_writeAPIKey[] = "V4C50O7XBUHEWO75";    // channel 2 write API key.
long thingspeak_channelID_1 = 830248; //PM and temp etc
long thingspeak_channelID_2 = 830268; //Gases
// dev#1 whitebtn
//char thingspeak_mqttPass[] = "U7TI8ACKX9B5SQWY";  // Change this to your MQTT API Key from Account > MyProfile.
//char thingspeak_ch1_writeAPIKey[] = "78T4WTUSUQF3I9TT";    // channel 1 write API key.
//char thingspeak_ch2_writeAPIKey[] = "7YSQXF6C71XEZCTO";    // channel 2 write API key.
//long thingspeak_channelID_1 = 845515; //PM and temp etc
//long thingspeak_channelID_2 = 845516; //Gases

char*standart_mqtt_server_address="mqtt.meteostation.online";
int standart_mqtt_server_port=1883;
char*standart_mqtt_server_login="kyivis";
char*standart_mqtt_server_password="smartcity";

char*standart_mqtt_service_server_address="157.230.121.25";
int standart_mqtt_service_server_port=1883;
char*standart_mqtt_service_server_login="";
char*standart_mqtt_service_server_password="";

byte loop_cntr, connect_attempt, stationsPrev = 0;
boolean prev_wifi_creditials_active, led, try_service_server_connect=false;

long timer_btn = 0;

unsigned long espLifeTime = millis();

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
  char thingspeak_mqttPass[20];
  char thingspeak_ch1_writeAPIKey[20];
  char thingspeak_ch2_writeAPIKey[20];
  long thingspeak_channelID_1;
  long thingspeak_channelID_2;
  float kNO;
  float kNO2;
  float kCO;
  float kSO2;
  float kH2S;
  float kO3low;
  float kO3high;
  float kC6H6;
} eepromData;

struct struct_wifiInfo {
  char ssid[35];
  uint8_t encryptionType;
  int32_t RSSI;
  int32_t channel;
  bool isHidden;
};

struct struct_wifiInfo *wifiInfo;
uint8_t count_wifiInfo;

DNSServer dnsServer;
WiFiClient mqtt_espClient;
WiFiClient mqtt_espService;
WiFiClient mqtt_thingspeak;

PubSubClient mqtt_publish_client(mqtt_espClient);
PubSubClient mqtt_service_client(mqtt_espService);
PubSubClient mqtt_thingspeak_client(mqtt_thingspeak);

StaticJsonDocument<1000> doc_in;
StaticJsonDocument<2000> doc_out;
WiFiServer server0(80);
ESP8266WebServer server(80);
RBD::Timer BtnTimer;
RBD::Timer CalibrationTimer;
RBD::Timer MqttConnectionTimer;
RBD::Button ButtonWifi(BTN);
