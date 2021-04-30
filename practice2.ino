#include <TM1637Display.h>
#define CLK 8
#define DIO 9
TM1637Display display(CLK, DIO);

uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };                               
uint8_t segto;

int Reset = 4;
int Stop = 3;
int Start = 2;
int state = 0;
int buzzerPin = 7;
int readTime;
int Sec1, Sec2, Mil1, Mil2;

void setup() {
  pinMode(Reset, INPUT_PULLUP);
  pinMode(Stop, INPUT_PULLUP);
  pinMode(Start, INPUT_PULLUP);
  display.setBrightness(0xF);                              
}

void loop() {
  show();
  // start 누르면 스탑워치 시작

  // stop 누르면 스탑워치 정지

  // reset 누르면 스탑워치 초기화

  // 버튼 누를때 마다 소리 울림
}

void sound() {
  tone(buzzerPin, 1200);
  delay(10);
  noTone(buzzerPin);
}

void times() {
  // 기존의 분,초를 초와 밀리초로 변환
  // (1초 = 100밀리초)
}

void show() {
  times();
  data[0] = display.encodeDigit(Sec1);                                   
  data[1] = display.encodeDigit(Sec2);
  data[2] = display.encodeDigit(Mil1);
  data[3] = display.encodeDigit(Mil2);

  segto = 0x80 | display.encodeDigit(Sec2);                                
  display.setSegments(data);
  display.setSegments(&segto, 1, 1);                                    
}
