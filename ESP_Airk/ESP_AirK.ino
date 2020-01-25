#include <Arduino.h>
#include <Hash.h>
#include <ArduinoJson.h>
#include <time.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <RBD_Timer.h>
#include <RBD_Button.h>
#include "GetConfigFromHtmlServer.h"
#include "GetMeasurementsData.h"
#include "GetConfiguration.h"
#include "mqttFunc.h"
#include "ESP_AirK.h"
#include "html-content.h"
#include "webserver_config.h"

/////////////////////////////////////////////////////////////////////////////

void InitialWriteEEPROM(){
  
  DPRINTLNFUNC("Writing initial values to EEPROM");
  EEPROM.begin(sizeof(eepromData));
  EEPROM.get(0, eepromData);
  eepromData.SetWifiMode=0;
  eepromData.DeviceState=1;
  eepromData.DeviceStatePrev=1;
  eepromData.MessageID=1;
  eepromData.TimeNow=0;
  sprintf(eepromData.WifiSsid,"ip-68");
  sprintf(eepromData.WifiPassword,"3503503503");
  sprintf(eepromData.WifiSsidOld,"");
  sprintf(eepromData.WifiPasswordOld,"");
  sprintf(eepromData.Description,"");
  sprintf(eepromData.DescriptiveLocation,"");
  sprintf(eepromData.LatitudeSet,"");
  sprintf(eepromData.LongtitudeSet,"");
  sprintf(eepromData.AltitudeSet,"");
  sprintf(eepromData.mqttServerAddress,"mqtt.meteostation.online");
  sprintf(eepromData.mqttServerUsername,"kyivis");
  sprintf(eepromData.mqttServerPassword,"smartcity");
  eepromData.mqttServerPort=1883;
  sprintf(eepromData.mqttServiceServerAddress,"mqtt.meteostation.online");
  sprintf(eepromData.mqttServiceServerUsername,"kyivis");
  sprintf(eepromData.mqttServiceServerPassword,"smartcity");
  eepromData.mqttServiceServerPort=1883;
  eepromData.mqttServerUserSettingsActive=0;
  strncpy(eepromData.mqttServerTopicTemplate,client_topic_template,strlen(client_topic_template)+1);
  sprintf(eepromData.thingspeak_mqttPass,"DJ733U6CSPL6LOC7");
  sprintf(eepromData.thingspeak_ch1_writeAPIKey,"FSZAZMF3MCR5L8YN");
  sprintf(eepromData.thingspeak_ch2_writeAPIKey,"RMTIGO47DQ79CWB9");
  eepromData.thingspeak_channelID_1=813133;
  eepromData.thingspeak_channelID_2=813265;
  eepromData.kNO=1;
  eepromData.kNO2=1;
  eepromData.kCO=1;
  eepromData.kSO2=1;
  eepromData.kH2S=1;
  eepromData.kO3low=1;
  eepromData.kO3high=1;
  eepromData.kC6H6=1;
  
  EEPROM.put(0, eepromData);
  EEPROM.commit();
  
  DPRINTLNFUNC("Writing initial values complete");
  ReadEEPROM();
  DPRINTLNFUNC("Ethernal loop");
  while(1) {
    DPRINTFUNC(".");
    digitalWrite(LED_PIN_BLUE, led);
    led = !led;
    delay(300);
  }
}

/////////////////////////////////////////////////////////////////////////////

int try_reconnect() {
  digitalWrite(LED_PIN_BLUE, LOW);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    
    DPRINTFUNC("Trying to connect ");
    
    if (prev_wifi_creditials_active) WiFi.begin(eepromData.WifiSsidOld, eepromData.WifiPasswordOld);
    else WiFi.begin(eepromData.WifiSsid, eepromData.WifiPassword);

    led = 1;
    for (int i=10;i>0;i--) {
      digitalWrite(LED_PIN_RED, led);
      led = !led;
      DPRINTFUNC(i);
      DPRINTFUNC(",");
      delay(500);
    }
    digitalWrite(LED_PIN_RED, LOW);
    
    if (WiFi.status() == WL_CONNECTED) break;
    else {
      DPRINTFUNC(" connect attempt "); 
      DPRINTFUNC(connect_attempt);
      DPRINTLNFUNC(" failed");
      connect_attempt += 1;
    }
    
    if (connect_attempt > 5) 
    {
      if (prev_wifi_creditials_active) {
        DPRINTLNFUNC("Can't reconnect with old pass, trying all again");
        prev_wifi_creditials_active = false;
      }
      else {
        DPRINTLNFUNC("Bad creditials, previous one activated now");
        prev_wifi_creditials_active = true;
      }
      connect_attempt = 0;
    }
    CheckWiFiButton();
  }
  
  if (prev_wifi_creditials_active){
    strncpy(eepromData.WifiSsid, eepromData.WifiSsidOld, 20);
    strncpy(eepromData.WifiPassword, eepromData.WifiPasswordOld, 20);
    DPRINTFUNC("Restoring prev cred to actual one");
    EEPROM.put(0, eepromData);
    EEPROM.commit();    
    prev_wifi_creditials_active=false;
  }
  
  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void Connect2WiFiNetwork(){
  WiFi.disconnect();
  WiFi.setOutputPower(20.5);
  WiFi.setPhyMode(WIFI_PHY_MODE_11G);
  DPRINTLNFUNC("Start ESP8266 working in station mode...");
  WiFi.mode(WIFI_STA); // ESP8266 works in station mode
  WiFi.begin(eepromData.WifiSsid, eepromData.WifiPassword);
  if (MDNS.begin(server_name.c_str())) MDNS.addService("http", "tcp", 80);
  delay(50);
}
//////////////////////////////////////////////////////////////////////////////    

