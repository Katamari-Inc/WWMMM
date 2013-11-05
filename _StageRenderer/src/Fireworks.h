//
//  Fireworks.h
//  StageRenderer
//
//  Created by Saqoosha on 2013/11/05.
//
//

#pragma once

#include "ofMain.h"


class Fireworks : public ofNode {
public:
    void setup();
    void customDraw();
    
    ofVboMesh mesh_;
    vector<float> center_;
    vector<float> offset_;
    vector<float> speed_;
    vector<float> axis_;
    ofShader shader_;
    ofImage palette_;
};
