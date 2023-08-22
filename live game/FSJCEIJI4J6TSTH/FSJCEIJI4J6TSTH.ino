#include <SPI.h>                             // Подключаем библиотеку SPI
#include <Adafruit_GFX.h>                    // Подключаем библиотеку Adafruit_GFX
#include <Max72xxPanel.h>                    // Подключаем библиотеку Max72xxPanel
#define A(x) (16+x)%16
int pinCS = 9;                               // Указываем к какому выводу подключен контакт CS
int numberOfHorizontalDisplays = 2;          // Количество матриц по горизонтали
int numberOfVerticalDisplays = 2;            // Количество матриц по-вертикали
int y = 0;
int out[16][16][2];
byte arr[16][16];

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

void trans ( int out[16][16][2]){
  for (int y = 0; y < 8; y++ ) {
    for (int x = 0; x < 8; x++ ) { // Передача массива
      out[x][y][0]=7-x;
      out[x][y][1]=y;
    }
  }
  for (int y = 8; y < 16; y++ ) {
    for (int x = 0; x < 8; x++ ) { // Передача массива
      out[x][y][0]=15-x;
      out[x][y][1]=y-8;
    }
  }
   for (int y = 8; y < 16; y++ ) {
    for (int x = 8; x < 16; x++ ) { // Передача массива
      out[x][y][0]=x-8;
      out[x][y][1]=23-y;
    }
  }
     for (int y = 0; y < 8; y++ ) {
    for (int x = 8; x < 16; x++ ) { // Передача массива
      out[x][y][0]=x;
      out[x][y][1]=15-y;
    }
  }

}
void check(byte arr[16][16]){
  byte tmp[16][16];
  for (int i =0; i<16; i++){
    for (int j =0; j<16; j++){
      int count=0;
      if (arr[A(i-1)][A(j-1)]==1) count++;
      if (arr[A(i-1)][A(j)]==1) count++;
      if (arr[A(i-1)][A(j+1)]==1) count++;
      if (arr[A(i)][A(j-1)]==1) count++;
      //if (arr[A(i)][A(j)]==1) count++;
      if (arr[A(i)][A(j+1)]==1) count++;
      if (arr[A(i+1)][A(j-1)]==1) count++;
      if (arr[A(i+1)][A(j)]==1) count++;
      if (arr[A(i+1)][A(j+1)]==1) count++;
      if (count==3 || (arr[i][j]==1 && count==2)) 
        tmp[i][j]=1;
      else 
        tmp[i][j]=0;
    }
  }
  for (int i =0; i<16; i++){
    for (int j =0; j<16; j++){
      arr[i][j]=tmp[i][j];
    }
  }
}
void setup() {
  matrix.setIntensity(1);                    // Задаем яркость от 0 до 15
  matrix.setRotation(4);                      // Направление текста 1,2,3,4
  trans(out);
  arr[0][0]=1;
  arr[0][1]=1;
  arr[1][0]=1;
  arr[2][0]=1;
  arr[1][2]=1;

  
}
void loop() {

  matrix.fillScreen(LOW);                       // Обнуление матрицы
  //matrix.write();

  for (int i =0; i<16; i++){
    for (int j =0; j<16; j++){
      //if (arr[i][j]==1){
        matrix.drawPixel(out[i][j][0], out[i][j][1], 1);
        matrix.write();
  check(arr);
  delay(200);
      //}
    }
  }
  matrix.write();
  check(arr);
  delay(200);
}
