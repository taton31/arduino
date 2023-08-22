void WiFi_start(){
  EEPROM.begin(200);
  EEPROM.get(0, lp);
  // пытаемся подключиться
  WiFi.mode(WIFI_STA);
//  WiFi.begin("Xiaomi_C749", "fynjy15456");
  WiFi.begin(lp.ssid, lp.pass);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    i += 1;
    if (i==25) {
      loginPortal();
      WiFi.mode(WIFI_STA);
      WiFi.begin(lp.ssid, lp.pass);
      //ESP.restart();
    }
    delay(500);
    Serial.print(i);
  }
  Serial.println(WiFi.localIP());
  Serial.println("WIFI_STARTED");
}

void build_wifi() {
  BUILD_BEGIN();
  GP.THEME(GP_DARK);

  GP.FORM_BEGIN("/login");
  GP.TEXT("lg", "Login", lp.ssid);
  GP.BREAK();
  GP.TEXT("ps", "Password", lp.pass);
  GP.SUBMIT("Сохранить");
  GP.FORM_END();

  BUILD_END();
}


void loginPortal() {
  Serial.println("Portal start");

  // запускаем точку доступа
  WiFi.mode(WIFI_AP);
  WiFi.softAP("WiFi Config");

  // запускаем портал
  GyverPortal portal_wifi;
  portal_wifi.attachBuild(build_wifi);
  portal_wifi.start(WIFI_AP);
  portal_wifi.attach(action_wifi);

  // работа портала
  while (portal_wifi.tick());
}

void action_wifi(GyverPortal& p) {
  if (p.form("/login")) {      // кнопка нажата
    p.copyStr("lg", lp.ssid);  // копируем себе
    p.copyStr("ps", lp.pass);
    EEPROM.put(0, lp);              // сохраняем
    EEPROM.commit();                // записываем
    WiFi.softAPdisconnect();        // отключаем AP
  }
}
