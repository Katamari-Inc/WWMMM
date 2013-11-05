//
//  Models.h
//  mapamokWWMMM
//
//  Created by Saqoosha on 2013/11/03.
//
//

#pragma once

#include "ofxAssimpModelLoader.h"
#include "CalibrationData.h"


class Stage : public ofNode {
public:
    void roll(float degrees) {
        euler_angles_.z += ofDegToRad(degrees);
        update();
    }
    void pitch(float degrees) {
        euler_angles_.x += ofDegToRad(degrees);
        update();
    }
    void resetTransform() {
        ofNode::resetTransform();
        euler_angles_.set(0);
    }
private:
    void update() {
		float c1 = cos(euler_angles_.x / 2);
		float c2 = cos(euler_angles_.y / 2);
		float c3 = cos(euler_angles_.z / 2);
		float s1 = sin(euler_angles_.x / 2);
		float s2 = sin(euler_angles_.y / 2);
		float s3 = sin(euler_angles_.z / 2);
        ofQuaternion q(s1 * c2 * c3 + c1 * s2 * s3,
                       c1 * s2 * c3 - s1 * c2 * s3,
                       c1 * c2 * s3 + s1 * s2 * c3,
                       c1 * c2 * c3 - s1 * s2 * s3);
        setOrientation(q);
    }
    ofVec3f euler_angles_;
};


class Ball : public ofNode {
public:
    void setup();
    void customDraw();
    
    ofxAssimpModelLoader loader_;
    ofMesh ball_;
    ofMesh core_;
    bool jumping_;
    ofVec3f pre_jump_pos_;
    bool visible_;
};


class Goal : public ofNode {
public:
    void setup();
    void reloadShader();
    void customDraw();
    
    ofxAssimpModelLoader loader_;
    ofMesh goal_;
    ofShader shader_;
};


class Ripple : public ofNode {
public:
    void setup();
    void start(ofVec3f position);
    void reloadShader();
    void customDraw();
    
    ofMesh mesh_;
    ofShader shader_;
    ofImage color_;
    float start_time_;
};

