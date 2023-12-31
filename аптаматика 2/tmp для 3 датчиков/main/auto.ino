
void build() {
  BUILD_BEGIN();
  GP.THEME(GP_DARK);
  GP.UPDATE("bmp_temp_label,bmp_pres_label_start,bmp_pres_label_now,T_cube_label_boost,INDICATOR_1,PWM_OPEN_CLOSE_Proc_0,PWM_OPEN_CLOSE_Proc_1,Title_head_selection,time_head_selection,PWM_DUTY_NUMBER,tmr_clapan_was_closed,T_col_label_up,T_col_label_mid,T_col_label_down,T_dif_label,T_dif_level_label_now,T_cube_label,T_jets_label", 900);

  
  GP_MAKE_BOX(GP.TITLE("Автоматика by Апта"); GP.COLOR("INDICATOR_1", valCol, true););
  GP.HR(); 
  GP.NAV_TABS("              Нагрев              ,          Отбор голов          ,      Отбор тела по верху      ,      Отбор тела по дельте      ,Температура в колонне, Температура в кубе ,Температура струи на выходе");

  GP.NAV_BLOCK_BEGIN();
    GP.FORM_BEGIN("/setting_9");
    GP.TITLE("Температура в кубе");
    GP.LABEL(String(T_cube), "T_cube_label_boost");
    GP.HR(); 
    GP_MAKE_BOX(GP.LABEL("ШИМ нагрева     "); GP.NUMBER("ten_relay_PWM", "Введите PWM нагрева", ten_relay_PWM);)
    GP_MAKE_BOX(GP.LABEL("Период нагрева "); GP.NUMBER_F("ten_relay_PERIOD", "Введите период нагрева", ten_relay_PERIOD);)
    GP_MAKE_BOX(GP.LABEL("Разгон"); GP.SWITCH("flag_boost", process_number == 9);)
    GP.SUBMIT("Сохранить");
    GP.FORM_END();
  GP.NAV_BLOCK_END();
 
  GP.NAV_BLOCK_BEGIN();
    GP.FORM_BEGIN("/setting_0");
//    GP_MAKE_BOX(GP.LABEL("PWM нагрева");GP.NUMBER("ten_relay_PWM", "Введите PWM нагрева", ten_relay_PWM););
    GP_MAKE_BLOCK(
        GP.TITLE("Расчетное количество голов: \n" + String(int(head_selection)) + " (мл)", "Title_head_selection");
        GP.TITLE("Расчетное время отбора: \n00:00:00", "time_head_selection");
        );
      GP_MAKE_BLOCK(
        GP.TITLE("Текущий отбор в час (мл)");
        GP.NUMBER("current_selection_hour", "Текущая скорость отбора (мл)", current_selection);
        GP.TITLE("Необходимый отбор (мл)");
        GP.NUMBER("goal_selection", "Необходимый отбор (мл)", goal_selection);
        );
     GP_MAKE_BLOCK(
        GP.TITLE("ШИМ отбор");
        GP.LABEL("Работа клапана: " + String(PWM_OPEN) + "/" + String(PWM_CLOSE), "PWM_OPEN_CLOSE_Proc_0");
        GP_MAKE_BOX(GP.LABEL("Период (сек)"); GP.NUMBER("PWM_PERIOD", "Введите период срабатывания", PWM_PERIOD);)
        GP_MAKE_BOX(GP.LABEL("Скважность "); GP.NUMBER_F("PWM_DUTY_NUMBER", "Введите скважность", PWM_DUTY);)
      );
    GP.SUBMIT("Сохранить");
    GP.FORM_END();
  GP.NAV_BLOCK_END();

  GP.NAV_BLOCK_BEGIN();
    GP.FORM_BEGIN("/setting_1");
//    GP_MAKE_BOX(GP.LABEL("PWM нагрева");GP.NUMBER("ten_relay_PWM", "Введите PWM нагрева", ten_relay_PWM););
    GP_MAKE_BLOCK(
      GP.LABEL("Установка температуры");
      GP.NUMBER_F("T_goal", "Введите температуру колонны", (T_goal_col == 0) ? T_col_mid : T_goal_col);
      GP_MAKE_BOX(GP.LABEL("Max отклонение");GP.NUMBER_F("deviation_stop", "Введите отклонение закрытия", deviation_stop););
      GP_MAKE_BOX(GP.LABEL("Min отклонение ");GP.NUMBER_F("deviation_start", "Введите отклонение открытия", deviation_start););
      GP_MAKE_BOX(GP.LABEL("Время стабилизации");GP.NUMBER("STABILIZATION_TIME", "Введите время стабилизации", STABILIZATION_TIME););
      );
    GP_MAKE_BLOCK(
      GP.TITLE("ШИМ отбор");
      GP.LABEL("Работа клапана: " + String(PWM_OPEN) + "/" + String(PWM_CLOSE), "PWM_OPEN_CLOSE_Proc_1");
      GP_MAKE_BOX(GP.LABEL("Период (сек)"); GP.NUMBER("PWM_PERIOD", "Введите период срабатывания", PWM_PERIOD);)
      GP_MAKE_BOX(GP.LABEL("Скважность "); GP.NUMBER_F("PWM_DUTY_NUMBER", "Введите скважность", PWM_DUTY);)
      GP_MAKE_BOX(GP.LABEL("Декремент   "); GP.NUMBER_F("PWM_DECREMENT", "Введите декремент", PWM_DECREMENT);)
      );
    GP.SUBMIT("Сохранить");
    GP.FORM_END();
  GP.NAV_BLOCK_END();

  GP.NAV_BLOCK_BEGIN();
    GP.FORM_BEGIN("/setting_2");
//    GP_MAKE_BOX(GP.LABEL("PWM нагрева");GP.NUMBER("ten_relay_PWM", "Введите PWM нагрева", ten_relay_PWM););
    GP_MAKE_BLOCK(
      GP_MAKE_BOX(GP.LABEL("Max отклонение");GP.NUMBER_F("deviation_stop", "Введите отклонение закрытия", deviation_stop););
      GP_MAKE_BOX(GP.LABEL("Min отклонение ");GP.NUMBER_F("deviation_start", "Введите отклонение открытия", deviation_start););
      GP_MAKE_BOX(GP.LABEL("Время стабилизации");GP.NUMBER("STABILIZATION_TIME", "Введите время стабилизации", STABILIZATION_TIME););
      );
    GP_MAKE_BLOCK(
      GP.TITLE("ШИМ отбор");
      GP.LABEL("Работа клапана: " + String(PWM_OPEN) + "/" + String(PWM_CLOSE), "PWM_OPEN_CLOSE_Proc_1");
      GP_MAKE_BOX(GP.LABEL("Период (сек)"); GP.NUMBER("PWM_PERIOD", "Введите период срабатывания", PWM_PERIOD);)
      GP_MAKE_BOX(GP.LABEL("Скважность "); GP.NUMBER_F("PWM_DUTY_NUMBER", "Введите скважность", PWM_DUTY);)
      GP_MAKE_BOX(GP.LABEL("Декремент   "); GP.NUMBER_F("PWM_DECREMENT", "Введите декремент", PWM_DECREMENT);)
      );
    GP.SUBMIT("Сохранить");
    GP.FORM_END();
  GP.NAV_BLOCK_END();
  
  GP.NAV_BLOCK_BEGIN();
    GP_MAKE_BOX(GP.TITLE("Температура вверху");GP.LABEL(String(T_col_up), "T_col_label_up"););
    GP_MAKE_BOX(GP.TITLE("Температура в середине");GP.LABEL(String(T_col_mid), "T_col_label_mid"););
    GP_MAKE_BOX(GP.TITLE("Температура внизу");GP.LABEL(String(T_col_down), "T_col_label_down"););
    GP_MAKE_BOX(GP.TITLE("Разница с целевой");GP.LABEL(String(T_col_mid - T_goal_col), "T_dif_label"););
    GP_MAKE_BOX(GP.TITLE("Наружняя температура");GP.LABEL(String(bmp180_temp), "bmp_temp_label"););
    GP.HR(); 
    GP_MAKE_BOX(GP.TITLE("Разница уровней (нач)");GP.LABEL(String(abs(T_mid_down_dif)), "T_dif_level_label_start"););
    GP_MAKE_BOX(GP.TITLE("Разница уровней (тек)");GP.LABEL(String(abs(T_col_down - T_col_mid)), "T_dif_level_label_now"););
    GP.HR(); 
    GP_MAKE_BOX(GP.TITLE("Давление (нач)");GP.LABEL(String(bmp180_pressure_start), "bmp_pres_label_start"););
    GP_MAKE_BOX(GP.TITLE("Давление (тек)");GP.LABEL(String(bmp180_pressure), "bmp_pres_label_now"););
        
    GP.PLOT_DARK<4, PLOT_DIF_SIZE>("plot_dif", names_dif, arr_T_dif, 100);
    GP.TITLE("Время закрытого клапана: " + tmr_clapan_was_closed, "tmr_clapan_was_closed");
  GP.NAV_BLOCK_END();
  
  GP.NAV_BLOCK_BEGIN();
    GP.TITLE("Температура в кубе");
    GP.LABEL(String(T_cube), "T_cube_label");
    GP.PLOT_DARK<1, PLOT_CUBE_SIZE>("plot_cube", names_cube, arr_T_cube, 100);
  GP.NAV_BLOCK_END();
  
  GP.NAV_BLOCK_BEGIN();
    GP.TITLE("Температура струи на выходе");
    GP.LABEL(String(T_jets), "T_jets_label");
    GP.PLOT_DARK<1, PLOT_JETS_SIZE>("plot_jets", names_jets, arr_T_jets, 100);
  GP.NAV_BLOCK_END();
  BUILD_END();
}


