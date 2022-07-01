#include <IRremote.h>

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

IRrecv irrecv_7(7);
decode_results results_7;

unsigned long irValue;

void setup(){
  irrecv_7.enableIRIn();
}

void loop(){
  if (irrecv_7.decode(&results_7)) {
    irValue= results_7.value;
    Serial.println(ir_item,HEX);
    irrecv_7.resume();
  } else {
    if (ir_item == 0xFF18E7) {
      ir_item = goAhead();

    }
    if (ir_item == 0xFF4AB5) {
      ir_item = goBack();

    }
    if (ir_item == 0xFF10EF) {
      ir_item = turnLeft();

    }
    if (ir_item == 0xFF5AA5) {
      ir_item = turnRight();

    }
    if (ir_item == 0xFF38C7) {
      ir_item = stop();

    }
  }

}
