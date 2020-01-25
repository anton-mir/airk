#pragma once
#include "webserver_helper_func.h"

void webserver_config() {

  String page_content = make_header(FPSTR(INTL_KONFIGURATION));
  String masked_pwd = "";
  last_page_load = millis();

  DPRINTLNFUNC(F("output config page ..."));

  //vars for getting data from wifi server
  boolean mqttServerUserSettingsActiveSrv = eepromData.mqttServerUserSettingsActive;
  char wlanssidSrv[20] = "";
  strncpy(wlanssidSrv,eepromData.WifiSsid,20);
  char wlanpwdSrv[20] = "";
  strncpy(wlanpwdSrv,eepromData.WifiPassword,20);
  char descriptionSrv[50] = "";
  strncpy(descriptionSrv,eepromData.Description,50);
  char descriptiveLocationSrv[50] = "";
  strncpy(descriptiveLocationSrv,eepromData.DescriptiveLocation,50);
  char latitudeSrv[10] = "";
  strncpy(latitudeSrv,eepromData.LatitudeSet,10);
  char longtitudeSrv[10] = "";
  strncpy(longtitudeSrv,eepromData.LongtitudeSet,10);
  char altitudeSrv[10] = "";
  strncpy(altitudeSrv,eepromData.AltitudeSet,10);
  char mqttServerAddressSrv[40] = "";
  strncpy(mqttServerAddressSrv,eepromData.mqttServerAddress,40);
  int mqttServerPortSrv = eepromData.mqttServerPort;
  char mqttServerUsernameSrv[40] = "";
  strncpy(mqttServerUsernameSrv,eepromData.mqttServerUsername,40);
  char mqttServerPasswordSrv[40] = "";
  strncpy(mqttServerPasswordSrv,eepromData.mqttServerPassword,40);
  char mqttServiceServerAddressSrv[40] = "";
  strncpy(mqttServiceServerAddressSrv,eepromData.mqttServiceServerAddress,40);
  int mqttServiceServerPortSrv = eepromData.mqttServiceServerPort;
  char mqttServiceServerUsernameSrv[40] = "";
  strncpy(mqttServiceServerUsernameSrv,eepromData.mqttServiceServerUsername,40);
  char mqttServiceServerPasswordSrv[40] = "";  
  strncpy(mqttServiceServerPasswordSrv,eepromData.mqttServiceServerPassword,40);
  char mqttServerTopicTemplateSrv[40] = "";
  strncpy(mqttServerTopicTemplateSrv,eepromData.mqttServerTopicTemplate,40);

  page_content += FPSTR(WEB_CONFIG_SCRIPT);

  if (server.method() == HTTP_GET) {
    page_content += F("<form method='POST' action='/config' style='width:100%;'>\n<b>");
    page_content += FPSTR(TXT_WLAN_CONFIG);
    page_content += F("</b><br/>");

    page_content += F("<table>");
    page_content += form_input("wlanssidSrv", FPSTR(TXT_WIFI_NAME), wlanssidSrv, 20);
    page_content += form_password("wlanpwdSrv", FPSTR(TXT_WIFI_PASSWORD), wlanpwdSrv, 20);
    page_content += F("<tr><td><hr/></td><td>");
    page_content += form_input("mqttServerAddressSrv", FPSTR(TXT_SRV_ADR), mqttServerAddressSrv, 40);
    page_content += form_input("mqttServerPortSrv", FPSTR(TXT_SRV_PORT), String(mqttServerPortSrv), 4);
    page_content += F("<tr><td><hr/></td><td>");
    page_content += form_input("mqttServerUsernameSrv", FPSTR(TXT_SRV_USERNAME), mqttServerUsernameSrv, 40);
    page_content += form_input("mqttServerPasswordSrv", FPSTR(TXT_SRV_PASSWORD), mqttServerPasswordSrv, 40);
    page_content += F("<tr><td><hr/></td><td>");
    page_content += form_input("mqttServerTopicTemplateSrv", FPSTR(TXT_TOPIC_TMPL), mqttServerTopicTemplateSrv, 40);
    page_content += form_input("descriptionSrv", FPSTR(TXT_DESCR), descriptionSrv, 50);
    page_content += form_input("descriptiveLocationSrv", FPSTR(TXT_DESCR_LOC), descriptiveLocationSrv, 50);
    page_content += form_input("latitudeSrv", FPSTR(TXT_LATITUDE), latitudeSrv, 10);
    page_content += form_input("longtitudeSrv", FPSTR(TXT_LONGTITUDE), longtitudeSrv, 10);
    page_content += form_input("altitudeSrv", FPSTR(TXT_ALTITUDE), altitudeSrv, 10);

    page_content += form_checkbox("mqttServerUserSettingsActiveSrv", FPSTR(TXT_MQTT_USERSETTINGS), mqttServerUserSettingsActiveSrv);
    page_content += F("<tr><td><hr/></td><td>");
    page_content += form_input("mqttServiceServerAddressSrv", FPSTR(TXT_SRV_ADR_SERVICE), mqttServiceServerAddressSrv, 40);
    page_content += form_input("mqttServiceServerPortSrv", FPSTR(TXT_SRV_PORT_SERVICE), String(mqttServiceServerPortSrv), 4);
    page_content += F("<tr><td><hr/></td><td>");
    page_content += form_input("mqttServiceServerUsernameSrv", FPSTR(TXT_SRV_USERNAME_SERVICE), mqttServiceServerUsernameSrv, 40);
    page_content += form_input("mqttServiceServerPasswordSrv", FPSTR(TXT_SRV_PASSWORD_SERVICE), mqttServiceServerPasswordSrv, 40);

    page_content += F("</table><br/>\n");

    page_content += F("<table>");
    page_content += form_submit(FPSTR(TXT_SAVE));
    page_content += F("</table><br/>");

  } else {

#define readCharParam(param) if (server.hasArg(#param)){ server.arg(#param).toCharArray(param, sizeof(param)); }
#define readIntParam(param)  if (server.hasArg(#param)){ int val = server.arg(#param).toInt(); if (val != 0){ param = val; }}
#define readPasswdParam(param) if (server.hasArg(#param)){ masked_pwd = ""; for (uint8_t i=0;i<server.arg(#param).length();i++) masked_pwd += "*"; if (masked_pwd != server.arg(#param) || server.arg(#param) == "") { server.arg(#param).toCharArray(param, sizeof(param)); } }
#define readBoolParam(param) param = false; if (server.hasArg(#param)){ param = server.arg(#param) == "1"; }

    boolean hasArgs = false;
    
    if (server.hasArg("wlanssidSrv") && server.hasArg("wlanpwdSrv") && server.arg("wlanssidSrv") != "") 
    {
      readCharParam(wlanssidSrv);
      readPasswdParam(wlanpwdSrv);
      strncpy(eepromData.WifiSsid, wlanssidSrv, 20);
      strncpy(eepromData.WifiPassword, wlanpwdSrv, 20);
      hasArgs = true;
    }

    if ( (server.hasArg("mqttServerAddressSrv") && server.arg("mqttServerAddressSrv") != "") &&
    (server.hasArg("mqttServerPortSrv") && server.arg("mqttServerPortSrv") != ""))
    {
      readCharParam(mqttServerAddressSrv);
      readIntParam(mqttServerPortSrv);
      strncpy(eepromData.mqttServerAddress, mqttServerAddressSrv, 40);
      eepromData.mqttServerPort = mqttServerPortSrv;
      hasArgs = true;
    }

    if (server.hasArg("mqttServerUsernameSrv") && server.hasArg("mqttServerPasswordSrv"))
    {
      readCharParam(mqttServerUsernameSrv);
      readCharParam(mqttServerPasswordSrv);
      strncpy(eepromData.mqttServerUsername, mqttServerUsernameSrv, 40);
      strncpy(eepromData.mqttServerPassword, mqttServerPasswordSrv, 40);
      hasArgs = true;
    }

    if ( (server.hasArg("mqttServiceServerAddressSrv") && server.arg("mqttServiceServerAddressSrv") != "") &&
    (server.hasArg("mqttServiceServerPortSrv") && server.arg("mqttServiceServerPortSrv") != ""))
    {
      readCharParam(mqttServiceServerAddressSrv);
      readIntParam(mqttServiceServerPortSrv);
      strncpy(eepromData.mqttServiceServerAddress, mqttServiceServerAddressSrv, 40);
      eepromData.mqttServiceServerPort = mqttServiceServerPortSrv;
      hasArgs = true;
    }

    if (server.hasArg("mqttServiceServerUsernameSrv") && server.hasArg("mqttServiceServerPasswordSrv"))
    {
      readCharParam(mqttServiceServerUsernameSrv);
      readCharParam(mqttServiceServerPasswordSrv);
      strncpy(eepromData.mqttServiceServerUsername, mqttServiceServerUsernameSrv, 40);
      strncpy(eepromData.mqttServiceServerPassword, mqttServiceServerPasswordSrv, 40);
      hasArgs = true;
    } 

    if (server.hasArg("mqttServerTopicTemplateSrv") && server.arg("mqttServerTopicTemplateSrv") != "")
    {
      readCharParam(mqttServerTopicTemplateSrv);
      strncpy(eepromData.mqttServerTopicTemplate, mqttServerTopicTemplateSrv, 40);
      hasArgs = true;
    } 

    if (server.hasArg("descriptionSrv") && server.arg("descriptionSrv") != "")
    {
      readCharParam(descriptionSrv);
      strncpy(eepromData.Description, descriptionSrv, 50);
      hasArgs = true;
    } 

    if (server.hasArg("descriptiveLocationSrv") && server.arg("descriptiveLocationSrv") != "")
    {
      readCharParam(descriptiveLocationSrv);
      strncpy(eepromData.DescriptiveLocation, descriptiveLocationSrv, 50);
      hasArgs = true;
    } 

    if (server.hasArg("latitudeSrv") && server.arg("latitudeSrv") != "")
    {
      readCharParam(latitudeSrv);
      strncpy(eepromData.LatitudeSet, latitudeSrv, 10);
      hasArgs = true;
    } 

    if (server.hasArg("longtitudeSrv") && server.arg("longtitudeSrv") != "")
    {
      readCharParam(longtitudeSrv);
      strncpy(eepromData.LongtitudeSet, longtitudeSrv, 10);
      hasArgs = true;
    } 

    if (server.hasArg("altitudeSrv") && server.arg("altitudeSrv") != "")
    {
      readCharParam(altitudeSrv);
      strncpy(eepromData.AltitudeSet, altitudeSrv, 10);
      hasArgs = true;
    } 
    
    if (server.hasArg("mqttServerUserSettingsActiveSrv"))
    {
      readBoolParam(mqttServerUserSettingsActiveSrv);
      eepromData.mqttServerUserSettingsActive = mqttServerUserSettingsActiveSrv;
      hasArgs = true;
    } 

    if (hasArgs){
      eepromData.SetWifiMode=0;
      EEPROM.put(0, eepromData);
      EEPROM.commit();
      page_content += FPSTR(TXT_SETTINGS_OK);
      DPRINTLNFUNC("OK saved eeprom");
      delay(100);
      ReadEEPROM();
    }

delay(1);
    
#undef readCharParam
#undef readIntParam
#undef readPasswdParam
#undef readBoolParam

    page_content += F("<br/><br/><a href='/reset?confirm=yes'>");
    page_content += FPSTR(TXT_RESET);
    page_content += F("</a>");

  }

  page_content += make_footer();
  server.sendHeader(F("Cache-Control"), F("no-cache, no-store, must-revalidate"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), F("0"));
  server.send(200, FPSTR(TXT_CONTENT_TYPE_TEXT_HTML), page_content);
}

