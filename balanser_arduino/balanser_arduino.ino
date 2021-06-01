#include "LART_shield.h"

double Kp = 0.01;  // коэффицент П регулятора
double Ki = 0.0003; // коэффицент И регулятора
float  Kd = 30.0;  // коэффицент Д регулятора
const int cheburashka = 12000; // значение акселерометра, больше которого (по модулю) робот упал
const int mas_size = 5; // размер массива для анализа
int mas[mas_size] = {0}; // история значений акселерометра
long int I = 0;
int e_old = 0;
int speed_help = 30; // на какой скорости мотор начинает крутиться

void setup() {
  Serial.begin(9600);
  setup_motor();
  setup_akselerometr();
  Serial.println("Hello! I'm balanser robot :)");
  mediana();
}

void loop() {
  robot_main(); // запускаем main робота
}

void robot_main() {
  int e = akselerometr(); // считываем акселерометр
  for (int i = mas_size-1; i>=0; i--) mas[i] = mas[i-1]; // сдигаем значения в истории
  mas[0] = e; // заносим текущее значение в историю
  e = mediana(); // выбираем значение из истории по медиане массива
  //Serial.println(e);
  if (abs(e)<cheburashka) { // робот "стоит" или немного наклонен
    int p = Kp*e;            // пропорциональный
    I += Ki*e;               // интегральный
    int d = Kd*(e - e_old);  // дифференциальный
    //int pid = p + I + d;
    int pid = p;
    //pid += speed_help*abs(pid)/pid;
    //Serial.println(pid);
    left_motor(pid);
    right_motor(pid);
    e_old = e;
  }
  else stop_motors(); // робот упал
}

int mediana() {
  int mas1[mas_size] = {0}; // обработка массива, создаем копию
  for (int i = 0; i<mas_size; i++) mas1[i] = mas[i]; // копируем
  for (int i = 0; i<mas_size-1; i++) { // сортируем пузырьком
    for (int u = i+1; u<mas_size; u++) {
      if (mas1[i]>mas1[u]) {
        int c = mas1[i];
        mas1[i] = mas1[u];
        mas1[u] = c;
      }
    }
  }
  //for (int i = 0; i<mas_size; i++) { Serial.print(mas1[i]); Serial.print(" "); } Serial.println(); // выводим для теста
  return mas1[mas_size/2];
}
