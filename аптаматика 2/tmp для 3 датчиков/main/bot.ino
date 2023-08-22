void bot_start(){
  bot.setChatID(MY_TG_ID);
  bot.attach(info_msg);
  Serial.println("BOT_STARTED");
}

void info_msg(FB_msg& msg) {
  if (msg.text == "/info") {
    if (braga_button_bool){
      String answer_msg;
      if (S_T_1) answer_msg += "Термометр №1: " + String(T_cube) + "\n";
      if (S_T_2) answer_msg += "Термометр №2: " + String(T_col_mid) + "\n";
      if (S_T_3) answer_msg += "Термометр №3: " + String(T_jets) + "\n";

      if (answer_msg != "") bot.sendMessage(answer_msg);
      else bot.sendMessage("Нет включенных термометров");
    }
    else if (!braga_button_bool){
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
  }
  else if (msg.text == "/send_log") {
    bot_sendFile();
  }
  else if (msg.text == "/del_log") {
    bot_deleteFile();
    bot.sendMessage("Удалено");
  }
}


void auto_info_msg() {
  if (braga_button_bool){
    String answer_msg;
    if (S_T_1) answer_msg += "Термометр №1: " + String(T_cube) + "\n";
    if (S_T_2) answer_msg += "Термометр №2: " + String(T_col_mid) + "\n";
    if (S_T_3) answer_msg += "Термометр №3: " + String(T_jets) + "\n";

    if (answer_msg != "") bot.sendMessage(answer_msg);
    else bot.sendMessage("Нет включенных термометров");
  }
  else if (!braga_button_bool){
    String answer_msg;

    answer_msg += "Температура в кубе: " + String(T_cube) + "\n";
    answer_msg += "Температура колонны внизу: " + String(T_col_down) + "\n";
    answer_msg += "Температура колонны в середине: " + String(T_col_mid) + "\n";
    answer_msg += "Температура колонны вверху: " + String(T_col_up) + "\n";
    answer_msg += "Разница с установленной температурой: " + String(abs(T_goal_col - T_col_mid)) + "\n";
    answer_msg += "Температура струи на выходе: " + String(T_jets) + "\n";
    bot.sendMessage(answer_msg);
  }
}
