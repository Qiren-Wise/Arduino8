
#include <IRremote.h>

long ir_item;
IRrecv irrecv_7(7);
decode_results results_7;

void goAhead() {
  pinMode(A0, OUTPUT);
  digitalWrite(A0,LOW);
  pinMode(A1, OUTPUT);
  digitalWrite(A1,HIGH);
  pinMode(A2, OUTPUT);
  digitalWrite(A2,HIGH);
  pinMode(A3, OUTPUT);
  digitalWrite(A3,LOW);
  analogWrite(9,120);
  analogWrite(5,120);
}

void goBack() {
  pinMode(A0, OUTPUT);
  digitalWrite(A0,HIGH);
  pinMode(A1, OUTPUT);
  digitalWrite(A1,LOW);
  pinMode(A2, OUTPUT);
  digitalWrite(A2,LOW);
  pinMode(A3, OUTPUT);
  digitalWrite(A3,HIGH);
  analogWrite(9,120);
  analogWrite(5,120);
}

void turnLeft() {
  pinMode(A0, OUTPUT);
  digitalWrite(A0,LOW);
  pinMode(A1, OUTPUT);
  digitalWrite(A1,HIGH);
  pinMode(A2, OUTPUT);
  digitalWrite(A2,HIGH);
  pinMode(A3, OUTPUT);
  digitalWrite(A3,LOW);
  analogWrite(9,0);
  analogWrite(5,120);
}

void turnRight() {
  pinMode(A0, OUTPUT);
  digitalWrite(A0,HIGH);
  pinMode(A1, OUTPUT);
  digitalWrite(A1,LOW);
  pinMode(A2, OUTPUT);
  digitalWrite(A2,LOW);
  pinMode(A3, OUTPUT);
  digitalWrite(A3,HIGH);
  analogWrite(9,120);
  analogWrite(5,0);
}

void stop() {
  pinMode(A0, OUTPUT);
  digitalWrite(A0,LOW);
  pinMode(A1, OUTPUT);
  digitalWrite(A1,HIGH);
  pinMode(A2, OUTPUT);
  digitalWrite(A2,HIGH);
  pinMode(A3, OUTPUT);
  digitalWrite(A3,LOW);
  analogWrite(9,0);
  analogWrite(5,0);
}

void setup(){
  irrecv_7.enableIRIn();
}

void loop(){
  if (irrecv_7.decode(&results_7)) {
    ir_item=results_7.value;
    String type="UNKNOWN";
    String typelist[14]={"UNKNOWN", "NEC", "SONY", "RC5", "RC6", "DISH", "SHARP", "PANASONIC", "JVC", "SANYO", "MITSUBISHI", "SAMSUNG", "LG", "WHYNTER"};
    if(results_7.decode_type>=1&&results_7.decode_type<=13){
      type=typelist[results_7.decode_type];
    }
    Serial.print("IR TYPE:"+type+"  ");
    Serial.println(ir_item,HEX);
    irrecv_7.resume();
  } else {
    if (ir_item == 0xFF18E7) {
      goAhead();

    }
    if (ir_item == 0xFF4AB5) {
      goBack();

    }
    if (ir_item == 0xFF10EF) {
      turnLeft();

    }
    if (ir_item == 0xFF5AA5) {
      turnRight();

    }
    if (ir_item == 0xFF38C7) {
       stop();

    }
  }

}
