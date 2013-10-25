#ifndef MOTOR_H_
#define MOTOR_H_

#include "L6470.h"

class Motor {
 public:
  Motor(int ss_pin);
  void setAsHome();
  void go(long position);
  void update();
  long position();

  L6470 *stepper_;
  long destination_;
};

#endif // MOTOR_H_