void ActiveWiFiState(){
      DPRINTLNFUNC("Device is in active and working state, now trying to connect wifi");
      digitalWrite(ONOFFPIN,HIGH);

      Connect2WiFiNetwork();

      if (!try_reconnect()) DPRINTLNFUNC("WiFi connected");
  
      DPRINTLNFUNC("WiFI Server started");
      DPRINTLNFUNC(WiFi.localIP());

      configTime(timezone * 3600, 0, "pool.ntp.org", "time.nist.gov");
      DPRINTLNFUNC("Waiting for time from internet");
      while (!time(nullptr)) {
        DPRINTFUNC(".");
        delay(1000);
      }
}

//////////////////////////////////////////////////////////////////////////////    

void ConnectWiFiDeviceisOff(){
      DPRINTLNFUNC("DEVICE IS OFF");
      digitalWrite(ONOFFPIN,LOW);
      
      Connect2WiFiNetwork();
      
      if (!try_reconnect()) DPRINTLNFUNC("WiFi connected");      
    }

//////////////////////////////////////////////////////////////////////////////    

void StartWiFiServerMode(){ 
  
  DPRINTLNFUNC("Started server mode for wifi WifiPassword configuration");

  IPAddress local_IP(192, 168, 4, 1);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);

  String SSID;
  uint8_t* BSSID;
  int channels_rssi[14];
  uint8_t AP_channel = 1;

  WiFi.disconnect(true);
  
  DPRINTLNFUNC("scan for wifi networks...");
  
  count_wifiInfo = WiFi.scanNetworks(false, true);
  wifiInfo = (struct_wifiInfo *) malloc(count_wifiInfo * 100);
  
  for (int i = 0; i < 14; i++) channels_rssi[i] = -100;
  
  for (int i = 0; i < count_wifiInfo; i++) {
    WiFi.getNetworkInfo(i, SSID, wifiInfo[i].encryptionType, wifiInfo[i].RSSI, BSSID, wifiInfo[i].channel, wifiInfo[i].isHidden);
    SSID.toCharArray(wifiInfo[i].ssid, 35);
    if (wifiInfo[i].RSSI > channels_rssi[wifiInfo[i].channel]) {
      channels_rssi[wifiInfo[i].channel] = wifiInfo[i].RSSI;
    }
  }
  
  if ((channels_rssi[1] < channels_rssi[6]) && (channels_rssi[1] < channels_rssi[11])) AP_channel = 1;
  else if ((channels_rssi[6] < channels_rssi[1]) && (channels_rssi[6] < channels_rssi[11])) AP_channel = 6;
  else AP_channel = 11;

    WiFi.mode(WIFI_AP);
    
    DPRINTFUNC("Setting soft-AP configuration ... ");
    DPRINTLNFUNC(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

    DPRINTFUNC("Setting soft-AP ... ");
    DPRINTLNFUNC(WiFi.softAP("AirK_config","",AP_channel,false,2) ? "Ready" : "Failed!");

    DPRINTFUNC("Start cycle, Soft-AP IP address = ");
    DPRINTLNFUNC(WiFi.softAPIP());

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", local_IP);             // 53 is port for DNS server

  unsigned long last_page_load = millis();
  unsigned long led_blink_timer = millis();
  led = HIGH;
  
  while ((millis() - last_page_load) < TIME_FOR_WIFI_CONFIG) {
    if (millis() - led_blink_timer > 500) 
    {
      digitalWrite(LED_PIN_GREEN, led);
      led = !led;
      led_blink_timer = millis();
    }
    dnsServer.processNextRequest();
    server.handleClient();
    yield();
  }

  digitalWrite(LED_PIN_GREEN, LOW);
  
  DPRINTFUNC("End cycle");
  // half second to answer last requests
  last_page_load = millis();
  while ((millis() - last_page_load) < 500) {
    dnsServer.processNextRequest();
    server.handleClient();
    yield();
  }

  WiFi.disconnect(true);
  WiFi.softAPdisconnect(true);
  dnsServer.stop();
  free(wifiInfo);

  DPRINTLNFUNC("End wifi server by 10 min timeout!");
  
  eepromData.SetWifiMode = 0;
  EEPROM.put(0, eepromData);
  EEPROM.commit();
  
  delay(100);
  ESP.restart();
  }

