#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxLeapMotion.h"
#include "pystring.h"


class Motor {
 public:
  Motor() : current(0), destination(0) {}
  int current;
  int destination;
};


class testApp : public ofBaseApp {
  
 public:
  void setup();
  void update();
  void draw();
  
  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);
  void resetButtonPushed();
  
  void sendCommand(string command);
  void setPosition(int index, int position);
  
  ofSerial serial_;
  vector<Motor*> motors_;
  float height_;
  
  ofxPanel panel_;
  ofxButton reset_button_;
  
  ofxLeapMotion leap_;
  vector<ofxLeapMotionSimpleHand> hands_;
};
