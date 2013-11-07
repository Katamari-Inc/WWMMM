//
//  PointItems.h
//  mapamokWWMMM
//
//  Created by Saqoosha on 2013/11/07.
//
//

#pragma once

#include "ofxAssimpModelLoader.h"
#include "ofxTween.h"
#include "ofxJSONElement.h"
#include "CalibrationData.h"


class PointItems : public ofNode {
    
    class Ripple : public ofNode {
        static ofxEasingLinear linear_easing;
        static ofxEasingCubic cubic_easing;
        
    public:
        void setup(ofVec3f p, float delay);
        void customDraw();
        
        ofVec3f position;
        ofxTween radius;
        ofxTween alpha;
    };
    
public:
    static float alpha;
    
    void setup(Json::Value &larges, Json::Value &smalls);
    void remove(int index);
    void removeLarge(int index) {
        remove(index);
    }
    void removeSmall(int index) {
        remove(num_larges_ + index);
    }
    void rippleComplete(int &i);
    void reset();
    void customDraw();
    
    ofMesh mesh_;
    int num_larges_;
    int num_smalls_;
    ofShader shader_;
    deque<Ripple*> ripples_;
};
