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
      
      if (!root_in["LifeTime"].isNull()) //measeruments message 
      {
          JsonObject Measurements = root_out.createNestedObject("measurements");
          JsonObject DeviceInfo = root_out.createNestedObject("deviceInfo");
          JsonObject DeviceLocation = root_out.createNestedObject("deviceLocation");
          JsonObject MessageInfo = root_out.createNestedObject("messageInfo");
          
          Measurements["lifeTime"] = root_in["LifeTime"];
          Measurements["state"] = root_in["State"];
          Measurements["pm1"] = root_in["PM1"];
          Measurements["pm25"] = root_in["PM25"];
          Measurements["pm10"] = root_in["PM10"];
          Measurements["fpm1"] = root_in["FPM1"];
          Measurements["fpm25"] = root_in["FPM25"];
          Measurements["fpm10"] = root_in["FPM10"];
          Measurements["pnum03"] = root_in["PNUM03"];
          Measurements["pnum05"] = root_in["PNUM05"];
          Measurements["pnum1"] = root_in["PNUM1"];
          Measurements["pnum25"] = root_in["PNUM25"];
          Measurements["pnum5"] = root_in["PNUM5"];
          Measurements["pnum10"] = root_in["PNUM10"];
          Measurements["temperature"] = root_in["Temperature"];
          Measurements["humidity"] = root_in["Humidity"];
          Measurements["pressure"] = root_in["Pressure"];
    
          //ppm convert

          //ozoneHigh_ppm
          JsonVariant ozoneHigh_ppm_variant = root_in["OzoneHigh_ppm"];
          float ozoneHigh_ppm_value = ozoneHigh_ppm_variant.as<float>();
          ozoneHigh_ppm_value *= eepromData.kO3high; 
          ozoneHigh_ppm_variant.set(ozoneHigh_ppm_value);
          Measurements["ozoneHigh_ppm"] = ozoneHigh_ppm_variant;

          //OzoneLow_ppb
          JsonVariant OzoneLow_ppb_variant = root_in["OzoneLow_ppb"];
          float OzoneLow_ppb_value = OzoneLow_ppb_variant.as<float>();
          OzoneLow_ppb_value *= eepromData.kO3low; 
          OzoneLow_ppb_variant.set(OzoneLow_ppb_value);
          Measurements["ozoneLow_ppb"] = OzoneLow_ppb_variant;

          //co_ppm
          JsonVariant CO_ppm_variant = root_in["CO_ppm"];
          float CO_ppm_value = CO_ppm_variant.as<float>();
          CO_ppm_value *= eepromData.kCO; 
          CO_ppm_variant.set(CO_ppm_value);
          Measurements["co_ppm"] = CO_ppm_variant;

          //no_ppm
          JsonVariant NO_ppm_variant = root_in["NO_ppm"];
          float NO_ppm_value = NO_ppm_variant.as<float>();
          NO_ppm_value *= eepromData.kNO; 
          NO_ppm_variant.set(NO_ppm_value);
          Measurements["no_ppm"] = NO_ppm_variant;
          
          //no2_ppm
          JsonVariant NO2_ppm_variant = root_in["NO2_ppm"];
          float NO2_ppm_value = NO2_ppm_variant.as<float>();
          NO2_ppm_value *= eepromData.kNO2; 
          NO2_ppm_variant.set(NO2_ppm_value);
          Measurements["no2_ppm"] = NO2_ppm_variant;
          
          //so2_ppm
          JsonVariant SO2_ppm_variant = root_in["SO2_ppm"];
          float SO2_ppm_value = SO2_ppm_variant.as<float>();
          SO2_ppm_value *= eepromData.kSO2; 
          SO2_ppm_variant.set(SO2_ppm_value);
          Measurements["so2_ppm"] = SO2_ppm_variant;
          
          //H2S_ppm
          JsonVariant H2S_ppm_variant = root_in["H2S_ppm"];
          float H2S_ppm_value = H2S_ppm_variant.as<float>();
          H2S_ppm_value *= eepromData.kH2S; 
          H2S_ppm_variant.set(H2S_ppm_value);
          Measurements["h2s_ppm"] = H2S_ppm_variant;

          
          //C6H6_ppm
          JsonVariant C6H6_ppm_variant = root_in["C6H6_ppm"];
          float C6H6_ppm_value = C6H6_ppm_variant.as<float>();
          C6H6_ppm_value *= eepromData.kC6H6; 
          C6H6_ppm_variant.set(C6H6_ppm_value);
          Measurements["c6h6_ppm"] = C6H6_ppm_variant;
          
          Measurements["integralOzone_ppm"] = root_in["IntegralOzone_ppm"];      
    
          //mg convert

          //ozoneHigh_mg
          JsonVariant ozoneHigh_mg_variant = root_in["OzoneHigh_mg"];
          float ozoneHigh_mg_value = ozoneHigh_mg_variant.as<float>();
          ozoneHigh_mg_value *= eepromData.kO3high; 
          ozoneHigh_mg_variant.set(ozoneHigh_mg_value);
          Measurements["ozoneHigh_mg"] = ozoneHigh_mg_variant;

          //OzoneLow_mg
          JsonVariant OzoneLow_mg_variant = root_in["OzoneLow_mg"];
          float OzoneLow_mg_value = OzoneLow_mg_variant.as<float>();
          OzoneLow_mg_value *= eepromData.kO3low; 
          OzoneLow_mg_variant.set(OzoneLow_mg_value);
          Measurements["ozoneLow_mg"] = OzoneLow_mg_variant;

          //co_mg
          JsonVariant CO_mg_variant = root_in["CO_mg"];
          float CO_mg_value = CO_mg_variant.as<float>();
          CO_mg_value *= eepromData.kCO; 
          CO_mg_variant.set(CO_mg_value);
          Measurements["co_mg"] = CO_mg_variant;

          //no_mg
          JsonVariant NO_mg_variant = root_in["NO_mg"];
          float NO_mg_value = NO_mg_variant.as<float>();
          NO_mg_value *= eepromData.kNO; 
          NO_mg_variant.set(NO_mg_value);
          Measurements["no_mg"] = NO_mg_variant;
          
          //no2_mg
          JsonVariant NO2_mg_variant = root_in["NO2_mg"];
          float NO2_mg_value = NO2_mg_variant.as<float>();
          NO2_mg_value *= eepromData.kNO2; 
          NO2_mg_variant.set(NO2_mg_value);
          Measurements["no2_mg"] = NO2_mg_variant;
          
          //so2_mg
          JsonVariant SO2_mg_variant = root_in["SO2_mg"];
          float SO2_mg_value = SO2_mg_variant.as<float>();
          SO2_mg_value *= eepromData.kSO2; 
          SO2_mg_variant.set(SO2_mg_value);
          Measurements["so2_mg"] = SO2_mg_variant;
          
          //H2S_mg
          JsonVariant H2S_mg_variant = root_in["H2S_mg"];
          float H2S_mg_value = H2S_mg_variant.as<float>();
          H2S_mg_value *= eepromData.kH2S; 
          H2S_mg_variant.set(H2S_mg_value);
          Measurements["h2s_mg"] = H2S_mg_variant;

          //C6H6_mg
          JsonVariant C6H6_mg_variant = root_in["C6H6_mg"];
          float C6H6_mg_value = C6H6_mg_variant.as<float>();
          C6H6_mg_value *= eepromData.kC6H6; 
          C6H6_mg_variant.set(C6H6_mg_value);
          Measurements["c6h6_mg"] = C6H6_mg_variant;
          
          Measurements["integralOzone_mg"] = root_in["IntegralOzone_mg"];      

          DeviceInfo["deviceId"] = DeviceID;
          DeviceInfo["description"] = String(eepromData.Description);
          DeviceInfo["ip"] = WiFi.localIP().toString();
          DeviceInfo["packetsSent"] = PacketsSent;
    
          DeviceLocation["descriptiveLocation"] = String(eepromData.DescriptiveLocation);
          DeviceLocation["latitudeSet"] = String(eepromData.LatitudeSet);
          DeviceLocation["longtitudeSet"] = String(eepromData.LongtitudeSet);
          DeviceLocation["altitudeSet"] = eepromData.AltitudeSet;
    
          MessageInfo["messageID"] = eepromData.MessageID;
          
          time_t now = time(nullptr);
          String timeValue = ctime(&now);
          timeValue.remove(timeValue.length() - 1);
          MessageInfo["messageDateTime"] = timeValue;
    
          char temp_var[40];
          sprintf(temp_var, "%d:%d:%ld", (int)now, DeviceID, eepromData.MessageID);
          MessageInfo["requestHash"] = sha1(temp_var);

          if (eepromData.mqttServerUserSettingsActive != 1) sprintf(mqtt_topic,client_topic_template,DeviceID,"data_message");
          else sprintf(mqtt_topic,eepromData.mqttServerTopicTemplate,DeviceID,"data_message");

        ///Publish thingspeak
        if (!mqtt_thingspeak_client.connected()) reconnectThingspeakClient();
        else 
        {
            DPRINTLNFUNC("\r\n thingspeak.com publish start");
            String thingspeak_message = String("field1="); 
            
            //Publish to first channel or to second channel
            if (thingspeak_publish_counter == 5)
            { 
              DPRINTLNFUNC("\r\n thingspeak.com first channel publish");
              ///First channel publish
              if (eepromData.mqttServerUserSettingsActive != 1) sprintf(mqtt_topic_thingspeak,thingspeak_topic_template,thingspeak_channelID_1,thingspeak_ch1_writeAPIKey);
              else sprintf(mqtt_topic_thingspeak,thingspeak_topic_template,eepromData.thingspeak_channelID_1,eepromData.thingspeak_ch1_writeAPIKey);
              
              String pm1_str = root_in["PM1"];
              thingspeak_message.concat(pm1_str);
  
              thingspeak_message.concat("&field2=");
              String pm25_str = root_in["PM25"];
              thingspeak_message.concat(pm25_str);
              
              thingspeak_message.concat("&field3=");
              String pm10_str = root_in["PM10"];
              thingspeak_message.concat(pm10_str);
              
              thingspeak_message.concat("&field4=");
              String temperature_str = root_in["Temperature"];
              thingspeak_message.concat(temperature_str);
              
              thingspeak_message.concat("&field5=");
              String humidity_str = root_in["Humidity"];
              thingspeak_message.concat(humidity_str);
  
              thingspeak_message.concat("&field6=");
              String preassure_str = root_in["Pressure"];
              thingspeak_message.concat(preassure_str);
              
              mqtt_thingspeak_client.publish(mqtt_topic_thingspeak, thingspeak_message.c_str());
            }
            else if (thingspeak_publish_counter == 10)
            {
              ///Second channel publish
              DPRINTLNFUNC("\r\n thingspeak.com second channel publish");
              
              if (eepromData.mqttServerUserSettingsActive != 1) sprintf(mqtt_topic_thingspeak,thingspeak_topic_template,thingspeak_channelID_2,thingspeak_ch2_writeAPIKey);
              else sprintf(mqtt_topic_thingspeak,thingspeak_topic_template,eepromData.thingspeak_channelID_2,eepromData.thingspeak_ch2_writeAPIKey);
              
              String ozone_low_str = root_in["OzoneLow_ppb"];
              thingspeak_message.concat(ozone_low_str);
  
              thingspeak_message.concat("&field2=");
              String ozone_high_str = root_in["OzoneHigh_ppm"];
              thingspeak_message.concat(ozone_high_str);
              
              thingspeak_message.concat("&field3=");
              String co_str = root_in["CO_ppm"];
              thingspeak_message.concat(co_str);
              
              thingspeak_message.concat("&field4=");
              String no_str = root_in["NO_ppm"];
              thingspeak_message.concat(no_str);
              
              thingspeak_message.concat("&field5=");
              String no2_str = root_in["NO2_ppm"];
              thingspeak_message.concat(no2_str);
  
              thingspeak_message.concat("&field6=");
              String so2_str = root_in["SO2_ppm"];
              thingspeak_message.concat(so2_str);
  
              thingspeak_message.concat("&field7=");
              String h2s_str = root_in["H2S_ppm"];
              thingspeak_message.concat(h2s_str);
  
              thingspeak_message.concat("&field8=");
              String c6h6_str = root_in["C6H6_ppm"];
              thingspeak_message.concat(c6h6_str);

              mqtt_thingspeak_client.publish(mqtt_topic_thingspeak, thingspeak_message.c_str());
              thingspeak_publish_counter = 0;
            }

            thingspeak_publish_counter++;
            
            DPRINTLNFUNC("Sending this topic to thingspeak:");
            DPRINTLNFUNC(thingspeak_message);
        }
          
      }
      else //debug voltage message
      {
          JsonObject Debug = root_out.createNestedObject("debug");
          Debug["V_Power"] = root_in["V_Power"];
          Debug["V_ozoneHigh"] = root_in["V_OzoneHigh"];
          Debug["V_ozoneLow"] = root_in["V_OzoneLow"];
          Debug["V_co"] = root_in["V_CO"];
          Debug["V_no"] = root_in["V_NO"];
          Debug["V_no2"] = root_in["V_NO2"];
          Debug["V_so2"] = root_in["V_SO2"];
          Debug["V_h2s"] = root_in["V_H2S"];
          Debug["V_c6h6"] = root_in["V_C6H6"];
          Debug["Rs_ozoneHigh"] = root_in["Rs_OzoneHigh"];
          Debug["Rs_ozoneLow"] = root_in["Rs_OzoneLow"];
          Debug["Rs_co"] = root_in["Rs_CO"];
          Debug["Rs_no2"] = root_in["Rs_NO2"];
          Debug["Rs_h2s"] = root_in["Rs_H2S"];
          Debug["Rs_c6h6"] = root_in["Rs_C6H6"];

          Debug["CAL_ozoneHigh"] = root_in["CAL_OzoneHigh"];
          Debug["CAL_ozoneLow"] = root_in["CAL_OzoneLow"];
          Debug["CAL_co"] = root_in["CAL_CO"];
          Debug["CAL_no2"] = root_in["CAL_NO2"];
          Debug["CAL_h2s"] = root_in["CAL_H2S"];
          Debug["CAL_c6h6"] = root_in["CAL_C6H6"];
          Debug["CAL_R0_ozoneHigh"] = root_in["CAL_OzoneHighR0"];
          Debug["CAL_R0_ozoneLow"] = root_in["CAL_OzoneLowR0"];
          Debug["CAL_R0_co"] = root_in["CAL_COR0"];
          Debug["CAL_R0_no2"] = root_in["CAL_NO2R0"];
          Debug["CAL_R0_h2s"] = root_in["CAL_H2SR0"];
          Debug["CAL_R0_c6h6"] = root_in["CAL_C6H6R0"];

          if (eepromData.mqttServerUserSettingsActive != 1) sprintf(mqtt_topic,client_topic_template,DeviceID,"debug_message");
          else sprintf(mqtt_topic,eepromData.mqttServerTopicTemplate,DeviceID,"debug_message");
      }

      serializeJson(root_out, json);

      DPRINTFUNC("JSON: ");
      DPRINTLNFUNC(json);
            
      PublishMqttDataMessage(mqtt_topic,json);
    }
  }
  delay(10);
}
