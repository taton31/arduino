#include <EEPROMex.h>


//-----------------------------------НАСТРОЙКИ------------------------------------
#define initial_calibration 0  // калибровка вольтметра 1 - включить, 0 - выключить
#define welcome 1              // приветствие (слова GYVER VAPE при включении), 1 - включить, 0 - выключить
#define battery_info 0         // отображение напряжения аккумулятора при запуске, 1 - включить, 0 - выключить
#define sleep_timer 10         // таймер сна в секундах
#define vape_threshold 3      // отсечка затяжки, в секундах
#define turbo_mode 1          // турбо режим 1 - включить, 0 - выключить
#define battery_percent 0      // отображать заряд в процентах, 1 - включить, 0 - выключить
#define battery_low 6.0       // нижний порог срабатывания защиты от переразрядки аккумулятора, в Вольтах!
//-----------------------------------НАСТРОЙКИ------------------------------------

int battery = A3;
int mosfet = 10;

//-----------кнопки-----------
#define butt_up 4      // кнопка вверх
#define butt_down 5    // кнпока вниз
#define butt_set 3     // кнопка выбора
#define butt_vape 2    // кнопка "парить"
//-----------кнопки-----------

//-----------флажки-----------
boolean up_state, down_state, set_state, vape_state, settings_flag, bat_flag;
boolean up_flag, down_flag, set_flag, set_flag_hold, set_hold, vape_btt, vape_btt_f, vape_release_check=0;
volatile boolean wake_up_flag, vape_flag=0;
boolean change_v_flag, change_w_flag, change_o_flag, change_m_flag;
volatile byte mode=0, mode_flag = 1;
boolean flag, all_bat = 1;          // флаг, разрешающий подать ток на койл (защита от КЗ, обрыва, разрядки)
boolean start=1, start2=1;
boolean push1=0, push2=0, push3=0, push4=0, push5=0;
boolean pash1=0, pash2=0, pash3=0, pash4=0, pash5=0;
//-----------флажки-----------


//-----------дисплей-----------
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#include <TimerOne.h>

//--------дисплей-------

float bat_vol, bat_volt_f=8, bat_old;   // хранит напряжение на акуме
int PWM, PWM_f;           // хранит PWM сигнал

//----переменные и коэффициенты для фильтра----
int PWM_old = 800;
float filter_k = 0.1;
float PWM_filter_k = 0.1;
float del;
//----переменные и коэффициенты для фильтра----

float volts;    // храним вольты
float my_vcc_const;  // константа вольтметра
unsigned long last_time, vape_press, set_press, last_vape, wake_timer, batt_lvl, up_time, down_time, sleep_time, resist_set, timer_start; // таймеры
float watts;    // храним вольты и ватты
float ohms;          // храним омы
int i,j;
volatile byte vape_mode, vape_release_count;

void setup(){
  start = 1;
  Serial.begin(9600);
  if (initial_calibration) calibration();  // калибровка, если разрешена

   //----читаем из памяти-----
  volts = 0.01*EEPROM.readInt(0);
  watts = 0.01*EEPROM.readInt(2);
  mode = EEPROM.readInt(4);
  ohms = EEPROM.readFloat(6);
  my_vcc_const = EEPROM.readFloat(8);
  //----читаем из памяти-----
  
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  symbols();
  Timer1.initialize(1500);          // таймер дисплея

  //---настройка кнопок и выходов-----
  analogReference( INTERNAL);
  pinMode(mosfet, OUTPUT);
  pinMode(battery, INPUT);
  pinMode(butt_vape, INPUT_PULLUP);
  pinMode(butt_set, INPUT_PULLUP);
  pinMode(butt_up, INPUT_PULLUP);
  pinMode(butt_down, INPUT_PULLUP);
  digitalWrite(mosfet, LOW);
   //---настройка кнопок и выходов-----

  //------приветствие-----
  /*if (welcome) {
    lcd.clear();
    lcd.print("Hello!!!");
    delay(1000);
  }*/
  //------приветствие-----


    
  if (battery_info) {
    //disp_send(BVOL);
    
  }
  timer_start=millis();
  digitalWrite(mosfet, 0);
}



