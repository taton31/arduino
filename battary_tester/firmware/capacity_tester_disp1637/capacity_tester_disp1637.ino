#define NUM_READS 100
#include <EEPROM.h>
//---- ПИНЫ ----
byte disp_vcc=5;  //питание дисплея
byte SCLK = 4;
byte RCLK = 3;
byte DIO = 2;
//---- ПИНЫ ----

#include <TM74HC595Display.h>
#include <timerone.h>
#include "GyverButton.h"
TM74HC595Display disp(SCLK, RCLK, DIO);

unsigned char SYM[39];

byte load_pin=0; //пин нагрузки (аналоговый!!!!)

byte butt_pin=11;  //состояние кнопки
byte butt_gnd=12;  //земля кнопки
GButton btn(butt_pin);

byte relay_pin=7; //пин реле
byte relay_vin=9; //пин реле
byte relay_gnd=8; //пин реле

const float typVbg = 1.081; // 1.0 -- 1.2
float Voff = 3.0; // напряжение отсечки (для Li-ion=2.5 В, для LiPo=3.0 В)
float R = 4.0; //сопротивление нагрузки
float R_arr[3] = {5.3, 7.8, 15.6};
int R_arr_ind = 0;
float I;
float cap = 0; //начальная ёмкость
float V;
float Vcc;
float Wh = 0;
unsigned long prevMillis;
unsigned long testStart;

String cap_string;
int st = -1;
int digit = 0;

void setup() {  
//  EEPROM.begin(100);
  int Ee_1; 
  int Ee_2;
//  EEPROM.get(0, Ee_1);
//  EEPROM.get(sizeof(int), Ee_2);
//  Voff = (float) Ee_1 / 10;
  R = R_arr[R_arr_ind];
  
  pinMode(relay_pin, OUTPUT);  //пин реле как выход
  pinMode(relay_vin, OUTPUT); //земля кнопки
  pinMode(relay_gnd, OUTPUT); //земля кнопки
  digitalWrite(relay_pin, HIGH); //выключить реле (оно обратное у меня)
  digitalWrite(relay_vin, HIGH);
  digitalWrite(relay_gnd , LOW);
  
  pinMode(butt_gnd, OUTPUT); //земля кнопки
  digitalWrite(butt_gnd, 0);
  
  
  pinMode(disp_vcc, OUTPUT); //питание дисплея
  digitalWrite(disp_vcc, 1);

  btn.setTimeout(1000);
  symbols();                         // объявление пачки символов для работы дисплея

  Timer1.initialize(1500);           // установка таймера на каждые 1500 микросекунд
  Timer1.attachInterrupt(timerIsr);  // запуск таймера
  
  disp.digit4showZero(0);

  Serial.begin(9600);  //открыть порт для связи с компом
  Serial.println("Press any key to start the test...");  //Отправьте любой символ для начала теста
  while (true) {
    if (st == -1)
    {
      //HI
      disp.clear();
      disp.set(SYM[27], 3);    // вывести 0. в четвёртый порт
      disp.set(SYM[28], 2);     // вывести 1 в третий порт
//      delay(500);
    }
    else if (st == 1){
      //V
      disp.clear();
      disp.set(SYM[36], 3);    // вывести 0. в четвёртый порт
      digit = 0;
      delay(1000);
      disp.clear();
      
      int max_dig = 1;
      int a = Voff * 10;
      while(0 != a)
        {
          a=(int)a/10;
          max_dig++;
        }

      while(true){
        btn.tick();
        if (btn.isHold()) break;
        if (btn.isSingle()) Voff = change_float(Voff, digit);
        if (btn.isDouble()) digit = (digit + 1) % max_dig;
        if (btn.isTriple()) {
          st = (st + 1) % 2;
          break;
        }
    
        blink_d(Voff, digit);
      }
      
    }
    else if (st == 0){
      //R
      disp.clear();
      disp.set(SYM[33], 3);    // вывести 0. в четвёртый порт
      digit = 0;
      delay(1000);
      disp.clear();

      int max_dig = 1;
      int a = R * 10;
      while(0 != a)
        {
          a=(int)a/10;
          max_dig++;
        }

      while(true){
        btn.tick();
        if (btn.isHold()) break;
        if (btn.isSingle()) {
          R_arr_ind = (R_arr_ind + 1) % 3;
          R = R_arr[R_arr_ind];
        }
        if (btn.isDouble()) digit = (digit + 1) % max_dig;
        if (btn.isTriple()) {
          st = (st + 1) % 2;
          break;
        }
    
        blink_d(R, digit);
      }
      
    }

    
    btn.tick();
    if (btn.isHolded()) {
      disp.clear();
      disp.set(SYM[26], 3);    // вывести 0. в четвёртый порт
      disp.set(SYM[32], 2);     // вывести 1 в третий порт
      delay(500);
      break;
      }  //или нажмите кнопку, чтобы начать тест!
    else if (btn.isTriple()){
      st = (st + 1) % 2;
    }
//    else {
//      
//    }
  }
  Serial.println("Test is launched...");
  Serial.print("s");
  Serial.print(" ");
  Serial.print("V");
  Serial.print(" ");
  Serial.print("mA");
  Serial.print(" ");
  Serial.print("mAh");
  Serial.print(" ");
  Serial.print("Wh");
  Serial.print(" ");
  Serial.println("Vcc");

  Serial.println(Voff);
  Serial.println(R);
//  EEPROM.put(0, (int) (Voff * 10));
//  EEPROM.put(sizeof(int), (int) (R * 10));
//  EEPROM.put(0, 30);
//  EEPROM.put(sizeof(int), 10);

//  EEPROM.commit();                // записываем
  digitalWrite(relay_pin, LOW); //Переключить реле (замкнуть акум на нагрузку)
  testStart = millis();  //время начала теста в системе отсчёта ардуины
  prevMillis = millis();  //время первого шага
  
}

