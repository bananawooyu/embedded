#include <TM1637Display.h>
#define CLK 8                                //7세그먼트 데이터 송수신을 위한 동기화 클럭핀번호
#define DIO 9                                //데이터 송수신을 위한 핀번호
TM1637Display display(CLK, DIO);             //TM1637 인스턴스를 선언

uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };     //7세그먼트 unsigned char 배열 초기화 ex)00:00 으로 표시
uint8_t segto;                               //7세그먼트 사이의 콜론 표현 변수

int SRST = 4;                                //start stop 버튼
int second = 3;                              //초 증가 버튼
int minute = 2;                              //분 증가 버튼
int state = 0;                               //타이머 작동 여부 상태 변수
int buzzerPin = 7;                           //수동 부저 출력 버튼
int readTime;                                //타이머 시간 변수
int Sec1, Sec2, Min1, Min2;                  //Sec1 : 10의 자리 초 Sec2 : 1의 자리 초
                                             //Min1 : 10의 자리 분 Min2 : 1의 자리 분

void setup(){
  pinMode(SRST, INPUT_PULLUP);
  pinMode(second, INPUT_PULLUP);
  pinMode(minute, INPUT_PULLUP);
  display.setBrightness(0xF);                //7세그먼트 밝기 조절
}

void loop()  {
  show();                                    //7세그먼트 Display에 표시 함수

  if (!digitalRead(second) && state == 0) {  //초 버튼을 누를시 1초 증가
    readTime++;
  }
  if (!digitalRead(minute) && state == 0) {  //분 버튼을 누를시 1분 증가
    readTime += 60;
  }
  if (!digitalRead(SRST) && state == 0) {    //start stop 버튼을 누르면 상태변수가 1로 변함
    state = 1;                               //state가 1일때 타이머가 작동 중 
    delay(100);
  }
  else if (!digitalRead(SRST) && state == 1) {  //start stop 버튼을 누르면 상태변수가 0로 변함
    state = 0;                                  //state가 0일때 타이머가 작동중지  
    delay(100);
  }

  if (state)                                  //상태변수가 1일때 countDown함수 작동
    countDown();
}

void countDown() {
  if (readTime > 0) {                         //readTime이 0을 초과할 때만 카운트 다운이 실행 됨
    readTime--;
    if (readTime == 0) {                      //타이머가 0초 일 때 1초 동안 부저가 울림
      show();
      tone(buzzerPin, 1200);
      delay(1000);
      noTone(buzzerPin);
      state = 0;
    }
  }
}

void times(){
  if (readTime > 5999) readTime = 5999;       //타이머가 99분 59초를 초과 할 시 99분 59초로 고정
  Min1 = (readTime / 60) / 10;                //
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
