#pragma once

#include "ofMain.h"
#include "ofxGui.h"

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
  void goClicked();

  ofBoxPrimitive box_;
  ofCamera camera_;

  ofxPanel panel_;
  ofxFloatSlider target_;
  ofxFloatSlider current_;
  ofxFloatSlider max_velocity_;
  ofxFloatSlider current_velocity_;
  ofxFloatSlider acceleration_;
  ofxToggle moving_;
  ofxButton go_button_;
  
  float start_time_;
  float duration_;
  float start_position_;
  float start_velocity_;
  float direction_;
};