//////////////////////////////////////////////////////////////////////////////    

void CalibrationFunctionProcess(){
      BtnTimer.restart();
    DPRINTLNFUNC("Button calibration pressed - wait 2 sec");

    while (ButtonWifi.isPressed()) {
      if (BtnTimer.onExpired()) {
        if (ButtonWifi.isPressed()) {
          DPRINTLNFUNC("BUTTON calibration pressed - calibration mode started");
          delay(5000); //wait stm32 to start 
          digitalWrite(CALIBRATIONPIN,LOW); //Start calibration
          CalibrationTimer.restart();
          while(1) {
            digitalWrite(LED_PIN_GREEN, led);
            led = !led;
            delay(500);
            if (CalibrationTimer.isExpired()) {
              digitalWrite(LED_PIN_GREEN, LOW);
                for (int i = 0; i < 25; i++) {
                      digitalWrite(LED_PIN_RED, 1);
                      delay(100);
                      digitalWrite(LED_PIN_RED, 0);
                      delay(100);
                    }
              ESP.restart();
            }
          }
        }
        else {
          BtnTimer.stop();
          break;
        }
      }
      delay(10);
    }
  }

//////////////////////////////////////////////////////////////////////////////    

void setup_webserver(){
  server_name = F("AirK-Server-");
  server_name += String(DeviceID);

  server.on("/", webserver_root);
  server.on("/config", webserver_config);
  server.on("/reset", webserver_reset);
  server.onNotFound(webserver_not_found);

  server.begin();
  DPRINTLNFUNC("Server started"); 
}

