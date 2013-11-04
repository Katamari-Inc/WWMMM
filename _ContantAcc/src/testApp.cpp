#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
  ofSetLogLevel(OF_LOG_VERBOSE);
  ofSetVerticalSync(true);
  ofSetFrameRate(60);
  
  box_.set(330, 10, 500);
  box_.setPosition(0, -300, 0);
  
  camera_.setPosition(0, 0, 1000);
  
  panel_.setup();
  panel_.add(target_.setup("Target", 300, -300, 300));
  panel_.add(current_.setup("Current", -300, -300, 300));
  panel_.add(max_velocity_.setup("Max Velocity", 100, 0, 200));
  panel_.add(current_velocity_.setup("Current Velocity", 0, -200, 200));
  panel_.add(acceleration_.setup("Acceleration", 50, 0, 200));
  panel_.add(moving_.setup("Moving", false));
  panel_.add(go_button_.setup("Go"));
  go_button_.addListener(this, &testApp::goClicked);
}

//--------------------------------------------------------------
void testApp::update() {
  if (moving_) {
//    float t = ofGetElapsedTimef() - start_time_;
//    if (t > duration_) {
//      t = duration_;
//      moving_ = false;
//    }
//    current_ = start_position_ + current_velocity_ * t;
    float t = ofGetElapsedTimef() - start_time_;
    float at = max_velocity_ / acceleration_;
    if (duration_ <= t) {
      current_velocity_ = 0;
      current_ = (float)target_;
      moving_ = false;
    } else if (duration_ / 2 < at) {
      if (t < duration_ / 2) {
        current_velocity_ = t * acceleration_ * direction_;
        current_ = t * current_velocity_ / 2 + start_position_;
      } else if (t < duration_) {
        current_velocity_ = (duration_ - t) * acceleration_ * direction_;
        current_ = (target_ - start_position_) - ((duration_ - t) * current_velocity_) / 2 + start_position_;
      }
    } else {
      if (t < at) {
        current_velocity_ = t * acceleration_ * direction_;
        current_ = t * current_velocity_ / 2 + start_position_;
      } else if (t < duration_ - at) {
        current_velocity_ = (float)max_velocity_ * direction_;
        current_ = (at * max_velocity_ / 2 + (t - at) * max_velocity_) * direction_ + start_position_;
      } else if (t < duration_) {
        current_velocity_ = (duration_ - t) * acceleration_ * direction_;
        current_ = (at * max_velocity_ + (duration_ - at * 2) * max_velocity_) * direction_ - (duration_ - t) * current_velocity_ / 2 + start_position_;
      }
    }
    box_.setPosition(0, current_, 0);
  }
}

//--------------------------------------------------------------
void testApp::draw() {
  ofBackground(128);
  camera_.begin();
  ofSetColor(255);
  box_.drawWireframe();
  camera_.end();
  
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
void testApp::goClicked() {
  if (current_ == target_) return;
  
//  float v = max_velocity_;
//  if (target_ < current_) {
//    v *= -1;
//  }
//  current_velocity_ = v;
//  start_time_ = ofGetElapsedTimef();
//  start_position_ = current_;
//  duration_ = (target_ - start_position_) / current_velocity_;
//  moving_ = true;
  
  float d = abs(target_ - current_);
  float at = max_velocity_ / acceleration_;
  if (d < at * max_velocity_) {
    duration_ = 2 * sqrt(d / acceleration_);
    // t2 = duration_ / 2;
    // v = t2 * acceleration_;
    // d = v * t2;
    // d = duration_ / 2 * acceleration_ * duration_ / 2;
  } else {
    duration_ = (d - at * max_velocity_) / max_velocity_ + at * 2;
  }
  start_time_ = ofGetElapsedTimef();
  start_position_ = current_;
  direction_ = current_ < target_ ? 1 : -1;
  moving_ = true;
}