void action() {
  if (portal.form("/setting_0")) { 
    process_number = 0;
    current_selection = portal.getInt("current_selection_hour");
    goal_selection = portal.getInt("goal_selection");
    
    PWM_PERIOD = portal.getInt("PWM_PERIOD");
    PWM_DUTY = portal.getFloat("PWM_DUTY_NUMBER");
  } 
  else if (portal.form("/setting_1")) {
    process_number = 1;
    float tmp_T_goal_col = portal.getFloat("T_goal");
    if (tmp_T_goal_col != T_goal_col) {
      T_goal_col = tmp_T_goal_col;
      bmp180_pressure_start = bmp180_pressure;
    }
    deviation_stop = portal.getFloat("deviation_stop");
    deviation_start = portal.getFloat("deviation_start");
    STABILIZATION_TIME = portal.getInt("STABILIZATION_TIME");

    PWM_PERIOD = portal.getInt("PWM_PERIOD");
    PWM_DUTY = portal.getFloat("PWM_DUTY_NUMBER");
    PWM_DECREMENT = portal.getFloat("PWM_DECREMENT");

    T_mid_down_dif = abs(T_col_down - T_col_mid);
  } 
  else if (portal.form("/setting_2")) {
    T_goal_col = T_col_mid;
    process_number = 2;
    bmp180_pressure_start = bmp180_pressure;
   
    deviation_stop = portal.getFloat("deviation_stop");
    deviation_start = portal.getFloat("deviation_start");
    STABILIZATION_TIME = portal.getInt("STABILIZATION_TIME");

    PWM_PERIOD = portal.getInt("PWM_PERIOD");
    PWM_DUTY = portal.getFloat("PWM_DUTY_NUMBER");
    PWM_DECREMENT = portal.getFloat("PWM_DECREMENT");

    T_mid_down_dif = abs(T_col_down - T_col_mid);
  } 
  else if (portal.form("/setting_9")) {
    ten_relay_PERIOD = portal.getFloat("ten_relay_PERIOD");
    ten_relay.setPeriod(int(1000 * ten_relay_PERIOD));
    
    if (portal.getBool("flag_boost")) {
      upd_ten_PWM(100);
      process_number = 9;
    }
    else if (!portal.getBool("flag_boost") && process_number == 9) {
      upd_ten_PWM(portal.getInt("ten_relay_PWM"));
      process_number = -99;
    }
    else upd_ten_PWM(portal.getInt("ten_relay_PWM"));
  }
  
  if (portal.update()) {
    portal.updateColor("INDICATOR_1", valCol);
    
  
    if (process_number == 0){
      String tmp = "Расчетное количество голов: \n" + String(int(head_selection)) + " (мл)";
      portal.updateString("Title_head_selection", tmp);
      if (current_selection == 0) time_head_selection = "00:00:00";
      tmp = "Расчетное время отбора: \n" + time_head_selection;
      portal.updateString("time_head_selection", tmp);
      tmp = "Работа клапана: " + String(PWM_OPEN) + "/" + String(PWM_CLOSE);
      portal.updateString("PWM_OPEN_CLOSE_Proc_0", tmp);
    }
    else if (process_number == 1){
      if (PWM_DUTY_CHANGED){
        portal.updateFloat("PWM_DUTY_NUMBER", PWM_DUTY);
        PWM_DUTY_CHANGED = false;
      }
      String tmp = "Работа клапана: " + String(PWM_OPEN) + "/" + String(PWM_CLOSE);
      portal.updateString("PWM_OPEN_CLOSE_Proc_1", tmp);
    }
    else if (process_number == 2){
      if (PWM_DUTY_CHANGED){
        portal.updateFloat("PWM_DUTY_NUMBER", PWM_DUTY);
        PWM_DUTY_CHANGED = false;
      }
      String tmp = "Работа клапана: " + String(PWM_OPEN) + "/" + String(PWM_CLOSE);
      portal.updateString("PWM_OPEN_CLOSE_Proc_1", tmp);
    }
    else if (process_number == 9){
      portal.updateFloat("T_cube_label_boost", T_cube);
    }

    portal.updateFloat("T_col_label_up", T_col_up);
    portal.updateFloat("T_col_label_mid", T_col_mid);
    portal.updateFloat("T_col_label_down", T_col_down);
    if (process_number == 1) portal.updateFloat("T_dif_label", T_col_mid - T_goal_col);
    portal.updateFloat("T_dif_level_label_now", abs(T_col_down - T_col_mid));

    portal.updateFloat("bmp_temp_label", bmp180_temp);
    portal.updateFloat("bmp_pres_label_start", bmp180_pressure_start);
    portal.updateFloat("bmp_pres_label_now", bmp180_pressure);

    if (tmr_clapan_was_closed != 0){
      String tmp = "Время закрытого клапана: " + tmr_clapan_was_closed;
      portal.updateString("tmr_clapan_was_closed", tmp);
    }
    
    portal.updateFloat("T_cube_label", T_cube);
    portal.updateFloat("T_jets_label", T_jets);
    
  }
}



