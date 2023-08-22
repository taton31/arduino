//#define V_pin A0
//#define V_coef 1.0
//
//
//
//////запускаем это
////// открываем «Инструменты» и жмем «Плоттер по последовательному соединению«.
////// крутим потенциометр, чтобы была идеальрная синусойда
////void setup() {
////Serial.begin(9600);
////}
////void loop() 
////{
////Serial.println((analogRead(A0)-512) / sqrt(2));
////
////delay(40);
////}
////y=2.3173x−1267.4310
////649 650 645 648 644 644 643 643 648 644 644 646 644 645
////237 238.7 228 235 226 225 221 222 232 224 225.4 229.5 226 227.5
//
//void setup() {
//Serial.begin(9600);
//}
//void loop(){
//  static int V_max = 0;
//  bool CURR_flag_ten_relay;
//  static bool LAST_flag_ten_relay;
//  float cur_V;
//  
//
//  static uint32_t tmr;
//    if (millis() - tmr< 1000) {
//      int V_tmp = analogRead(V_pin);
//      if (V_tmp > V_max) V_max = V_tmp;
//      
//    }
//    else {
//      cur_V = 2.3173 * V_max - 1267.4310;
//      V_max = 0;
//      tmr = millis();
//      float zzz = 0.5 * (220/cur_V) * (220/cur_V);
//      Serial.print("Тек. вольтаж: ");
//      Serial.print(cur_V);
//      Serial.print("  ");
//      Serial.print("PWM: ");
//      Serial.print(zzz);
//      Serial.print("  ");
//      Serial.print(220*220*0.5);
//      Serial.print("  ");
//      Serial.print(cur_V*cur_V*zzz);
//      Serial.println("  ");
//      
//
//      
//    }
//}
