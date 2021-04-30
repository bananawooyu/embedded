#include <Arduino.h>
#include <TM1637Display.h>
#define CLK 8
#define DIO 9
TM1637Display display(CLK, DIO);

uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 }; // 초기값으로 00:00 셋팅
uint8_t segto;

int Reset = 4;
int Stop = 3;
int Start = 2;
int state = 0;
int buzzerPin = 7;
int readTime;
int Sec1, Sec2, Mil1, Mil2;

void setup()
{
  pinMode(Reset, INPUT_PULLUP);
  pinMode(Stop, INPUT_PULLUP);
  pinMode(Start, INPUT_PULLUP);
  display.setBrightness(0xF);  //최대 밝기 (1~7 밝기 조절)
  Serial.begin(9600);
}

void loop()  {
  show();

  if (!digitalRead(Start) && state == 0) {
    state = 1;
    sound();
  }
  else if (!digitalRead(Stop) && state == 1) {
    state = 0;
    sound();
  }
  if (!digitalRead(Reset)) {
    state = 0;
    readTime = 0;
    sound();
    delay(100);
  }
  if (state)
    readTime++;

}

void sound() {
  tone(buzzerPin, 1200);
  delay(10);
  noTone(buzzerPin);
}

void times()
{
  Sec1 = (readTime / 100) / 10;
  Sec2 = (readTime / 100) % 10;
  Mil1 = (readTime % 100) / 10;
  Mil2 = (readTime % 100) % 10;
}

void show() {
  times();
  data[0] = display.encodeDigit(Sec1);  // 초시계
  data[1] = display.encodeDigit(Sec2);
  data[2] = display.encodeDigit(Mil1);
  data[3] = display.encodeDigit(Mil2);

  segto = 0x80 | display.encodeDigit(Sec2);  // 초시계용 ':'도트
  display.setSegments(data);
  display.setSegments(&segto, 1, 1); // uint8_t segments[], uint8_length, uint8_pos length = 길이 / pos = 시작 위치
}
