//
//  PointItems.cpp
//  mapamokWWMMM
//
//  Created by Saqoosha on 2013/11/07.
//
//

#include "PointItems.h"


float PointItems::alpha = 1.0;


void PointItems::setup(Json::Value &larges, Json::Value &smalls) {
    mesh_.setMode(OF_PRIMITIVE_POINTS);
    ofFloatColor c(1.0, 1.0, 0.0);
    num_larges_ = larges.size();
    for (int i = 0; i < num_larges_; i += 3) {
        ofVec3f v;
        v.x = larges[i].asFloat() - 512;
        v.z = larges[i + 1].asFloat() - 679;
        v.y = larges[i + 2].asFloat() + 6;
        v *= 0.3;
        mesh_.addVertex(v);
        mesh_.addColor(c);
    }
    num_larges_ /= 3;
    
    c.g = 0.0;
    num_smalls_ = smalls.size();
    for (int i = 0; i < num_smalls_; i += 3) {
        ofVec3f v;
        v.x = smalls[i].asFloat() - 512;
        v.z = smalls[i + 1].asFloat() - 679;
        v.y = smalls[i + 2].asFloat() + 6;
        v *= 0.3;
        mesh_.addVertex(v);
        mesh_.addColor(c);
    }
    
    shader_.load("shaders/pointitems");
    setPosition(0, 10, 0);
}


void PointItems::remove(int index) {
    ofVec3f v = mesh_.getVertex(index);
    mesh_.setColor(index, ofFloatColor(0));
    
    int n = index < num_larges_ ? 10 : 1;
    for (int i = 0; i < n; i++) {
        Ripple *r = new Ripple();
        r->setup(v, i * 200);
        ofAddListener(r->radius.end_E, this, &PointItems::rippleComplete);
        ripples_.push_back(r);
    }
}


void PointItems::rippleComplete(int &i) {
    Ripple *r = ripples_.front();
    ripples_.pop_front();
    delete r;
}


void PointItems::reset() {
    for (int i = 0; i < mesh_.getNumVertices(); i++) {
        mesh_.setColor(i, ofFloatColor(1, i < num_larges_ ? 1 : 0, 0));
    }
}


void PointItems::customDraw() {
    ofPushStyle();
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    shader_.begin();
    shader_.setUniform1f("alpha", alpha);
    mesh_.drawVertices();
    shader_.end();
    
    ofNoFill();
    ofSetColor(255, 0, 0);
    ofSetLineWidth(1);
    ofDisableDepthTest();
    for (auto it = ripples_.begin(); it != ripples_.end(); it++) {
        (*it)->draw();
    }
    ofEnableDepthTest();
    
    ofPopStyle();
}


#pragma mark - Ripple

ofxEasingLinear PointItems::Ripple::linear_easing;
ofxEasingCubic PointItems::Ripple::cubic_easing;


void PointItems::Ripple::setup(ofVec3f p, float delay) {
    position = p;
    radius.setParameters(cubic_easing, ofxTween::easeOut, 0, 10, 1000, delay);
    alpha.setParameters(linear_easing, ofxTween::easeInOut, 255, 0, 500, delay + 500);
    setPosition(p);
    setOrientation(ofVec3f(90, 0, 0));
}


void PointItems::Ripple::customDraw() {
    ofSetColor(17, 195, 210, alpha.update());
    ofCircle(0, 0, 0, radius.update());
}

