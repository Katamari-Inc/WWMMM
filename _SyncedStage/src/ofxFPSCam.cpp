//
//  ofxFPSCam.cpp
//  ofxfpscam
//
//  Created by Saqoosha on 11/11/18.
//  Copyright (c) 2011 Saqoosha. All rights reserved.
//

#include "ofxFPSCam.h"


ofxFPSCam::ofxFPSCam()
    : move_speed(10),
      rotate_speed(1),
      velocity_side(0),
      velocity_forward(0),
      velocity_upward(0),
      velocity_pan(0),
      velocity_tilt(0) {

    ofRegisterKeyEvents(this);
    setKeyAssignDvorak();
}


void ofxFPSCam::setKeyAssignDvorak() {
    key_assign[LEFT] = 97;
    key_assign[RIGHT] = 101;
    key_assign[FORWARD] = 44;
    key_assign[BACK] = 111;
    key_assign[UP] = 112;
    key_assign[DOWN] = 117;
    key_assign[PAN_LEFT] = 356;
    key_assign[PAN_RIGHT] = 358;
    key_assign[TILT_UP] = 357;
    key_assign[TILT_DOWN] = 359;
}


void ofxFPSCam::setKeyAssignASCII() {
  key_assign[LEFT] = 97; // a
  key_assign[RIGHT] = 100; // d
  key_assign[FORWARD] = 119; // w
  key_assign[BACK] = 115; // s
  key_assign[UP] = 114; // r
  key_assign[DOWN] = 102; // u
  key_assign[PAN_LEFT] = 356;
  key_assign[PAN_RIGHT] = 358;
  key_assign[TILT_UP] = 357;
  key_assign[TILT_DOWN] = 359;
}


void ofxFPSCam::update() {
    float v;
    v = key_state[key_assign[RIGHT]] ? move_speed : 0;
    v -= key_state[key_assign[LEFT]] ? move_speed : 0;
    velocity_side += (v - velocity_side) * 0.2;
    truck(velocity_side);
    
    v = key_state[key_assign[BACK]] ? move_speed : 0;
    v -= key_state[key_assign[FORWARD]] ? move_speed : 0;
    velocity_forward += (v - velocity_forward) * 0.2;
    dolly(velocity_forward);
    
    v = key_state[key_assign[UP]] ? move_speed : 0;
    v -= key_state[key_assign[DOWN]] ? move_speed : 0;
    velocity_upward += (v - velocity_upward) * 0.2;
    boom(velocity_upward);
    
    v = key_state[key_assign[PAN_LEFT]] ? rotate_speed : 0;
    v -= key_state[key_assign[PAN_RIGHT]] ? rotate_speed : 0;
    velocity_pan += (v - velocity_pan) * 0.2;
    rotate(velocity_pan, ofVec3f(0, 1, 0));
    
    v = key_state[key_assign[TILT_UP]] ? rotate_speed : 0;
    v -= key_state[key_assign[TILT_DOWN]] ? rotate_speed : 0;
    velocity_tilt += (v - velocity_tilt) * 0.2;
    rotate(velocity_tilt, getSideDir());
}


void ofxFPSCam::keyPressed(ofKeyEventArgs &event) {
//    ofLog(OF_LOG_VERBOSE, "keyPressed: %d", event.key);
    key_state[event.key] = true;
}


void ofxFPSCam::keyReleased(ofKeyEventArgs &event) {
//    ofLog(OF_LOG_VERBOSE, "keyReleased: %d", event.key);
    key_state[event.key] = false;
}



