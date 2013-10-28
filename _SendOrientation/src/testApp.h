#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

class testApp : public ofBaseApp{
  
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

  ofxOscSender sender_;
  float pitch_;
  float roll_;
  ofQuaternion orientation_;
  ofBoxPrimitive box_;
  
  void send() {
    ofQuaternion qp, qr;
    qp.makeRotate(pitch_, 1, 0, 0);
    qr.makeRotate(roll_, 0, 0, 1);
    orientation_ = qr * qp;
    ofxOscMessage message;
    message.setAddress("/world/orientation");
    message.addFloatArg(orientation_.x());
    message.addFloatArg(orientation_.y());
    message.addFloatArg(orientation_.z());
    message.addFloatArg(orientation_.w());
    sender_.sendMessage(message);
  }
};