void loop(){ 
  //-------опрос кнопок-------
  up_state = !digitalRead(butt_up);
  down_state = !digitalRead(butt_down);
  set_state = !digitalRead(butt_set);
  vape_state = !digitalRead(butt_vape);
  //-------опрос кнопок-------

  push();
  //pash();
  if (start && millis() - timer_start<1000) {
    lcd.setCursor(5,0);
    lcd.print("Hello!");
  }
  else start=0;
  if(!up_state && !down_state && !set_state && !vape_state && all_bat) {
    sleep_time = millis();
    all_bat = 0;
  }
  if(up_state || down_state || set_state || vape_state) {
    lcd.backlight();
    all_bat = 1;
  }
  if (millis() - sleep_time > 20000 && !all_bat) lcd.noBacklight();
  

  if (millis() - last_time > 100) {                       // 20 раз в секунду измеряем напряжение
    last_time = millis();
    bat_vol = (analogRead(battery) * my_vcc_const) / 1023;    // измерить напряжение аккумулятора в миллиВольтах
    del = 0.99/(0.99*9.7);
    bat_vol = 4.06/3.68 * bat_vol / del;
    
    bat_volt_f = filter_k * bat_vol + (1 - filter_k) * bat_volt_f;  // фильтруем
    bat_vol = bat_volt_f;
    if (vape_state) bat_flag = 1;
    if (millis() - batt_lvl>60000 || (!vape_state && bat_flag) || (start2 && !start)){
      start2=0;
      bat_flag=0;
      batt_lvl=millis();
      bat_old = bat_vol;                                // фильтруем
      lcd.clear();
      lcd.print("Battary lvl:");
      lcd.print(bat_vol);
      lcd.setCursor(0,1);
      if (mode==0) {
        lcd.print("Varivolt:");
        lcd.print(volts);
        lcd.print("V");
      }
      else if (mode > 0) {
        lcd.print("Variwatt:");
        lcd.print(watts);
        lcd.print("W");
      }
      if (bat_vol < 6.0) { // если напряжение меньше минимального
        delay(20);
        bat_vol = (analogRead(battery) * my_vcc_const) / 1023;    // измерить напряжение аккумулятора в миллиВольтах
    del = 0.99/(0.99*9.7);
    bat_vol = 4.06/3.68 * bat_vol / del;
    bat_volt_f = filter_k * bat_vol + (1 - filter_k) * bat_volt_f;  // фильтруем
    bat_vol = bat_volt_f;
    if (bat_vol < 6.0){
        flag = 0;                             // прекратить работу            
        lcd.clear();
        lcd.print("Battary lvl:");
        lcd.print(bat_vol);
        lcd.setCursor(0,1);
        lcd.print("Low battery");
        digitalWrite(mosfet, 0);
        while (1); // перейти в бесконечный цикл (поможет только перезагрузка)
      }
      }
    }
  }
  
  
   //service_mode();  // раскомментировать для отладки кнопок

 //---отработка нажатия SET и изменение режимов---
  
    if (set_state && !set_flag){
      set_flag = 1;
      if (mode == 1) resist_set = millis();
      settings_flag = 1;
    }
      
    if (!set_state && set_flag) {  // если нажали-отпустили
      
      if (millis() - resist_set>1000 && mode == 1) mode = 2;
      else{
        if (change_m_flag ==1){
        mode++;
        if (mode > 1) mode =0;}
      }
      set_flag = 0;
      mode_flag = 1;
      change_m_flag = 1;
            
    }
    // ----------------------отработка нажатия SET и изменение режимов---------------------------

    // ------------------режим ВАРИВОЛЬТ-------------------
    if (mode == 0 && !vape_state && settings_flag) {
      if (mode_flag) {                    
        mode_flag = 0;
        lcd.clear();
        lcd.print("Varivolt:");
        lcd.setCursor(0,1);
        lcd.print(volts);
      }
      //---------кнопка ВВЕРХ--------
      if (up_state && !up_flag) {
        volts += 0.1;
        up_time = millis();
        volts = min(volts, bat_vol);  // ограничение сверху на текущий заряд акума
        up_flag = 1;
        lcd.setCursor(0,1);
        lcd.print(volts);
        lcd.print("V");
      }
      if (up_state && up_flag && (millis()-up_time)>550) {
        volts += 1.0;
        up_time = millis();
        volts = min(volts, bat_vol);  // ограничение сверху на текущий заряд акума
        up_flag = 1;
        lcd.setCursor(0,1);
        lcd.print(volts);
        lcd.print("V");
      }
      if (!up_state && up_flag) {
        up_flag = 0;
        change_v_flag = 1;
      }
      //---------кнопка ВВЕРХ--------

      //---------кнопка ВНИЗ--------
      if (down_state && !down_flag) {
        volts -= 0.1;
        down_time = millis();
        volts = max(volts, 0);
        down_flag = 1;
        lcd.setCursor(0,1);
        lcd.print(volts);
        lcd.print("V");
      }
      if (down_state && down_flag && (millis()-down_time)>550) {
        volts -= 1.0;
        down_time = millis();
        volts = max(volts, 0);
        down_flag = 1;
        lcd.setCursor(0,1);
        lcd.print(volts);
        lcd.print("V");
      }
      if (!down_state && down_flag) {
        down_flag = 0;
        change_v_flag = 1;
        
      }
      //---------кнопка ВНИЗ--------
       // отобразить на дисплее
    }
    // ------------------режим ВАРИВОЛЬТ-------------------


    // ------------------режим ВАРИВАТТ-------------------
    if (mode == 1 && !vape_state && settings_flag) {
      if (mode_flag) { // приветствие
        resist_set = millis();
        mode_flag = 0;
        lcd.clear();
        lcd.print("Variwatt:");
        lcd.setCursor(0,1);
        lcd.print(watts);
        lcd.print("W");
      }
      //---------кнопка ВВЕРХ--------
      if (up_state && !up_flag) {
        up_time = millis();
        watts += 1;
        byte maxW = (sq((float)bat_vol)) / ohms;
        watts = min(watts, maxW);               // ограничение сверху на текущий заряд акума и сопротивление
        up_flag = 1;
        lcd.setCursor(0,1);
        lcd.print(watts);
        lcd.print("W");
      }
      if (up_state && up_flag && (millis()-up_time)>550){
        watts += 10;
        up_time = millis();
        byte maxW = (sq((float)bat_vol)) / ohms;
        watts = min(watts, maxW);               // ограничение сверху на текущий заряд акума и сопротивление
        up_flag = 1;
        lcd.setCursor(0,1);
        lcd.print(watts);
        lcd.print("W");
      }
      if (!up_state && up_flag) {
        up_flag = 0;
        change_w_flag = 1;
      }
      //---------кнопка ВВЕРХ--------

      //---------кнопка ВНИЗ--------
      if (down_state && !down_flag) {
        watts -= 1;
        down_time = millis();
        watts = max(watts, 0);
        down_flag = 1;
        lcd.setCursor(0,1);
        lcd.print(watts);
        lcd.print("W");
      }
      if (down_state && down_flag && (millis()-down_time)>550){
        watts -= 10;
        down_time = millis();
        watts = max(watts, 0);
        down_flag = 1;
        lcd.setCursor(0,1);
        lcd.print(watts);
        lcd.print("W");
      }
      if (!down_state && down_flag) {
        down_flag = 0;
        change_w_flag = 1;
      }
      //---------кнопка ВНИЗ--------
              // отобразить на дисплее
    }
    // ------------------режим ВАРИВАТТ--------------

    // ----------режим установки сопротивления-----------
    if (mode == 2 && !vape_state && settings_flag) {
      if (mode_flag) {                     // приветствие
        mode_flag = 0;
        lcd.clear();
        lcd.print("Resistance:");
        lcd.setCursor(0,1);
        lcd.print (ohms);  
        lcd.write(byte(4));
        
      }
      //---------кнопка ВВЕРХ--------
      if (up_state && !up_flag) {
        ohms += 0.05;
        up_time = millis();
        ohms = min(ohms, 3);
        up_flag = 1;
        lcd.setCursor(0,1);
        lcd.print (ohms); 
        lcd.write(byte(4)); 
      }
      if (up_state && up_flag && (millis()-up_time)>550) {
        ohms += 0.25;
        up_time = millis();
        ohms = min(ohms, 3);
        up_flag = 1;
        lcd.setCursor(0,1);
        lcd.print (ohms);  
        lcd.write(byte(4));
      }
      
      if (!up_state && up_flag) {
        up_flag = 0;
        change_o_flag = 1;
      }
      //---------кнопка ВВЕРХ--------

      //---------кнопка ВНИЗ--------
      if (down_state && !down_flag) {
        ohms -= 0.05;
        down_time = millis();
        ohms = max(ohms, 0);
        down_flag = 1;
        lcd.setCursor(0,1);
        lcd.print (ohms);  
        lcd.write(byte(4));
      }
      if (down_state && down_flag && (millis()-down_time)>550) {
        ohms -= 0.25;
        down_time = millis();
        ohms = max(ohms, 0);
        down_flag = 1;
        lcd.setCursor(0,1);
        lcd.print (ohms); 
        lcd.write(byte(4)); 
      }
      if (!down_state && down_flag) {
        down_flag = 0;
        change_o_flag = 1;
      }
      //---------кнопка ВНИЗ--------
            // отобразить на дисплее
    }
    // ----------режим установки сопротивления-----------

    //---------отработка нажатия кнопки парения-----------
    if (vape_state) {
      

      if (!vape_flag) {
        i=0;
        j=0;
        vape_flag = 1;
        vape_mode = 1;            // первичное нажатие
        delay(20);                // анти дребезг (сделал по-тупому, лень)
        vape_press = millis();    // первичное нажатие
      }

      if (vape_release_count == 1 && !vape_release_check) {
        vape_release_check=1;
        i=0;
        j=0;
        vape_mode = 2;               // двойное нажатие
        delay(20);                   // анти дребезг (сделал по-тупому, лень)
      }
   
      if (millis() - vape_press > vape_threshold * 1000) {  // "таймер затяжки"
        vape_mode = 0;
        digitalWrite(mosfet, 0);
      }
      
      if (((millis() - vape_press) % (500/vape_mode))<10){
          if (i==0) lcd.clear();
          lcd.setCursor((i - 1 + 16)%16,0);
          lcd.write(' ');
          lcd.setCursor((i + 16)%16,0);
          lcd.write(byte(0));
          lcd.setCursor((i + 1 + 16)%16,0);
          lcd.write(byte(1));
          lcd.setCursor((i - 1 + 16)%16,1);
          lcd.write(' ');
          lcd.setCursor((i + 16)%16,1);
          lcd.write(byte(2));
          lcd.setCursor((i + 1 + 16)%16,1);
          lcd.write(byte(3));
          i++;
         }
         
         
      if (vape_mode == 1) {                                           // обычный режим парения
        
        
                              // мигать медленно
        if (mode == 0) {                                              // если ВАРИВОЛЬТ
          PWM = (float)volts / bat_vol * 1024;                     // считаем значение для ШИМ сигнала
          if (PWM > 1023) PWM = 1023;                                 // ограничил PWM "по тупому", потому что constrain сука не работает!
          PWM_f = PWM_filter_k * PWM + (1 - PWM_filter_k) * PWM_old;  // фильтруем
          PWM_old = PWM_f;                                            // фильтруем
        }
        //analogWrite(mosfet, PWM_f*255/1024);
        
        if (mode > 0) {                                              // если ВАРИВat
          PWM = (float)sqrt(watts * ohms) / bat_vol * 1024;                     // считаем значение для ШИМ сигнала
          if (PWM > 1023) PWM = 1023;                                 // ограничил PWM "по тупому", потому что constrain сука не работает!
          PWM_f = PWM_filter_k * PWM + (1 - PWM_filter_k) * PWM_old;  // фильтруем
          PWM_old = PWM_f;                                            // фильтруем
        }
        //analogWrite(mosfet, PWM_f*255/1024);
        Timer1.pwm(mosfet, PWM_f);          
      }
      if (vape_mode == 2 && turbo_mode) {                             // турбо режим парения (если включен)
        
        digitalWrite(mosfet, 1);                                      // херачить на полную мощность
      }
      settings_flag = 0;
      vape_btt = 1;
    }
    //else digitalWrite(mosfet, 0);

    if (!vape_state && vape_btt) {  // если кнопка ПАРИТЬ отпущена
      if (millis() - vape_press > 180) {
        vape_release_count = 0;
        vape_mode = 0;
        vape_flag = 0;
      }
      vape_btt = 0;
      if (vape_mode == 1) {
        vape_release_check=0;
        vape_release_count = 1;
        vape_press = millis();
      }
      
      digitalWrite(mosfet, 0);
      
      mode_flag = 0;

      // если есть изменения в настройках, записать в память
      if (change_v_flag) {
        EEPROM.writeInt(0, 100*volts);
        change_v_flag = 0;
      }
      if (change_w_flag) {
        EEPROM.writeInt(2, 100*watts);
        change_w_flag = 0;
      }
      if (change_o_flag) {
        EEPROM.writeFloat(6, ohms);
        change_o_flag = 0;
      }
      if (change_m_flag) {
        EEPROM.writeInt(4, mode);
        change_m_flag = 0;
      }
      // если есть изменения в настройках, записать в память
    }

    //---------отработка нажатия кнопки парения-----------
  

} // конец loop

