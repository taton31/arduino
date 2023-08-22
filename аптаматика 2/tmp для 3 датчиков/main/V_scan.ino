//void V_scan_start(){
///////////////////////////////////////  analogReference(INTERNAL);
//    pinMode(V_pin, INPUT);
//    Serial.println("V_SCAN_STARTED");
//}

//int calc_average(int VALUE){
//  static int i = 0;
//  static int VALUES[AVERAGE_SIZE];
//  static long int AVERAGE_SUM = 0;
//  if (++i >= V_average_size) i = 0;
//  AVERAGE_SUM -= VALUES[i];
//  AVERAGE_SUM += VALUE;
//  VALUES[i] = VALUE;
//  return (AVERAGE_SUM / AVERAGE_SIZE)
//}

void relay_tick(){
  bool CURR_flag_ten_relay;
  static bool LAST_flag_ten_relay;
  static float cur_V;
  
  //сейчас измерение идет пока тен отключен (расскомментить, чтобы поменять)
  CURR_flag_ten_relay = ten_relay.tick(); // CURR_flag_ten_relay = !ten_relay.tick();

  

  if (!CURR_flag_ten_relay){
//    cur_V = calc_average(analogRead(V_pin)) * V_coef;
    cur_V = 220.0;
  }
  if (!LAST_flag_ten_relay && CURR_flag_ten_relay){
    ten_relay.setPWM(byte(1.0 * ten_relay_PWM * (220/cur_V) * (220/cur_V) * 255/100));
  }

  

//  Serial.print("Тек. вольтаж: ");
//  Serial.print(cur_V);
//  Serial.print("  ");
//  Serial.print("PWM: ");
//  Serial.print(byte(1.0 * ten_relay_PWM * (220/cur_V) * (220/cur_V) * 255/100));
//  Serial.print("  ");
//  Serial.print("Q: ");
//  Serial.print(byte(1.0 * ten_relay_PWM * (220/cur_V) * (220/cur_V) * 255/100) * cur_V * cur_V);
//  Serial.print("proc: ");
//  Serial.print(process_number);
//  Serial.print("  ");
//  Serial.println("  ");

  LAST_flag_ten_relay = CURR_flag_ten_relay;
}
