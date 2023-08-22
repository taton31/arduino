void bot_start(){
  bot.setChatID(MY_TG_ID);
  bot.attach(info_msg);
  Serial.println("BOT_STARTED");
}

void info_msg(FB_msg& msg) {
  if (msg.text == "/info") {
      String answer_msg;
      
      answer_msg += "Температура колонны вверху: " + String(T_col_up) + "\n";
      answer_msg += "Температура колонны в середине: " + String(T_col_mid) + "\n";
      answer_msg += "Температура колонны внизу: " + String(T_col_down) + "\n";
      bot.sendMessage(answer_msg);
      
      answer_msg = "";
      answer_msg += "Температура в кубе: " + String(T_cube) + "\n";
      answer_msg += "Разница с установленной температурой: " + String(abs(T_goal_col - T_col_mid)) + "\n";
      answer_msg += "Температура струи на выходе: " + String(T_jets) + "\n";
      bot.sendMessage(answer_msg);
    
  }
  else if (msg.text == "/send_log") {
    digitalWrite(RELE, LOW);
    bot_sendFile();
  }
  else if (msg.text == "/del_log") {
    digitalWrite(RELE, LOW);
    bot_deleteFile();
    bot.sendMessage("Удалено");
  }
  else if (msg.text == "/stop") {
    digitalWrite(RELE, LOW);
    upd_ten_PWM(0);
    process_finished_ = true;
    bot.sendMessage("Выгон остановлен");
  }
}


void auto_info_msg() {
    String answer_msg;

    answer_msg += "Температура в кубе: " + String(T_cube) + "\n";
    answer_msg += "Температура колонны внизу: " + String(T_col_down) + "\n";
    answer_msg += "Температура колонны в середине: " + String(T_col_mid) + "\n";
    answer_msg += "Температура колонны вверху: " + String(T_col_up) + "\n";
    answer_msg += "Разница с установленной температурой: " + String(abs(T_goal_col - T_col_mid)) + "\n";
    answer_msg += "Температура струи на выходе: " + String(T_jets) + "\n";
    bot.sendMessage(answer_msg);
  
}
