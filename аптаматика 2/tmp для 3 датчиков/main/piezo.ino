
void piezo_set(int len, int tones[], int times[]){
  piezo.len = len;
  piezo.times = times;
  piezo.tones = tones;
  piezo.cur = 0;
}

void piezo_tick(){
  if (!piezo_on) return;
  
  static uint32_t piezo_timer;
  if (piezo.cur < piezo.len && millis() - piezo_timer > piezo.times[piezo.cur]) {
    piezo_timer = millis();
    tone(piezoPin, piezo.tones[piezo.cur], piezo.times[piezo.cur]);
    piezo.cur++;
  }}
