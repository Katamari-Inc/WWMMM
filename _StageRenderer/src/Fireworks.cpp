//
//  Fireworks.cpp
//  StageRenderer
//
//  Created by Saqoosha on 2013/11/05.
//
//

#include "Fireworks.h"


void Fireworks::setup() {
    mesh_ = ofMesh::icosphere(10, 2);
    vector<ofMeshFace> tris = mesh_.getUniqueFaces();
    
    float globalOffset = ofRandomuf();
    for (auto it = tris.begin(); it != tris.end(); it++) {
        ofVec3f v = (it->getVertex(0) + it->getVertex(1) + it->getVertex(2)) / 3.0f;
        float l = floor((1.0f - pow(ofRandomuf(), 3.0f)) * 3.0f);
        ofVec3f center = ofVec3f(v).normalize() * (l + 1) * 8;
        float offset = l / 3.0f + ofRandomuf() * 0.1f + globalOffset;
        float speed = ofRandomuf();
        ofVec3f axis(ofRandomf(), ofRandomf(), ofRandomf());
        axis.normalize();
        for (int i = 0;  i < 3;  i++) {
            it->setVertex(i, it->getVertex(i) - v);
            center_.push_back(center.x);
            center_.push_back(center.y);
            center_.push_back(center.z);
            offset_.push_back(offset);
            speed_.push_back(speed);
            axis_.push_back(axis.x);
            axis_.push_back(axis.y);
            axis_.push_back(axis.z);
        }
    }
    mesh_.setUsage(GL_STATIC_DRAW);
    mesh_.setFromTriangles(tris);
    
    shader_.load("fireworks");
    palette_.loadImage("color_palette.jpg");
    
    setScale(5);
}


void Fireworks::customDraw() {
    ofPushStyle();
    shader_.begin();
    shader_.setUniform1f("t", fmod(ofGetElapsedTimef(), 7.0f));
    shader_.setUniformTexture("color", palette_, 0);
    shader_.setAttribute3fv("center", &center_[0]);
    shader_.setAttribute1fv("offset", &offset_[0]);
    shader_.setAttribute1fv("speed", &speed_[0]);
    shader_.setAttribute3fv("axis", &axis_[0]);
    mesh_.draw(OF_MESH_FILL);
    shader_.end();
    ofPopStyle();
}
