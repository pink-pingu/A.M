#include <SoftwareSerial.h>
#include <Stepper.h>

const int steps = 2048; //360도 돌기

SoftwareSerial BTSerial(3, 2);
Stepper PoseStep(steps,11,9,10,8);

char value = '0'; //수동제어
char power = '0'; //전원
char store = '0'; //임시 저장
char state = '0'; //상태

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  PoseStep.setSpeed(14);
  Serial.println("준비 완료");
  
  PoseStep.step(16);
  PoseStep.step(-16); //모터 실행 확인

  power = '5'; // 5=on, 4=off
  store = '0'; 
  value = '0';
  state = '7'; //7=비실행 상태, 6=실행된 상태
}

void loop() {
  if(Serial.available())
    BTSerial.write(Serial.read());

  if(BTSerial.available())
    Serial.write(BTSerial.read()); //블루투스 설정

  while (1) {
    if (Serial.available()) { //파이썬, 시리얼통신 입력
      store = Serial.read();
      save();

     if(power == '5' && state == '7'){
      turn();
      store = '0';
     }
    }

    if (BTSerial.available()) { //블루투스 입력
      store = BTSerial.read();
      save();

     if(store == '3'){
        turn();
        store = '0';
      }
    }
  }
}

void turn() { //모터 작동
  if(value == '1') {
      Serial.print("정회전");
      for(int i=0; i<5; i++){ //한바퀴
        PoseStep.step(steps);
      }
      Serial.print("끝\n");
      state = '6'; //실행된 상태로 변경
      
    }
    else if(value == '2') {
      Serial.print("역회전");
      for(int i=0; i<5; i++){
        PoseStep.step(-steps);
      }
      Serial.print("끝\n");
      state = '7'; //비실행된 상태로 변경
    }
}

void save() { //변수 값 저장
  if(store == '5' || store == '4') { //전원
        power = store;
        Serial.print("power");
        Serial.println(power);
      }
  else if(store == '1') { 
    if(state == '7') { //비실행 상태라면, 내려간 상태
      value = store;
    }
    else if(state == '6') { //실행 상태라면, 올라간 상태
     
    }
        Serial.print("value");
        Serial.println(value);
      }
  else if(store == '3') { //수동제어, 앱
    Serial.println(store);
    Serial.println(value);
    
    if(value =='1'){
      value = '2';
      Serial.print("change value");
      Serial.println(value);
    }
    else if(value =='2' || value =='0') {
      value='1';
      Serial.print("change value");
      Serial.println(value);
    }
  }
}