void head_process(){
  
  // счетчик отбора голов
  static uint32_t tmr_head_selection = millis();
  if (millis() - tmr_head_selection > 750) {
    head_selection += 1.0 * current_selection * (millis() - tmr_head_selection) / (60 * 60 * 1000);
    tmr_head_selection = millis();
    int tmp_sec = (goal_selection - head_selection) / (current_selection / 60.0 / 60);
    time_head_selection = int_to_hour(tmp_sec / (60 * 60)) + int_to_min(tmp_sec / 60 % 60) + int_to_sec(tmp_sec % 60 % 60);
    static bool flag_send = true;
    if (flag_send && tmp_sec / 60 % 60 == 5 && tmp_sec / (60 * 60) == 0){
      bot.sendMessage("До конца отбора голов осталось 5 минут");
      flag_send = false;
      piezo_set(3,times_2,tones_2);
    }
  }

  // Работа шим
  PWM_tick();
  
  //индикация
  valCol.setHEX(0x00ff00);
}

void body_process(){
  if (process_finished_ || T_goal_col == 0) return;
  
  static bool flag_closed_window = true;
  static uint32_t tmr_clapan_was_closed_TIME;
  static uint32_t tmr_closed_window;
  if (flag_closed_window && T_col_mid - T_goal_col < deviation_stop){
    
    //тик шим
    PWM_tick();
    
    //индикация
    valCol.setHEX(0x00ff00);
    
  }
  else {
    if (flag_closed_window){
      tmr_clapan_was_closed_TIME = millis();
      if (PWM_DUTY > 0){
        PWM_DUTY -= PWM_DECREMENT;
        PWM_DUTY_CHANGED = true;
      }
      if (PWM_DUTY  < 10 && PWM_DUTY_CHANGED){
        process_finished_ = true;
        bot.sendMessage("!!!Выгон закончен по декременту!!!");
        piezo_set(4,times_3,tones_3);
      }
    }
    flag_closed_window = false;
    digitalWrite(RELE, LOW);  

    int tmp_s = (millis() - tmr_clapan_was_closed_TIME)/1000;
    tmr_clapan_was_closed = int_to_min(tmp_s / 60) + int_to_sec(tmp_s % 60);

    //индикация
    valCol.setHEX(0x0000ff);

    if (millis() - tmr_clapan_was_closed_TIME >= 10*60*1000){
      process_finished_ = true;
      bot.sendMessage("!!!Выгон закончен!!!");
      piezo_set(4,times_3,tones_3);
    }
    if (T_col_mid - T_goal_col < deviation_start && millis() - tmr_closed_window >= STABILIZATION_TIME*1000) flag_closed_window = true;//переписать как ниже!! всегда millis() - tmr_closed_window будет меньше нужного
    else tmr_closed_window = millis();
  }
}

