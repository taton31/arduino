 #include <GyverPortal.h>
#include <EEPROM.h>
#include <microDS18B20.h>
#include <PWMrelay.h>
#include <FastBot.h>
#include <LCD_1602_RUS.h>
#include <Adafruit_BMP085.h>
#include <FS.h>
#include <LittleFS.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////arduino pins
//#define wifi_Button D3
//#define braga_button D1
#define RELE D6


//////////////////////////////////////////////////////////////////////////////////////////////////////////piezo
#define piezoPin D8
struct {
  int len = 0;
  int cur = 0;
  int *times;
  int *tones;
} piezo;

bool piezo_on = false;
int tones_1[] = {1000,3000,1000,2000,1000,3000,1000,2000};
int times_1[] = {500,500,500,500,500,500,500,500};

int tones_2[] = {500,1000,1500};
int times_2[] = {500,500,500};

int tones_3[] = {2000,1500,1000,500};
int times_3[] = {500,500,500,500};


//////////////////////////////////////////////////////////////////////////////////////////////////////////telegram
#define BOT_TOKEN "5629897246:AAH4sN5jS9feWW9ckIY5kZfeHC9u1EyO8IY"
#define MY_TG_ID "1723464345"
FastBot bot(BOT_TOKEN);


//////////////////////////////////////////////////////////////////////////////////////////////////////////GyverPortal
GyverPortal portal;
#define PLOT_CUBE_SIZE 100
#define PLOT_DIF_SIZE 200
#define PLOT_JETS_SIZE 100

int16_t arr_T_cube[1][PLOT_CUBE_SIZE];
const char *names_cube[] = {"Температура в кубе",};
const char *names_cube_B[] = {"Термометр №1",};

int16_t arr_T_dif[3][PLOT_DIF_SIZE];
const char *names_dif[] = {"Температура в середине","Температура внизу", "Требуемая температура",};

int16_t arr_T_dif_B[1][PLOT_DIF_SIZE];
const char *names_dif_B[] = {"Термометр №2",};

int16_t arr_T_jets[1][PLOT_JETS_SIZE];
const char *names_jets[] = {"Температура на выходе",};
const char *names_jets_B[] = {"Термометр №3",};

GPcolor valCol;


//////////////////////////////////////////////////////////////////////////////////////////////////////////V_scan
//#define V_pin A0
//#define V_coef 0.5
//#define V_average_size 50


//////////////////////////////////////////////////////////////////////////////////////////////////////////ten
#define ten_relay_pin D3
int ten_relay_PWM = 0;
float ten_relay_PERIOD = 0.5;
PWMrelay ten_relay(ten_relay_pin, true);


//////////////////////////////////////////////////////////////////////////////////////////////////////////lcd
LCD_1602_RUS lcd(0x27, 16, 2);
byte stop_name = 0;

uint8_t char_T_up[] = {0x00,0x04,0x0E,0x15,0x04,0x04,0x04,0x00};
uint8_t char_T_down[] = {0x00,0x04,0x04,0x04,0x15,0x0E,0x04,0x00};
uint8_t char_T_mid[] = {0x00,0x00,0x00,0x1F,0x1F,0x00,0x00,0x00};
uint8_t char_T_cube[] = {0x00,0x1B,0x0A,0x11,0x11,0x11,0x11,0x1F};
uint8_t char_T_jets[] = {0x18,0x1C,0x06,0x03,0x03,0x01,0x01,0x00};


//////////////////////////////////////////////////////////////////////////////////////////////////////////ds18b20
//MicroDS18B20<D7> sensor1; 
 
#define ds18b20_count 3
MicroDS18B20<D7, DS_ADDR_MODE> sensor[ds18b20_count];
uint8_t ds18b20_addr[][8] = {
{0x28, 0xFF, 0x64, 0x18, 0x22, 0x4B, 0xE3, 0x0A}, //green
{0x28, 0xFF, 0x64, 0x18, 0x2D, 0xBC, 0x1A, 0x4A}, //blue
{0x28, 0xFF, 0x64, 0x18, 0x2D, 0xBD, 0xDC, 0x99}, //black
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////bmp
Adafruit_BMP085 bmp;
float bmp180_pressure = 0;
float bmp180_pressure_start = 0;
float bmp180_temp = 0;

float k_average_pres = 0.1; 
float k_average_temp = 0.1; 

bool flag_bmp_is_online;


//////////////////////////////////////////////////////////////////////////////////////////////////////////Tech
int process_number = -1;
bool process_finished_ = false;

bool flag_head = false;
bool flag_boost = false;
float head_selection = 0;
int current_selection = 0;
int goal_selection = 0;
String time_head_selection = "00:00:00";

int STABILIZATION_TIME = 15;
String tmr_clapan_was_closed = "00:00";

float deviation_stop = 0.15;
float deviation_start = 0.07;
float T_goal_col;
float T_cube;
float T_col_down;
float T_col_mid;
float T_col_up;
float T_mid_down_dif = 0;
float T_jets;

  //////////////////////////////////////////////////////////////////////////////////////////////////////////PWM
  float PWM_OPEN = 0;
  float PWM_CLOSE = 0;
  bool PWM_MODE = true;
  bool PWM_FLAG_TEORETICAL_DUTY = false;
  bool PWM_DUTY_CHANGED = false;
  float PWM_DECREMENT = 10.0;
  int PWM_PERIOD = 8;
  float PWM_DUTY = 0;
  int PWM_CURRENT_SELECTION = 0;
  int PWM_TEORETICAL_DUTY = 0;
  int PWM_GOAL_ML = 900;

  //////////////////////////////////////////////////////////////////////////////////////////////////////////braga
  bool S_T_1 = false;
  bool S_T_2 = false;
  bool S_T_3 = false;
  bool braga_button_bool;


////////////////////////////////////////////////////////////////////////////////////////////////////////////WiFi
struct LoginPass {
  char ssid[20];
  char pass[20];
};
LoginPass lp;


////////////////////////////////////////////////////////////////////////////////////////////////////////////LittleFS
#define Log_file_dir "/Log.txt"
#define Log_file "Log.txt"
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3*3600);


void setup() {

  
  pins_start();

  START_FS();
  
  BMP180_start();
  
  LCD_start();
  
  ds18b20_start();
  
  //V_scan_start();

  WiFi_start();
  
  Portal_start();
  
  bot_start();
}






void loop() {
  LCD_Temp();
  
  if (braga_button_bool){
    braga_loop();
  }
  else if (!braga_button_bool){
    read_auto_temp();
    get_bmp180_data();
    
    if (process_number == 0){
      head_process();  
    }
    else if (process_number == 1){
      body_process();
    }
    else if (process_number == 2){
      body_process_delta();
    }
    else if (process_number == 9){
      boost_process();
    }
    else if (process_number == -1){
      start_process();
    }
    else if (process_number == -99){
      wait_process();
    }

    WARNINGS();
  }
    
  update_graph_data();
  
  
  portal.tick();
  bot.tick();
  log_tick();
  relay_tick();
  piezo_tick();
}
    