void service_mode() {
  if (set_state && !set_flag) {
    set_flag = 1;
    Serial.println("SET pressed");
  }
  if (!set_state && set_flag) {
    set_flag = 0;
    Serial.println("SET released");
  }
  if (up_state && !up_flag) {
    up_flag = 1;
    Serial.println("UP pressed");
  }
  if (!up_state && up_flag) {
    up_flag = 0;
    Serial.println("UP released");
  }
  if (down_state && !down_flag) {
    down_flag = 1;
    Serial.println("DOWN pressed");
  }
  if (!down_state && down_flag) {
    down_flag = 0;
    Serial.println("DOWN released");
  }
  if (vape_state && !vape_flag) {
    vape_flag = 1;
    Serial.println("VAPE pressed");
  }
  if (!vape_state && vape_flag) {
    vape_flag = 0;
    Serial.println("VAPE released");
  }
}


void calibration() {
  //--------калибровка----------
  for (byte i = 0; i < 7; i++) EEPROM.writeInt(i, 0);          // чистим EEPROM для своих нужд
  my_vcc_const = 1.1;
  Serial.print("Real VCC is: "); Serial.println(readVcc());     // общаемся с пользователем
  Serial.println("Write your VCC (in millivolts)");
  while (Serial.available() == 0); int Vcc = Serial.parseInt(); // напряжение от пользователя
  float real_const = (float)1.1 * Vcc / readVcc();              // расчёт константы
  Serial.print("New voltage constant: "); Serial.println(real_const, 3);
  EEPROM.writeFloat(8, real_const);                             // запись в EEPROM
  while (1); // уйти в бесконечный цикл
  //------конец калибровки-------
}








