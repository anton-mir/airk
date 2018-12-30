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
      DPRINTLNFUNC(" try again in 3 seconds");
      delay(10);
    }
    attempt++;
    if (attempt > 3) {
      for (int i=0;i<100;i++){
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
    if (mqtt_service_client.connect(clientId.c_str(),eepromData.mqttServerUserSettingsActive!=1?standart_mqtt_service_server_login:eepromData.mqttServerUsername,eepromData.mqttServerUserSettingsActive!=1?standart_mqtt_service_server_password:eepromData.mqttServerPassword)) {
      DPRINTLNFUNC("connected");
      sprintf(mqtt_topic,eepromData.mqttServerUserSettingsActive!=1?client_topic_template:eepromData.mqttServerTopicTemplate,DeviceID,"status");
      mqtt_service_client.publish(mqtt_topic, "online");

      sprintf(mqtt_topic, "/airk_device/%d/service_message/#", DeviceID);
      mqtt_service_client.subscribe(mqtt_topic);

      DPRINTLNFUNC("Service mqtt subscribed to all topics");
    } else {
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