void webserver_root() {
  if (WiFi.status() != WL_CONNECTED) {
    server.sendHeader(F("Location"), F("http://192.168.4.1/config"));
    server.send(302, FPSTR(TXT_CONTENT_TYPE_TEXT_HTML), "");
  } else {
    String page_content = make_header(" ");
    last_page_load = millis();
    
    DPRINTLNFUNC(F("output root page ..."));
    
    page_content += FPSTR(WEB_ROOT_PAGE_CONTENT);
    page_content.replace("{t}", FPSTR(INTL_AKTUELLE_WERTE));
    page_content.replace(F("{map}"), FPSTR(INTL_KARTE_DER_AKTIVEN_SENSOREN));
    page_content.replace(F("{conf}"), FPSTR(INTL_KONFIGURATION));
    page_content.replace(F("{conf_delete}"), FPSTR(INTL_KONFIGURATION_LOSCHEN));
    page_content.replace(F("{restart}"), FPSTR(INTL_SENSOR_NEU_STARTEN));
    page_content += make_footer();
    server.send(200, FPSTR(TXT_CONTENT_TYPE_TEXT_HTML), page_content);
  }
}

void webserver_reset() {
  DPRINTLNFUNC(F("output reset page ..."));
  
  String page_content = make_header(FPSTR(INTL_SENSOR_NEU_STARTEN));
  last_page_load = millis();

  if (server.method() == HTTP_GET) {
    page_content += FPSTR(WEB_RESET_CONTENT);
    page_content.replace("{t}", FPSTR(INTL_SENSOR_WIRKLICH_NEU_STARTEN));
    page_content.replace("{b}", FPSTR(INTL_NEU_STARTEN));
    page_content.replace("{c}", FPSTR(INTL_ABBRECHEN));
  } else {
  delay(50);
  WiFi.forceSleepBegin();
  delay(50);
  WiFi.disconnect(true);
  delay(50);
  WiFi.softAPdisconnect(true);
  delay(50);
  dnsServer.stop();
  delay(50);
  DPRINTLNFUNC("End wifi server and restart");
  delay(50);
  ESP.restart();
  }
  page_content += make_footer();
  server.send(200, FPSTR(TXT_CONTENT_TYPE_TEXT_HTML), page_content);
}
