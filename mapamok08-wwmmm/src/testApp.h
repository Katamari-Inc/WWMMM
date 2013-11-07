#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxAssimpModelLoader.h"
#include "ofxProCamToolkit.h"
#include "ofxAutoControlPanel.h"
#include "ofxOsc.h"
#include "ofxTween.h"
#include "LineArt.h"
#include "pystring.h"
#include "MotorManager.h"
#include "CalibrationData.h"
#include "Models.h"
#include "PointItems.h"


class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
    void exit();
	void keyPressed(int key);
    void mouseMoved(int x, int y);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	void setupMesh();
	void setupControlPanel();

	void calibrateWithReferencePoints();
	void saveCalibration();
    void loadCalibration();
    void resetCalibration();
    
    void startIntro();
    void resetToTitle();
    void explodeFireworks();
    void fireworksCompleted(FireworksEventArgs &e);
    
	void render();
	void drawLabeledPoint(int label, ofVec2f position, ofColor color, ofColor bg = ofColor::black, ofColor fg = ofColor::white);
	void drawSelectionMode();
	void drawRenderMode();

	void setb(string name, bool value);
	void seti(string name, int value);
	void setf(string name, float value);
	bool getb(string name);
	int geti(string name);
	float getf(string name);

    inline CalibrationMesh *selectedMesh() { return calibration_meshes_[selected_mesh_]; }
    inline CalibrationPoint &selectedPoint() { return calibration_meshes_[selected_mesh_]->points[selected_point_]; }

    float getClosestPointOnMeshes(float x, float y, int &mesh_index, int &point_index);
	float getClosestImagePoint(float x, float y, int &mesh_index, int &point_index);
    
    
	ofxAutoControlPanel panel;
    
	ofxAssimpModelLoader model_;
	ofEasyCam camera_;
    vector<CalibrationMesh*> calibration_meshes_;
    Stage stage0_;
    ofNode stage_;
    ofImage floor_texture_;
    Ocean ocean_;
    Ball ball_;
    Goal goal_;
    Ripple ripple_;
    ofNode root0_;
    ofNode root_;
    PointItems point_items_;
    deque<Fireworks*> fireworks_;
    
    ofQuaternion target_orientation_;
    ofQuaternion current_orientation_;

    ofxTween white_tween_;
    ofxTween visibility_tween_;
    ofxTween stage_level_tween_;
    ofxTween stage_tilt_tween_;
    ofxTween rotation_amount_tween_;
    ofxTween item_alpha_tween_;
    
    int selected_mesh_;
    int selected_point_;
    int hovering_mesh_;
    int hovering_point_;
	
	cv::Mat rotation_vector_;
    cv::Mat translation_vector_;
	ofMatrix4x4 model_matrix_;
	ofxCv::Intrinsics intrinsics_;
	bool calibration_ready_;
	
    MotorManager motor_manager_;
    bool needs_update_motor_;
    
    string world_state_;
    
    ofxOscReceiver osc_receiver_;
};
