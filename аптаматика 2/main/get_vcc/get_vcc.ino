#define V_pin A0
#define V_coef 1.0
bool flag = false;
const int NUM_READ = 10; 

float runMiddleArifm(int newVal) {  // принимает новое значение
  static byte idx = 0;                // индекс
  static int valArray[NUM_READ];    // массив
  valArray[idx] = newVal;             // пишем каждый раз в новую ячейку
  if (++idx >= NUM_READ) idx = 0;     // перезаписывая самое старое значение
  int average = 0;                  // обнуляем среднее
  for (int i = 0; i < NUM_READ; i++) {
    average += valArray[i];           // суммируем
  }
  return (int)average / NUM_READ;   // возвращаем
}

////запускаем это
//// открываем «Инструменты» и жмем «Плоттер по последовательному соединению«.
//// крутим потенциометр, чтобы была идеальрная синусойда
void setup() {
Serial.begin(9600);
}
//void loop() 
//{
//Serial.println(analogRead(A0));
//
////delay(40);
//}
//y=2.3173x−1267.4310
//649 650 645 648 644 644 643 643 648 644 644 646 644 645
//237 238.7 228 235 226 225 221 222 232 224 225.4 229.5 226 227.5

void loop() 
{
  static float cur_V = 0;
  static long int max_V = 0;
  static int i=1;
  
  static uint32_t tmr_del = millis();
  static uint32_t tmrlog;
      if (millis() - tmr_del >500){
      tmr_del = millis();
      Serial.println(max_V/i);
      max_V = 0;
      i=1;
        flag = !flag;}

      if (flag){
      max_V += abs(analogRead(V_pin) - 558); 
      i++;
      
//      delay(10);
      }
      
    }
