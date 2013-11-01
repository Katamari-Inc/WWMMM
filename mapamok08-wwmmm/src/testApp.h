#pragma once

//#define USE_BOOST_SERIALIZATION

#ifdef USE_BUSE_BOOST_SERIALIZATION
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#endif

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxAssimpModelLoader.h"
#include "ofxProCamToolkit.h"
#include "ofxAutoControlPanel.h"
#include "ofxOsc.h"
#include "LineArt.h"
#include "pystring.h"
#include "MotorManager.h"


#ifdef USE_BOOST_SERIALIZATION
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
#endif


class CalibrationPoint {
  public:
    CalibrationPoint() : enabled(false) {}
    ofVec3f object;
    ofVec2f image;
    bool enabled;
    
  private:
#ifdef USE_BOOST_SERIALIZATION
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &archive, unsigned int version) {
        archive & enabled;
    }
#endif
};


class CalibrationMesh : public ofNode {
  public:
    CalibrationMesh(ofMesh &mesh) {
        object_mesh = mesh;
        int n = object_mesh.getNumVertices();
        points.resize(n);
        for (int i = 0; i < n; i++) {
            points[i].object = object_mesh.getVertex(i);
        }
    }
    
    void project() {
        projected_mesh = getProjectedMesh(object_mesh);
    }
    
    ofVec3f getProjected(int index) {
        return projected_mesh.getVertex(index);
    }
    
	ofVboMesh object_mesh;
	ofMesh projected_mesh;
    vector<CalibrationPoint> points;
    
  private:
#ifdef USE_BOOST_SERIALIZATION
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &archive, unsigned int version) {
        archive & points;
    }
#endif
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
    float getClosestPointOnMeshes(float x, float y, int &mesh_index, int &point_index);
	float getClosestImagePoint(float x, float y, int &mesh_index, int &point_index);
    
    
	ofxAutoControlPanel panel;

	ofxAssimpModelLoader model_;
	ofEasyCam camera_;
    vector<CalibrationMesh*> calibration_meshes_;

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
    float pitch_;
    float roll_;
    
    void applyOrientation() {
        ofQuaternion qp, qr;
        qp.makeRotate(pitch_, 1, 0, 0);
        qr.makeRotate(roll_, 0, 0, 1);
        qp *= qr;
        motor_manager_.setOrientation(qp);
    }
    
    ofxOscReceiver osc_receiver_;
};
