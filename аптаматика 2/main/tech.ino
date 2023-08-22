String int_to_min(int T){
  if (T == 0) return "00:";
  else if (T/10 == 0) return "0" + String(T) + ":";
  else return String(T) + ":";
}

String int_to_sec(int T){
  if (T == 0) return "00";
  else if (T/10 == 0) return "0" + String(T);
  else return String(T);
}


String int_to_hour(int T){
  if (T == 0) return "";
  else if (T/10 == 0) return String(T) + ":";
  else return String(T) + ":";
}

void push_all_arr(int val, int16_t * arr, int _size){
  for (int i = 0; i < _size; i++){
    arr[i] = val;
  }
}


void EEPROM_get(){
  EEPROM.begin(200);
  EEPROM.get(sizeof(lp), mqtt_server);              
  EEPROM.get(sizeof(lp)+sizeof(mqtt_server), ds18b20_addr[0]);
  EEPROM.get(sizeof(lp)+sizeof(mqtt_server)+sizeof(ds18b20_addr[0]), ds18b20_addr[1]);
  EEPROM.get(sizeof(lp)+sizeof(mqtt_server)+2*sizeof(ds18b20_addr[0]), ds18b20_addr[2]);
  EEPROM.get(sizeof(lp)+sizeof(mqtt_server)+3*sizeof(ds18b20_addr[0]), ds18b20_addr[3]);
  EEPROM.end();
}

void EEPROM_put(){
  EEPROM.begin(200);
  

  EEPROM.put(sizeof(lp), mqtt_server);              
  EEPROM.put(sizeof(lp)+sizeof(mqtt_server), ds18b20_addr[0]);
  EEPROM.put(sizeof(lp)+sizeof(mqtt_server)+sizeof(ds18b20_addr[0]), ds18b20_addr[1]);
  EEPROM.put(sizeof(lp)+sizeof(mqtt_server)+2*sizeof(ds18b20_addr[0]), ds18b20_addr[2]);
  EEPROM.put(sizeof(lp)+sizeof(mqtt_server)+3*sizeof(ds18b20_addr[0]), ds18b20_addr[3]);
  
  EEPROM.commit();                // записываем
  EEPROM.end();
}
void APM_test(){
  static int APM = 0;
  static uint32_t tmr_11;
  if (millis() - tmr_11 < 1000) {
      APM = APM+1;
    }
  else { portal.log.println(APM); APM=0;tmr_11 = millis();}
}


void update_graph_data(){
  static uint32_t tmr_2;
  int tmr_step;
  tmr_step = 1000*60;//10*60*10*6*8;
  
  if (millis() - tmr_2 >= tmr_step) { 
    tmr_2 = millis();

    if (T_cube != 0 && arr_T_cube[0][0] == 0) push_all_arr(T_cube * 100, arr_T_cube[0], PLOT_CUBE_SIZE);
    if (T_col_mid != 0 && arr_T_dif[0][0] == 0) push_all_arr(T_col_mid * 100, arr_T_dif[0], PLOT_DIF_SIZE);
    
    if (T_col_up != 0 && arr_T_dif[1][0] == 0) push_all_arr(T_col_up * 100, arr_T_dif[1], PLOT_DIF_SIZE);
    if (T_col_down != 0 && arr_T_dif[2][0] == 0) push_all_arr(T_col_down * 100, arr_T_dif[2], PLOT_DIF_SIZE);
    if (T_goal_col != 0 && arr_T_dif[3][0] == 0) push_all_arr(T_goal_col * 100, arr_T_dif[3], PLOT_DIF_SIZE);
   
//    if (T_jets != 0 && arr_T_jets[0][0] == 0) push_all_arr(T_jets * 100, arr_T_jets[0], PLOT_JETS_SIZE);
     
    
    GPaddInt(int(T_cube*100), arr_T_cube[0], PLOT_CUBE_SIZE);
    GPaddInt(int(T_col_mid*100), arr_T_dif[0], PLOT_DIF_SIZE);
    
    GPaddInt(int(T_col_up*100), arr_T_dif[1], PLOT_DIF_SIZE);
    GPaddInt(int(T_col_down*100), arr_T_dif[2], PLOT_DIF_SIZE);
    GPaddInt(int(T_goal_col*100), arr_T_dif[3], PLOT_DIF_SIZE);
    
//    GPaddInt(int(T_jets*100), arr_T_jets[0], PLOT_JETS_SIZE);
  }
}


void Portal_start(){
  
  Serial.println("start auto portal");
  portal.attachBuild(build);
  portal.start(WIFI_AP);
  portal.attach(action);
 
  portal.enableOTA();
  portal.log.start(30);
  Serial.println("PORTAL_STARTED");
  
}


void pins_start(){
  
  pinMode(RELE, OUTPUT);
  pinMode(piezoPin, OUTPUT);
  pinMode(ten_relay_pin, OUTPUT);
  digitalWrite(RELE, LOW);
  digitalWrite(ten_relay_pin, LOW);
  valCol.setHEX(0x000000);
  Serial.begin(115200);
  
  Serial.println("PINS_STARTED");
}
