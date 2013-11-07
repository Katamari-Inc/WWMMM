#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxFPSCam.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "ofxTween.h"
#include "ofxJSONElement.h"
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
        mesh_.setMode(OF_PRIMITIVE_TRIANGLES);
        
        float w = 910;
        float h = 610;
        float l = 20;
        float lx = l;
        float lx2 = cos(ofDegToRad(60)) * l;
        float ly = cos(ofDegToRad(30)) * l;
        float nx = ceil(w / lx) + 2;
        float ny = ceil(h / ly) + 2;
        float centerX = ((nx - 1) * lx + lx2) / 2;
        float centerY = (ny - 1) * ly / 2;
        for (int y = 0; y < ny; y++) {
            float offsetX = y % 2 ? 0 : lx2;
            for (int x = 0; x < nx; x++) {
                ofVec3f v = ofVec3f(x * lx + offsetX - centerX, 0, y * ly - centerY);
                mesh_.addVertex(v);
                mesh_.addTexCoord(ofVec2f((v.x - centerX) / w, (v.z - centerY) / h));
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
        for (auto it = tris.begin(); it != tris.end(); it++) {
            ofFloatColor c(ofRandomuf(), ofRandomuf(), ofRandomuf());
            it->setColor(0, c);
            it->setColor(1, c);
            it->setColor(2, c);
        }
        mesh_.setFromTriangles(tris);

        face_texture_.loadImage("colors_line.png");
        line_texture_.loadImage("colors_line_dark.png");
        
        shader_.load("ocean");
        
//        setOrientation(ofVec3f(0, 30, 0));
    }
    
    void reloadShader() {
        shader_.unload();
        shader_.load("ocean");
    }
    
    void customDraw() {
        ofPushStyle();

        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0, 1.0);
//        face_texture_.bind();
//        line_texture_.bind();
        shader_.begin();
//        shader_.setUniform1i("tex", 0);
        shader_.setUniformTexture("tex", line_texture_, 0);
        shader_.setUniform1f("tick", ofGetElapsedTimef());
        shader_.setUniformMatrix4f("modelMatrix", getLocalTransformMatrix());
        shader_.setUniform1f("white", white);
        mesh_.drawFaces();
        shader_.end();
//        face_texture_.unbind();
//        line_texture_.unbind();
        glDisable(GL_POLYGON_OFFSET_FILL);

//        line_texture_.bind();
//        shader_.begin();
//        shader_.setUniform1i("tex", 0);
//        shader_.setUniform1f("tick", ofGetElapsedTimef());
//        shader_.setUniformMatrix4f("modelMatrix", getLocalTransformMatrix());
//        mesh_.drawWireframe();
//        shader_.end();
//        line_texture_.unbind();

        ofPopStyle();
    }
    
    ofMesh mesh_;
    ofImage face_texture_;
    ofImage line_texture_;
    ofShader shader_;
    float white;
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


class PointItems : public ofNode {
    
    class Ripple : public ofNode {
        static ofxEasingLinear linear_easing;
        static ofxEasingCubic cubic_easing;
    public:
        void setup(ofVec3f p, float delay) {
            position = p;
            radius.setParameters(cubic_easing, ofxTween::easeOut, 0, 10, 1000, delay);
            alpha.setParameters(linear_easing, ofxTween::easeInOut, 255, 0, 500, delay + 500);
            setPosition(p);
            setOrientation(ofVec3f(90, 0, 0));
        }
        void customDraw() {
            ofSetColor(17, 195, 210, alpha.update());
            ofCircle(0, 0, 0, radius.update());
        }
        ofVec3f position;
        ofxTween radius;
        ofxTween alpha;
    };
    
public:
    static float white;
    void setup(Json::Value &larges, Json::Value &smalls) {
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

        shader_.load("pointitems");
        setPosition(0, 20, 0);
    }
    
    void removeLarge(int index) {
        remove(index);
    }
    
    void removeSmall(int index) {
        remove(num_larges_ + index);
    }
    
    void remove(int index) {
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
    
    void rippleComplete(int &i) {
        Ripple *r = ripples_.front();
        ripples_.pop_front();
        delete r;
    }
    
    void reset() {
        for (int i = 0; i < mesh_.getNumVertices(); i++) {
            mesh_.setColor(i, ofFloatColor(1, i < num_larges_ ? 1 : 0, 0));
        }
    }
    
    void customDraw() {
        ofPushStyle();
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
        shader_.begin();
        shader_.setUniform1f("white", white);
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
    
    ofMesh mesh_;
    int num_larges_;
    int num_smalls_;
    ofShader shader_;
    deque<Ripple*> ripples_;
};


class testApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void goClicked();
    
    ofxAssimpModelLoader loader_;
    ofxFPSCam camera_;
    ofMesh floors_;
    ofMesh elevators_;
    ofMesh bridges_;
    ofMesh ocean_;
//    ofImage floors_texture_;
    ofTexture floors_texture_;
    ofImage ocean_texture_;
    ofMatrix4x4 stage_transform_matrix_;
    Ball ball_;
    Goal goal_;
    Ocean ocean2_;
    Ripple ripple_;
    Fireworks fireworks_;
    PointItems point_items_;
    ofShader random_shader_;
    ofImage random_texture_;
    
    ofxOscReceiver receiver_;
    
    ofxPanel panel_;
    ofxFloatSlider rotation_;
    ofxFloatSlider white_;
    ofxButton go_button_;

    ofxTween white_tween_;
};

