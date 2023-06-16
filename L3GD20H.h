#pragma once

#include <Arduino.h>
#include <Wire.h>

#define L3GD20H_ADDR   0x6b
#define L3GD20H_CTRL1  0x20
#define L3GD20H_CTRL2  0x21
#define L3GD20H_CTRL3  0x22
#define L3GD20H_CTRL4  0x23
#define L3GD20H_CTRL5  0x24

#define L3GD20H_OUT_X_L  0x28
#define L3GD20H_OUT_X_H  0x29
#define L3GD20H_OUT_Y_L  0x2a
#define L3GD20H_OUT_Y_H  0x2b
#define L3GD20H_OUT_Z_L  0x2c
#define L3GD20H_OUT_Z_H  0x2d

#define L3GD20H_WHO_AM_I 0x0f


class L3GD20H{
public:
  void init(uint32_t i2cClock = 100000, uint32_t timeout = 25000);
  void get(int16_t *x, int16_t *y, int16_t *z);
  void calibration();

  void getX(int16_t *x);
  void getY(int16_t *y);
  void getZ(int16_t *z);

private:
  int16_t x0, y0, z0;

  uint8_t read(uint8_t reg);

};