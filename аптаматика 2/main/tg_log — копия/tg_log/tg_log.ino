
void setup() {
 Serial.begin(115200);
 pinMode(D3,OUTPUT);
 
}

void loop() {
 


 digitalWrite(D3, LOW);
 delay(500);
 digitalWrite(D3, 1);
 delay(500);



}
