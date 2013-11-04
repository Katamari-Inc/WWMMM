#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxFPSCam.h"
#include "ofxOsc.h"
#include "ofxGui.h"


class Ball : public ofNode {
public:
    void setup() {
        loader_.loadModel("ball.obj");
        ofPoint sceneMax = loader_.getSceneMax();
        ofPoint sceneMin = loader_.getSceneMin();
        float size = MAX(sceneMax.x - sceneMin.x, MAX(sceneMax.y - sceneMin.y, sceneMax.z - sceneMin.z));
        setScale(5.4 * 2 * 0.3 / size);
        ball_ = loader_.getMesh(0);
    }
    
    void customDraw() {
        ofPushStyle();
        ofSetLineWidth(1);
        ofSetColor(255, 0, 0, 128);
        ball_.drawWireframe();
        ofPopStyle();
    }

    ofxAssimpModelLoader loader_;
    ofMesh ball_;
};


class testApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxAssimpModelLoader loader_;
    ofxFPSCam camera_;
    ofMesh floors_;
    ofMesh elevators_;
    ofMesh bridges_;
    ofMesh ocean_;
    ofImage floors_texture_;
    ofImage ocean_texture_;
    ofMatrix4x4 stage_transform_matrix_;
    Ball ball_;
    
    ofxOscReceiver receiver_;
    
    ofxPanel panel_;
    ofxFloatSlider rotation_;
};
