#include "LART_shield.h"
#include "shield_pins.h"
#include "Wire.h"

#pragma once

void setup_motor() {
  for (int n = 0; n<motor_count; n++) {
    pinMode(motor_D1[n], OUTPUT);
    pinMode(motor_D2[n], OUTPUT);
  }
}

void setup_akselerometr() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

void motor(int n, int speed) {
  if (n>motor_count || n<1) return;
  n--;
  digitalWrite(motor_D1[n], speed>0);
  digitalWrite(motor_D2[n], speed<0);
  analogWrite(motor_E[n], map(constrain(abs(speed),0,100),0,100,0,255));
}

void stop_motors() {
  for (int n = 0; n<motor_count; n++) {
    digitalWrite(motor_D1[n], 0);
    digitalWrite(motor_D2[n], 0);
    analogWrite(motor_E[n], 0);
  }
}

void left_motor(int speed) {
  motor(1,speed);
}

void right_motor(int speed){
  motor(2,speed);
}

int akselerometr() {
  int16_t data[7];  
  // массив данных
  // [accX, accY, accZ, temp, gyrX, gyrY, gyrZ]
  // acc - ускорение, gyr - угловая скорость, temp - температура (raw)
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  for (byte i = 0; i < 7; i++) {
    data[i] = Wire.read() << 8 | Wire.read();
  }
  return data[1];
}
