#define V_pin A0
#define V_coef 0.5
void setup() {
Serial.begin(9600);
}

//запускаем это
// открываем «Инструменты» и жмем «Плоттер по последовательному соединению«.
// крутим потенциометр, чтобы была идеальрная синусойда
//void loop() 
//{
//Serial.println(analogRead(A0));
//
//delay(100);
//}




//раз в секунду считаем напряжение в розетке

void loop(){
  static int V_max = 0;
  bool CURR_flag_ten_relay;
  static bool LAST_flag_ten_relay;
  float cur_V;
  

  static uint32_t tmr;
    if (millis() - tmr< 1000) {
      int V_tmp = abs(analogRead(V_pin) - 512);
      if (V_tmp > V_max) V_max = V_tmp;
      
    }
    else {
      cur_V = V_max * V_coef;
      V_max = 0;
      tmr = millis();
      
      Serial.print("Тек. вольтаж: ");
      Serial.print(cur_V);
      Serial.print("  ");
      Serial.print("PWM: ");
      Serial.print(byte(50 * (220/cur_V) * (220/cur_V) * 255/100));
      Serial.println("  ");

      
    }
}
  
