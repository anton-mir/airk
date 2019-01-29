#include <Hash.h>
#include <ArduinoJson.h>
#include <time.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <RBD_Timer.h>
#include <RBD_Button.h>
#include "GetConfigFromHtmlServer.h"
#include "GetMeasurementsData.h"
#include "GetConfiguration.h"
#include "mqttFunc.h"
#include "ESP_AirK.h"

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
  sprintf(eepromData.WifiSsid,"Hollywood Zoo");
  sprintf(eepromData.WifiPassword,"ankatanka1177");
  sprintf(eepromData.WifiSsidOld," ");
  sprintf(eepromData.WifiPasswordOld," ");
  sprintf(eepromData.Description,"");
  sprintf(eepromData.DescriptiveLocation," ");
  sprintf(eepromData.LatitudeSet," ");
  sprintf(eepromData.LongtitudeSet," ");
  sprintf(eepromData.AltitudeSet," ");
  sprintf(eepromData.mqttServerAddress,"46.101.166.167");
  sprintf(eepromData.mqttServerUsername," ");
  sprintf(eepromData.mqttServerPassword," ");
  eepromData.mqttServerPort=1883;
  sprintf(eepromData.mqttServiceServerAddress,"192.168.88.230");
  sprintf(eepromData.mqttServiceServerUsername," ");
  sprintf(eepromData.mqttServiceServerPassword," ");
  eepromData.mqttServiceServerPort=1883;
  eepromData.mqttServerUserSettingsActive=0;
  strncpy(eepromData.mqttServerTopicTemplate,client_topic_template,strlen(client_topic_template)+1);
  
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
  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    DPRINTFUNC(".");
    digitalWrite(LED_PIN_RED, led);
    led = !led;
    connect_attempt += 1;
    if (connect_attempt > 10 && prev_wifi_creditials_active == false) {
      DPRINTFUNC("Bad creditials, previous one activated now");
      prev_wifi_creditials_active = true;
      connect_attempt = 0;
      WiFi.begin(eepromData.WifiSsidOld, eepromData.WifiPasswordOld);
    }
    else if (connect_attempt > 10 && prev_wifi_creditials_active == true) {
      DPRINTLNFUNC("Can't reconnect with old pass, restarting and turning off mqttServerUserSettingsActive option");
      eepromData.mqttServerUserSettingsActive = 0;
      eepromData.DeviceState = 2;
      EEPROM.put(0, eepromData);
      EEPROM.commit();
      delay(100);
//      ESP.reset();
      return 1;
    }
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


