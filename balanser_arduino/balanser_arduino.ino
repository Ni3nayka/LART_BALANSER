#include "LART_shield.h"

#define ROBOT_ENABLE 0

int speed_help = 40; // на какой скорости мотор начинает крутиться
double Kk = 0.002;  // коэффицент К регулятора
double Kp = 0.7;  // коэффицент П регулятора
double Ki = 0.0003; // коэффицент И регулятора
float  Kd = 30.0;  // коэффицент Д регулятора
const int cheburashka = 100; // значение акселерометра, больше которого (по модулю) робот упал
const int mas_size = 3; // размер массива для анализа
int mas[mas_size] = {0}; // история значений акселерометра
long int I = 0;
int e_old = 0;

// PID
#include "PID_v1.h"
double originalSetpoint = 0;//-0.6;//Требуемое значение стабилизации
double setpoint = originalSetpoint;
double input, output;
double Kp1 = 25;    
double Kd1 = 0.5;  
double Ki1 = 420;  
PID pid(&input, &output, &setpoint, Kp1, Ki1, Kd1, DIRECT);
// Постоянные для вычилсения угла наклона 
// *********************************************************************
#define dt 20                       // промежуток времени в милисикундах 
#define rad2degree 57.3              // переменная для перевода из радиан в градусы 
#define Filter_gain 0.95             // переменная для фильтра angle = angle_gyro*Filter_gain + angle_accel*(1-Filter_gain)
// *********************************************************************
unsigned long t=0; // Переменная времени 

void setup() {
  Serial.begin(9600);
  setup_motor();
  setup_akselerometr();
  Serial.println("Hello! I'm balanser robot :)");
  //mediana();

  // PID
  pid.SetMode(AUTOMATIC);
  pid.SetSampleTime(10);
  pid.SetOutputLimits(-100, 100); 
}

void loop() {
  //robot_main(); // запускаем main робота
  robot_main_2(); // запускаем main робота
}

float clamp(float v, float minv, float maxv){
    if( v>maxv )
        return maxv;
    else if( v<minv )
        return minv;
    return v;
}

double gyro_angle() {
  double ay = akselerometr() / (4096.0*4);//4096.0;
  // на случай, если на акселерометр действуют посторонние силы, которые могут
  // увеличить ускорение датчика свыше 1G, установим границы от -1G до +1G  
  ay = constrain(ay, -1.0, 1.0);
  // функция acos возвращает угол в радианах, так что преобразуем
  // его в градусы при помощи коэффициента TO_DEG
  if (ay >= 0) ay = 90 - rad2degree*acos(ay);
  else ay = rad2degree*acos(-ay) - 90;
  Serial.print("ay: "); Serial.println(ay);
  return -ay;
}

void robot_main_2() {
  t=millis(); 
  
  
  
 //Вывод оси времени 
  //Serial.println(((float)(millis()-t)/(float)dt)*100);
  while((millis()-t) < dt){ // Making sure the cycle time is equat to dt
  // Do nothing
  }
  input=gyro_angle();//Угол наклона относительно оси OY является входным параметром для ПИД регулятора

  if (abs(input)<60) {
    pid.Compute();
    int PWM=output;//Выходной сигнал ШИМ передаем на двигатели 
    //Serial.print("PWM: "); Serial.println(PWM);
    left_motor(-PWM);
    right_motor(-PWM);
  }
  else stop_motors(); // робот упал
  
}

void robot_main() {
  int e = akselerometr()/128; // считываем акселерометр
  for (int i = mas_size-1; i>=0; i--) mas[i] = mas[i-1]; // сдигаем значения в истории
  mas[0] = e; // заносим текущее значение в историю
  e = mediana(); // выбираем значение из истории по медиане массива
  Serial.println(e);
  if (abs(e)<cheburashka && 1) { // робот "стоит" или немного наклонен
    int r = speed_help*znak(e);
    long int k = Kk*pow(e,3);
    int p = Kp*e;            // пропорциональный
    I += Ki*e;               // интегральный
    int d = Kd*(e - e_old);  // дифференциальный
    int pid = r*0 + p*0 + I*0 + d*0 + k*1;
    //int pid = p; // + r
    //Serial.println(pid);
    left_motor(pid);
    right_motor(pid);
    e_old = e;
  }
  else stop_motors(); // робот упал
}

int znak(int wer) {
  if (wer>0) return 1;
  if (wer<0) return -1;
  return 0;
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
