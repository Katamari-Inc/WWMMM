//
//  CalibrationData.h
//  mapamokWWMMM
//
//  Created by Saqoosha on 2013/11/03.
//
//

#pragma once

#include "boost/serialization/serialization.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "ofxProCamToolkit.h"


namespace boost {
    namespace serialization {
        template <class Archive>
        void serialize(Archive &archive, ofVec3f &v, const unsigned int version) {
            archive & v.x & v.y & v.z;
        }
        template <class Archive>
        void serialize(Archive &archive, ofVec2f &v, const unsigned int version) {
            archive & v.x & v.y;
        }
    }
}


class CalibrationPoint {
public:
    CalibrationPoint() : enabled(false) {}
    ofVec3f object;
    ofVec2f image;
    bool enabled;
    
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &archive, unsigned int version) {
        archive & object & image & enabled;
    }
};


class CalibrationMesh : public ofNode {
public:
    static float white;
    static float visibility;
    
    CalibrationMesh(ofMesh &mesh) : useTexture(false), useShader(false) {
        object_mesh = mesh;
        int n = object_mesh.getNumVertices();
        points.resize(n);
        reset();
    }
    
    void reset() {
        for (int i = 0; i < points.size(); i++) {
            points[i].object = object_mesh.getVertex(i);
            points[i].image.set(0);
            points[i].enabled = false;
        }
    }
    
    void project() {
        projected_mesh = getProjectedMesh(object_mesh);
    }
    
    ofVec3f getProjected(int index) {
        return projected_mesh.getVertex(index);
    }
    
    bool loadTexture(string filename) {
        useTexture = texture.loadImage(filename);
        return useTexture;
    }
    
    bool loadShader(string filename) {
        shader_name = filename;
        useShader = shader.load(filename);
        return useShader;
    }
    
    void reloadShader() {
        shader.unload();
        useShader = shader.load(shader_name);
    }
    
    void customDraw() {
        ofPushStyle();
        ofSetColor(color);
        if (useTexture) texture.bind();
        if (useShader) {
            shader.begin();
            shader.setUniformMatrix4f("modelMatrix", getLocalTransformMatrix());
            if (useTexture) shader.setUniform1i("tex", 0);
            shader.setUniform1f("white", CalibrationMesh::white);
            shader.setUniform1f("visibility", CalibrationMesh::visibility);
        }
        object_mesh.drawFaces();
        if (useShader) shader.end();
        if (useTexture) texture.unbind();
        ofPopStyle();
    }
    
	ofVboMesh object_mesh;
	ofMesh projected_mesh;
    vector<CalibrationPoint> points;
    bool useTexture;
    ofImage texture;
    ofColor color;
    string shader_name;
    ofShader shader;
    bool useShader;
    
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &archive, unsigned int version) {
        archive & points;
    }
};