void setup() {
  
  BtnTimer.setTimeout(2000);
  BtnTimer.stop();

  CalibrationTimer.setTimeout(120000); //2min wait for calibration to end
  CalibrationTimer.stop();

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

///////////CALIBRATION////////////////////////////////////////////////////////////////////////////////////////////////

    if (ButtonWifi.isPressed()) {
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
              ESP.reset();
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  if (eepromData.SetWifiMode == 0) {
    
    DPRINTLNFUNC("Working mode activated");

  //If we wanna use other mqtt server mqttServerUserSettingsActive must be 1
  if (eepromData.mqttServerUserSettingsActive != 1) {
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
  else if (eepromData.mqttServerUserSettingsActive == 1){
    mqtt_publish_client.setServer(eepromData.mqttServerAddress, eepromData.mqttServerPort);
    mqtt_service_client.setServer(eepromData.mqttServiceServerAddress, eepromData.mqttServiceServerPort);
  }
  
  mqtt_publish_client.setCallback(client_callback);
  mqtt_service_client.setCallback(service_callback);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //WiFi Config start
    
    if (eepromData.DeviceState == 1) { 

      DPRINTLNFUNC("Device is in active and working state, now trying to connect wifi");
      digitalWrite(ONOFFPIN,HIGH);

      WiFi.mode(WIFI_STA);
      WiFi.begin(eepromData.WifiSsid, eepromData.WifiPassword);

      if (!try_reconnect()) DPRINTLNFUNC("WiFi connected");
  
      server.begin();
      DPRINTLNFUNC("WiFI Server started");
      DPRINTLNFUNC(WiFi.localIP());

      configTime(2 * 3600, 0, "pool.ntp.org", "time.nist.gov");
      DPRINTLNFUNC("Waiting for time from internet");
      while (!time(nullptr)) {
        DPRINTFUNC(".");
        delay(1000);
      }
    }

    ///////////////////////////////////////////
    
    else if (eepromData.DeviceState == 2) {
      DPRINTLNFUNC("RECONNECT WIFI STATE ACTIVATED");
      digitalWrite(ONOFFPIN,HIGH);
      WiFi.mode(WIFI_STA);
      WiFi.begin(eepromData.WifiSsid, eepromData.WifiPassword);
      if (!try_reconnect()) {
        DPRINTLNFUNC("WiFi connected");
        eepromData.DeviceState = 1;
      }
      else {
        DPRINTLNFUNC("Cant connect to wifi anyway afrer all attempts, start in server mode for wifi WifiPassword configuration");
        eepromData.SetWifiMode = 1;     
        EEPROM.put(0, eepromData);
        EEPROM.commit();
        ESP.reset();
      }
      
      EEPROM.put(0, eepromData);
      EEPROM.commit();
    }

    ///////////////////////////////////////////
    
    else if (eepromData.DeviceState == 0) {
      DPRINTLNFUNC("DEVICE IS OFF");
      digitalWrite(ONOFFPIN,LOW);
      WiFi.mode(WIFI_STA);
      WiFi.begin(eepromData.WifiSsid, eepromData.WifiPassword);
      if (!try_reconnect()) DPRINTLNFUNC("WiFi connected");      
    }
  } 

    //WiFi Config end
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //WiFi Server mode start - gettings wifi settings
    
  else if (eepromData.SetWifiMode == 1) { 
    DPRINTLNFUNC("Started server mode for wifi WifiPassword configuration");

    IPAddress local_IP(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 9);
    IPAddress subnet(255, 255, 255, 0);

    DPRINTFUNC("Setting soft-AP configuration ... ");
    DPRINTLNFUNC(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

    DPRINTFUNC("Setting soft-AP ... ");
    DPRINTLNFUNC(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");

    DPRINTFUNC("Soft-AP IP address = ");
    DPRINTLNFUNC(WiFi.softAPIP());

    server.begin();
    DPRINTLNFUNC("Server started");
  }
  else DPRINTLNFUNC("Wrong eepromData.SetWifiMode !!!!");
  
    //WiFi Server mode end
    ///////////////////////////////////////////

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
  
  ///////////////////////////////////////////
  //Save timestamp

  if (loop_cntr >= 2000) {
    eepromData.TimeNow = time(nullptr);
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }

  ///////////////////////////////////////////
  
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
    DPRINTFUNC(" set_wifi_mode=");
    DPRINTLNFUNC(eepromData.SetWifiMode);
    
    if (WiFi.softAPgetStationNum() != stationsPrev) {
      DPRINTFUNC("New client: Stations connected = ");
      DPRINTLNFUNC(WiFi.softAPgetStationNum());
      stationsPrev = WiFi.softAPgetStationNum();
    }
  }
  loop_cntr++;

  ///////////////////////////////////////////
  //Button set WiFi
  
  if (ButtonWifi.isPressed()) {
    BtnTimer.restart();
    DPRINTLNFUNC("ButtonWifi.isPressed");

    while (ButtonWifi.isPressed()) {
      if (BtnTimer.onExpired()) {
        if (ButtonWifi.isPressed()) {
          DPRINTLNFUNC("BUTTON SET WIFI PRESSED - RESTARTING");
          if (eepromData.SetWifiMode != 1) eepromData.SetWifiMode = 1;
          else eepromData.SetWifiMode = 0; 

          EEPROM.put(0, eepromData);
          EEPROM.commit();
          ESP.reset();
        }
        else {
          BtnTimer.stop();
          break;
        }
      }
      delay(10);
    }
  }

  ///////////////////////////////////////////  

  if (eepromData.SetWifiMode == 1) GetConfigFromHtmlServer();
  else if (eepromData.SetWifiMode == 0) {
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
    if (eepromData.DeviceState == 1) GetMeasurementsData(); 
  }

  if (eepromData.DeviceState == 1 && eepromData.SetWifiMode != 1 && WiFi.status() != WL_CONNECTED) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(eepromData.WifiSsid, eepromData.WifiPassword);
    if (!try_reconnect()) DPRINTLNFUNC("WiFi connected");
  }
  delay(10);
}
