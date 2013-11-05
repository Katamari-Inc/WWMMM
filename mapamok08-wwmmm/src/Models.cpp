//
//  Models.cpp
//  mapamokWWMMM
//
//  Created by Saqoosha on 2013/11/05.
//
//

#include "Models.h"


#pragma mark Ball

void Ball::setup() {
    loader_.loadModel("ball.obj");
    ofPoint sceneMax = loader_.getSceneMax();
    ofPoint sceneMin = loader_.getSceneMin();
    float size = MAX(sceneMax.x - sceneMin.x, MAX(sceneMax.y - sceneMin.y, sceneMax.z - sceneMin.z));
    setScale(5.4 * 2 * 0.3 / size);
    ball_ = loader_.getMesh(1);
    core_ = loader_.getMesh(0);
    jumping_ = true;
}


void Ball::customDraw() {
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


//--------------------------------------------------------------------------------------------------
#pragma mark - Goal

void Goal::setup() {
    loader_.setScaleNomalization(false);
    loader_.loadModel("goal.obj");
    goal_ = loader_.getMesh(0);
    setPosition(28.2, 51.607, 187.8);
    reloadShader();
}


void Goal::reloadShader() {
    shader_.unload();
    shader_.load("shaders/goal");
}


void Goal::customDraw() {
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


//--------------------------------------------------------------------------------------------------
#pragma mark - Ripple

void Ripple::setup() {
    float l = 20;
    float lx = l;
    float lx2 = cos(ofDegToRad(60)) * l;
    float ly = cos(ofDegToRad(30)) * l;
    float nx, ny;
    nx= ny = 50;
    float centerX = ((nx - 1) * lx + lx2) / 2;
    float centerY = (ny - 1) * ly / 2;
    for (int y = 0; y < ny; y++) {
        float offsetX = y % 2 ? 0 : lx2;
        for (int x = 0; x < nx; x++) {
            mesh_.addVertex(ofVec3f(x * lx + offsetX - centerX, y * ly - centerY, 0));
            if (x > 0 && y > 0) {
                int i = (y - 1) * nx + x - 1;
                int j = y * nx + x - 1;
                if (y % 2) {
                    mesh_.addTriangle(i, j + 1, j);
                    mesh_.addTriangle(i, i + 1, j + 1);
                } else {
                    mesh_.addTriangle(i, i + 1, j);
                    mesh_.addTriangle(i + 1, j + 1, j);
                }
            }
        }
    }
    vector<ofMeshFace> tris = mesh_.getUniqueFaces();
    for (auto it = tris.begin();  it != tris.end(); it++) {
        ofVec3f v = (it->getVertex(0) + it->getVertex(1) + it->getVertex(2)) / 3;
        it->setNormal(0, v);
        it->setNormal(1, v);
        it->setNormal(2, v);
        ofFloatColor c(ofRandomf(), ofRandomf(), ofRandomf());
        it->setColor(0, c);
        it->setColor(1, c);
        it->setColor(2, c);
    }
    mesh_.setFromTriangles(tris);
    
    shader_.load("shaders/ripple");
    color_.loadImage("color_palette.jpg");
    
    setScale(0.5);
    setOrientation(ofVec3f(0, 90, 90));
    
    start_time_ = 0;
}


void Ripple::start(ofVec3f position) {
    setPosition(position);
    start_time_ = ofGetElapsedTimef();
}


void Ripple::reloadShader() {
    shader_.unload();
    shader_.load("shaders/ripple");
}


void Ripple::customDraw() {
    float d = ofGetElapsedTimef() - start_time_;
    if (d > 4.0f) return;
    
    ofPushStyle();
    shader_.begin();
    shader_.setUniform1f("radius", fmod(ofGetElapsedTimef(), 4.0f) / 4.0 * 500);
    shader_.setUniformTexture("palette", color_, 0);
    mesh_.drawFaces();
    shader_.end();
    ofPopStyle();
}

