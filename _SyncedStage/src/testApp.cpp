#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    receiver_.setup(8001);
    box_.set(330, 10, 500);
    camera_.setPosition(0, 0, 500);
}

//--------------------------------------------------------------
void testApp::update() {
    while (receiver_.hasWaitingMessages()) {
        ofxOscMessage message;
        receiver_.getNextMessage(&message);
        if (message.getAddress() == "/world/orientation") {
            ofQuaternion q;
            q.set(message.getArgAsFloat(0), message.getArgAsFloat(1), message.getArgAsFloat(2), message.getArgAsFloat(3));
            cout << q << endl;
            box_.setOrientation(q);
        }
    }
    camera_.update();
}

//--------------------------------------------------------------
void testApp::draw() {
    ofBackground(50);
    
    //  ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
    camera_.begin();
    ofNoFill();
    ofSetColor(255);
    box_.drawWireframe();
    camera_.end();
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