void loop() {
  Vcc = readVcc(); //хитрое считывание опорного напряжения (функция readVcc() находится ниже)
  V = (readAnalog(load_pin) * Vcc) / 1023.000; //считывание напряжения АКБ
  I = V/R; //расчет тока по закону Ома, в Амперах
  cap += I*(millis()-prevMillis)/3600000*1000; //расчет емкости АКБ в мАч
  Wh += I*V *(millis() - prevMillis)/3600000; //расчет емкости АКБ в ВтЧ
  prevMillis = millis();
  sendData(); // отправка данных
  
  if (V < Voff && V > 0.1) { //выключение нагрузки при достижении минимального напряжения
    digitalWrite(relay_pin, HIGH);  //разорвать цепь (отключить акб от нагрузки)
    Serial.println("Test is done");  //тест закончен
    
    while (2 > 1) {  //бесконечный цикл, чтобы loop() не перезапустился + моргаем результатом!
      disp.digit4(round(cap));
      delay(1000);
      disp.clear();
      delay(300);
    }
  }
}

void blink_d(float a, int digit){
  static long st = millis();
  static bool vis = false;
  
   if (millis() - st > 500){
    st = millis();
    vis = !vis;
    disp.clear();
    disp.float_dot(a, 1);
   }
   if (vis) {
    disp.set(SYM[38], digit);
   }
//   else disp.float_dot(a, 1);
   
}

void print_d(){
  static long st = millis();
  static bool vis = true;
  
   if (millis() - st > (vis) ? 10000 : 2000){ 
    st = millis();
    vis = !vis;
   }
   disp.clear();
   if (vis) {
    disp.digit4(round(cap));
   }
   else {
    disp.float_dot((float)round(V * 100) / 100, 2);
   }
}

float change_float(float a, int dig){
  int ch = a*10 / pow(10, dig);
  ch = ch % 10;
  if (ch == 9) a = a - 9 * pow (10, dig - 1);
  else a = a + pow (10, dig - 1);
  return a;
}

void sendData() {       //функция, которая посылает данные в порт
  Serial.print((millis() - testStart) / 1000);
  Serial.print(" ");
  Serial.print(V, 3);
  Serial.print(" ");
  Serial.print(I, 1);
  Serial.print(" ");
  Serial.print(cap, 0);
  Serial.print(" ");
  Serial.print(Wh, 2);
  Serial.print(" ");
  Serial.println(Vcc, 3);   
//  cap_string=String(round(cap));
//  disp_print(cap_string);
  print_d();
}

//----------Функция точного определения опорного напряжения для измерения напряжения на акуме-------
float readAnalog(int pin) {  
  // read multiple values and sort them to take the mode
  int sortedValues[NUM_READS];
  for (int i = 0; i < NUM_READS; i++) {
    delay(25);    
    int value = analogRead(pin);
    int j;
    if (value < sortedValues[0] || i == 0) {
      j = 0; //insert at first position
    }
    else {
      for (j = 1; j < i; j++) {
        if (sortedValues[j - 1] <= value && sortedValues[j] >= value) {
          // j is insert position
          break;
        }
      }
    }
    for (int k = i; k > j; k--) {
      // move all values higher than current reading up one position
      sortedValues[k] = sortedValues[k - 1];
    }
    sortedValues[j] = value; //insert current reading
  }
  //return scaled mode of 10 values
  float returnval = 0;
  for (int i = NUM_READS / 2 - 5; i < (NUM_READS / 2 + 5); i++) {
    returnval += sortedValues[i];
  }
  return returnval / 10;
}
//----------Функция точного определения опорного напряжения для измерения напряжения на акуме КОНЕЦ-------


