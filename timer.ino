#include <Arduino.h>
#include <TM1637Display.h>
#define CLK 8
#define DIO 9
TM1637Display display(CLK, DIO);

uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 }; // 초기값으로 00:00 셋팅
uint8_t segto;

int SRST = 4;
int second = 3;
int minute = 2;
int state = 0;
int buzzerPin = 7;
int readTime;
int Sec1, Sec2, Min1, Min2;

void setup()
{
  pinMode(SRST, INPUT_PULLUP);
  pinMode(second, INPUT_PULLUP);
  pinMode(minute, INPUT_PULLUP);
  display.setBrightness(0xF);  //최대 밝기 (1~7 밝기 조절)
  Serial.begin(9600);
}

void loop()  {
  show();

  if (!digitalRead(second) && state == 0) {
    readTime++;
  }
  if (!digitalRead(minute) && state == 0) {
    readTime += 60;
  }
  if (!digitalRead(SRST) && state == 0) {
    state = 1;
    delay(100);
  }
  else if (!digitalRead(SRST) && state == 1) {
    state = 0;
    delay(100);
  }

  if (state)
    countDown();

}

void countDown() {
  if (readTime > 0) {
    readTime--;
    show();
    if (readTime == 0) {
      tone(buzzerPin, 1200);
      delay(1000);
      noTone(buzzerPin);
      state = 0;
    }
  }
}

void times()
{
  if (readTime > 5999) readTime = 5999;
  Min1 = (readTime / 60) / 10;
  Min2 = (readTime / 60) % 10;
  Sec1 = (readTime % 60) / 10;
  Sec2 = (readTime % 60) % 10;
}

void show() {
  times();
  data[0] = display.encodeDigit(Min1);  // 초시계
  data[1] = display.encodeDigit(Min2);
  data[2] = display.encodeDigit(Sec1);
  data[3] = display.encodeDigit(Sec2);

  segto = 0x80 | display.encodeDigit(Min2);  // 초시계용 ':'도트
  display.setSegments(data);
  display.setSegments(&segto, 1, 1); // uint8_t segments[], uint8_length, uint8_pos length = 길이 / pos = 시작 위치
  delay(100);
  //display.setSegments(data);  //도트가 깜빡이게 하기 위해 반복 출력

}