void ds18b20_start(){
//  sensor1.setResolution(12);
  for (int i = 0; i < ds18b20_count; i++) {
    sensor[i].setAddress(ds18b20_addr[i]);
  }
  Serial.println("DS18B20_STARTED");
}

void read_auto_temp(){
  
  static uint32_t tmr_1;
    if (millis() - tmr_1>= 800) {
      tmr_1 = millis();
      
//      sensor[i].getTemp()
//      sensor[i].requestTemp();

      if (sensor[0].readTemp()) T_col_down = sensor[0].getTemp();
      sensor[0].requestTemp();
      if (sensor[1].readTemp()) T_col_mid = sensor[1].getTemp();
      sensor[1].requestTemp();
      if (sensor[2].readTemp()) T_col_up = sensor[2].getTemp();
      sensor[2].requestTemp();
      if (sensor[3].readTemp()) T_cube = sensor[3].getTemp();
      sensor[3].requestTemp();
      
//      if (sensor1.readTemp()) T_col_mid = sensor1.getTemp();
//      sensor1.requestTemp();
    }
}
