#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxFPSCam.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "Fireworks.h"


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
};


class Goal : public ofNode {
public:
    void setup() {
        loader_.setScaleNomalization(false);
        loader_.loadModel("goal.obj");
        goal_ = loader_.getMesh(0);
        setPosition(25.2, 61.607, 187.8);
    }
    
    void customDraw() {
        ofRotateY(-ofGetElapsedTimef() * 30);
        ofPushStyle();
        glDepthMask(GL_FALSE);
        ofSetColor(255);
        goal_.drawFaces();
        glDepthMask(GL_TRUE);
        ofSetColor(0, 188, 235, 64);
        ofSetLineWidth(1);
        goal_.drawWireframe();
        ofPopStyle();
    }
    
    ofxAssimpModelLoader loader_;
    ofMesh goal_;
};


class Ocean : public ofNode {
public:
    void setup() {
        int c = 610.0f / 20.0f;
        int r = 910.0f / (sqrt(3) * 10.0f);
        mesh_ = ofMesh::plane(610.0f, 910.0f, c, r);
        ofVec3f *v = mesh_.getVerticesPointer();
        for (int i = 0; i < r; i += 2, v += c * 2) {
            for (int j = 1; j < c - 1; j++) {
                v[j].x -= 10.0f;
            }
        }
        
        face_texture_.loadImage("colors_line.png");
        line_texture_.loadImage("colors_line_dark.png");
        
        shader_.load("ocean");
        
        setOrientation(ofVec3f(0, 90, 90));
    }
    
    void reloadShader() {
        shader_.unload();
        shader_.load("ocean");
    }
    
    void customDraw() {
        ofPushStyle();

        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0, 1.0);
        face_texture_.bind();
        shader_.begin();
        shader_.setUniform1i("tex", 0);
        shader_.setUniform1f("tick", ofGetElapsedTimef());
        mesh_.drawFaces();
        shader_.end();
        face_texture_.unbind();
        glDisable(GL_POLYGON_OFFSET_FILL);

        line_texture_.bind();
        shader_.begin();
        shader_.setUniform1i("tex", 0);
        shader_.setUniform1f("tick", ofGetElapsedTimef());
        mesh_.drawWireframe();
        shader_.end();
        line_texture_.unbind();

        ofPopStyle();
    }
    
    ofMesh mesh_;
    ofImage face_texture_;
    ofImage line_texture_;
    ofShader shader_;
};



class Ripple : public ofNode {
public:
    void setup() {
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
        
        shader_.load("ripple");
        color_.loadImage("color_palette.jpg");
        
        setScale(0.5);
        setOrientation(ofVec3f(0, 90, 90));
    }

    void reloadShader() {
        shader_.unload();
        shader_.load("ripple");
    }
    
    void customDraw() {
        ofPushStyle();
        shader_.begin();
        shader_.setUniform1f("radius", fmod(ofGetElapsedTimef(), 4.0f) / 4.0 * 500);
        shader_.setUniformTexture("palette", color_, 0);
        mesh_.drawFaces();
        shader_.end();
        ofPopStyle();
    }
    
    ofMesh mesh_;
    ofShader shader_;
    ofImage color_;
};



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
    Ball ball_;
    Goal goal_;
    Ocean ocean2_;
    Ripple ripple_;
    Fireworks fireworks_;
    
    ofxOscReceiver receiver_;
    
    ofxPanel panel_;
    ofxFloatSlider rotation_;
};
