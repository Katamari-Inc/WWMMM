//
//  MotorManager.h
//  MovingProjection
//
//  Created by Saqoosha on 2013/10/27.
//
//

#pragma once

#include "ofMain.h"
#include "pystring.h"


class Motor {
public:
  Motor() : current(0), destination(0) {}
  int current;
  int destination;
};


class MotorManager {
public:
  MotorManager() : height_(0.0f), roll_(0.0f), pitch_(0.0f), is_ready_(false) {
  }
  
  void setup(string port) {
    buffer_[0] = NULL;
    serial_.listDevices();
    serial_.setup(port, 57600);
    
    motors_.push_back(new Motor());
    motors_.push_back(new Motor());
    motors_.push_back(new Motor());
  }
  
  void update() {
    while (serial_.available() > 0) {
      unsigned char c = serial_.readByte();
      if (c == '\n') {
        vector<string> tokens;
        pystring::split(string((char *)buffer_), tokens);
        if (tokens[0] == "P") {
          int id;
          istringstream(tokens[1]) >> id;
          int pos;
          istringstream(tokens[2]) >> pos;
          motors_[id]->current = pos;
          cout << "current: " << id << " = " << pos << endl;
          is_ready_ = true;
        } else {
          cout << buffer_ << endl;
        }
        bufp_ = 0;
        buffer_[0] = NULL;
      } else if (isprint(c)) {
        buffer_[bufp_++] = c;
        buffer_[bufp_] = NULL;
      }
    }
  }
  
  void sendCommand(string command) {
    cout << "COMMAND: " << command << endl;
    serial_.writeBytes((unsigned char *)command.c_str(), command.length());
    serial_.writeByte('\n');
  }
  
  void setPosition(int index, float millimeter) {
    Motor *motor = motors_[index];
    if (motor->destination == millimeter) return;
    motor->destination = millimeter;
    
    int position = -millimeter / (50.0f * M_PI) * 200 * 128;
    stringstream str;
    str << "P " << index << " " << position;
    sendCommand(str.str());
  }
  
  void setHeight(float millimeter) {
    height_ = millimeter;
    sync();
  }
  
  float getHeight() {
    return height_;
  }
  
  void move(int motor_id, float amount) {
    setPosition(motor_id, motors_[motor_id]->destination + amount);
  }
  
  void roll(float degree) {
    roll_ = ofClamp(roll_ + degree, -20, 20);
    sync();
  }
  
  void pitch(float degree) {
    pitch_ = ofClamp(pitch_ + degree, -20, 20);
    sync();
  }
  
  void sync() {
    mat_.makeRotationMatrix(roll_, 0, 0, 1);
    mat_.postMultRotate(pitch_, 1, 0, 0);
    ofVec3f p = mat_ * ofVec3f(225, 0, 190);
    setPosition(1, p.y + height_);
    p = mat_ * ofVec3f(-225, 0, 190);
    setPosition(2, p.y + height_);
    p = mat_ * ofVec3f(0, 0, -190);
    setPosition(0, p.y + height_);
  }
  
  void initOrigin() {
    height_ = 0;
    roll_ = 0;
    pitch_ = 0;
    mat_.makeIdentityMatrix();
    sendCommand("SETHOME");
  }
  
  void reset() {
    height_ = 0;
    roll_ = 0;
    pitch_ = 0;
    mat_.makeIdentityMatrix();
    sync();
  }
  
  
  bool is_ready_;
  ofSerial serial_;
  unsigned char buffer_[1024];
  int bufp_ = 0;
  
  vector<Motor*> motors_;
  
  ofMatrix4x4 mat_;
  float height_;
  float roll_;
  float pitch_;
};
