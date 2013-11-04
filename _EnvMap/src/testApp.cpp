#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    camera_.setPosition(0, 0, 1000);
    cubeMap_.loadImages("skybox/px.jpg", "skybox/nx.jpg", "skybox/py.jpg", "skybox/ny.jpg", "skybox/pz.jpg", "skybox/nz.jpg");
//    cubeMap_.loadImages("colorbox/px.png", "colorbox/nx.png", "colorbox/py.png", "colorbox/ny.png", "colorbox/pz.png", "colorbox/nz.png");
    shader_.load("shaders/sphere.vert", "shaders/sphere.frag");
    
    sphere_.setRadius(300);
    sphere_.setPosition(-400, 0, 0);
    sphere_.setMode(OF_PRIMITIVE_TRIANGLES);
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
                shader_.setUniformMatrix4f("cameraMatrix", camera_.getLocalTransformMatrix().getPtr());
                float a = (ofSignedNoise(ofGetElapsedTimef() * 0.3) + 0.2);
                float b = (ofSignedNoise(ofGetElapsedTimef() * 0.3 + 1000) + 0.2);
                sphere_.rotate(a, 1, 0, 0);
                sphere_.rotate(b, 0, 0, 1);
                sphere_.draw();
                box_.rotate(a, 1, 0, 0);
                box_.rotate(b, 0, 0, 1);
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


void testApp::keyPressed(int key) {
    switch (key) {
        case ' ':
            shader_.unload();
            shader_.load("shaders/sphere.vert", "shaders/sphere.frag");
            break;
    }
}
