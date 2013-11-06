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
    static void setup();
    static ofImage random;
    static float white;
    static float visibility;
    
    CalibrationMesh(ofMesh &mesh);
    
    void reset();
    void project();
    ofVec3f getProjected(int index);
    
    bool loadTexture(string filename);
    bool loadShader(string filename);
    void reloadShader();
    void customDraw();
    
	ofVboMesh object_mesh;
	ofMesh projected_mesh;
    vector<CalibrationPoint> points;

    bool visible;
    bool useTexture;
    ofTexture texture;
    string shader_name;
    ofShader shader;
    bool useShader;
    float polygonOffset;
    
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &archive, unsigned int version) {
        archive & points;
    }
};

