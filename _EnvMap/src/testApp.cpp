#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
  camera_.setPosition(0, 0, 1000);
  cubeMap_.loadImages("skybox/px.jpg", "skybox/nx.jpg", "skybox/py.jpg", "skybox/ny.jpg", "skybox/pz.jpg", "skybox/nz.jpg");
  shader_.load("shaders/sphere.vert", "shaders/sphere.frag");

  sphere_.setRadius(300);
  sphere_.setPosition(-400, 0, 0);
  sphere_.setMode( OF_PRIMITIVE_TRIANGLES );
  vector<ofMeshFace> triangles = sphere_.getMesh().getUniqueFaces();
  sphere_.getMesh().setFromTriangles( triangles, true );
  sphere_.enableNormals();
  
  
  box_.set(500);
  box_.setPosition(400, 0, 0);
}

//--------------------------------------------------------------
void testApp::update() {
  camera_.update();
}

//--------------------------------------------------------------
void testApp::draw() {
  camera_.begin(); {
    glDepthMask(GL_FALSE); {
      ofSetColor(255);
      cubeMap_.bind(); {
        ofPushMatrix(); {
          ofTranslate(camera_.getPosition());
          cubeMap_.drawSkybox(1000);
        }
        ofPopMatrix();
      }
      cubeMap_.unbind();
    }
    glDepthMask(GL_TRUE);

    ofEnableDepthTest();
    cubeMap_.bind(); {
      shader_.begin(); {
        shader_.setUniform1i("envMap", 0);
        sphere_.rotate(0.5f, 0.0f, 1.0f, 0.0f);
        sphere_.draw();
        box_.rotate(0.5f, 0.0f, 1.0f, 0.0f);
        box_.draw();
      }
      shader_.end();
    }
    cubeMap_.unbind();
  }
  camera_.end();
  

  ofDisableDepthTest();
  ofSetColor(255);
  cubeMap_.debugDrawCubemapFaces(128);
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
