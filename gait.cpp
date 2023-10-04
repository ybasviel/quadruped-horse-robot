#include "gait.h"

MPU6050 accelgyro;
Madgwick MadgwickFilter;

int16_t ax, ay, az;
int16_t gx, gy, gz;
float roll, pitch, yaw;
float formerRoll = 0, formerPitch = 0, formerYaw = 180;

double t = 0;

/* 右前 左前 右後 左後 */
/* 関節は体に近いほうから */
AdvancedServo servo[4][2];

MODE mode = MODE::STOP;


double rCorrection = 0;
double lCorrection = 0;

//TaskHandle_t control_task;

void getPosition(){
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  MadgwickFilter.updateIMU(gx / 131.0, gy / 131.0, gz / 131.0, ax / 16384.0, ay / 16384.0, az / 16384.0);
  pitch = MadgwickFilter.getRoll();
  roll = MadgwickFilter.getPitch();
  yaw = MadgwickFilter.getYaw();

  // 本体のあれに合わせて入れ替え
  const float T = 10;
  const float T_i = 20;

  roll = T_i / (T + T_i) * formerRoll + T / (T + T_i) * roll;
  pitch = T_i / (T + T_i) * formerPitch + T / (T + T_i) * pitch;
  yaw = T_i / (T + T_i) * formerYaw + T / (T + T_i) * yaw;
}


void GAIT::begin() {

  Wire.begin();

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

  delay(1000);

  accelgyro.initialize();
    delay(300);
    MadgwickFilter.begin(100);

    getPosition();

    formerRoll = roll;
    formerPitch = pitch;
    formerYaw = yaw;

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
  getPosition();
  formerRoll = roll;
    formerPitch = pitch;
    formerYaw = yaw;

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

  delay(10);
}




void GAIT::trot(bool reverse) {
  double x, y;
  double theta1, theta2;

  const double offset[4] = { 0, M_PI, M_PI, 0 };

  double rollGain = (roll - formerRoll) * 0.05 + roll * 0.05;
  double pitchGain = (pitch - formerPitch) * 3.0 + pitch * 1.5;

  double yf[4] = { 0 + rollGain - pitchGain, 0 - rollGain - pitchGain, 0 + rollGain + pitchGain, 0 - rollGain + pitchGain};
  double zf[4] = { 0 + rCorrection, 0 + lCorrection, 0 + rCorrection, 0 + lCorrection };
  //double zf[4] = { gainZ, -gainZ, gainZ, -gainZ };

  for (uint8_t i = 0; i < 4; i++) {
    x = (0 + zf[i]) * cos(-(t + offset[i]));
    y = 75 + yf[i] + 5 * sin(-(t + offset[i]));

    //if (y > 75) { y = 75 + yf[i]; }

    IK(x, y, &theta1, &theta2);

    write(theta1 * 180 / M_PI, theta2 * 180 / M_PI, (LEG)i);
  }
  if (reverse) {
    t -= 0.15;
  } else {
    t += 0.15;
  }
}


void GAIT::stop() {

  double theta1, theta2;

  double rollGain = (roll - formerRoll) * 0.6 + roll * 0.3;
  double pitchGain = (pitch - formerPitch) * 3.0 + pitch * 1.5;
  double yf[4] = { 0 + rollGain - pitchGain, 0 - rollGain - pitchGain, 0 + rollGain + pitchGain, 0 - rollGain + pitchGain};
  
  for (uint8_t i = 0; i < 4; i++) {
    IK(0, 80 + yf[i], &(theta1), &(theta2));
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