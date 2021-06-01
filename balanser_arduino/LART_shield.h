// https://alexgyver.ru/lessons/library-writing/
#pragma once
#include <Arduino.h>

void setup_motor();
void setup_akselerometr();
void motor(int n, int speed);
void left_motor(int speed);
void right_motor(int speed);
void stop_motors();
int akselerometr();
