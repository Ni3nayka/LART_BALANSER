// l293d:
const int motor_count = 2;
const int motor_D1[motor_count] = {7,5};
const int motor_D2[motor_count] = {8,4};
const int motor_E[motor_count]  = {6,3};

//pins:
//A7, A6, A3
//D13, D12, D11, D10, D9, D1, D0

//mpu6050:
const int MPU_addr = 0x68; // адрес датчика
//scl = A5
//sda = A4
