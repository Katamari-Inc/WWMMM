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
  
  panel_.setup();
  reset_button_.addListener(this, &testApp::resetButtonPushed);
  panel_.add(reset_button_.setup("SET AS HOME"));

  height_ = 0;
  box_.set(540, 5, 380);
  
  is_ready_ = false;
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
//      } else if (tokens[0] == "Ready") {
        is_ready_ = true;
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
  if (is_ready_ && leap_.isFrameNew() && hands_.size()) {
    ofQuaternion q;
    q.makeRotate(ofPoint(0, -1, 0), hands_[0].handNormal);
    ofVec3f r = q.getEuler();
    float deg = 20;
    rot_.x += (ofClamp(r.x, -deg, deg) - rot_.x) / 20.f;
    rot_.z += (ofClamp(r.z, -deg, deg) - rot_.z) / 20.f;
    mat_.makeIdentityMatrix();
    mat_.postMultRotate(-rot_.z, 1, 0, 0);
    mat_.postMultRotate(-rot_.x, 0, 0, 1);
    height_ += (hands_[0].handPos.y - height_) / 20.f;
//    mat_.postMultTranslate(0, ofClamp(h, 0.f, 500.0f), 0);

    ofVec3f p = mat_ * ofVec3f(225, 0, 190);
    setPosition(1, 200 + p.y + height_);
    p = mat_ * ofVec3f(-225, 0, 190);
    setPosition(2, 200 + p.y + height_);
    p = mat_ * ofVec3f(0, 0, -190);
    setPosition(0, 200 + p.y + height_);
  }
  leap_.markFrameAsOld();
}

//--------------------------------------------------------------
void testApp::draw() {
  ofBackground(50);
  
  panel_.draw();
  
  ofTranslate(ofGetWidth() * .5, ofGetHeight() * .5 + 300 - height_, -500);
  ofMultMatrix(mat_);
  ofNoFill();
  ofSetColor(255);
  box_.drawWireframe();
  
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
  switch (key) {
    case '1':
      setPosition(0, motors_[0]->destination + 1);
      break;
    case '\'':
      setPosition(0, motors_[0]->destination - 1);
      break;
    case '2':
      setPosition(1, motors_[1]->destination + 1);
      break;
    case ',':
      setPosition(1, motors_[1]->destination - 1);
      break;
    case '3':
      setPosition(2, motors_[2]->destination + 1);
      break;
    case '.':
      setPosition(2, motors_[2]->destination - 1);
      break;
    case 'r':
      setPosition(0, 0);
      setPosition(1, 0);
      setPosition(2, 0);
      break;
    case ' ':
      sendCommand("HALT");
      is_ready_ = false;
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
  sendCommand("SETHOME");
}

//--------------------------------------------------------------
void testApp::sendCommand(string command) {
  cout << "COMMAND: " << command << endl;
  serial_.writeBytes((unsigned char *)command.c_str(), command.length());
  serial_.writeByte('\n');
}


//--------------------------------------------------------------
void testApp::setPosition(int index, float millimeter) {
  Motor *motor = motors_[index];
  if (motor->destination == millimeter) return;
  motor->destination = millimeter;
  
  int position = -millimeter / (50.0f * M_PI) * 200 * 128;
  stringstream str;
  str << "P " << index << " " << position;
  sendCommand(str.str());
}
