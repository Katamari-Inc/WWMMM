#include "Motor.h"

Motor::Motor(int ss_pin) {
  stepper_ = new L6470(ss_pin);
  stepper_->init();
  // delay(1000);
  stepper_->setAcc(1000);
  stepper_->setDec(1000);
  stepper_->setMaxSpeed(600);
  stepper_->setMinSpeed(1);
  stepper_->setMicroSteps(128);
  stepper_->setThresholdSpeed(1000);
  // stepper_->setOverCurrent(6000);
  // stepper_->setStallCurrent(3000);

  setAsHome();
}

void Motor::setAsHome() {
  stepper_->setAsHome();
  destination_ = 0;
}

void Motor::go(long position) {
  destination_ = position;
}

void Motor::update() {
  long current = stepper_->getPos();
  float speed = float(destination_ - current) * 0.03;
  if (fabs(speed) > 1) {
    if (speed > 0) {
      stepper_->run(FWD, speed);
    } else {
      stepper_->run(REV, -speed);
    }
  // } else if (destination_ != current) {
  //   stepper_->goTo(destination_);
  } else {
    stepper_->hardStop();
  }
}

long Motor::position() {
  return stepper_->getPos();
}

void Motor::halt() {
  stepper_->free();
}