long readVcc() { //функция чтения внутреннего опорного напряжения, универсальная (для всех ардуин)
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA, ADSC)); // measuring
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both
  long result = (high << 8) | low;

  result = my_vcc_const * 1000 / result; // расчёт реального VCC
  return result; // возвращает VCC
}


//moi simbol



void symbols (){
    byte slesh1[8]=
  {
  0b11100,
  0b01110,
  0b00111,
  0b00011,
  0b00001,
  0b00000,
  0b00000,
  0b00000
  };
  
  byte slesh2[8]=
  {
  0b00000,
  0b00000,
  0b00000,
  0b10000,
  0b11000,
  0b11100,
  0b01110,
  0b00111
  };
  
  byte slesh3[8]=
  {
  0b00000,
  0b00000,
  0b00000,
  0b00001,
  0b00011,
  0b00111,
  0b01110,
  0b11100
  };
  
  byte slesh4[8]=
  {
  0b00111,
  0b01110,
  0b11100,
  0b11000,
  0b10000,
  0b00000,
  0b00000,
  0b00000
  };

  byte Omega[8]=
  {
  0b01110,
  0b11011,
  0b10001,
  0b10001,
  0b11011,
  0b01010,
  0b11011,
  0b00000
  };


  lcd.createChar(0, slesh1);
  lcd.createChar(1, slesh2);
  lcd.createChar(2, slesh3);
  lcd.createChar(3, slesh4);
  lcd.createChar(4, Omega);

}

