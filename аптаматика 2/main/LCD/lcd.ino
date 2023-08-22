//#define Cursor(A, B) setCursor(B, A);  
#include <LCD_1602_RUS.h>
#include <Adafruit_BMP085.h>
LCD_1602_RUS lcd(0x27, 16, 2);
byte stop_name = 0;
Adafruit_BMP085 bmp;
uint8_t char_T_up[] = {0x00,0x04,0x0E,0x15,0x04,0x04,0x04,0x00};
uint8_t char_T_down[] = {0x00,0x04,0x04,0x04,0x15,0x0E,0x04,0x00};
uint8_t char_T_mid[] = {0x00,0x00,0x00,0x1F,0x1F,0x00,0x00,0x00};
uint8_t char_T_cube[] = {0x00,0x1B,0x0A,0x11,0x11,0x11,0x11,0x1F};
uint8_t char_T_jets[] = {0x18,0x1C,0x06,0x03,0x03,0x01,0x01,0x00};

/////////////////////////////////////////////////////////////////////////////////////////////////////////

#define BOT_TOKEN "5629897246:AAH4sN5jS9feWW9ckIY5kZfeHC9u1EyO8IY"
#define MY_TG_ID "1723464345"

#define PLOT_CUBE_SIZE 100
#define PLOT_DIF_SIZE 200
#define PLOT_JETS_SIZE 100

#define piezoPin D5

#define wifi_Button D3
#define braga_button D1

//#define V_pin A0
//#define V_coef 0.5
//#define V_average_size 50




#define RELE D0

#define red D6
#define green D8
#define blue D7

#define ten_relay_pin D66
int ten_relay_PWM = 50;
float ten_relay_PERIOD = 1.0;
//PWMrelay ten_relay(ten_relay_pin);

int process_number = -1;





bool flag_red = false;
bool flag_green = false;
bool flag_blue = false;



int16_t arr_T_cube[1][PLOT_CUBE_SIZE];
const char *names_cube[] = {"Температура в кубе",};
const char *names_cube_B[] = {"Термометр №1",};


int16_t arr_T_dif[4][PLOT_DIF_SIZE];
const char *names_dif[] = {"Температура в середине","Температура вверху","Температура внизу", "Требуемая температура",};

int16_t arr_T_dif_B[1][PLOT_DIF_SIZE];
const char *names_dif_B[] = {"Термометр №2",};


int16_t arr_T_jets[1][PLOT_JETS_SIZE];
const char *names_jets[] = {"Температура на выходе",};
const char *names_jets_B[] = {"Термометр №3",};

bool process_finished = false;

bool flag_head = false;
bool flag_boost = false;
float head_selection = 0;
int current_selection = 0;
int goal_selection = 0;
String time_head_selection = "00:00:00";

int STABILIZATION_TIME = 15;
String tmr_clapan_was_closed = "00:00";

float PWM_OPEN = 0;
float PWM_CLOSE = 0;
bool PWM_MODE = true;
bool PWM_FLAG_TEORETICAL_DUTY = false;
bool PWM_DUTY_CHANGED = false;
float PWM_DECREMENT = 0.15;
int PWM_PERIOD = 8;
float PWM_DUTY = 0;
int PWM_CURRENT_SELECTION = 0;
int PWM_TEORETICAL_DUTY = 0;
int PWM_GOAL_ML = 900;

bool S_T_1 = false;
bool S_T_2 = false;
bool S_T_3 = false;

bool braga_button_bool;

float deviation_stop = 0.15;
float deviation_start = 0.05;
float T_goal_col;
float T_cube;
float T_col_down;
float T_col_mid;
float T_col_up;
float T_mid_down_dif = 0;
float T_jets;



void setup()
{
  Serial.begin(9600);
  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }
  T_cube=22.15;
  T_col_up=13.25;
  T_col_mid=98.75;
  T_col_down=77.11;
  T_jets=44.33;
 // Wire.begin(RX, TX);
   pinMode(D3,OUTPUT);
  lcd.init();               
  lcd.backlight();
  lcd.createChar(0, char_T_up);
  lcd.createChar(1, char_T_down);
  lcd.createChar(2, char_T_mid);
  lcd.createChar(3, char_T_cube);
  lcd.createChar(4, char_T_jets);
  lcd.clear();
//  lcd.home();
//  lcd.write(0);
//  lcd.write(1);
//  lcd.write(2);
//  lcd.write(3);
//  lcd.write(4);
}

void loop()
{

T_col_mid=bmp.readTemperature();
T_col_down=bmp.readPressure()/133.333;
  LCD_Temp();
   digitalWrite(D3, LOW);
 delay(500);
 digitalWrite(D3, 1);
 delay(500);

}


//void LCD_start_process(){
//  static uint32_t tmr_LCD_start;
//  static byte i = 0;
//  if (millis() - tmr_LCD_start > 1000){
//    tmr_LCD_start = millis();
//    
//    if (i < stop_name){
//      i++;
//      lcd.clear();
//      lcd.setCursor(5, 0);
//      lcd.print("Старт"); // 2 0
//    }
//    else if (i == stop_name){
//      i++;
//      lcd.clear();
//      lcd.setCursor(0, 0);
//      lcd.write(2);
//      lcd.print("="); 
//      lcd.print(T_col_mid);
//      lcd.print(" "); 
//      lcd.write(1);
//      lcd.print("="); 
//      lcd.print(T_col_down);
//      lcd.setCursor(0, 1);
//      lcd.write(3);
//      lcd.print("="); 
//      lcd.print(T_cube);
//      lcd.print(" "); 
//      lcd.write(4);
//      lcd.print("="); 
//      lcd.print(T_jets);
//    }
//    else{
//      lcd.setCursor(2, 0);
//      lcd.print(T_col_mid);
//      lcd.setCursor(10, 0);
//      lcd.print(T_col_down);
//      lcd.setCursor(2, 1);
//      lcd.print(T_cube);
//      lcd.setCursor(10, 1);
//      lcd.print(T_jets);
//    }
//  }
//}

void LCD_Temp(){
  static uint32_t tmr_LCD_start;
  static byte i = 0;
  if (millis() - tmr_LCD_start > 1000){
    tmr_LCD_start = millis();
    T_col_mid++;
    
    if (i < stop_name){
      i++;
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("Старт"); // 2 0
    }
    else if (i == stop_name){
      i++;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(2);
      lcd.print("="); 
      lcd.print(T_col_mid);
      lcd.setCursor(8, 0);
      lcd.write(1);
      lcd.print("="); 
      lcd.print(T_col_down);
      lcd.setCursor(0, 1);
      lcd.write(0);
      lcd.print("="); 
      lcd.print(T_col_up);
      lcd.setCursor(8, 1);
      lcd.write(3);
      lcd.print("="); 
      lcd.print(T_cube);
    }
    else{
      lcd.setCursor(2, 0);
      lcd.print(T_col_mid);
      lcd.setCursor(10, 0);
      lcd.print(T_col_down);
      lcd.setCursor(2, 1);
      lcd.print(T_col_up);
      lcd.setCursor(10, 1);
      lcd.print(T_cube);
    }
  }
}
