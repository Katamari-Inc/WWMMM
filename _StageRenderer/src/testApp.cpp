#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);

    if (!loader_.loadModel("stage.obj")) {
        ofExit(1);
    }
    ocean_ = loader_.getMesh(0);
    bridges_ = loader_.getMesh(1);
    elevators_ = loader_.getMesh(2);
    floors_ = loader_.getMesh(3);

    ofDisableArbTex();
    floors_texture_.loadImage("http-aid-dcc.png");
    ocean_texture_.loadImage("ocean.png");
    
    ball_.setRadius(2);
    ball_.setResolution(0);
    ball_.setPosition(0, 0, 0);
    
    receiver_.setup(8001);
    
    panel_.setup("Hoge");
    panel_.add(rotation_.setup("Rotation", 1.0f, 0.0f, 1.0f));
}

//--------------------------------------------------------------
void testApp::update() {
    camera_.update();
    
    while (receiver_.hasWaitingMessages()) {
        ofxOscMessage message;
        receiver_.getNextMessage(&message);
        string address = message.getAddress();
        if (address == "/ball/position") {
            ball_.setPosition(message.getArgAsFloat(0) * .3, message.getArgAsFloat(1) * .3, message.getArgAsFloat(2) * .3);
        } else if (address == "/ball/orientation") {
            ball_.setOrientation(ofQuaternion(message.getArgAsFloat(0), message.getArgAsFloat(1), message.getArgAsFloat(2), message.getArgAsFloat(3)));
        } else if (address == "/world/orientation") {
            ofQuaternion q(message.getArgAsFloat(0), message.getArgAsFloat(1), message.getArgAsFloat(2), message.getArgAsFloat(3));
            q.slerp(rotation_, ofQuaternion(), q);
            stage_transform_matrix_.makeRotationMatrix(q);
        }
    }
}

//--------------------------------------------------------------
void testApp::draw() {
    ofBackground(128);
    
    ofVec3f p = ball_.getPosition() * stage_transform_matrix_;

    ofEnableDepthTest();
    camera_.begin();
    ofDrawAxis(100);
    
    ocean_texture_.bind();
    ocean_.drawFaces();
    ocean_texture_.unbind();
    
    ofPushMatrix();
    ofTranslate(0, -p.y + 50, 0);
    ofMultMatrix(stage_transform_matrix_);
    
    ofPushStyle();
    ofSetColor(64, 148, 65);
    bridges_.drawFaces();
    ofPopStyle();
    
    ofPushStyle();
    ofSetColor(219, 79, 79);
    elevators_.drawFaces();
    ofPopStyle();
    
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);
    floors_texture_.bind();
    floors_.drawFaces();
    floors_texture_.unbind();
    glDisable(GL_POLYGON_OFFSET_FILL);
    
//    ofPopMatrix();
    
    ofPushStyle();
    ofNoFill();
    ofSetColor(255, 0, 0);
    ofSetLineWidth(2);
    ofPushMatrix();
    ofTranslate(0, 20, 0);
    ball_.drawWireframe();
    ofPopMatrix();
    ofPopStyle();
    
    ofPopMatrix();
    
    camera_.end();
    
    ofDisableDepthTest();
    panel_.draw();

    
//    stringstream s;
//    ofVec3f p = ball_.getPosition();
//    s << "X: " << ofToString(p.x, 2) << endl
//    << "Y: " << ofToString(p.y, 2) << endl
//    << "Y: " << ofToString(p.z, 2);
//    ofDrawBitmapStringHighlight(s.str(), 10, 20);
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
