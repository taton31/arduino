void LCD_start(){
  lcd.init();               
  lcd.backlight();
  lcd.createChar(0, char_T_up);
  lcd.createChar(1, char_T_down);
  lcd.createChar(2, char_T_mid);
  lcd.createChar(3, char_T_cube);
//  lcd.createChar(4, char_T_jets);
  lcd.clear();
  Serial.println("LCD_STARTED");
}


void LCD_Temp(){
  static uint32_t tmr_LCD_start;
  static uint32_t tmr_LCD_reload;
  static byte i = 0;
  static bool flag_backlight = false;
  if (millis() - tmr_LCD_start > 1000){
    tmr_LCD_start = millis();

    if (process_finished_){
      if (flag_backlight) lcd.backlight();
      else lcd.noBacklight();
      flag_backlight = !flag_backlight;
    }
    
    if (millis() - tmr_LCD_reload > 5 * 60 * 1000){
      tmr_LCD_reload = millis();
      i--;
    }
    if (i < stop_name){
      i++;
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("Старт"); // 2 0
    }
    else if (i == stop_name){
      i++;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(2);
      lcd.print("="); 
      lcd.print(T_col_mid);
      lcd.setCursor(8, 0);
      lcd.write(1);
      lcd.print("="); 
      lcd.print(T_col_down);
      lcd.setCursor(0, 1);
      lcd.write(0);
      lcd.print("="); 
      lcd.print(T_col_up);
      lcd.setCursor(8, 1);
      lcd.write(3);
      lcd.print("="); 
      lcd.print(T_cube);
    }
    else{
      lcd.setCursor(2, 0);
      lcd.print(T_col_mid);
      lcd.setCursor(10, 0);
      lcd.print(T_col_down);
      lcd.setCursor(2, 1);
      lcd.print(T_col_up);
      lcd.setCursor(10, 1);
      lcd.print(T_cube);
    }
  }
}