void push(){
  if (!all_bat) return;
  if (vape_state || set_state) {
    push1=0;
    push2=0;
    push3=0;
    push4=0;
    push5=0;
  }
  if (up_state && !down_state && !set_state && !vape_state && !push1){
    push1=1;
  }
  if (!up_state && down_state && !set_state && !vape_state && push1 && !push2){
    push2=1;
  }
  else if (up_state && !up_flag && (push1 && !push2)){
    push1=0;
    push2=0;
    push3=0;
    push4=0;
    push5=0;
  }
  if (up_state && !down_state && !set_state && !vape_state && push1 && push2 && push3 && !push4){
    push4=1;
  } else if (down_state && !up_flag && (push3 && !push4)){
    push1=0;
    push2=0;
    push3=0;
    push4=0;
    push5=0;
  }
  if (up_state && !down_state && !set_state && !vape_state && push1 && push2 && !push3){
    push3=1;
  } else if (down_state && !down_flag && (push2 && !push3)){
    push1=0;
    push2=0;
    push3=0;
    push4=0;
    push5=0;
  }
  if (!up_state && down_state && !set_state && !vape_state && push1 && push2 && push3 && push4 &&!push5){
    lcd.clear();
    lcd.print(" dripka - pipka ");
    
  } else if (down_state && !up_flag&& (push4)){
    push1=0;
    push2=0;
    push3=0;
    push4=0;
    push5=0;
  }
  
  
}

