//Пин, к которому подключен пьезодинамик.
int piezoPin = 3;

struct {
  int len = 0;
  int cur = 0;
  int *times;
  int *tones;
} piezo;

int tones_1[] = {1000,3000,1000,2000,1000,3000,1000,2000};
int times_1[] = {500,500,500,500,500,500,500,500};

int tones_2[] = {500,1000,1500};
int times_2[] = {500,500,500};

int tones_3[] = {2000,1500,1000,500};
int times_3[] = {500,500,500,500};
void setup() {
 Serial.begin(115200);
 
  
//   piezo_set(8,times_1,tones_1);
//   piezo_set(3,times_2,tones_2);
   piezo_set(4,times_3,tones_3);
}

void loop() {
 

  piezo_tick();


}

void piezo_set(int len, int times[], int tones[]){
  piezo.len = len;
  piezo.times = times;
  piezo.tones = tones;
  piezo.cur = 0;

  
}

void piezo_tick(){
  static uint32_t piezo_timer;
  if (piezo.cur < piezo.len && millis() - piezo_timer > piezo.times[piezo.cur]) {
    
    piezo_timer = millis();
    tone(D8, piezo.tones[piezo.cur], piezo.times[piezo.cur]);
    piezo.cur++;
//  piezo.times& = piezo.times[1];
  }}
