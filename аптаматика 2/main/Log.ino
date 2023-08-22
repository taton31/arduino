
void bot_sendFile() {
  digitalWrite(RELE, LOW);
  bot.sendMessage("Отправляю");
  
  File file;
  file = LittleFS.open(Log_file_dir, "r");
  if (file){
    bot.sendFile(file, FB_DOC, Log_file);
    file.close();
  }
  else bot.sendMessage("Нет файла");
}



void appendFile(const String message) {
  int len = message.length();
  char message_char [len];
  message.toCharArray(message_char, len);
  
  File file = LittleFS.open(Log_file_dir, "a");
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  file.println(message_char);
  file.close();
}

void bot_deleteFile() {
  if (LittleFS.remove(Log_file_dir)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void START_FS(){
  LittleFS.format();
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }
  timeClient.begin();
}

void log_tick(){
  static uint32_t tmr_log;
 
    if (millis() - tmr_log > 1000*60) {
      String answer_msg;
      tmr_log = millis();
      timeClient.update();
      answer_msg += timeClient.getFormattedTime() + ";";
      answer_msg += "U=" + String(T_col_up) + ";";
      answer_msg += "M=" + String(T_col_mid) + ";";
      answer_msg += "D=" + String(T_col_down) + ";";
      answer_msg += "C=" + String(T_cube) + ";";
      answer_msg += "Diff=" + String(abs(abs(T_col_down - T_col_mid) - abs(T_mid_down_dif))) + ";";
//      answer_msg += "Tdif=" + String(abs(T_goal_col - T_col_mid)) + ";";
//      answer_msg += "Tj=" + String(T_jets) + ";";
//      answer_msg += "To=" + String(bmp180_temp) + ";";
//      answer_msg += "P=" + String(bmp180_pressure) + ";";
      appendFile(answer_msg);
    }
  
  
}
