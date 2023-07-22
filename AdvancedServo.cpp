#include "AdvancedServo.h"

void AdvancedServo::setOffset(int offset = 0) {
  this->offset = offset;
}


void AdvancedServo::setReverse(int reversePoint) {
  this->isReverse = true;
  this->reversePoint = reversePoint;
}


void AdvancedServo::unsetReverse() {
  this->isReverse = false;
}


int AdvancedServo::read() {
  return Servo::read() - this->offset;
}


void AdvancedServo::write(int value) {
  if (this->isReverse) {
    Servo::write(this->reversePoint - (value + this->offset));
  } else {
    Servo::write(value + this->offset);
  }
}