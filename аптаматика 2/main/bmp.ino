void BMP180_start(){
  flag_bmp_is_online = bmp.begin();
  Serial.println("BMP180_STARTED");
}

void get_bmp180_data(){
  if (!flag_bmp_is_online) return;
  
  static uint32_t tmr_bmp180_scan;
  if (millis() - tmr_bmp180_scan > 1000*60) {
    tmr_bmp180_scan = millis();
    RunningAverage_pres(bmp.readPressure()/133.322);
    RunningAverage_temp(bmp.readTemperature());
  }
}

void RunningAverage_pres(float newVal) {
  if (bmp180_pressure == 0) bmp180_pressure = newVal;
  bmp180_pressure += (newVal - bmp180_pressure) * k_average_pres;
}

void RunningAverage_temp(float newVal) {
  if (bmp180_temp == 0) bmp180_temp = newVal;
  bmp180_temp += (newVal - bmp180_temp) * k_average_temp;
}