//----------фильтр данных (для уменьшения шумов и разброса данных)-------
float readVcc() {
  // read multiple values and sort them to take the mode
  float sortedValues[NUM_READS];
  for (int i = 0; i < NUM_READS; i++) {
    float tmp = 0.0;
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    ADCSRA |= _BV(ADSC); // Start conversion
    delay(25);
    while (bit_is_set(ADCSRA, ADSC)); // measuring
    uint8_t low = ADCL; // must read ADCL first - it then locks ADCH
    uint8_t high = ADCH; // unlocks both
    tmp = (high << 8) | low;
    float value = (typVbg * 1023.0) / tmp;
    int j;
    if (value < sortedValues[0] || i == 0) {
      j = 0; //insert at first position
    }
    else {
      for (j = 1; j < i; j++) {
        if (sortedValues[j - 1] <= value && sortedValues[j] >= value) {
          // j is insert position
          break;
        }
      }
    }
    for (int k = i; k > j; k--) {
      // move all values higher than current reading up one position
      sortedValues[k] = sortedValues[k - 1];
    }
    sortedValues[j] = value; //insert current reading
  }
  //return scaled mode of 10 values
  float returnval = 0;
  for (int i = NUM_READS / 2 - 5; i < (NUM_READS / 2 + 5); i++) {
    returnval += sortedValues[i];
  }
  return returnval / 10;
}
//----------фильтр данных (для уменьшения шумов и разброса данных) КОНЕЦ-------
//
//void disp_print(String x) {
//  disp.point(POINT_OFF);
//  switch (x.length()) {         //кароч тут измеряется длина строки и соотвествено выводится всё на дисплей
//  case 1:
//    disp.display(0,18);
//    disp.display(1,18);
//    disp.display(2,18);
//    disp.display(3,x[0]- '0');
//    break;
//  case 2:
//    disp.display(0,18);
//    disp.display(1,18);
//    disp.display(2,x[0]- '0');
//    disp.display(3,x[1]- '0');
//    break;
//  case 3:
//    disp.display(0,18);
//    disp.display(1,x[0]- '0');
//    disp.display(2,x[1]- '0');
//    disp.display(3,x[2]- '0');
//    break;
//  }
//}


void symbols() {
  // обычные
  SYM[0] = 0xC0; //0
  SYM[1] = 0xF9; //1
  SYM[2] = 0xA4; //2
  SYM[3] = 0xB0; //3
  SYM[4] = 0x99; //4
  SYM[5] = 0x92; //5
  SYM[6] = 0x82; //6
  SYM[7] = 0xF8; //7
  SYM[8] = 0x80; //8
  SYM[9] = 0x90; //9

  // с точкой
  SYM[10] = 0b01000000; //0.
  SYM[11] = 0b01111001; //1.
  SYM[12] = 0b00100100; //2.
  SYM[13] = 0b00110000; //3.
  SYM[14] = 0b00011001; //4.
  SYM[15] = 0b00010010; //5.
  SYM[16] = 0b00000010; //6.
  SYM[17] = 0b01111000; //7.
  SYM[18] = 0b00000000; //8.
  SYM[19] = 0b00010000; //9.

  // буквы
  SYM[20] = 0x88; //A
  SYM[21] = 0x83; //b
  SYM[22] = 0xC6; //C
  SYM[23] = 0xA1; //d
  SYM[24] = 0x86; //E
  SYM[25] = 0x8E; //F
  SYM[26] = 0xC2; //G
  SYM[27] = 0x89; //H
  SYM[28] = 0xF9; //I
  SYM[29] = 0xF1; //J
  SYM[30] = 0xC3; //L
  SYM[31] = 0xA9; //n
  SYM[32] = 0xC0; //O
  SYM[33] = 0x8C; //P
  SYM[34] = 0x98; //q
  SYM[35] = 0x92; //S
  SYM[36] = 0xC1; //U
  SYM[37] = 0x91; //Y
  SYM[38] = 0b11110111; // _
}

void timerIsr() {   // прерывание таймера
  disp.timerIsr();  // пнуть дисплей
}
