#include <TM1637Display.h>
#define CLK 8
#define DIO 9
TM1637Display display(CLK, DIO);

uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };
uint8_t segto;

int SRST = 4;
int second = 3;
int minute = 2;
int state = 0;
int buzzerPin = 7;
int readTime;
int Sec1, Sec2, Min1, Min2;

void setup() {
  pinMode(SRST, INPUT_PULLUP);
  pinMode(second, INPUT_PULLUP);
  pinMode(minute, INPUT_PULLUP);
  display.setBrightness(0xF);
}

void loop() {
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

  if (!digitalRead(second) && !digitalRead(minute)) {
    readTime = 0;
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

void times() {
  if (readTime > 5999) readTime = 0;
  Min1 = (readTime / 60) / 10;
  Min2 = (readTime / 60) % 10;
  Sec1 = (readTime % 60) / 10;
  Sec2 = (readTime % 60) % 10;
}

void show() {
  times();
  data[0] = display.encodeDigit(Min1);
  data[1] = display.encodeDigit(Min2);
  data[2] = display.encodeDigit(Sec1);
  data[3] = display.encodeDigit(Sec2);

  segto = 0x80 | display.encodeDigit(Min2);
  display.setSegments(data);
  display.setSegments(&segto, 1, 1);
  delay(100);
}
