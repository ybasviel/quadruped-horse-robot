#include <math.h>
#include "gait.h"

double t = 0;

/* 右前 左前 右後 左後 */
/* 関節は体に近いほうから */
AdvancedServo servo[4][2];

MODE mode = MODE::STOP;

L3GD20H gyro;

int16_t formerGyroX = 0, formerGyroY = 0, formerGyroZ = 0;
int16_t gyroX, gyroY, gyroZ;

double rCorrection = 0;
double lCorrection = 0;

//TaskHandle_t control_task;


void GAIT::begin() {

  gyro.init(GYRO_I2C_CLOCK, 500);

  delay(500);

  for (int i = 0; i < 4; i++) {
    servo[i][0].attach(servoPin[i][0], 500, 2400);
    servo[i][1].attach(servoPin[i][1], 500, 2400);

    servo[i][0].setOffset(servoOffset[i][0]);
    servo[i][1].setOffset(servoOffset[i][1]);

    if(i%2 != 0){
      servo[i][0].setReverse(180);
      servo[i][1].setReverse(180);
    }
  }

  //MsTimer2::set(20, control);
  //MsTimer2::start();
  //xTaskCreateUniversal(this->control, "control servo", 8192, NULL, 1, &control_task, APP_CPU_NUM);
}

void GAIT::set(MODE m) {
  mode = m;
}

void GAIT::setDirection(double r, double l) {
  rCorrection = r;
  lCorrection = l;
}


void GAIT::write(double theta1, double theta2, LEG leg) {
  servo[int(leg)][0].write(int(theta1));
  servo[int(leg)][1].write(90 - int(theta2));
}



void GAIT::control() {
  while (1) {
    gyro.get(&gyroX, &gyroY, &gyroZ);

    switch (mode) {
      case MODE::TROT:
        trot();
        break;

      case MODE::STOP:
        stop();
        break;

      default:
        stop();
        break;
    }

    formerGyroX = gyroX;
    formerGyroY = gyroY;
    formerGyroZ = gyroZ;

    delay(20);
  }
}




void GAIT::trot(bool reverse) {
  double x, y;
  double theta1, theta2;

  const double offset[4] = { 0, M_PI, M_PI, 0 };

  double gainY = (gyroY - formerGyroY) / 20000.0 + gyroY / 10000.0;
  //double gainZ = (gyroZ-formerGyroZ)/10000.0 + gyroZ/5000.0;

  double yf[4] = { 0 + gainY, 0 - gainY, 0 + gainY, 0 - gainY };
  double zf[4] = { 0 + rCorrection, 0 + lCorrection, 0 + rCorrection, 0 + lCorrection };
  //double zf[4] = { gainZ, -gainZ, gainZ, -gainZ };

  for (uint8_t i = 0; i < 4; i++) {
    x = (0 + zf[i]) * cos(-(t + offset[i]));
    y = 80 + 10 * sin(-(t + offset[i]));

    if (y > 80) { y = 80 + yf[i]; }

    IK(x, y, &theta1, &theta2);

    write(theta1 * 180 / M_PI, theta2 * 180 / M_PI, (LEG)i);
  }
  if (reverse) {
    t -= 0.3;
  } else {
    t += 0.3;
  }
}


void GAIT::stop() {

  double theta1, theta2;

  IK(0, 80, &(theta1), &(theta2));
  for (uint8_t i = 0; i < 4; i++) {
    write(theta1 * 180 / M_PI, theta2 * 180 / M_PI, (LEG)i);
  }
}


void GAIT::IK(double x, double y, double *theta1, double *theta2) {
  *theta1 = -acos((x * x + y * y + ARM_LENGTH_1 * ARM_LENGTH_1 - ARM_LENGTH_2 * ARM_LENGTH_2) / (2 * ARM_LENGTH_1 * sqrt(x * x + y * y))) + atan2(y, x);
  *theta2 = atan2((y - ARM_LENGTH_1 * sin(*theta1)), (x - ARM_LENGTH_1 * cos(*theta1))) - *theta1;

  return;
}

int8_t GAIT::powOfMinus1(int32_t n) {
  if (n % 2) {
    return 1;
  }
  return -1;
}