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
        ball_ = loader_.getMesh(1);
        core_ = loader_.getMesh(0);
        jumping_ = true;
    }
    
    void customDraw() {
        ofPushStyle();
        glDepthMask(GL_FALSE);
        ofSetLineWidth(5);
        ofSetColor(39, 185, 246);
        ball_.drawWireframe();
        glDepthMask(GL_TRUE);
        ofSetColor(0, 255, 255);
        core_.drawFaces();
        ofSetColor(230);
        ball_.drawFaces();
        ofPopStyle();
    }
    
    ofxAssimpModelLoader loader_;
    ofMesh ball_;
    ofMesh core_;
    bool jumping_;
    ofVec3f pre_jump_pos_;
};


class Goal : public ofNode {
public:
    void setup() {
        loader_.setScaleNomalization(false);
        loader_.loadModel("goal.obj");
        goal_ = loader_.getMesh(0);
        setPosition(28.2, 51.607, 187.8);
        reloadShader();
    }
    
    void reloadShader() {
        shader_.unload();
        shader_.load("shaders/goal");
    }
    
    void customDraw() {
        ofRotateY(-ofGetElapsedTimef() * 30);
        ofPushStyle();
        shader_.begin();
        shader_.begin();
        ofMatrix4x4 m;
        shader_.setUniformMatrix4f("modelMatrix", m);
        shader_.setUniform1f("white", CalibrationMesh::white);
        shader_.setUniform1f("visibility", CalibrationMesh::visibility);
        glDepthMask(GL_FALSE);
        ofSetColor(255);
        goal_.drawFaces();
        glDepthMask(GL_TRUE);
        ofSetColor(0, 188, 235, 64);
        ofSetLineWidth(1);
        goal_.drawWireframe();
        shader_.end();
        ofPopStyle();
    }
    
    ofxAssimpModelLoader loader_;
    ofMesh goal_;
    ofShader shader_;
};
