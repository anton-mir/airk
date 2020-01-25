#pragma once
#include "ESP_AirK.h"
#include "GetConfiguration.h"

void PublishMqttDataMessage(char *mqtt_topic, char *mqtt_message) {
  ++PacketsSent;
  ++eepromData.MessageID;
  mqtt_publish_client.publish(mqtt_topic, mqtt_message);
  DPRINTLNFUNC("Mqtt data message published");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void thingspeak_callback(char* topic, byte* payload, unsigned int length) {
  DPRINTFUNC("thingspeak doesn`t have callback functionality");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void client_callback(char* topic, byte* payload, unsigned int length) {
  DPRINTFUNC("Client doesn`t have callback functionality");
  sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"error");
  PublishMqttDataMessage(mqtt_topic, "wrong_request");
  mqtt_service_client.publish(mqtt_topic, "wrong_request_at_data_mqtt_server");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void service_callback(char* topic, byte* payload, unsigned int length) {
  char msg[length+1];
  
  DPRINTFUNC("Message arrived [");
  DPRINTFUNC(topic);
  DPRINTLNFUNC("] ");
  DPRINTFUNC("Message length=[");
  DPRINTFUNC(length);
  DPRINTLNFUNC("] ");
  unsigned int i;
  for (i = 0; i < length; i++) {
    msg[i]=payload[i];
    DPRINTFUNC((char)payload[i]);
  }
  msg[i++]='\0';
  DPRINTLNFUNC();
  DPRINTFUNC("Message_payload msg=");
  DPRINTLNFUNC(msg);
  
  //STATE MESSAGE IF TURNING ON OR OFF
  if (strstr(topic,"service_message/state")!=NULL) {
  sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"state");
  if (strstr(msg,"on")!=NULL) {
    DPRINTLNFUNC("\r\nTURNING ON STM32");
    digitalWrite(ONOFFPIN,HIGH);
    eepromData.DeviceStatePrev = eepromData.DeviceState;
    eepromData.DeviceState = 1;
    mqtt_service_client.publish(mqtt_topic,(char*)"on");
  }
  else if (strstr(msg,"off")!=NULL) {
    DPRINTLNFUNC("\r\nTURNING OFF STM32");
    digitalWrite(ONOFFPIN,LOW);
    eepromData.DeviceStatePrev = eepromData.DeviceState;
    eepromData.DeviceState = 0;
    mqtt_service_client.publish(mqtt_topic,(char*)"off");
  }
  EEPROM.put(0, eepromData);
  EEPROM.commit();
  }
  //DESCRIPTION MESSAGE
  else if (strstr(topic,"service_message/description")!=NULL) {
    DPRINTLNFUNC("Changing description");
    strncpy(eepromData.Description, msg, length+1);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"description");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }
  //DESCRIPTIVA LOCATION MESSAGE
  else if (strstr(topic,"service_message/descriptivelocation")!=NULL) {
    DPRINTLNFUNC("Changing descriptivelocation");
    strncpy(eepromData.DescriptiveLocation, msg, length+1);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"descriptivelocation");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }
  //LATITUDE SETTED MESSAGE
  else if (strstr(topic,"service_message/latitudeset")!=NULL) {
    DPRINTLNFUNC("Changing latitudeset");
    strncpy(eepromData.LatitudeSet, msg, length+1);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"latitudeset");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }      
  //LONGTITUDE SETTED MESSAGE
  else if (strstr(topic,"service_message/longtitudeset")!=NULL) {
    DPRINTLNFUNC("Changing longtitudeset");
    strncpy(eepromData.LongtitudeSet, msg, length+1);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"longtitudeset");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }        
  //ALTITUDE SETTED MESSAGE
  else if (strstr(topic,"service_message/altitudeset")!=NULL) {
    DPRINTLNFUNC("Changing altitudeset");
    strncpy(eepromData.AltitudeSet, msg, length+1);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"altitudeset");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }
  //WIFI SSID SETTED MESSAGE
  else if (strstr(topic,"service_message/wifissid")!=NULL) {
    DPRINTLNFUNC("Changing WifiSsid");
    strncpy(eepromData.WifiSsidOld, eepromData.WifiSsid, 20);
    strncpy(eepromData.WifiSsid, msg, length+1);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"wifissid");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }
  //WIFI PASSWORD SETTED MESSAGE  
  else if (strstr(topic,"service_message/wifipassword")!=NULL) {
    DPRINTLNFUNC("Changing wifi password");
    strncpy(eepromData.WifiPasswordOld, eepromData.WifiPassword, 20);
    strncpy(eepromData.WifiPassword, msg, length+1);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"wifipassword");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }
  //MQTT SERVER SETTED MESSAGE  
  else if (strstr(topic,"service_message/mqttserveraddress")!=NULL) {
    DPRINTLNFUNC("Changing mqtt server address");
    strncpy(eepromData.mqttServerAddress, msg, length+1);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"mqttserveraddress");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }
  //MQTT USERNAME SETTED MESSAGE  
  else if (strstr(topic,"service_message/mqttserverusername")!=NULL) {
    DPRINTLNFUNC("Changing mqttserver username");
    strncpy(eepromData.mqttServerUsername, msg, length+1);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"mqttserverusername");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }
  //MQTT PASSWORD SETTED MESSAGE  
  else if (strstr(topic,"service_message/mqttserverpassword")!=NULL) {
    DPRINTLNFUNC("Changing mqttserver password");
    strncpy(eepromData.mqttServerPassword, msg, length+1);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"mqttserverpassword");    
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }      
  //MQTT PORT SETTED MESSAGE  
  else if (strstr(topic,"service_message/mqttserverport")!=NULL) {
    DPRINTFUNC("Changing mqttserver port to");
    eepromData.mqttServerPort=strtol(msg, NULL, 10);
    DPRINTLNFUNC(eepromData.mqttServerPort);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"mqttserverport");    
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }   
  //MQTT SERVICE SERVER ADDRESS SETTED MESSAGE  
  else if (strstr(topic,"service_message/mqttserviceserveraddress")!=NULL) {
    DPRINTLNFUNC("Changing mqtt service server address");
    strncpy(eepromData.mqttServiceServerAddress, msg, length+1);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"mqttserviceserveraddress");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }
  //MQTT SERVICE SERVER USERNAME SETTED MESSAGE  
  else if (strstr(topic,"service_message/mqttserviceserverusername")!=NULL) {
    DPRINTLNFUNC("Changing mqtt service server username");
    strncpy(eepromData.mqttServiceServerUsername, msg, length+1);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"mqttserviceserverusername");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }
  //MQTT SERVICE SERVER PASSWORD SETTED MESSAGE  
  else if (strstr(topic,"service_message/mqttserviceserverpassword")!=NULL) {
    DPRINTLNFUNC("Changing mqtt service server password");
    strncpy(eepromData.mqttServiceServerPassword, msg, length+1);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"mqttserviceserverpassword");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }      
  //MQTT SERVICE SERVER PORT SETTED MESSAGE  
  else if (strstr(topic,"service_message/mqttserviceserverport")!=NULL) {
    DPRINTFUNC("Changing mqtt service server port to ");
    eepromData.mqttServiceServerPort=strtol(msg, NULL, 10);
    DPRINTLNFUNC(eepromData.mqttServiceServerPort);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"mqttserviceserverport");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }  
  //MQTT CLIENT SERVER MESSAGE TOPIC TEMPLATE
  else if (strstr(topic,"service_message/mqttservertopictemplate")!=NULL) {
    DPRINTFUNC("Changing mqtt service server topic template to");
    strncpy(eepromData.mqttServerTopicTemplate, msg, length+1);
    DPRINTLNFUNC(eepromData.mqttServerTopicTemplate);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"mqttservertopictemplate");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }
  //THINGSPEAK MQTTPASS
  else if (strstr(topic,"service_message/thingspeakmqttpass")!=NULL) {
    DPRINTFUNC("Changing thingspeak mqtt pass");
    strncpy(eepromData.thingspeak_mqttPass, msg, length+1);
    DPRINTLNFUNC(eepromData.thingspeak_mqttPass);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"thingspeakmqttpass");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }
  //THINGSPEAK thingspeak_ch1_writeAPIKey
  else if (strstr(topic,"service_message/thingspeakch1apikey")!=NULL) {
    DPRINTFUNC("Changing thingspeak channel1 api key");
    strncpy(eepromData.thingspeak_ch1_writeAPIKey, msg, length+1);
    DPRINTLNFUNC(eepromData.thingspeak_ch1_writeAPIKey);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"thingspeakch1apikey");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }
  //THINGSPEAK thingspeak_ch2_writeAPIKey
  else if (strstr(topic,"service_message/thingspeakch2apikey")!=NULL) {
    DPRINTFUNC("Changing thingspeak channel2 api key");
    strncpy(eepromData.thingspeak_ch2_writeAPIKey, msg, length+1);
    DPRINTLNFUNC(eepromData.thingspeak_ch2_writeAPIKey);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"thingspeakch2apikey");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }
  //THINGSPEAK thingspeak_channelID_1
  else if (strstr(topic,"service_message/thingspeakchannelid1")!=NULL) {
    DPRINTFUNC("Changing thingspeak channel id1");
    eepromData.thingspeak_channelID_1=strtol(msg, NULL, 10);
    DPRINTLNFUNC(eepromData.thingspeak_channelID_1);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"thingspeakchannelid1");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }
  //THINGSPEAK thingspeak_channelID_2
  else if (strstr(topic,"service_message/thingspeakchannelid2")!=NULL) {
    DPRINTFUNC("Changing thingspeak channel id2");
    eepromData.thingspeak_channelID_2=strtol(msg, NULL, 10);
    DPRINTLNFUNC(eepromData.thingspeak_channelID_1);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"thingspeakchannelid2");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }    
  //GET MEASUREMENTS MESSAGE  
  else if (strstr(topic,"service_message/getmeasurementsdata")!=NULL) {
    if (strstr(msg,"send")!=NULL) {
      DPRINTLNFUNC("Publishing last measurements json data");
      sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"getmeasurementsdata");
      mqtt_service_client.publish(mqtt_topic,(char*)"sending");      
      mqtt_service_client.publish(mqtt_topic,json);
    }
  }
  //GET CONFIG MESSAGE  
  else if (strstr(topic,"service_message/getconfigdata")!=NULL) {
    if (strstr(msg,"send")!=NULL) {
      DPRINTLNFUNC("Publishing config json data");
      sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"getconfigdata");
      mqtt_service_client.publish(mqtt_topic,(char*)"sending");      
      GetConfigData();
    }
  }
  //GET kO3high
  else if (strstr(topic,"service_message/kO3high")!=NULL) {
    DPRINTFUNC("Changing kO3high");
    eepromData.kO3high=atof(msg);
    DPRINTLNFUNC(eepromData.kO3high);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"kO3high");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }
  //GET kO3low
  else if (strstr(topic,"service_message/kO3low")!=NULL) {
    DPRINTFUNC("Changing kO3low");
    eepromData.kO3low=atof(msg);
    DPRINTLNFUNC(eepromData.kO3low);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"kO3low");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }
  //GET kC6H6
  else if (strstr(topic,"service_message/kC6H6")!=NULL) {
    DPRINTFUNC("Changing kC6H6");
    eepromData.kC6H6=atof(msg);
    DPRINTLNFUNC(eepromData.kC6H6);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"kC6H6");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }      
  //GET kH2S
  else if (strstr(topic,"service_message/kH2S")!=NULL) {
    DPRINTFUNC("Changing kH2S");
    eepromData.kH2S=atof(msg);
    DPRINTLNFUNC(eepromData.kH2S);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"kH2S");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }    
  //GET kSO2
  else if (strstr(topic,"service_message/kSO2")!=NULL) {
    DPRINTFUNC("Changing kSO2");
    eepromData.kSO2=atof(msg);
    DPRINTLNFUNC(eepromData.kSO2);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"kSO2");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  } 
  //GET kCO
  else if (strstr(topic,"service_message/kCO")!=NULL) {
    DPRINTFUNC("Changing kCO");
    eepromData.kCO=atof(msg);
    DPRINTLNFUNC(eepromData.kCO);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"kCO");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  } 
  //GET kNO2
  else if (strstr(topic,"service_message/kNO2")!=NULL) {
    DPRINTFUNC("Changing kNO2");
    eepromData.kNO2=atof(msg);
    DPRINTLNFUNC(eepromData.kNO2);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"kNO2");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  } 
  //GET kNO
  else if (strstr(topic,"service_message/kNO")!=NULL) {
    DPRINTFUNC("Changing kNO");
    eepromData.kNO=atof(msg);
    DPRINTLNFUNC(eepromData.kNO);
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"kNO");
    mqtt_service_client.publish(mqtt_topic,(char*)"set");
    EEPROM.put(0, eepromData);
    EEPROM.commit();
  }    
  //RESET MESSAGE  
  else if (strstr(topic,"service_message/reset")!=NULL) {
    if (strstr(msg,"reset")!=NULL) {
      DPRINTLNFUNC("RESETTING");
      sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"reset");
      mqtt_service_client.publish(mqtt_topic,(char*)"resetting");      
      delay(100);
      ESP.reset();
    }
  }  
  //USER SETTINGS PASS AND SSID ACTIVE OPTION
  else if (strstr(topic,"service_message/mqttusersettingsactive")!=NULL) {
    sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"mqttusersettingsactive");
    if (strstr(msg,"on")!=NULL) {
      DPRINTLNFUNC("Changing mqttServerUserSettingsActive state to on");
      eepromData.mqttServerUserSettingsActive = 1;
      mqtt_service_client.publish(mqtt_topic,(char*)"on");
    }
    else if (strstr(msg,"off")!=NULL) {
      DPRINTLNFUNC("Changing mqttServerUserSettingsActive state to off");
      eepromData.mqttServerUserSettingsActive = 0;
      mqtt_service_client.publish(mqtt_topic,(char*)"off");
    }
    EEPROM.put(0, eepromData);
    EEPROM.commit();
    delay(100);
    ESP.reset();    
  }  
  else DPRINTLNFUNC("Got wrong mqtt message"); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void reconnectThingspeakClient() {
  digitalWrite(LED_PIN_BLUE, 0);
  byte attempt = 0;
  while (!mqtt_thingspeak_client.connected()) {
    PRINTVAR(mqtt_thingspeak_client.connected());
    DPRINTFUNC("Attempting Thingspeak client MQTT connection...");
    
    String clientID = "AirK-thingspeak-";
    clientID += DeviceID;

    if (mqtt_thingspeak_client.connect(clientID.c_str(),thingspeak_mqttUserName,eepromData.mqttServerUserSettingsActive!=1?thingspeak_mqttPass:eepromData.thingspeak_mqttPass)) {
      DPRINTLNFUNC("connected");
    } else {
      DPRINTFUNC("failed, rc=");
      // Print reason the connection failed.
      // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
      DPRINTFUNC(mqtt_thingspeak_client.state());
      DPRINTLNFUNC(" trying again...");
      delay(10);
    }
    attempt++;
    if (attempt > 3) {
      for (int i=0;i<50;i++){
           digitalWrite(LED_PIN_RED, led);
           led = !led;
           delay(100);
      }
      digitalWrite(LED_PIN_RED, 0);
      return;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void reconnectPublishClient() {
  digitalWrite(LED_PIN_BLUE, 0);
  byte attempt = 0;
  while (!mqtt_publish_client.connected()) {
    PRINTVAR(mqtt_publish_client.connected());
    DPRINTFUNC("Attempting Publish client MQTT connection...");
    String clientId = "AirK-publish-";
    clientId += DeviceID;
    if (mqtt_publish_client.connect(clientId.c_str(),eepromData.mqttServerUserSettingsActive!=1?standart_mqtt_server_login:eepromData.mqttServerUsername,eepromData.mqttServerUserSettingsActive!=1?standart_mqtt_server_password:eepromData.mqttServerPassword)) {
      DPRINTLNFUNC("connected");

      if (eepromData.mqttServerUserSettingsActive != 1) sprintf(mqtt_topic,client_topic_template,DeviceID,"status");
      else sprintf(mqtt_topic,eepromData.mqttServerTopicTemplate,DeviceID,"status");
      
      mqtt_publish_client.publish(mqtt_topic, "ready");
    } else {
      DPRINTFUNC("failed, rc=");
      DPRINTFUNC(mqtt_publish_client.state());
      DPRINTLNFUNC(" trying again...");
      delay(10);
    }
    attempt++;
    if (attempt > 3) {
      for (int i=0;i<50;i++){
           digitalWrite(LED_PIN_RED, led);
           led = !led;
           delay(100);
      }
      digitalWrite(LED_PIN_RED, 0);
      return;
    }
  }
}

void reconnectServiceClient() {
  byte attempt = 0;
  while (!mqtt_service_client.connected()) {
    DPRINTFUNC("Attempting Service client MQTT connection...");
    String clientId = "AirK-service-";
    clientId += DeviceID;
    if (mqtt_service_client.connect(clientId.c_str(),eepromData.mqttServerUserSettingsActive!=1?standart_mqtt_service_server_login:eepromData.mqttServiceServerUsername,eepromData.mqttServerUserSettingsActive!=1?standart_mqtt_service_server_password:eepromData.mqttServiceServerPassword)) 
    {
      DPRINTLNFUNC("connected");
      sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"status");
      mqtt_service_client.publish(mqtt_topic, "online");

      sprintf(mqtt_topic, "/airk_device/%d/service_message/#", DeviceID);
      mqtt_service_client.subscribe(mqtt_topic);

      DPRINTLNFUNC("Service mqtt subscribed to all topics");
    } 
    else {
      DPRINTFUNC("failed connect to Service mqtt server, rc = ");
      DPRINTFUNC(mqtt_publish_client.state());
      DPRINTLNFUNC(" try again");
      delay(10);
    }
    attempt++;
    if (attempt > 3) {
      digitalWrite(LED_PIN_BLUE, 0);
      for (int i=0;i<10;i++){
           digitalWrite(LED_PIN_RED, led);
           led = !led;
           delay(100);
      }
      mqtt_service_client.setServer(standart_mqtt_service_server_address, standart_mqtt_service_server_port);
      if (try_service_server_connect==false) {
        mqtt_service_client.connect(clientId.c_str(),standart_mqtt_service_server_login,standart_mqtt_service_server_password);
        try_service_server_connect=true;
      }
      digitalWrite(LED_PIN_RED, 0);
      return;
    }
  }
}
