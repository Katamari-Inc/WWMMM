//
//  ofxFPSCam.h
//  ofxfpscam
//
//  Created by Saqoosha on 11/11/18.
//  Copyright (c) 2011 Saqoosha. All rights reserved.
//

#pragma once

#include <map>
#include "ofCamera.h"
#include "ofEvents.h"


class ofxFPSCam : public ofCamera {
 public:
  typedef enum {
    LEFT = 0,
    RIGHT,
    FORWARD,
    BACK,
    UP,
    DOWN,
    PAN_LEFT,
    PAN_RIGHT,
    TILT_UP,
    TILT_DOWN
  } CamCommand;
  
  ofxFPSCam();
  
  void setKeyAssignDvorak();
  void setKeyAssignASCII();
  
  void update();
  void keyPressed(ofKeyEventArgs &event);
  void keyReleased(ofKeyEventArgs &event);
  
  std::map<CamCommand, int> key_assign;
  std::map<int, bool> key_state;
  
  float move_speed;
  float rotate_speed;
  
  float velocity_side;
  float velocity_forward;
  float velocity_upward;
  float velocity_pan;
  float velocity_tilt;
};

