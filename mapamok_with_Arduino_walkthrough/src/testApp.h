#pragma once

#define USE_CAMERA_COLOR_SAMPLER

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxAssimpModelLoader.h"
#include "ofxProCamToolkit.h"
#include "LineArt.h"
#include "ControlPanel.h"
#include "MotorControl.h"

#ifdef USE_CAMERA_COLOR_SAMPLER
#include "CameraColorSampler.h"
#endif

class testApp : public ofBaseApp {
public:
	
    void setup();
	void update();
	void draw();
	void exit();
	void keyPressed(int key);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	void setupControlPanel();
	void setupMesh();
	void drawLabeledPoint(int label, ofVec2f position, ofColor color, ofColor bg = ofColor::black, ofColor fg = ofColor::white);
	void updateRenderMode();
	void drawSelectionMode();
	void drawRenderMode();
	void render();
	void saveCalibration();
	
	ofxAssimpModelLoader model;	
	ofEasyCam cam;
	ofVboMesh objectMesh;
	ofMesh imageMesh;
	
	vector<cv::Point3f> objectPoints;
	vector<cv::Point2f> imagePoints;
	vector<bool> referencePoints;
	
	cv::Mat rvec, tvec;
	ofMatrix4x4 modelMatrix;
	ofxCv::Intrinsics intrinsics;
	bool calibrationReady;
	
	Poco::Timestamp lastFragTimestamp, lastVertTimestamp;
	ofShader shader;
    
#ifdef USE_CAMERA_COLOR_SAMPLER
    CameraColorSampler camSampler;
#endif
    
    MotorControl motor;
};
