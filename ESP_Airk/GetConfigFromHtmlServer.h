#pragma once
#include "mqttFunc.h"
#include "html-content.h"

void ReadEEPROM(){
    EEPROM.begin(sizeof(eepromData));
    EEPROM.get(0, eepromData);
    PRINTVAR(sizeof(eepromData));
    DPRINTLNFUNC("GOT DATA FROM EEPROM:");
    PRINTVAR(eepromData.DeviceState);
    PRINTVAR(eepromData.DeviceStatePrev);
    PRINTVAR(eepromData.SetWifiMode);
    PRINTVAR(eepromData.WifiSsid);
    PRINTVAR(eepromData.WifiPassword);
    PRINTVAR(eepromData.WifiSsidOld);
    PRINTVAR(eepromData.WifiPasswordOld);    
    PRINTVAR(eepromData.Description);
    PRINTVAR(eepromData.DescriptiveLocation);
    PRINTVAR(eepromData.LatitudeSet);
    PRINTVAR(eepromData.LongtitudeSet);
    PRINTVAR(eepromData.AltitudeSet);
    PRINTVAR(eepromData.mqttServerAddress);
    PRINTVAR(eepromData.mqttServerPort);
    PRINTVAR(eepromData.mqttServerUsername);
    PRINTVAR(eepromData.mqttServerPassword);
    PRINTVAR(eepromData.mqttServiceServerAddress);
    PRINTVAR(eepromData.mqttServiceServerPort);
    PRINTVAR(eepromData.mqttServiceServerUsername);
    PRINTVAR(eepromData.mqttServiceServerPassword);   
    PRINTVAR(eepromData.mqttServerUserSettingsActive); 
    PRINTVAR(eepromData.mqttServerTopicTemplate);  
    PRINTVAR(eepromData.thingspeak_mqttPass);
    PRINTVAR(eepromData.thingspeak_ch1_writeAPIKey);
    PRINTVAR(eepromData.thingspeak_ch2_writeAPIKey);
    PRINTVAR(eepromData.thingspeak_channelID_1);
    PRINTVAR(eepromData.thingspeak_channelID_2);
    PRINTVAR(eepromData.kNO);
    PRINTVAR(eepromData.kNO2);
    PRINTVAR(eepromData.kCO);
    PRINTVAR(eepromData.kSO2);
    PRINTVAR(eepromData.kH2S);
    PRINTVAR(eepromData.kO3low);
    PRINTVAR(eepromData.kO3high);
    PRINTVAR(eepromData.kC6H6);
}

//////////////////////////////////////////////////////////////
// * Webserver page not found
 
void webserver_not_found() {
  
  if (WiFi.status() != WL_CONNECTED) {
    if ((server.uri().indexOf(F("success.html")) != -1) || (server.uri().indexOf(F("detect.html")) != -1)) {
      server.send(200,FPSTR(TXT_CONTENT_TYPE_TEXT_HTML),FPSTR(WEB_IOS_REDIRECT));
    } else {
      server.sendHeader(F("Location"), F("http://192.168.4.1/config"));
      server.send(302, FPSTR(TXT_CONTENT_TYPE_TEXT_HTML), "");
    }
  } else {
    server.send(404, FPSTR(TXT_CONTENT_TYPE_TEXT_PLAIN), F("Not found."));
  }
}
