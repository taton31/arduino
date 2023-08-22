

void scale_start() {
  Serial.println("Initialization scale");
  scale.begin(scale_1, scale_2); // начало работы с HX711, указываются пины данных
  scale.set_scale(); // установка коэффициента преобразования
  scale.tare(); // аппаратная калибровка измерения нуля///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  EEPROM.get(100, calibrationValue);
  scale.set_scale(calibrationValue);
  portal.log.println("Калиб. коэф.");
  portal.log.println(calibrationValue);
  Serial.println("Scale started");
}

void get_weight() {
  static uint32_t tmr_3 = millis();
  if (millis() - tmr_3 > 1000){ 
    tmr_3 = millis();
    weight = scale.get_units(); // считываем значение в весовых единицах
    float curr_V = weight * ro;
    int zzzzz =  (60 * 60 * 1000) / (millis() - tmr_2);    
    callibration_current = curr_V * zzzzz;
    portal.log.print("weight: ");
    portal.log.println(weight);
    portal.log.print("curr vol: ");
    portal.log.println(curr_V);
    portal.log.print("1h vol: ");
    portal.log.println(callibration_current);
    portal.log.print("Time: ");
    portal.log.println(((millis() - tmr_2)/1000));
    portal.log.println();
  }
}


void calib_tick(){
  if (!flag_calib) return;
  static bool start_ = true;

    if (start_){ 
      portal.log.println("Калибровка");
      scale.set_scale();
      scale.tare();///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      portal.log.println("5 сек на вес");
      start_ = false;
    }
    if (millis() - tmr_4 > 7000){ 
        float reading = scale.get_units();
      calibrationValue = reading / knownWeight;
//      calibrationValue = reading / knownWeight / scale.get_scale();
      EEPROM.put(100, calibrationValue);
      EEPROM.commit(); // запись значения в EEPROM
      portal.log.println("Откалибровано");
      portal.log.println(calibrationValue);
      scale.set_scale(calibrationValue);
      flag_calib = false;
    }
}
