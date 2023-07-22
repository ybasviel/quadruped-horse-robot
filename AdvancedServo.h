#pragma once

#include <Arduino.h>

#if defined(ESP32)
#include <ESP32Servo.h>
#else
#include <Servo.h>
#endif


class AdvancedServo : public Servo {
public:
  void setOffset(int offset);
  void setReverse(int reversePoint);
  void unsetReverse();

  void write(int value);
  int read();

  int offset = 0;
  int reversePoint = 0;
  bool isReverse = false;

private:
};
