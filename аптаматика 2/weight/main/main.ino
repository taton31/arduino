#include <GyverPortal.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include "uMQTTBroker.h"
#include <HX711.h>

#define scale_1 12
#define scale_2 14 
float knownWeight = 100.0; // заранее известный вес, глобальная переменная
float calibrationValue = 0; // переменная для сохранения калибровочного коэффициента в EEPROM
HX711 scale; // создание экземпляра HX711
bool flag_calib = false;
uint32_t tmr_4;
uint32_t tmr_2;

GyverPortal portal;
struct LoginPass {
  char ssid[20];
  char pass[20];
};
LoginPass lp;
float PWM_DUTY=0;
int calibration_goal;

float ro = 1.267;
int callibration_current = 0;
bool flag_success = false;
bool flag_test = false;
float weight = 0;
const int time_test = 12;//5 минут

class myMQTTBroker: public uMQTTBroker
{
public:
    virtual bool onConnect(IPAddress addr, uint16_t client_count) {
      Serial.println(addr.toString()+" connected");
      return true;
    }

    virtual void onDisconnect(IPAddress addr, String client_id) {
      Serial.println(addr.toString()+" ("+client_id+") disconnected");
    }

    virtual bool onAuth(String username, String password, String client_id) {
      Serial.println("Username/Password/ClientId: "+username+"/"+password+"/"+client_id);
      return true;
    }
    
    virtual void onData(String topic, const char *data, uint32_t length) {
      if (topic == "start"){
        flag_success = true;
        char data_str[length+1];
      os_memcpy(data_str, data, length);
      data_str[length] = '\0';
      portal.log.println("start");
      char * pch = strtok (data_str,";");
      calibration_goal = atoi(pch);
      pch = strtok (NULL, ";");
      PWM_DUTY = atof(pch);
      portal.log.println(calibration_goal);
      portal.log.println(PWM_DUTY);

        return;
      }
      
      char data_str[length+1];
      os_memcpy(data_str, data, length);
      data_str[length] = '\0';
      
      portal.log.println(strtok(data_str, ";"));
      portal.log.println(strtok(data_str, ";"));
      //printClients();
    }

    // Sample for the usage of the client info methods

    virtual void printClients() {
      for (int i = 0; i < getClientCount(); i++) {
        IPAddress addr;
        String client_id;
         
        getClientAddr(i, addr);
        getClientId(i, client_id);
        Serial.println("Client "+client_id+" on addr: "+addr.toString());
      }
    }
};
myMQTTBroker myBroker;

void setup()
{
//  flag_test = true; //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Serial.begin(115200);
  WiFi_start();
  Serial.println("Starting MQTT broker");
  myBroker.init();
  myBroker.subscribe("start");

  Serial.println("start auto portal");
  portal.attachBuild(build);
  portal.start(WIFI_AP);
  portal.attach(action);
 
  portal.enableOTA();
  portal.log.start(64);
  scale_start();
  Serial.println("PORTAL_STARTED");
  tmr_2 = millis();
}



void loop()
{
//  APM_test();
  
  static uint32_t tmr_1 = millis();
  static uint32_t tmr_3 = millis();
  
  if (millis() - tmr_2 < time_test * 60 * 1000){ 
    if (flag_test) {
      get_weight();
      delay(0);
    }
  }
  else {
    tmr_2 = millis();
    if (flag_test) {
      delay(0);
      float x = callibration_current / PWM_DUTY;
      myBroker.publish("data", (String) callibration_current + (String)";" + (String) (calibration_goal / x) + (String) ";");
      flag_test = false;
    }
  }

  

  if (!flag_test){
    if (millis() - tmr_3 > 200){ 
      tmr_3 = millis();
      delay(0);
      callibration_current=0;
      weight = scale.get_units();
    }
  }
  // wait a second
  if (flag_success && millis() - tmr_1 > 500){
    tmr_1 = millis();
    tmr_2 = millis();
    delay(0);
    myBroker.publish("success", (String)"0");
    flag_success = false;
    flag_test = true;
    scale.tare();
    delay(0);
  }
  delay(0);
  calib_tick();
  portal.tick();
}
