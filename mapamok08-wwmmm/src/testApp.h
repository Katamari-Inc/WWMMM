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


class CalibrationPoint {
  public:
    CalibrationPoint() : index(0), enabled(false) {}
    int index;
    cv::Point3f object;
    cv::Point2f image;
    bool enabled;
};


class CalibrationMesh {
  public:
    CalibrationMesh(ofMesh &mesh) {
        object_mesh = mesh;
        int n = object_mesh.getNumVertices();
        points.resize(n);
        for (int i = 0; i < n; i++) {
            points[i].index = i;
            points[i].object = ofxCv::toCv(object_mesh.getVertex(i));
        }
    }
    
    ofVec3f getProjected(int index) {
        return projected_mesh.getVertex(index);
    }
    
	ofVboMesh object_mesh;
	ofMesh projected_mesh;
    vector<CalibrationPoint> points;
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
	
    
	ofxAutoControlPanel panel;

	ofxAssimpModelLoader model_;
	ofEasyCam camera_;
    vector<CalibrationMesh*> calibration_meshes_;
//	ofVboMesh object_mesh_;
//	ofMesh projected_mesh_;
//	ofLight light_;
	
//	vector<cv::Point3f> object_points_;
//	vector<cv::Point2f> image_points_;
//	vector<bool> reference_points_;
    
//    ofVec3f hover_point_;
//    ofVec3f selected_point_;
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
