#include "Motor.h"
#include "SerialCommand.h"

#define NUM_MOTORS 3

SerialCommand command_;
Motor *motors_[NUM_MOTORS];

bool halted = false;
unsigned long t = 0;

void setup() {
  Serial.begin(57600);

  command_.addCommand("ECHO", echo);
  command_.addCommand("P", setPosition);
  command_.addCommand("SETHOME", setHome);
  command_.addCommand("HALT", haltAll);
  command_.setDefaultHandler(unrecognized);

  L6470::setup();
  motors_[0] = new Motor(7);
  motors_[1] = new Motor(8);
  motors_[2] = new Motor(9);

  t = millis();
  Serial.println("Ready");
}

void loop() {
  if (halted) return;

  command_.readSerial();

  for (int i = 0; i < NUM_MOTORS; i++) {
    motors_[i]->update();
  }

  unsigned long now = millis();
  if (now - t > 1000) {
    t = now;
    for (int i = 0; i < NUM_MOTORS; i++) {
      Serial.print("P ");
      Serial.print(i);
      // Serial.print(" ");
      // Serial.print(motors_[i]->destination_);
      Serial.print(" ");
      Serial.println(motors_[i]->position());
    }
  }
}

void echo() {
  char *arg;
  arg = command_.next();
  if (arg != NULL) {
    Serial.print("ECHO ");
    Serial.println(arg);
  } else {
    Serial.println("ECHO (NO ARGS)");
  }
}

void setPosition() {
  char *arg = command_.next();
  if (arg == NULL) return;
  int id = atoi(arg);
  if (id < 0 || NUM_MOTORS - 1 < id) return;

  arg = command_.next();
  if (arg == NULL) return;
  long pos = atol(arg);

  motors_[id]->go(pos);
}

void setHome() {
  for (int i = 0; i < NUM_MOTORS; i++) {
    motors_[i]->setAsHome();
  }
}

void haltAll() {
  for (int i = 0; i < NUM_MOTORS; i++) {
    motors_[i]->halt();
  }
  halted = true;
}

void unrecognized(const char *command) {
  Serial.println("What?");
}
