#pragma once
#include "mqttFunc.h"


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
}

//////////////////////////////////////////////////////////////

void GetConfigFromHtmlServer() {

  WiFiClient client = server.available();
  if (!client) return;

  while (!client) {
    digitalWrite(LED_PIN_BLUE, led);
    delay(100);
    led = !led;
  }

  DPRINTLNFUNC("New client connected to server");

  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><style>body {font-family: Arial;}input[type=text], select {width: 100%;padding: 12px 20px;margin: 8px 0;display: block;border: 1px solid #ccc;border-radius: 4px;box-sizing: border-box;}input[type=submit] {width: 100%;background-color: #4CAF50;color: white;padding: 14px 20px;margin: 8px 0;border: none;border-radius: 4px;cursor: pointer;}input[type=submit]:hover {background-color: #45a049;}div.container {border-radius: 5px;background-color: #f2f2f2;width: 50%;padding: 20px;}select{width: 100%;padding:16px20px;border: 1;border-radius: 4px;background-color: white;}</style><body><h3>AirK device settings form</h3><p>Please fill and press SUBMIT to save</p><div class=\"container\"><form action=\"/\"><label for=\"dDescription\">Device owner description</label><input type=\"text\" id=\"dDescription\" name=\"Description\" placeholder=\"Who is device owner?\"><label for=\"dState\">Working state</label><select id=\"dStateOn\" name=\"State\"><option value=\"On\">Device is On</option><option value=\"Off\">Device is Off</option></select><label for=\"dWiFiName\">WiFiName</label><input type=\"text\" id=\"dWiFiName\" name=\"WiFiName\" placeholder=\"Local wifi hotspot name\"><label for=\"dWiFiPassword\">WiFiPassword</label><input type=\"text\" id=\"dWiFiPassword\" name=\"WiFiPassword\" placeholder=\"WiFi hotspot password\"><label for=\"dDescriptiveLocation\">Device location</label><input type=\"text\" id=\"dDescriptiveLocation\" name=\"DescriptiveLocation\" placeholder=\"Where device is located? (text)\"><label for=\"dLatitudeSet\">GPS coordinates: LATITUDE</label><input type=\"text\" id=\"dLatitudeSet\" name=\"LatitudeSet\" placeholder=\"Enter latitude (check google maps)\"><label for=\"dLongtitudeSet\">GPS coordinates: LONGTITUDE</label><input type=\"text\" id=\"dLongtitudeSet\" name=\"LongtitudeSet\" placeholder=\"Enter longtitude (check google maps)\"><label for=\"dAltitudeSet\">Device ALTITUDE</label><input type=\"text\" id=\"dAltitudeSet\" name=\"AltitudeSet\" placeholder=\"Enter height from ground level\"><label for=\"dMqttServerAddress\">Mqtt Server Address</label><input type=\"text\" id=\"dMqttServerAddress\" name=\"MqttServerAddress\" placeholder=\"Enter mqtt server address\"><label for=\"dMqttServerPort\">Mqtt Server Port</label><input type=\"text\" id=\"dMqttServerPort\" name=\"MqttServerPort\" placeholder=\"MQTT server port\"><label for=\"dMqttServerLogin\">Mqtt Server Login</label><input type=\"text\" id=\"dMqttServerLogin\" name=\"MqttServerLogin\" placeholder=\"MQTT server login\"><label for=\"dMqttServerPassword\">Mqtt Server Password</label><input type=\"text\" id=\"dMqttServerPassword\" name=\"MqttServerPassword\" placeholder=\"MQTT server password\"><label for=\"dMqttServiceServerAddress\">Mqtt Service Server Address</label><input type=\"text\" id=\"dMqttServiceServerAddress\" name=\"MqttServiceServerAddress\" placeholder=\"Enter mqtt Service server address\"><label for=\"dMqttServiceServerPort\">Mqtt Service Server Port</label><input type=\"text\" id=\"dMqttServiceServerPort\" name=\"MqttServiceServerPort\" placeholder=\"MQTT Service server port\"><label for=\"dMqttServiceServerLogin\">Mqtt Service Server Login</label><input type=\"text\" id=\"dMqttServiceServerLogin\" name=\"MqttServiceServerLogin\" placeholder=\"MQTT Service server login\"><label for=\"dMqttServiceServerPassword\">Mqtt Service Server Password</label><input type=\"text\" id=\"dMqttServiceServerPassword\" name=\"MqttServiceServerPassword\" placeholder=\"MQTT Service server password\"><label for=\"dMqttTopicTemplate\">Mqtt Topic Template</label><input type=\"text\" id=\"dMqttTopicTemplate\" name=\"MqttTopicTemplate\" placeholder=\"Mqtt Topic Template\"><input type=\"submit\" value=\"SUBMIT\"></form></div></body></html>";
  client.print(s);

  byte clientTimeCnt = 255;
  while (!client.available() && clientTimeCnt) {
    digitalWrite(LED_PIN_BLUE, led);
    DPRINTFUNC(".");
    delay(100);
    led = !led;
    clientTimeCnt--;
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  DPRINTFUNC("GOT REPLY FROM SERVER=");
  DPRINTLNFUNC(req);

  if (req.indexOf("Description") != -1) { //Skipping - not the form

    int DescriptionValueIndex = req.indexOf("Description") + strlen("Description") + 1;
    int StateValueIndex = req.indexOf("State", DescriptionValueIndex) + strlen("State") + 1;
    int WiFiNameValueIndex = req.indexOf("WiFiName", StateValueIndex) + strlen("WiFiName") + 1;
    int WiFiPasswordValueIndex = req.indexOf("WiFiPassword", WiFiNameValueIndex) + strlen("WiFiPassword") + 1;
    int DescriptiveLocationValueIndex = req.indexOf("DescriptiveLocation", WiFiPasswordValueIndex) + strlen("DescriptiveLocation") + 1;
    int LatitudeSetValueIndex = req.indexOf("LatitudeSet", DescriptiveLocationValueIndex) + strlen("LatitudeSet") + 1;
    int LongtitudeSetValueIndex = req.indexOf("LongtitudeSet", LatitudeSetValueIndex) + strlen("LongtitudeSet") + 1;
    int AltitudeSetValueIndex = req.indexOf("AltitudeSet", LongtitudeSetValueIndex) + strlen("AltitudeSet") + 1;
    int MqttServerAddressValueIndex = req.indexOf("MqttServerAddress", AltitudeSetValueIndex) + strlen("MqttServerAddress") + 1;
    int MqttServerPortValueIndex = req.indexOf("MqttServerPort", MqttServerAddressValueIndex) + strlen("MqttServerPort") + 1;
    int MqttServerLoginValueIndex = req.indexOf("MqttServerLogin", MqttServerPortValueIndex) + strlen("MqttServerLogin") + 1;
    int MqttServerPasswordValueIndex = req.indexOf("MqttServerPassword", MqttServerLoginValueIndex) + strlen("MqttServerPassword") + 1;
    int MqttServiceServerAddressValueIndex = req.indexOf("MqttServiceServerAddress", AltitudeSetValueIndex) + strlen("MqttServiceServerAddress") + 1;
    int MqttServiceServerPortValueIndex = req.indexOf("MqttServiceServerPort", MqttServiceServerAddressValueIndex) + strlen("MqttServiceServerPort") + 1;
    int MqttServiceServerLoginValueIndex = req.indexOf("MqttServiceServerLogin", MqttServiceServerPortValueIndex) + strlen("MqttServiceServerLogin") + 1;
    int MqttServiceServerPasswordValueIndex = req.indexOf("MqttServiceServerPassword", MqttServiceServerLoginValueIndex) + strlen("MqttServiceServerPassword") + 1;    
    int MqttTopicTemplateValueIndex = req.indexOf("MqttTopicTemplate", MqttServiceServerPasswordValueIndex) + strlen("MqttTopicTemplate") + 1;    
    int EndreqValueIndex = req.indexOf("HTTP/1.1", MqttTopicTemplateValueIndex);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String DescriptionGet = req.substring(DescriptionValueIndex, StateValueIndex - strlen("State") - 2);
    DescriptionGet.trim();
    DescriptionGet.replace('+', ' ');
    DescriptionGet.replace("%2c", ",");
    DescriptionGet.replace("%2C", ",");
    DescriptionGet.replace("%2F", "/");
    DescriptionGet.replace("%25", "%");    

    String StateGet = req.substring(StateValueIndex, WiFiNameValueIndex - strlen("WiFiName") - 2);
    String WiFiNameGet = req.substring(WiFiNameValueIndex, WiFiPasswordValueIndex - strlen("WiFiPassword") - 2);
    String WiFiPasswordGet = req.substring(WiFiPasswordValueIndex, DescriptiveLocationValueIndex - strlen("DescriptiveLocation") - 2);

    String DescriptiveLocationGet = req.substring(DescriptiveLocationValueIndex, LatitudeSetValueIndex - strlen("LatitudeSet") - 2);
    DescriptiveLocationGet.trim();
    DescriptiveLocationGet.replace('+', ' ');
    DescriptiveLocationGet.replace("%2c", ",");
    DescriptiveLocationGet.replace("%2C", ",");
    DescriptiveLocationGet.replace("%2F", "/");
    DescriptiveLocationGet.replace("%25", "%");    

    String LatitudeSetGet = req.substring(LatitudeSetValueIndex, LongtitudeSetValueIndex - strlen("LongtitudeSet") - 2);
    LatitudeSetGet.replace('+', ' ');
    LatitudeSetGet.trim();
    LatitudeSetGet.replace("%2c", ",");
    LatitudeSetGet.replace("%2C", ",");
    LatitudeSetGet.replace("%2F", "/");
    LatitudeSetGet.replace("%25", "%");    

    String LongtitudeSetGet = req.substring(LongtitudeSetValueIndex, AltitudeSetValueIndex - strlen("AltitudeSet") - 2);
    LongtitudeSetGet.replace('+', ' ');
    LongtitudeSetGet.trim();
    LongtitudeSetGet.replace("%2c", ",");
    LongtitudeSetGet.replace("%2C", ",");    
    LongtitudeSetGet.replace("%2F", "/");
    LongtitudeSetGet.replace("%25", "%");    

    String AltitudeSetGet = req.substring(AltitudeSetValueIndex, MqttServerAddressValueIndex - strlen("MqttServerAddress") - 2);
    AltitudeSetGet.replace('+', ' ');
    AltitudeSetGet.replace("%2c", ",");
    AltitudeSetGet.replace("%2C", ",");
    AltitudeSetGet.replace("%2F", "/");
    AltitudeSetGet.replace("%25", "%");    
    AltitudeSetGet.trim();

    String MqttServerAddressGet = req.substring(MqttServerAddressValueIndex, MqttServerPortValueIndex - strlen("MqttServerPort") - 2);
    MqttServerAddressGet.trim();
    MqttServerAddressGet.replace('+', ' ');
    MqttServerAddressGet.replace("%2c", ",");
    MqttServerAddressGet.replace("%2C", ",");
    MqttServerAddressGet.replace("%2F", "/");
    MqttServerAddressGet.replace("%25", "%");    

    String MqttServerPortGet = req.substring(MqttServerPortValueIndex, MqttServerLoginValueIndex - strlen("MqttServerLogin") - 2);    

    String MqttServerLoginGet = req.substring(MqttServerLoginValueIndex, MqttServerPasswordValueIndex - strlen("MqttServerPassword") - 2);
    MqttServerLoginGet.trim();
    MqttServerLoginGet.replace('+', ' ');
    MqttServerLoginGet.replace("%2c", ",");
    MqttServerLoginGet.replace("%2C", ",");
    MqttServerLoginGet.replace("%2F", "/");
    MqttServerLoginGet.replace("%25", "%");    

    String MqttServerPasswordGet = req.substring(MqttServerPasswordValueIndex, MqttServiceServerAddressValueIndex - strlen("MqttServiceServerAddress") - 2);
    MqttServerPasswordGet.trim();
    MqttServerPasswordGet.replace('+', ' ');
    MqttServerPasswordGet.replace("%2c", ",");
    MqttServerPasswordGet.replace("%2C", ",");
    MqttServerPasswordGet.replace("%2F", "/");
    MqttServerPasswordGet.replace("%25", "%");    

    String MqttServiceServerAddressGet = req.substring(MqttServiceServerAddressValueIndex, MqttServiceServerPortValueIndex - strlen("MqttServiceServerPort") - 2);
    MqttServiceServerAddressGet.trim();
    MqttServiceServerAddressGet.replace('+', ' ');
    MqttServiceServerAddressGet.replace("%2c", ",");
    MqttServiceServerAddressGet.replace("%2C", ",");
    MqttServiceServerAddressGet.replace("%2F", "/");
    MqttServiceServerAddressGet.replace("%25", "%");    

    String MqttServiceServerPortGet = req.substring(MqttServiceServerPortValueIndex, MqttServiceServerLoginValueIndex - strlen("MqttServiceServerLogin") - 2);    

    String MqttServiceServerLoginGet = req.substring(MqttServiceServerLoginValueIndex, MqttServiceServerPasswordValueIndex - strlen("MqttServiceServerPassword") - 2);
    MqttServiceServerLoginGet.trim();
    MqttServiceServerLoginGet.replace('+', ' ');
    MqttServiceServerLoginGet.replace("%2c", ",");
    MqttServiceServerLoginGet.replace("%2C", ",");
    MqttServiceServerLoginGet.replace("%2F", "/");
    MqttServiceServerLoginGet.replace("%25", "%");    

    String MqttServiceServerPasswordGet = req.substring(MqttServiceServerPasswordValueIndex, MqttTopicTemplateValueIndex - strlen("MqttTopicTemplate") - 2);
    MqttServiceServerPasswordGet.trim();
    MqttServiceServerPasswordGet.replace('+', ' ');
    MqttServiceServerPasswordGet.replace("%2c", ",");
    MqttServiceServerPasswordGet.replace("%2C", ",");    
    MqttServiceServerPasswordGet.replace("%2F", "/");
    MqttServiceServerPasswordGet.replace("%25", "%");    

    String MqttTopicTemplateGet = req.substring(MqttTopicTemplateValueIndex, EndreqValueIndex - 1);
    MqttTopicTemplateGet.trim();
    MqttTopicTemplateGet.replace('+', ' ');
    MqttTopicTemplateGet.replace("%2c", ",");
    MqttTopicTemplateGet.replace("%2C", ",");
    MqttTopicTemplateGet.replace("%2F", "/");
    MqttTopicTemplateGet.replace("%25", "%");        

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    DescriptionGet.toCharArray(eepromData.Description, DescriptionGet.length() + 1);
    DescriptiveLocationGet.toCharArray(eepromData.DescriptiveLocation, DescriptiveLocationGet.length() + 1);
    LatitudeSetGet.toCharArray(eepromData.LatitudeSet, LatitudeSetGet.length() + 1);
    LongtitudeSetGet.toCharArray(eepromData.LongtitudeSet, LongtitudeSetGet.length() + 1);
    AltitudeSetGet.toCharArray(eepromData.AltitudeSet, AltitudeSetGet.length() + 1);
    MqttServerAddressGet.toCharArray(eepromData.mqttServerAddress, MqttServerAddressGet.length() + 1);
    eepromData.mqttServerPort=MqttServerPortGet.toInt();
    MqttServerLoginGet.toCharArray(eepromData.mqttServerUsername, MqttServerLoginGet.length() + 1);
    MqttServerPasswordGet.toCharArray(eepromData.mqttServerPassword, MqttServerPasswordGet.length() + 1);
    MqttServiceServerAddressGet.toCharArray(eepromData.mqttServiceServerAddress, MqttServiceServerAddressGet.length() + 1);
    eepromData.mqttServiceServerPort=MqttServiceServerPortGet.toInt();
    MqttServiceServerLoginGet.toCharArray(eepromData.mqttServiceServerUsername, MqttServiceServerLoginGet.length() + 1);
    MqttServiceServerPasswordGet.toCharArray(eepromData.mqttServiceServerPassword, MqttServiceServerPasswordGet.length() + 1);
    MqttTopicTemplateGet.toCharArray(eepromData.mqttServerTopicTemplate, MqttTopicTemplateGet.length() + 1);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    DPRINTLNFUNC("GOT DATA FROM SERVER:");
    PRINTVAR(eepromData.Description);
    PRINTVAR(StateGet);
    PRINTVAR(WiFiNameGet);
    PRINTVAR(WiFiPasswordGet);
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
    PRINTVAR(eepromData.mqttServerTopicTemplate);    

    if (StateGet == "Off") eepromData.DeviceState = 0;
    else eepromData.DeviceState = 1;

    if (!WiFiNameGet.equals("") && !WiFiPasswordGet.equals("")) {
      char buf_name[20];
      char buf_password[20];

      if (WiFiNameGet.length() > 20) WiFiNameGet.remove(20);
      if (WiFiPasswordGet.length() > 20) WiFiPasswordGet.remove(20);
      WiFiPasswordGet.replace('+', ' ');
      WiFiNameGet.replace('+', ' ');

      DPRINTLNFUNC("creditials1=");
      DPRINTLNFUNC(WiFiNameGet);
      DPRINTLNFUNC(WiFiPasswordGet);
      
      WiFiNameGet.toCharArray(buf_name, WiFiNameGet.length() + 1);
      WiFiPasswordGet.toCharArray(buf_password, WiFiPasswordGet.length() + 1);
      
      DPRINTLNFUNC("creditials2=");
      DPRINTLNFUNC(buf_name);
      DPRINTLNFUNC(buf_password);
      
      strncpy(eepromData.WifiSsidOld, eepromData.WifiSsid, 20);
      strncpy(eepromData.WifiPasswordOld, eepromData.WifiPassword, 20);
      strncpy(eepromData.WifiSsid, buf_name, 20);
      strncpy(eepromData.WifiPassword, buf_password, 20);
    }

    eepromData.SetWifiMode = 0;
    EEPROM.put(0, eepromData);
    EEPROM.commit();
    DPRINTLNFUNC("parameters saved to eeprom");

    // Prepare the response
    s = "\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n\r\n<body><h3>Saved, thank you</h3></body></html>";

    // Send the response to the client
    client.print(s);
    delay(1000);
    
    DPRINTLNFUNC("Client disonnected");

    PRINTVAR(WiFiNameGet);
    PRINTVAR(WiFiPasswordGet);

    ReadEEPROM();
    
    delay(500);
    ESP.reset();
  }
  
  DPRINTLNFUNC("Exit form wifi set function");
  client.flush();
}
