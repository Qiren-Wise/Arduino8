#define PIN_ir 2
#define PIN_beep 4
#include <IRremote.h>

#define DZFW "$DKT:1,1!"//低姿复位
#define DKSQJ "$DKT:5,5!"//低快速前进
#define DKSHT "$DKT:6,5!"//低快速后退
#define DKSZZ "$DKT:7,5!"//低快速左转
#define DKSYZ "$DKT:8,5!"//低快速右转
void beep_on_once() {
  pinMode(PIN_beep, OUTPUT);
  digitalWrite(PIN_beep,HIGH);
  delay(1000);
  pinMode(PIN_beep, OUTPUT);
  digitalWrite(PIN_beep,LOW);
  delay(1000);
}

IRrecv irrecv_PIN_ir(PIN_ir);
decode_results results_PIN_ir;

unsigned long irValue;

void look_ir() {
  if (irrecv_PIN_ir.decode(&results_PIN_ir)) {
    irValue= results_PIN_ir.value;
    switch (irValue) {
     case 0xFFA25D:
      beep_on_once();
      Serial.println(DZFW);
      break;
     case 0xFF02FD:
      beep_on_once();
      Serial.println(DKSQJ);
      break;
     case 0xFF9867:
      beep_on_once();
      Serial.println(DKSHT);
      break;
     case 0xFFE01F:
      beep_on_once();
      Serial.println(DKSZZ);
      break;
     case 0xFF906F:
      beep_on_once();
      Serial.println(DKSYZ);
      break;
     case 0xFFA857:
      beep_on_once();
      Serial.println(DZFW);
      break;
    }
    irrecv_PIN_ir.resume();
  } else {
  }
}

void setup(){
  Serial.begin(115200); //串口初始化

  beep_on_once();
  beep_on_once();
  beep_on_once();
  Serial.println("Init ok~");
  irrecv_PIN_ir.enableIRIn();
}

void loop(){
  look_ir();

}
