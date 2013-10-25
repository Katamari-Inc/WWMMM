#include "testApp.h"

unsigned char buf[1024];
int len = 0;

//--------------------------------------------------------------
void testApp::setup() {
  //  ofSetLogLevel(OF_LOG_VERBOSE);
  ofSetFrameRate(60);
  ofSetVerticalSync(true);
  
  buf[0] = NULL;
  motors_.push_back(new Motor());
  motors_.push_back(new Motor());
  motors_.push_back(new Motor());
  
  leap_.open();
  
  serial_.listDevices();
  serial_.setup("/dev/tty.usbmodem1411", 57600);
  
  height_ = 150.0f;
  
  panel_.setup();
  reset_button_.addListener(this, &testApp::resetButtonPushed);
  panel_.add(reset_button_.setup("SET AS HOME"));
}

//--------------------------------------------------------------
void testApp::update() {
  while (serial_.available() > 0) {
    unsigned char c = serial_.readByte();
    if (c == '\n') {
      vector<string> tokens;
      pystring::split(string((char *)buf), tokens);
      if (tokens[0] == "P") {
        int id;
        istringstream(tokens[1]) >> id;
        int pos;
        istringstream(tokens[2]) >> pos;
        motors_[id]->current = pos;
        cout << "current: " << id << " = " << pos << endl;
      } else {
        cout << buf << endl;
      }
      len = 0;
      buf[0] = NULL;
    } else if (isprint(c)) {
      buf[len++] = c;
      buf[len] = NULL;
    }
  }
  
  hands_ = leap_.getSimpleHands();
  if (leap_.isFrameNew() && hands_.size()) {
    height_ = ofClamp(hands_[0].handPos.y, 150.0f, 500.0f);
    int p = -(height_ - 150.0f) / (50.0f * M_PI) * 200 * 128 * 2;
    setPosition(0, p);
    setPosition(1, p);
    setPosition(2, p);
  }
  leap_.markFrameAsOld();
}

//--------------------------------------------------------------
void testApp::draw() {
  panel_.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
  switch (key) {
    case '1':
      setPosition(0, motors_[0]->destination + 100);
      break;
    case '\'':
      setPosition(0, motors_[0]->destination - 100);
      break;
    case '2':
      setPosition(1, motors_[1]->destination + 100);
      break;
    case ',':
      setPosition(1, motors_[1]->destination - 100);
      break;
    case '3':
      setPosition(2, motors_[2]->destination + 100);
      break;
    case '.':
      setPosition(2, motors_[2]->destination - 100);
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
void testApp::resetButtonPushed() {
  sendCommand("SETHOME\n");
}

//--------------------------------------------------------------
void testApp::sendCommand(string command) {
  serial_.writeBytes((unsigned char *)command.c_str(), command.length());
}


//--------------------------------------------------------------
void testApp::setPosition(int index, int position) {
  Motor *motor = motors_[index];
  if (motor->destination == position) return;
  motor->destination = position;
  
  stringstream str;
  str << "P " << index << " " << motor->destination << "\n";
  sendCommand(str.str());
}
