#pragma once

void GetMeasurementsData() {

  if (loop_cntr >= 200) DPRINTLNFUNC("GetMeasurementsData function");

  if (Serial.available() > 0) {
    Serial.readBytesUntil('\0', json, 2000);
    DPRINTLNFUNC("============buffer from stm32 before deserializeJson=================");
    DPRINTLNFUNC(json);
    DeserializationError error = deserializeJson(doc_in, json);

    if (error) {
      DPRINTFUNC("DeserializeJson failed: ");
      DPRINTLNFUNC(error.c_str());
    }
    else {
      deserializeJson(doc_out, "{}");
      JsonObject root_in = doc_in.as<JsonObject>();
      serializeJsonPretty(root_in, Serial);

      JsonObject root_out = doc_out.as<JsonObject>();
      JsonObject Data = root_out.createNestedObject("Data");
      JsonObject Measurements = Data.createNestedObject("Measurements");
      JsonObject DeviceInfo = Data.createNestedObject("DeviceInfo");
      JsonObject DeviceLocation = Data.createNestedObject("DeviceLocation");

      Measurements["LifeTime"] = root_in["LifeTime"];
      Measurements["State"] = root_in["State"];
      Measurements["Power"] = root_in["Power"];
      Measurements["InternalTemp"] = root_in["InternalTemp"];
      Measurements["PM1"] = root_in["PM1"];
      Measurements["PM25"] = root_in["PM25"];
      Measurements["PM10"] = root_in["PM10"];
      Measurements["FPM1"] = root_in["FPM1"];
      Measurements["FPM25"] = root_in["FPM25"];
      Measurements["FPM10"] = root_in["FPM10"];
      Measurements["PNUM03"] = root_in["PNUM03"];
      Measurements["PNUM05"] = root_in["PNUM05"];
      Measurements["PNUM1"] = root_in["PNUM1"];
      Measurements["PNUM25"] = root_in["PNUM25"];
      Measurements["PNUM5"] = root_in["PNUM5"];
      Measurements["PNUM10"] = root_in["PNUM10"];
      Measurements["Temperature"] = root_in["Temperature"];
      Measurements["Humidity"] = root_in["Humidity"];
      Measurements["TemperatureDHT"] = root_in["TemperatureDHT"];
      Measurements["Pressure"] = root_in["Pressure"];
      Measurements["OzoneHigh"] = root_in["OzoneHigh"];
//      Measurements["OzoneLow"] = root_in["OzoneLow"];
      Measurements["CO"] = root_in["CO"];
//      Measurements["CO2"] = root_in["CO2"];
//      Measurements["NO"] = root_in["NO"];
      Measurements["NO2"] = root_in["NO2"];
      Measurements["SO2"] = root_in["SO2"];
      Measurements["C6H6"] = root_in["C6H6"];
      Measurements["C6H6_digital"] = root_in["C6H6_digital"];
//      Measurements["LatitudeMeasured"] = root_in["LatitudeMeasured"];
//      Measurements["LongtitudeMeasured"] = root_in["LongtitudeMeasured"];

      DeviceInfo["ID"] = DeviceID;
      DeviceInfo["Description"] = String(eepromData.Description);
      DeviceInfo["IP"] = WiFi.localIP().toString();
      DeviceInfo["PacketsSent"] = PacketsSent;

      DeviceLocation["DescriptiveLocation"] = String(eepromData.DescriptiveLocation);
      DeviceLocation["LatitudeSet"] = String(eepromData.LatitudeSet);
      DeviceLocation["LongtitudeSet"] = String(eepromData.LongtitudeSet);
      DeviceLocation["AltitudeSet"] = eepromData.AltitudeSet;

      root_out["MessageID"] = eepromData.MessageID;
      root_out["ErrCode"] = root_in["ErrCode"];
      
      time_t now = time(nullptr);
      String timeValue = ctime(&now);
      timeValue.remove(timeValue.length() - 1);
      root_out["MessageDateTime"] = timeValue;

      char temp_var[40];
      sprintf(temp_var, "%d:%d:%d", (int)now, DeviceID, eepromData.MessageID);
      root_out["RequestHash"] = sha1(temp_var);

      serializeJson(root_out, json);

      DPRINTFUNC("JSON: ");
      DPRINTLNFUNC(json);

      if (eepromData.mqttServerUserSettingsActive != 1) sprintf(mqtt_topic,client_topic_template,DeviceID,"data_message");
      else sprintf(mqtt_topic,eepromData.mqttServerTopicTemplate,DeviceID,"data_message");
            
      PublishMqttDataMessage(mqtt_topic,json);
    }
  }
  delay(10);
}
