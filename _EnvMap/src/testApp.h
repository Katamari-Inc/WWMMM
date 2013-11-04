#pragma once

#include "ofMain.h"
#include "ofxCubeMap.h"
#include "ofxFPSCam.h"

class testApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
    ofxCubeMap cubeMap_;
    ofxFPSCam camera_;
    ofIcoSpherePrimitive sphere_;
    ofBoxPrimitive box_;
    ofShader shader_;
};
