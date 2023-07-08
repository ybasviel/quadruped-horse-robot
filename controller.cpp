#include "controller.h"
#include "gait.h"

int Controller::player = 0;
int Controller::battery = 0;
//MODE Controller::mode = MODE::MOVE;

extern GAIT gait;

void Controller::init() {
  Ps3.attach(Controller::notify);
  Ps3.attachOnConnect(Controller::onConnect);
  Ps3.begin("c4:5b:be:33:ba:22");

  Serial.print("Connect.");

  while (!Ps3.isConnected()) {
    Serial.print(".");
    delay(700);
  }
  Serial.printf("\nConnected!\n\n");

  Ps3.setPlayer(Controller::player);
}

void Controller::notify() {
  //まるバツ四角三角
  if (Ps3.event.button_down.cross) {
  }
  if (Ps3.event.button_up.cross) {
    gait.set(MODE::STOP);
  }
  if (Ps3.event.button_down.square) {
  }
  if (Ps3.event.button_up.square) {
  }
  if (Ps3.event.button_down.triangle) {
  }
  if (Ps3.event.button_up.triangle) {
    gait.set(MODE::STEP);
  }

  if (Ps3.event.button_down.circle) {
  }
  if (Ps3.event.button_up.circle) {
    gait.set(MODE::TROT);
  }

  //十字キー
  if (Ps3.event.button_down.up) {
  }
  if (Ps3.event.button_up.up) {
    gait.set(MODE::TROT);
  }
  if (Ps3.event.button_down.right) {
  
  }
  if (Ps3.event.button_up.right) {
    gait.set(MODE::TURN_R);
  }
  if (Ps3.event.button_down.down) {
  }
  if (Ps3.event.button_up.down) {
    gait.set(MODE::TROT_BACK);
  }

  if (Ps3.event.button_down.left) {
    
  }
  if (Ps3.event.button_up.left) {
    gait.set(MODE::TURN_L);
  }

  //トリガー1
  if (Ps3.event.button_down.l1) {
  }
  if (Ps3.event.button_up.l1) {
  }

  if (Ps3.event.button_down.r1) {
  }
  if (Ps3.event.button_up.r1) {
  }

  //トリガー2
  if (Ps3.event.button_down.l2) {
    //snake.resume();
  }
  if (Ps3.event.button_up.l2) {
    //snake.suspend();
  }

  if (Ps3.event.button_down.r2) {
  }
  if (Ps3.event.button_up.r2) {
  }

  //スティック押し込み
  if (Ps3.event.button_down.l3) {
  }
  if (Ps3.event.button_up.l3) {
    //snake.reset();
  }

  if (Ps3.event.button_down.r3) {
  }
  if (Ps3.event.button_up.r3) {
  }

  //いろいろ
  if (Ps3.event.button_down.select) {
  }
  if (Ps3.event.button_up.select) {
  }

  if (Ps3.event.button_down.start) {
  }
  if (Ps3.event.button_up.start) {
  }

  if (Ps3.event.button_down.ps) {
  }
  if (Ps3.event.button_up.ps) {
  }


  //アナログスティック
  if (abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly) > 2) {
    float x = Ps3.data.analog.stick.lx;
    float y = Ps3.data.analog.stick.ly;

    /*double r = y/128.0 
    double l = y/128.0*/
    float norm   = sqrt(x * x + y * y) * 12 / 128.0;
    if(y > 0){
      norm = -norm;
    } 
    //float theta  = 90 * atan(x / y);

    Serial.print(norm);
    Serial.print(",");
    Serial.println(norm*(128+x)/128.0);

    if(x < 0 ){
      gait.setDirection(norm,  norm*(128+x)/128.0 );
    } else {
      gait.setDirection(norm*(128-x)/128.0,  norm );
    }
  } else if (abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly) <= 2) {
  }

  if (abs(Ps3.event.analog_changed.stick.rx) + abs(Ps3.event.analog_changed.stick.ry) > 2) {
    

  } else if (abs(Ps3.event.analog_changed.stick.rx) + abs(Ps3.event.analog_changed.stick.ry) <= 2) {
  }


  //---------------------- Battery events ---------------------
  if (Controller::battery != Ps3.data.status.battery) {
    Controller::battery = Ps3.data.status.battery;
    Serial.print("The controller battery is ");
    if (Controller::battery == ps3_status_battery_charging) Serial.println("charging");
    else if (Controller::battery == ps3_status_battery_full) Serial.println("FULL");
    else if (Controller::battery == ps3_status_battery_high) Serial.println("HIGH");
    else if (Controller::battery == ps3_status_battery_low) Serial.println("LOW");
    else if (Controller::battery == ps3_status_battery_dying) Serial.println("DYING");
    else if (Controller::battery == ps3_status_battery_shutdown) Serial.println("SHUTDOWN");
    else Serial.println("UNDEFINED");
  }
}

void Controller::onConnect() {
  //Serial.printf("\nConnected.\n\n");
}

void Controller::setPlayerLED(){
  if (!Ps3.isConnected()) return;
  Ps3.setPlayer(1);
  Ps3.setPlayer(Controller::player);
}