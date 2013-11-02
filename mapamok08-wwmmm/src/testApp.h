#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxAssimpModelLoader.h"
#include "ofxProCamToolkit.h"
#include "ofxAutoControlPanel.h"
#include "ofxOsc.h"
#include "LineArt.h"
#include "pystring.h"
#include "MotorManager.h"

#include "boost/serialization/serialization.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"


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
    CalibrationMesh(ofMesh &mesh) : useTexture(false) {
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
    }
    
    void customDraw() {
        ofPushStyle();
        ofSetColor(color);
        if (useTexture) texture.bind();
        object_mesh.drawFaces();
        if (useTexture) texture.unbind();
        ofPopStyle();
    }
    
	ofVboMesh object_mesh;
	ofMesh projected_mesh;
    vector<CalibrationPoint> points;
    bool useTexture;
    ofImage texture;
    ofColor color;
    
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &archive, unsigned int version) {
        archive & points;
    }
};


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


class testApp : public ofBaseApp {
public:
	void setb(string name, bool value);
	void seti(string name, int value);
	void setf(string name, float value);
	bool getb(string name);
	int geti(string name);
	float getf(string name);
    
    inline CalibrationMesh *selectedMesh() {
        return calibration_meshes_[selected_mesh_];
    }
    inline CalibrationPoint &selectedPoint() {
        return calibration_meshes_[selected_mesh_]->points[selected_point_];
    }
	
	void setup();
	void update();
	void draw();
    void exit();
	void keyPressed(int key);
    void mouseMoved(int x, int y);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	void setupControlPanel();
	void setupMesh();
	void drawLabeledPoint(int label, ofVec2f position, ofColor color, ofColor bg = ofColor::black, ofColor fg = ofColor::white);
	void updateRenderMode();
	void drawSelectionMode();
	void drawRenderMode();
	void render();
    void loadCalibration();
	void saveCalibration();
    void resetCalibration();
    float getClosestPointOnMeshes(float x, float y, int &mesh_index, int &point_index);
	float getClosestImagePoint(float x, float y, int &mesh_index, int &point_index);
    
    
	ofxAutoControlPanel panel;
    
	ofxAssimpModelLoader model_;
	ofEasyCam camera_;
    vector<CalibrationMesh*> calibration_meshes_;
    Stage stage_;
    ofImage floor_texture_;
    ofImage ocean_texture_;
    ofIcoSpherePrimitive ball_;
    
    int selected_mesh_;
    int selected_point_;
    int hovering_mesh_;
    int hovering_point_;
	
	cv::Mat rotation_vector_;
    cv::Mat translation_vector_;
	ofMatrix4x4 model_matrix_;
	ofxCv::Intrinsics intrinsics_;
	bool calibration_ready_;
	
    //	Poco::Timestamp lastFragTimestamp, lastVertTimestamp;
    //	ofShader shader;
    
    MotorManager motor_manager_;
    
    ofxOscReceiver osc_receiver_;
};