void CheckWiFiButton(){
 if (ButtonWifi.isPressed() && BtnTimer.onExpired()) 
   {
      DPRINTLNFUNC("BUTTON SET WIFI HELD - RESTARTING");
      if (eepromData.SetWifiMode != 1) eepromData.SetWifiMode = 1;
      else eepromData.SetWifiMode = 0; 
      EEPROM.put(0, eepromData);
      EEPROM.commit();
      ESP.restart();
   }
 else if (ButtonWifi.isPressed() && !BtnTimer.isActive()) 
   {
      BtnTimer.restart();
      DPRINTLNFUNC("ButtonWifi Pressed");
   }
 else if (ButtonWifi.isPressed() && BtnTimer.isActive()) 
   {
   DPRINTLNFUNC(BtnTimer.getValue());
   }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  
  BtnTimer.setTimeout(2000);
  BtnTimer.stop();

  CalibrationTimer.setTimeout(120000); //2min wait for calibration to end
  CalibrationTimer.stop();

  esp_chipid = String(ESP.getChipId());

  Serial.begin(115200);
  while (!Serial) continue;
  DPRINTLNFUNC("Serial started");

  pinMode(LED_PIN_BLUE, OUTPUT);
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);
  pinMode(FULLRESETPIN, INPUT_PULLUP);
  pinMode(ONOFFPIN, OUTPUT);
  pinMode(CALIBRATIONPIN, OUTPUT);

  digitalWrite(LED_PIN_BLUE, LOW);
  digitalWrite(LED_PIN_RED, LOW);
  digitalWrite(LED_PIN_GREEN, LOW);
  digitalWrite(CALIBRATIONPIN, HIGH);

  for (int i = 0; i < 20; i++) {
    digitalWrite(LED_PIN_BLUE, 1);
    delay(100);
    digitalWrite(LED_PIN_BLUE, 0);
    delay(100);
  }
  PRINTVAR(digitalRead(FULLRESETPIN));
  if (digitalRead(FULLRESETPIN)==0) InitialWriteEEPROM();
  ReadEEPROM();
  MqttConnectionTimer.setTimeout(60000);

    //CALIBRATION
    if (ButtonWifi.isPressed()) CalibrationFunctionProcess();
    setup_webserver();

    if (eepromData.SetWifiMode == 0)
    {
    
      DPRINTLNFUNC("Working mode activated");
      //If we wanna use other mqtt server mqttServerUserSettingsActive must be 1
      if (eepromData.mqttServerUserSettingsActive != 1) 
      {
          DPRINTLNFUNC("!!! USING STANDARD MQTT SETTINGS !!!");
          mqtt_publish_client.setServer(standart_mqtt_server_address, standart_mqtt_server_port);
          mqtt_service_client.setServer(standart_mqtt_service_server_address, standart_mqtt_service_server_port);
          
          PRINTVAR(standart_mqtt_server_address);
          PRINTVAR(standart_mqtt_server_port);
          PRINTVAR(standart_mqtt_server_login);
          PRINTVAR(standart_mqtt_server_password);
          
          PRINTVAR(standart_mqtt_service_server_address);
          PRINTVAR(standart_mqtt_service_server_port);
          PRINTVAR(standart_mqtt_service_server_login);
          PRINTVAR(standart_mqtt_service_server_password);
      
          PRINTVAR(client_topic_template);
      }
      else if (eepromData.mqttServerUserSettingsActive == 1)
      {
          mqtt_publish_client.setServer(eepromData.mqttServerAddress, eepromData.mqttServerPort);
          mqtt_service_client.setServer(eepromData.mqttServiceServerAddress, eepromData.mqttServiceServerPort);
      }
  
    mqtt_publish_client.setCallback(client_callback);
    mqtt_service_client.setCallback(service_callback);

    // Set Thingspeak data mqtt
    mqtt_thingspeak_client.setServer(mqtt_server_thingspeak, port_thingspeak);
    mqtt_thingspeak_client.setCallback(thingspeak_callback);
    
    //WiFi Config start
    if (eepromData.DeviceState)  ActiveWiFiState();
    else ConnectWiFiDeviceisOff();
    
  } 
  else if (eepromData.SetWifiMode == 1) StartWiFiServerMode();     //WiFi Server mode start - get NEW wifi settings
  else DPRINTLNFUNC("Wrong eepromData.SetWifiMode !!!!");
  
  DPRINTFUNC("EXIT FROM SETUP:");
  DPRINTFUNC(" DeviceState=");
  DPRINTFUNC(eepromData.DeviceState);
  DPRINTFUNC(" eepromData.DeviceStatePrev=");
  DPRINTFUNC(eepromData.DeviceStatePrev);
  DPRINTFUNC(" set_wifi_mode=");
  DPRINTLNFUNC(eepromData.SetWifiMode);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  
  espLifeTime = millis();
  
  if (espLifeTime > ESP_AUTORESET_INTERVAL) ESP.restart();
  
  if (loop_cntr >= 200) {
    loop_cntr = 0;
    if (eepromData.SetWifiMode == 1) digitalWrite(LED_PIN_GREEN, led);
    else if (eepromData.DeviceState == 0) digitalWrite(LED_PIN_BLUE, LOW);
    else digitalWrite(LED_PIN_BLUE, led);
    led = !led;
    
    DPRINTFUNC("LOOP STATUS: DeviceState=");
    DPRINTFUNC(eepromData.DeviceState);
    DPRINTFUNC(" eepromData.DeviceStatePrev=");
    DPRINTFUNC(eepromData.DeviceStatePrev);
    DPRINTFUNC(" espLifeTime=");
    DPRINTFUNC(espLifeTime);
    DPRINTFUNC(" set_wifi_mode=");
    DPRINTLNFUNC(eepromData.SetWifiMode);
    
    if (WiFi.softAPgetStationNum() != stationsPrev) {
      DPRINTFUNC("New client: Stations connected = ");
      DPRINTLNFUNC(WiFi.softAPgetStationNum());
      stationsPrev = WiFi.softAPgetStationNum();
    }
  }

  //Button set WiFi
  CheckWiFiButton();

  if (mqtt_publish_client.connected()) mqtt_publish_client.loop();
  else if (MqttConnectionTimer.isExpired()) {
    reconnectPublishClient();
    MqttConnectionTimer.restart();
  }
  
  if (mqtt_service_client.connected()) mqtt_service_client.loop();
  else if (MqttConnectionTimer.isExpired()) {
    try_service_server_connect=false;
    reconnectServiceClient();
    MqttConnectionTimer.restart();
  }

  if (mqtt_thingspeak_client.connected()) mqtt_thingspeak_client.loop();
  else if (MqttConnectionTimer.isExpired()) {
    reconnectThingspeakClient();
    MqttConnectionTimer.restart();
  }
  
  if (eepromData.DeviceState) GetMeasurementsData(); 
    
  if (WiFi.status() != WL_CONNECTED) 
  {
    Connect2WiFiNetwork();
    if (!try_reconnect()) DPRINTLNFUNC("WiFi connected");
  }
  
  loop_cntr++;
  delay(10);
}
