#pragma once

#include <Arduino.h>
#include <Ps3Controller.h>

namespace Controller {
  extern int player;
  extern int battery;

  void init();

  void notify();
  void onConnect();
  void setPlayerLED();
}
