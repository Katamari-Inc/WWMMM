#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
  ofSetLogLevel(OF_LOG_VERBOSE);
  ofSetVerticalSync(true);
  ofSetFrameRate(60);
  
  loader_.loadModel("stage.dae");
  floors_ = loader_.getMeshHelper(0);
  elevators_ = loader_.getMeshHelper(1);
  bridges_ = loader_.getMeshHelper(2);
  ocean_ = loader_.getMeshHelper(3);
}

//--------------------------------------------------------------
void testApp::update() {
  camera_.update();
}

//--------------------------------------------------------------
void testApp::draw() {
  ofBackground(0);
 
  ofEnableDepthTest();
  camera_.begin();
  ofDrawAxis(100);

  ofPushMatrix();
  ofMultMatrix(ocean_.matrix);
  ocean_.getTexturePtr()->bind();
  ocean_.vbo.drawElements(GL_TRIANGLES, ocean_.indices.size());
  ocean_.getTexturePtr()->unbind();
  ofPopMatrix();
  
//  ofTranslate(0, sin(ofGetElapsedTimef()) * 200, 0);
  
  ofPushStyle();
  ofPushMatrix();
  ofMultMatrix(bridges_.matrix);
  ofSetColor(64, 148, 65);
  bridges_.vbo.drawElements(GL_TRIANGLES, bridges_.indices.size());
  ofPopMatrix();
  ofPopStyle();

  ofPushStyle();
  ofPushMatrix();
  ofMultMatrix(elevators_.matrix);
  ofSetColor(219, 79, 79);
  elevators_.vbo.drawElements(GL_TRIANGLES, elevators_.indices.size());
  ofPopMatrix();
  ofPopStyle();
  
  ofMultMatrix(floors_.matrix);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.0, 1.0);
  floors_.getTexturePtr()->bind();
  floors_.vbo.drawElements(GL_TRIANGLES, floors_.indices.size());
  floors_.getTexturePtr()->unbind();
  glDisable(GL_POLYGON_OFFSET_FILL);
  ofPushMatrix();
  ofPopMatrix();

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
