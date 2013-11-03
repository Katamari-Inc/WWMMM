//
//  Models.h
//  mapamokWWMMM
//
//  Created by Saqoosha on 2013/11/03.
//
//

#pragma once

#include "ofxAssimpModelLoader.h"


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
