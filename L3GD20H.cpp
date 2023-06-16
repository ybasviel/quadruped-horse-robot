#include "L3GD20H.h"

void L3GD20H::init(uint32_t i2cClock, uint32_t timeout){
  Wire.begin();
  Wire.setClock(i2cClock);
  //Wire.setWireTimeout(timeout, true);

  Wire.beginTransmission(L3GD20H_ADDR);
  Wire.write(L3GD20H_CTRL1);
  Wire.write(0b00001111);
  Wire.endTransmission();

  this->calibration();
}

void L3GD20H::get(int16_t *x, int16_t *y, int16_t *z) {  
  this->getX(x);
  this->getY(y);
  this->getZ(z);

  /*Serial.print(*x);
  Serial.print(",");
  Serial.print(*y);
  Serial.print(",");
  Serial.println(*z);*/
}

void L3GD20H::getX(int16_t *x){
  int highBit, lowBit;
  
  lowBit = this->read(L3GD20H_OUT_X_L);
  highBit = this->read(L3GD20H_OUT_X_H);
  *x = ((highBit << 8) | lowBit) - this->x0 ;
}

void L3GD20H::getY(int16_t *y){
  int highBit, lowBit;

  lowBit = this->read(L3GD20H_OUT_Y_L);
  highBit = this->read(L3GD20H_OUT_Y_H);
  *y = ((highBit << 8) | lowBit) - this->y0 ;
}

void L3GD20H::getZ(int16_t *z){
  int highBit, lowBit;

  lowBit = this->read(L3GD20H_OUT_Z_L);
  highBit = this->read(L3GD20H_OUT_Z_H);
  *z = ((highBit << 8) | lowBit) - this->z0 ;
}



void L3GD20H::calibration(){
  int16_t xsum = 0, ysum = 0, zsum = 0;
  int16_t x, y, z;
 
  this->x0 = 0;
  this->y0 = 0;
  this->z0 = 0;

  for (uint8_t i = 0; i < 100; i++) {
    this->get(&x, &y, &z);

    xsum += x;
    ysum += y;
    zsum += z;
    delay(10);
  }
  this->x0 = xsum / 100;
  this->y0 = ysum / 100;
  this->z0 = zsum / 100;
}

uint8_t L3GD20H::read(uint8_t reg){
  Wire.beginTransmission(L3GD20H_ADDR);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(L3GD20H_ADDR, 1);
  return Wire.read();
}