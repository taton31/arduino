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



void update_graph_data(){
  static uint32_t tmr_2;
  int tmr_step;
  if (!braga_button_bool) tmr_step = 1000*60*20 / PLOT_DIF_SIZE; //10 min
  else tmr_step = 10*60*10*6*8;
  
  if (millis() - tmr_2 >= tmr_step) { 
    tmr_2 = millis();

    if (T_cube != 0 && arr_T_cube[0][0] == 0) push_all_arr(T_cube * 100, arr_T_cube[0], PLOT_CUBE_SIZE);
    if (T_col_mid != 0 && arr_T_dif[0][0] == 0) push_all_arr(T_col_mid * 100, arr_T_dif[0], PLOT_DIF_SIZE);
    if (!braga_button_bool){
      if (T_col_down != 0 && arr_T_dif[2][0] == 0) push_all_arr(T_col_down * 100, arr_T_dif[1], PLOT_DIF_SIZE);
      if (T_goal_col != 0 && arr_T_dif[3][0] == 0) push_all_arr(T_goal_col * 100, arr_T_dif[2], PLOT_DIF_SIZE);
    }
    if (T_jets != 0 && arr_T_jets[0][0] == 0) push_all_arr(T_jets * 100, arr_T_jets[0], PLOT_JETS_SIZE);
     
    
    GPaddInt(int(T_cube*100), arr_T_cube[0], PLOT_CUBE_SIZE);
    GPaddInt(int(T_col_mid*100), arr_T_dif[0], PLOT_DIF_SIZE);
    if (!braga_button_bool){
      GPaddInt(int(T_col_up*100), arr_T_dif[1], PLOT_DIF_SIZE);
      GPaddInt(int(T_col_down*100), arr_T_dif[2], PLOT_DIF_SIZE);
      GPaddInt(int(T_goal_col*100), arr_T_dif[3], PLOT_DIF_SIZE);
    }
    GPaddInt(int(T_jets*100), arr_T_jets[0], PLOT_JETS_SIZE);
  }
}


void Portal_start(){
  if (braga_button_bool){
    Serial.println("start braga portal");
    portal.attachBuild(build_braga);
    portal.start(WIFI_AP);
    portal.attach(action_braga);
  }
  else{
    Serial.println("start auto portal");
    portal.attachBuild(build);
    portal.start(WIFI_AP);
    portal.attach(action);
  }
  portal.enableOTA();
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
  
//  braga_button_bool = !digitalRead(braga_button);
  braga_button_bool = false;
  Serial.println("PINS_STARTED");
}