void body_process_delta(){
  if (process_finished_ || T_col_mid == 0) return;
  
  static bool flag_closed_window = true;
  static uint32_t tmr_clapan_was_closed_TIME;
  static uint32_t tmr_closed_window;
//  T_mid_down_dif = T_col_down - T_col_mid;
  if (flag_closed_window && abs(abs(T_col_down - T_col_mid) - abs(T_mid_down_dif)) < deviation_stop){
    
    //тик шим
    PWM_tick();
    
    //индикация
    valCol.setHEX(0x00ff00);
    
  }
  else {
    if (flag_closed_window){
      tmr_clapan_was_closed_TIME = millis();
      if (PWM_DUTY > 0){
        PWM_DUTY -= PWM_DECREMENT;
        PWM_DUTY_CHANGED = true;
      }
      if (PWM_DUTY  < 10 && PWM_DUTY_CHANGED){
        process_finished_ = true;
        bot.sendMessage("!!!Выгон закончен по декременту!!!");
        piezo_set(4,times_3,tones_3);
      }
    }
    flag_closed_window = false;
    digitalWrite(RELE, LOW);  

    int tmp_s = (millis() - tmr_clapan_was_closed_TIME)/1000;
    tmr_clapan_was_closed = int_to_min(tmp_s / 60) + int_to_sec(tmp_s % 60);

    //индикация
    valCol.setHEX(0x0000ff);

    if (millis() - tmr_clapan_was_closed_TIME >= 10*60*1000){
      process_finished_ = true;
      bot.sendMessage("!!!Выгон закончен!!!");
      piezo_set(4,times_3,tones_3);
    }
    if (abs(abs(T_col_down - T_col_mid) - abs(T_mid_down_dif)) < deviation_start)
    {
      if (millis() - tmr_closed_window >= STABILIZATION_TIME*1000) flag_closed_window = true;
    }
    else tmr_closed_window = millis();
  }
}

