#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
  ofSetLogLevel(OF_LOG_VERBOSE);
//  sender_.setup("127.0.0.1", 8001);
  sender_.setup("192.168.4.44", 8001);
  box_.set(330, 10, 500);
}

//--------------------------------------------------------------
void testApp::update() {

}

//--------------------------------------------------------------
void testApp::draw() {
  ofBackground(64);
  
  ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
  ofMatrix4x4 m;
  m.setRotate(orientation_);
  ofMultMatrix(m);
  
  ofNoFill();
  ofSetColor(255);
  box_.drawWireframe();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
  switch (key) {
    case OF_KEY_LEFT:
    case 'h':
      roll_ -= 1;
      send();
      break;
    case OF_KEY_RIGHT:
    case 'n':
      roll_ += 1;
      send();
      break;
    case OF_KEY_DOWN:
    case 't':
      pitch_ -= 1;
      send();
      break;
    case OF_KEY_UP:
    case 'c':
      pitch_ += 1;
      send();
      break;
    case ' ':
      pitch_ = roll_ = 0;
      send();
      break;
  }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
  if (button == OF_MOUSE_BUTTON_1) {
    float l = MAX(ofGetWidth(), ofGetHeight()) * 0.5;
    roll_ = ((float)x - ofGetWidth() * 0.5) / l * 40;
    pitch_ = -((float)y - ofGetHeight() * 0.5) / l * 40;
//    ofLog(OF_LOG_VERBOSE, "%f, %f", roll_, pitch_);
    send();
  }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo) { 

}
