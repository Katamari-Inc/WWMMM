#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
  ofSetLogLevel(OF_LOG_VERBOSE);
//  serial_.listDevices();
  serial_.setup("/dev/tty.usbmodem1411", 57600);
  
  slider_.addListener(this, &testApp::sliderChanged);
  panel_.setup();
  panel_.add(slider_.setup("VALUE", 0, -50000, 50000));
}

//--------------------------------------------------------------
void testApp::update() {
  unsigned char buf[256];
  memset(buf, 0, 256);
  int n;
  while ((n = serial_.readBytes(buf, 255)) > 0) {
    buf[n] = NULL;
    cout << buf;
  }
}

//--------------------------------------------------------------
void testApp::draw() {
  panel_.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {

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

//--------------------------------------------------------------
void testApp::sliderChanged(int &value) {
  ofLog(OF_LOG_VERBOSE, "value: %d", value);
  stringstream str;
  str << "P 0 " << value << "\n";
  string s = str.str();
  serial_.writeBytes((unsigned char *)s.c_str(), s.length());
}