void boost_process(){
  digitalWrite(RELE, LOW);
  valCol.setHEX(0x00ff00);
  
    
  if (T_cube > 73 && ten_relay_PWM == 100){
    valCol.setHEX(0x0000ff);
    upd_ten_PWM(50);
    bot.sendMessage("Разогрев почти закончен");
    auto_info_msg();
    piezo_set(3,times_2,tones_2);
  }
}


void start_process(){
  digitalWrite(RELE, LOW);
  valCol.setHEX(0x00ff72);
//  upd_ten_PWM(0);
}


void wait_process(){
  digitalWrite(RELE, LOW);
  valCol.setHEX(0x00ff00);
}

void WARNINGS(){
  if (!process_finished_){
    if (T_cube > 99 || T_col_mid > 95 || T_col_up > 98 || T_col_down > 95 || T_jets > 55){
      process_finished_ = true;
      bot.sendMessage("Температура слишком высокая\nВыгон остановлен");
      auto_info_msg();
      piezo_set(8,times_1,tones_1);
    }     
    if ( (process_number == 1 || process_number == 2) && ((T_goal_col != 0 && abs(T_goal_col - T_col_mid) > 1.5) || T_col_down - T_col_mid - T_mid_down_dif > 1.5)){
      process_finished_ = true;
      bot.sendMessage("!!!АВАРИЯ!!!");
      bot.sendMessage("РАЗНИЦА ТЕМПЕРАТУР СЛИШКОМ ВЫСОКАЯ: " + String(abs(T_goal_col - T_col_mid)));
      auto_info_msg();
      piezo_set(8,times_1,tones_1);
    }   
  }
  else {
    upd_ten_PWM(0);

    //индикация
    valCol.setHEX(0xff0000);
  }
}


void PWM_tick(){
  static uint32_t tmr_PWM_MODE;
  if (millis() - tmr_PWM_MODE > 1000*PWM_PERIOD) tmr_PWM_MODE = millis();
  
  int tmp = millis() - tmr_PWM_MODE;
  if (tmp < 10*PWM_PERIOD*PWM_DUTY) {
    PWM_OPEN = (10*PWM_PERIOD*PWM_DUTY - tmp) / 1000;
    PWM_CLOSE = PWM_PERIOD - PWM_PERIOD*PWM_DUTY / 100;
    digitalWrite(RELE, HIGH);
  }
  else if (tmp > 10*PWM_PERIOD*PWM_DUTY){
    PWM_OPEN = PWM_PERIOD*PWM_DUTY / 100;
    PWM_CLOSE = (PWM_PERIOD - PWM_PERIOD*PWM_DUTY / 100) - (tmp - 10*PWM_PERIOD*PWM_DUTY) / 1000;
    digitalWrite(RELE, LOW);
  } 
}


void upd_ten_PWM(int a){
  ten_relay_PWM = a;
  ten_relay.setPWM(ten_relay_PWM*255/100);
}
