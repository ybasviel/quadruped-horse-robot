#pragma once

#include <Arduino.h>
//#include <freertos/FreeRTOS.h>
#include <ESP32Servo.h>
#include "L3GD20H.h"

#define ARM_LENGTH_1 35.0
#define ARM_LENGTH_2 55.0

/*
  10000   low speed mode
  100000  standard mode
  400000  fast mode
  1000000 fast mode plus
  3400000 high speed mode
*/
#define GYRO_I2C_CLOCK 400000

enum LEG {
  RIGHT_FRONT = 0,
  LEFT_FRONT = 1,
  RIGHT_REAR = 2,
  LEFT_REAR = 3
};

enum class MODE{
  TROT,
  STOP
};


/* 右前 左前 右後 左後 */
/* 関節は体に近いほうから */
const uint8_t servoPin[4][2] = { { 12, 13 }, { 25, 26 }, { 16, 17 }, { 18, 19 } };

const int8_t servoOffset[4][2] = { { 0, 5 }, { 0, 0 }, { 7, 15 }, { 0, 0 } };

class GAIT {
public:
  void begin();

  void set(MODE m);

  void control();

  void setDirection(double r, double l);

private:
  static void IK(double x, double y, double *theta1, double *theta2);

  static void trot(bool reverse = false);
  static void stop();

  static void write(double theta1, double theta2, LEG leg);

  static int8_t powOfMinus1(int32_t n);
};