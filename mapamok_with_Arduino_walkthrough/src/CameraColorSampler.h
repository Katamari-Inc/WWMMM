//
//  CameraColorSampler.h
//  vase
//
//  Created by kikko on 11/16/12.
//
//

#ifndef __vase__CameraColorSampler__
#define __vase__CameraColorSampler__

#include "ofMain.h"
#include "ofxMacamPs3Eye.h"
#include "ofxCv.h"
#include "ControlPanel.h"

using namespace ofxCv;

class CamDebugDraw : public ofBaseDraws {
public:
    CamDebugDraw() {
        cam = NULL;
    };
    virtual ~CamDebugDraw(){};
    
    void draw(float x, float y) {
        draw(x, y, getWidth(), getHeight());
    };
    
    void draw(float x, float y, float w, float h) {
        cam->draw(x, y, w, h);
        ofPushMatrix();
        float scaleX = w / getWidth();
        float scaleY = h / getHeight();
        ofPushStyle();
        ofNoFill();
        ofSetColor(255, 0, 0);
        ofRect(x+getf("roi_x")*getWidth()* scaleX,
               y+getf("roi_y")*getHeight()* scaleY,
               getf("roi_width") * getWidth() * scaleX,
               getf("roi_height") * getHeight() * scaleY);
        ofPopStyle();
        ofPopMatrix();
    };
    
    float getHeight() { return 640.0f; };
	float getWidth() { return 320.0f; };
    
    ofBaseDraws* cam;
};


class CameraColorSampler {
    
public:
    CameraColorSampler();
    ~CameraColorSampler();
    
    void setup();
    void setupControlTab();
    void update();
    
    void panelValueChanged(guiCallbackData& data);
    float avg_color[3];
    
private:
    
    void listen(guiBaseObject* item);
    void initValues();
    
    CamDebugDraw debugDraw;
    ofxMacamPs3Eye ps3eye;
};

#endif /* defined(__vase__CameraColorSampler__) */
