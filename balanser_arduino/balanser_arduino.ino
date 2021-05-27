#include "shield_pins.h"

void setup() {
  Serial.begin(9600);
  for (int n = 0; n<motor_count; n++) {
    pinMode(motor_D1[n], OUTPUT);
    pinMode(motor_D2[n], OUTPUT);
  }
  Serial.println("Hello!");
  motor(1, 100); // проблема с кручением в обратную сторону, скорее всего в speed>0 (строка 27)
  delay(1000);
  motor(1, 0);
  Serial.println("end");
}

void loop() {
  
}

int akselerometr() {
  
}

void motor(int n, int speed) {
  if (n>motor_count || n<1) return;
  n--;
  digitalWrite(motor_D1[n], speed>0);
  digitalWrite(motor_D2[n], speed<0);
  analogWrite(motor_E[n], map(constrain(abs(speed),0,100),0,100,0,255));
  //digitalWrite(7, 0);
  //digitalWrite(8, 1);
  //analogWrite(6, 255);
}

void stop_motors() {
  for (int n = 0; n<motor_count; n++) {
    digitalWrite(motor_D1[n], 0);
    digitalWrite(motor_D2[n], 0);
    analogWrite(motor_E[n], 0);
  }
}