void pash(){
  if (!all_bat) return;
  if (vape_state) {
    pash1=0;
    pash2=0;
    pash3=0;
    pash4=0;
    pash5=0;
    Serial.println(11);
  }
  if (set_state && !down_state && !up_state && !vape_state && !pash1){
    pash1=1;
    Serial.println(1);
  }
  if (!up_state && down_state && !set_state && !vape_state && pash1 && !pash2){
    pash2=1;
    Serial.println(2);
  }
  else if (((up_state && !up_flag) || (set_state && !set_flag))  && (pash1 && !pash2)){
    pash1=0;
    pash2=0;
    pash3=0;
    pash4=0;
    pash5=0;
    Serial.println(012);
  }
  if (up_state && !down_state && !set_state && !vape_state && pash1 && pash2 && !pash3){
    pash3=1;
    Serial.println(3);
  } else if ( ((down_state && !down_flag) || (set_state && !set_flag)) && (pash2 && !pash3)){
    pash1=0;
    pash2=0;
    pash3=0;
    pash4=0;
    pash5=0;
    Serial.println(13);
  }
  if (!up_state && !down_state && set_state && !vape_state && pash1 && pash2 && pash3 && !pash4){
    pash4=1;
    Serial.print(4);
    lcd.clear();
    lcd.print(" dripka - pipka ");
  } else if ( ((down_state && !down_flag) || (up_state && !up_flag)) && (pash3 && !pash4)){
    pash1=0;
    pash2=0;
    pash3=0;
    pash4=0;
    pash5=0;
    Serial.println(14);
  }
 
  
  
}
