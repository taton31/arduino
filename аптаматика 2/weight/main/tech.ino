const int NUM_READ = 16; 
float runMiddleArifmOptim(float newVal) {
  static int t = 0;
  static float vals[NUM_READ];
  static float average = 0;
  if (++t >= NUM_READ) t = 0; // перемотка t
  average -= vals[t];         // вычитаем старое
  average += newVal;          // прибавляем новое
  vals[t] = newVal;           // запоминаем в массив
  return ((float)average / NUM_READ);
}


void build() {
  BUILD_BEGIN();
  GP.THEME(GP_DARK);
  GP.UPDATE("weight,callibration_current", 1000);
  GP.LABEL(String(weight), "weight");
  GP.LABEL(String(callibration_current), "callibration_current");
  GP.AREA_LOG(15);
  GP.HR(); 
  GP.FORM_BEGIN("/calib");
    GP.NUMBER_F("knownWeight", "известный вес", knownWeight);
    GP.SUBMIT("Калибровка");
  GP.FORM_END();
  GP.FORM_BEGIN("/tare");
    GP.SUBMIT("Сброс тары");
  GP.FORM_END();
  BUILD_END();
}


void action() {
  if (portal.form("/calib")) { 
    knownWeight = portal.getFloat("knownWeight");
    tmr_4 = millis();
//    calib();
    flag_calib = true;
  } 
  if (portal.form("/tare")) { 
    scale.tare();
  }
  if (portal.update()) {
    portal.updateFloat("weight", weight);
    portal.updateInt("callibration_current", callibration_current);
  }
}

void APM_test(){
  static int APM = 0;
  static uint32_t tmr_11;
  if (millis() - tmr_11 < 1000) {
      APM = APM+1;
    }
  else { Serial.println(APM); APM=0;tmr_11 = millis();}
}
