void mqtt_start(){
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void callback(String topic, byte *data, uint32_t length) {
      if (topic == "success"){
        flag_mqtt_send_success = true;
        tmr_check = millis();
        return;
      }
      else if (topic == "data"){
        char data_str[length+1];
        os_memcpy(data_str, data, length);
        data_str[length] = '\0';
        char * pch = strtok (data_str,";");
        calibration_current = atoi(pch);
        pch = strtok (NULL, ";");
        if (atof(pch) > 1)
          PWM_DUTY = atof(pch);
          PWM_DECREMENT = PWM_DUTY / 12;

        float tmp = 1.0 * (calibration_goal - calibration_current) / calibration_goal;
        portal.log.println (tmp);
        if ( tmp > -0.06 && tmp < 0.1 ){
          calibration_flag = false;
          portal.log.println ("CALIBRATION SUCCESS");
        }
        else {
          flag_mqtt_send_success = false;
        }
      }
    }
//
//void callback(char* topic, byte* payload, unsigned int length) {
//  portal.log.println(strtok(payload, ";"));
//  portal.log.println(strtok(payload, ";"));
//  
//  for (int i = 0; i < length; i++) {
//    portal.log.println((char)payload[i]);
//  }
////  Serial.println();
//
//// // Switch on the LED if an 1 was received as first character
////  if ((char)payload[0] == '1') {
////    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
////    // but actually the LED is on; this is because
////    // it is active low on the ESP-01)
////  } else {
////    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
////  }
//
//}


void reconnect() {
  static uint32_t tmr_rec;
  static int count = 0;
  if (count == 6) {
    portal.log.println ("ERROR MQTT CONN");
    calibration_flag = false;
  }
  if (!client.connected() && millis() - tmr_rec > 1000) {
      tmr_rec = millis();
      String clientId = "ESP8266Client-";
      clientId += String(random(0xffff), HEX);
      // Attempt to connect
      if (!client.connect(clientId.c_str())) {
        count++;}
      else{
        client.subscribe("data");
        client.subscribe("success");
      }
    }
}


void calibration(){
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
 
  
  static uint32_t tmr_cal = millis();
  if (flag_mqtt_send_success) calibration_tmr = (millis() - tmr_check) / 1000;
  
  if (!flag_mqtt_send_success && millis() - tmr_cal > 1000){
    tmr_cal = millis();
    snprintf (msg, MSG_BUFFER_SIZE, "%ld;%f;", calibration_goal, PWM_DUTY);
    client.publish("start", msg);
    if(mqtt_send_try++ > 10) {
      portal.log.println ("ERROR MQTT SEND");
      calibration_flag = false;
    }
  }
  
  
  
//  static uint32_t tmr_cal = millis();
//  calibration_tmr = (millis() - tmr_cal) / 1000;
//  //проверяю не пришли ли данные
//  if (пришли данные){
//    calibration_current = данные;
//    tmr_cal = millis();
//    float tmp = (calibration_goal - calibration_current) / calibration_goal;
//    if ( tmp > 0 && tmp < 0.05 ){
//      PWM_DUTY = данные;
//      PWM_DECREMENT = PWM_DUTY / 10;
//      calibration_flag = false;
//    }
//  }
}
