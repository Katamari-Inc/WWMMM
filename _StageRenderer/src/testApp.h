#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxFPSCam.h"
#include "ofxOsc.h"
#include "ofxGui.h"

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
    
    ofIcoSpherePrimitive ball_;
    
    ofxOscReceiver receiver_;
    
    ofxPanel panel_;
    ofxFloatSlider rotation_;
};
