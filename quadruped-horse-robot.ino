#include <ESP32Servo.h>
#include "controller.h"
#include "gait.h"

/* ESP32 WROOM-DA Module */

GAIT gait;

void setup() {
  delay(2000);

  Serial.begin(115200);
  Controller::init();

  delay(1000);
  gait.begin();

  //delay(3000);
  
  //gait.set(MODE::TROT);
}

void loop() {
  //Controller::setPlayerLED();
  gait.control();
}
