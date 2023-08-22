const float k = 0.3;
const int time_break_scan = 15 * 1000;
const int time_scan = 350;
const int V_arr_size = 5;
int max_V_arr[V_arr_size];

float expRunningAverage(float newVal) {
  static float filVal = 220;
  if (newVal > 235) newVal = 235;
  if (newVal < 210) newVal = 210;
  filVal += (newVal - filVal) * k;
  return filVal;
}

void V_scan_start(){
/////////////////////////////////////  
    analogReference(EXTERNAL);
    pinMode(V_pin, INPUT);
    Serial.println("V_SCAN_STARTED");
}


void relay_tick(){
  if (process_finished_) {
    digitalWrite(RELE, LOW);
    ten_relay.setPWM(0);
    return;
  }
  bool CURR_flag_ten_relay;
  static bool LAST_flag_ten_relay;
  static float cur_V = 0;
  static int max_V = 1000;
  
  //сейчас измерение идет пока тен отключен (расскомментить, чтобы поменять)
  CURR_flag_ten_relay = ten_relay.tick(); // CURR_flag_ten_relay = !ten_relay.tick();


  static uint32_t tmr_break_scan = millis();
  static uint32_t tmr_scan = millis();

  if (!CURR_flag_ten_relay && !flag_clap && millis() - tmr_break_scan > time_break_scan){
    digitalWrite(RELE, LOW);
    tmr_break_scan = millis();
    tmr_scan = millis();
    
    while (millis() - tmr_scan < time_scan){
      delay(0);
      portal.tick();
      int tmp = analogRead(V_pin); 
      if (tmp > 655){
        for (int j = 0; j < V_arr_size; j++){
          if (tmp > max_V_arr[j]) {
            max_V_arr[j] = tmp;
            break;
          }
        }
      }
    }
    max_V = 1000;
    for (int j = 0; j < V_arr_size; j++){
       if (max_V_arr[j] < max_V) max_V = max_V_arr[j];
    }
    
//    portal.log.println(max_V);
    cur_V = 1.9138 * max_V - 1063.3862;
//    portal.log.println(cur_V);
    cur_V = expRunningAverage(cur_V);
//    portal.log.println(cur_V);
//    portal.log.println(byte(1.0 * ten_relay_PWM * (220.0/cur_V) * (220.0/cur_V)));
//    portal.log.println("__");
    ten_relay.setPWM(byte(1.0 * ten_relay_PWM * (220.0/cur_V) * (220.0/cur_V) * 255/100));

    cur_V = 0;
    for (int j = 0; j < V_arr_size; j++){
       max_V_arr[j] = 0;
    }
  }

//  static uint32_t tmr_del = millis();
//  static uint32_t tmrlog;
//  
//  if (!CURR_flag_ten_relay){
//    if (millis() - tmr_del >= 23){
//      tmr_del = millis();
//      int tmp = analogRead(V_pin); 
//      if (tmp > max_V) max_V = tmp;
//    }
//  }
//  if (!LAST_flag_ten_relay && CURR_flag_ten_relay){
////    cur_V = 1.8365 * runMaxArifm(max_V) - 1006.7714;
//    cur_V = (float) runMaxArifm(max_V);
//    ten_relay.setPWM(byte(1.0 * ten_relay_PWM * (220.0/cur_V) * (220.0/cur_V) * 255/100));
////            Serial.print("Тек. вольтаж: ");
////            Serial.print(cur_V);
////            Serial.print("  ");
////            Serial.print("PWM: ");
////            Serial.print(byte(1.0 * ten_relay_PWM * (220/cur_V) * (220/cur_V) * 255/100));
////            Serial.print("  ");
////            Serial.print("Q: ");
////            Serial.print(byte(1.0 * ten_relay_PWM * (220/cur_V) * (220/cur_V) * 255/100) * cur_V * cur_V);
////            Serial.print("proc: ");
////            Serial.print("  ");
////            Serial.print(process_number);
////            Serial.println("  ");
//
//    tmrlog = millis();
//    portal.log.println(cur_V);
//    portal.log.println(byte(1.0 * ten_relay_PWM * (220.0/cur_V) * (220.0/cur_V)));
//    portal.log.println("______");
//  
//
//  
//    cur_V = 0;
//    max_V = 0;
//  }
  LAST_flag_ten_relay = CURR_flag_ten_relay;
}
