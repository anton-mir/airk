#pragma once
#include "mqttFunc.h"

void GetConfigData() {

      DPRINTLNFUNC("Sending configuration to mqtt server");

      deserializeJson(doc_out, "{}");
      JsonObject root_out = doc_out.as<JsonObject>();

      root_out["SetWifiMode"] = String(eepromData.SetWifiMode);
      root_out["DeviceState"] = String(eepromData.DeviceState);
      root_out["DeviceStatePrev"] = String(eepromData.DeviceStatePrev);
      root_out["MessageID"] = String(eepromData.MessageID);
      root_out["TimeNow"] = String(eepromData.TimeNow);
      root_out["TimeNow"] = String(eepromData.TimeNow);
      root_out["WifiSsid"] = String(eepromData.WifiSsid);
      root_out["WifiPassword"] = String(eepromData.WifiPassword);
      root_out["WifiSsidOld"] = String(eepromData.WifiSsidOld);
      root_out["WifiPasswordOld"] = String(eepromData.WifiPasswordOld);
      root_out["Description"] = String(eepromData.Description);
      root_out["DescriptiveLocation"] = String(eepromData.DescriptiveLocation);
      root_out["LatitudeSet"] = String(eepromData.LatitudeSet);
      root_out["LongtitudeSet"] = String(eepromData.LongtitudeSet);
      root_out["AltitudeSet"] = String(eepromData.AltitudeSet);
      root_out["mqttServerAddress"] = String(eepromData.mqttServerAddress);
      root_out["mqttServerUsername"] = String(eepromData.mqttServerUsername);
      root_out["mqttServerPassword"] = String(eepromData.mqttServerPassword);
      root_out["mqttServerPort"] = String(eepromData.mqttServerPort);
      root_out["mqttServiceServerAddress"] = String(eepromData.mqttServiceServerAddress);
      root_out["mqttServiceServerUsername"] = String(eepromData.mqttServiceServerUsername);
      root_out["mqttServiceServerPassword"] = String(eepromData.mqttServiceServerPassword);
      root_out["mqttServiceServerPort"] = String(eepromData.mqttServiceServerPort);
      root_out["mqttServerUserSettingsActive"] = String(eepromData.mqttServerUserSettingsActive);
      root_out["mqttServerTopicTemplate"] = String(eepromData.mqttServerTopicTemplate);

      serializeJson(root_out, json_config);

      DPRINTFUNC("JSON CONFIG: ");
      DPRINTLNFUNC(json_config);
      
      if (eepromData.mqttServerUserSettingsActive != 1) sprintf(mqtt_topic,client_topic_template,DeviceID,"config_message");
      else sprintf(mqtt_topic,eepromData.mqttServerTopicTemplate,DeviceID,"config_message");
      
      DPRINTLNFUNC(mqtt_topic);
      mqtt_service_client.publish(mqtt_topic,json_config);
//      mqtt_service_client.publish("test_topic",json_config);

}
