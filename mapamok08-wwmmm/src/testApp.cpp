#include "testApp.h"


using namespace ofxCv;
using namespace cv;


float CalibrationMesh::white = 1.0f;
float CalibrationMesh::visibility = 1.0f;


#pragma mark oF Event Handlers

void testApp::setup() {
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
    ofSetVerticalSync(true);
//    ofSetFullscreen(true);
	ofSetWindowTitle("mapamokWWMMM");
    ofSetEscapeQuitsApp(false);
    
    selected_mesh_ = -1;
    selected_point_ = -1;
    hovering_mesh_ = -1;
    hovering_point_ = -1;
    
    calibration_ready_ = false;
    setupMesh();
    setupControlPanel();
    
    ofxXmlSettings settings;
    string port = "";
    if (settings.loadFile("settings.xml")) {
        port = settings.getValue("port", "/dev/tty.usbmodem1421");
        ofLog(OF_LOG_NOTICE, "Port: %s", port.c_str());
    }
    motor_manager_.setup(port);
    needs_update_motor_ = false;
    
    osc_receiver_.setup(8001);
}


void testApp::update() {
	if (getb("selectionMode")) {
        camera_.enableMouseInput();
	} else {
		calibrateWithReferencePoints();
		camera_.disableMouseInput();
	}
    
    ofxOscMessage message;
    while (osc_receiver_.getNextMessage(&message)) {
        string address = message.getAddress();
        if (address == "/ball/position") {
            ball_.setPosition(message.getArgAsFloat(0) * .3, message.getArgAsFloat(1) * .3 + 10, message.getArgAsFloat(2) * .3);
            if (!ball_.jumping_) {
                ball_.pre_jump_pos_.set(ball_.getPosition());
            }
            needs_update_motor_ = true;
        } else if (address == "/ball/orientation") {
            ball_.setOrientation(ofQuaternion(message.getArgAsFloat(0), message.getArgAsFloat(1), message.getArgAsFloat(2), message.getArgAsFloat(3)));
        } else if (address == "/ball/jumping") {
            ball_.jumping_ = message.getArgAsInt32(0);
        } else if (address == "/world/orientation") {
            ofQuaternion q;
            q.set(message.getArgAsFloat(0), message.getArgAsFloat(1), message.getArgAsFloat(2), message.getArgAsFloat(3));
            q.slerp(getf("rotationAmount"), ofQuaternion(), q);
            stage0_.setOrientation(q);
            needs_update_motor_ = true;
        } else if (address == "/world/state") {
            cout << "World state: " << message.getArgAsString(0) << endl;
        }
    }
    
    ofQuaternion q;
    q.slerp(getf("rotationEasing"), stage_.getOrientationQuat(), stage0_.getOrientationQuat());
    stage_.setOrientation(q);
    root_.setPosition(root0_.getPosition().interpolate(root_.getPosition(), 1 - getf("elevationEasing")));
    needs_update_motor_ = true;
    
    if (!getb("selectionMode")) {
        stage_.setPosition(0, 0, 0);
        if (!getb("setupMode")) {
            ofVec3f p = ball_.pre_jump_pos_;
            p.y = ofClamp(p.y, 40, 90) - 90;
            p = p * stage_.getLocalTransformMatrix();
            stage_.setPosition(0, -p.y, 0);
        }
        motor_manager_.setTransformMatrix(stage_.getGlobalTransformMatrix());
        needs_update_motor_ = false;
    }
    
    motor_manager_.update();
}


void testApp::draw() {
	ofBackground(geti("backgroundColor"));
    
    if (getb("reloadShader")) {
        auto it = calibration_meshes_.begin();
        for (; it != calibration_meshes_.end(); it++) {
            (*it)->reloadShader();
        }
        setb("reloadShader", false);
    }
    if (getb("loadCalibration")) {
		loadCalibration();
		setb("loadCalibration", false);
	}
	if (getb("saveCalibration")) {
		saveCalibration();
		setb("saveCalibration", false);
	}
    if (getb("resetCalibration")) {
        resetCalibration();
        setb("resetCalibration", false);
    }
    
	if (getb("selectionMode")) {
		drawSelectionMode();
	} else {
		drawRenderMode();
	}
}


void testApp::exit() {
    keyPressed('a');
    ofSleepMillis(1000);
}


void testApp::keyPressed(int key) {
    float speed = ofGetKeyPressed(OF_KEY_SHIFT) ? 5 : 1;
    
    setb("arrowing",false);
    if (getb("selectionMode") || selected_point_ >= 0) {
        if (key == OF_KEY_LEFT || key == OF_KEY_UP || key == OF_KEY_RIGHT|| key == OF_KEY_DOWN){
            setb("arrowing", true);
            if (selected_point_ >= 0){
                ofVec2f &p = selectedPoint().image;
                switch(key) {
                    case OF_KEY_LEFT: p.x -= 1.0 / speed; break;
                    case OF_KEY_RIGHT: p.x += 1.0 / speed; break;
                    case OF_KEY_UP: p.y -= 1.0 / speed; break;
                    case OF_KEY_DOWN: p.y += 1.0 / speed; break;
                }
            }
        }
    } else {
        switch (key) {
            case OF_KEY_UP:
                root0_.boom(speed);
                needs_update_motor_ = true;
                break;
            case OF_KEY_DOWN:
                root0_.boom(-speed);
                needs_update_motor_ = true;
                break;
            case 'n':
                stage0_.roll(-speed);
                needs_update_motor_ = true;
                break;
            case 'h':
                stage0_.roll(speed);
                needs_update_motor_ = true;
                break;
            case 'c':
                stage0_.pitch(-speed);
                needs_update_motor_ = true;
                break;
            case 't':
                stage0_.pitch(speed);
                needs_update_motor_ = true;
                break;
        }
    }
    
    switch (key) {
        case OF_KEY_BACKSPACE: // delete selected
            if (selected_point_ >= 0) {
                selectedPoint().enabled = false;
                selectedPoint().image = ofVec2f::zero();
                selected_mesh_ = -1;
                selected_point_ = -1;
            }
            break;
            
        case '\n': // deselect
            selected_mesh_ = -1;
            selected_point_ = -1;
            break;
            
        case ' ': // toggle render/select mode
            setb("selectionMode", !getb("selectionMode"));
            break;
            
        case 'f':
            ofToggleFullscreen();
            break;
            
        case '1':
            motor_manager_.move(0, speed);
            break;
        case '\'':
        case 'q':
            motor_manager_.move(0, -speed);
            break;
        case '2':
            motor_manager_.move(1, speed);
            break;
        case ',':
        case 'w':
            motor_manager_.move(1, -speed);
            break;
        case '3':
            motor_manager_.move(2, speed);
            break;
        case '.':
        case 'e':
            motor_manager_.move(2, -speed);
            break;
        case 'o':
        case 's':
            motor_manager_.initOrigin();
            stage0_.resetTransform();
            stage_.resetTransform();
            root0_.resetTransform();
            root_.resetTransform();
            needs_update_motor_ = true;
            break;
        case 'a':
            motor_manager_.reset();
            stage0_.resetTransform();
            root0_.resetTransform();
            needs_update_motor_ = true;
            break;
        
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            stage0_.resetTransform();
            root0_.setPosition(0, (key - '5' + 1) * 100, 0);
            needs_update_motor_ = true;
            break;
    }
}


void testApp::mouseMoved(int x, int y) {
    float distance = getb("selectionMode") ? getClosestPointOnMeshes(x, y, hovering_mesh_, hovering_point_) : getClosestImagePoint(x, y, hovering_mesh_, hovering_point_);
    if (distance > getf("selectionRadius")) {
        hovering_mesh_ = -1;
        hovering_point_ = -1;
    }
}


void testApp::mousePressed(int x, int y, int button) {
//	setb("selected", getb("hoverSelected"));
//	seti("selectionChoice", geti("hoverChoice"));
    setb("arrowing", false);
	if (hovering_point_ >= 0) {
        selected_mesh_ = hovering_mesh_;
        selected_point_ = hovering_point_;
		setb("dragging", true);
	} else {
        selected_mesh_ = -1;
        selected_point_ = -1;
    }
}


void testApp::mouseReleased(int x, int y, int button) {
	setb("dragging", false);
}


//--------------------------------------------------------------------------------------------------
#pragma mark - Setup

void testApp::setupMesh() {
	if (!model_.loadModel("stage.obj")) {
        ofExit(1);
    }
    ofDisableArbTex();
    for (int i = 0; i < model_.getNumMeshes(); i++) {
        ofMesh m = model_.getMesh(i);
        CalibrationMesh *mesh = new CalibrationMesh(m);
        calibration_meshes_.push_back(mesh);
        if (m.getNumVertices() > 20) {
            mesh->setParent(stage_);
        }
        switch (i) {
            case 0:
                mesh->loadTexture("ocean.png");
                mesh->loadShader("shaders/ocean");
                break;
            case 1:
//                mesh->color.set(64, 148, 65);
                mesh->loadShader("shaders/bridge");
                break;
            case 2:
//                mesh->color.set(219, 79, 79);
                mesh->loadShader("shaders/elevator");
                break;
            case 3:
                mesh->loadTexture("http-aid-dcc.png");
                mesh->loadShader("shaders/floor");
                break;
        }
    }
    
    ball_.setup();
    ball_.setParent(stage_);
    goal_.setup();
    goal_.setParent(stage_);
    
    stage_.setParent(root_);
    
    stage0_.setParent(root0_);
}


void testApp::setupControlPanel() {
	panel.setup("mapamokWWMMM", 5, 5, 300, 700);
    //	panel.msg = "tab hides the panel, space toggles render/selection mode, 'f' toggles fullscreen.";
	
	panel.addPanel("Interaction");
	panel.addToggle("setupMode", "setupMode", true);
	panel.addSlider("scale", "scale", 1, .1, 3);
	panel.addSlider("backgroundColor", "backgroundColor", 0, 0, 255, true);
    panel.addSlider("rotationAmount", "rotationAmount", 0.3, 0, 1);
    panel.addSlider("rotationEasing", "rotationEasing", 0.05, 0, 0.3);
    panel.addSlider("elevationEasing", "elevationEasing", 0.05, 0, 0.3);
    vector<string> boxNames;
    boxNames.push_back("faces");
    boxNames.push_back("fullWireframe");
    boxNames.push_back("outlineWireframe");
    boxNames.push_back("occludedWireframe");
    panel.addMultiToggle("drawMode", "drawMode", 3, boxNames );
//    vector<string> shaderNames;
//    shaderNames.push_back("none");
//    shaderNames.push_back("lights");
//    shaderNames.push_back("shader");
//    panel.addMultiToggle("shading", "shading", 0, shaderNames );
    panel.addSlider("white", "white", CalibrationMesh::white, 0.0, 1.0);
    panel.addSlider("visibility", "visibility", CalibrationMesh::visibility, 0.0, 1.0);
    panel.addToggle("reloadShader", "reloadShader", false);
	panel.addToggle("loadCalibration", "loadCalibration", false);
	panel.addToggle("saveCalibration", "saveCalibration", false);
	panel.addToggle("resetCalibration", "resetCalibration", false);
	
//	panel.addPanel("Highlight");
//	panel.addToggle("highlight", "highlight", false);
//	panel.addSlider("highlightPosition", "highlightPosition", 0, 0, 1);
//	panel.addSlider("highlightOffset", "highlightOffset", .1, 0, 1);
	
	panel.addPanel("Calibration");
	panel.addSlider("aov", "aov", 80, 50, 100);
	panel.addToggle("CV_CALIB_FIX_ASPECT_RATIO", "CV_CALIB_FIX_ASPECT_RATIO", true);
	panel.addToggle("CV_CALIB_FIX_K1", "CV_CALIB_FIX_K1", true);
	panel.addToggle("CV_CALIB_FIX_K2", "CV_CALIB_FIX_K2", true);
	panel.addToggle("CV_CALIB_FIX_K3", "CV_CALIB_FIX_K3", true);
	panel.addToggle("CV_CALIB_ZERO_TANGENT_DIST", "CV_CALIB_ZERO_TANGENT_DIST", true);
	panel.addToggle("CV_CALIB_FIX_PRINCIPAL_POINT", "CV_CALIB_FIX_PRINCIPAL_POINT", false);
	
	panel.addPanel("Rendering");
	panel.addSlider("lineWidth", "lineWidth", 1, 1, 8, true);
//	panel.addToggle("useSmoothing", "useSmoothing", false);
//	panel.addToggle("useFog", "useFog", false);
//	panel.addSlider("fogNear", "fogNear", 200, 0, 1000);
//	panel.addSlider("fogFar", "fogFar", 1850, 0, 2500);
	panel.addSlider("screenPointSize", "screenPointSize", 2, 1, 16, true);
	panel.addSlider("selectedPointSize", "selectedPointSize", 8, 1, 16, true);
	panel.addSlider("selectionRadius", "selectionRadius", 12, 1, 32);
//	panel.addSlider("lightX", "lightX", 200, -1000, 1000);
//	panel.addSlider("lightY", "lightY", 400, -1000, 1000);
//	panel.addSlider("lightZ", "lightZ", 800, -1000, 1000);
//	panel.addToggle("randomLighting", "randomLighting", false);
	
	panel.addPanel("Internal");
//	panel.addToggle("validShader", "validShader", true);
	panel.addToggle("selectionMode", "selectionMode", true);
//	panel.addToggle("hoverSelected", "hoverSelected", false);
//	panel.addSlider("hoverChoice", "hoverChoice", 0, 0, object_points_.size(), true);
//	panel.addToggle("selected", "selected", false);
	panel.addToggle("dragging", "dragging", false);
	panel.addToggle("arrowing", "arrowing", false);
//	panel.addSlider("selectionChoice", "selectionChoice", 0, 0, object_points_.size(), true);
	panel.addSlider("slowLerpRate", "slowLerpRate", .001, 0, .01);
	panel.addSlider("fastLerpRate", "fastLerpRate", 1, 0, 1);
}


//--------------------------------------------------------------------------------------------------
#pragma mark - Calibration

void testApp::calibrateWithReferencePoints() {
	// generate camera matrix given aov guess
	float aov = getf("aov");
	Size2i image_size(ofGetWidth(), ofGetHeight());
	float f = image_size.width * ofDegToRad(aov); // i think this is wrong, but it's optimized out anyway
	Point2f c = Point2f(image_size) * (1. / 2);
	Mat1d camera_matrix = (Mat1d(3, 3) <<
                           f, 0, c.x,
                           0, f, c.y,
                           0, 0, 1);
    
	// generate flags
#define getFlag(flag) (panel.getValueB((#flag)) ? flag : 0)
	int flags =
    CV_CALIB_USE_INTRINSIC_GUESS |
    getFlag(CV_CALIB_FIX_PRINCIPAL_POINT) |
    getFlag(CV_CALIB_FIX_ASPECT_RATIO) |
    getFlag(CV_CALIB_FIX_K1) |
    getFlag(CV_CALIB_FIX_K2) |
    getFlag(CV_CALIB_FIX_K3) |
    getFlag(CV_CALIB_ZERO_TANGENT_DIST);
	
	vector<vector<Point3f> > reference_object_points(1);
	vector<vector<Point2f> > reference_image_points(1);
    for (int j = 0; j < calibration_meshes_.size(); j++) {
        CalibrationMesh *m = calibration_meshes_[j];
        int n = m->points.size();
        for (int i = 0; i < n; i++) {
            CalibrationPoint &p = m->points[i];
            if (p.enabled) {
                reference_object_points[0].push_back(toCv(p.object));
                reference_image_points[0].push_back(toCv(p.image));
            }
        }
    }
	const static int min_points = 6;
	if (reference_object_points[0].size() >= min_points) {
        vector<Mat> rvecs, tvecs;
        Mat distCoeffs;
		calibrateCamera(reference_object_points, reference_image_points, image_size, camera_matrix, distCoeffs, rvecs, tvecs, flags);
		rotation_vector_ = rvecs[0];
		translation_vector_ = tvecs[0];
		intrinsics_.setup(camera_matrix, image_size);
		model_matrix_ = makeMatrix(rotation_vector_, translation_vector_);
		calibration_ready_ = true;
	} else {
		calibration_ready_ = false;
	}
}


void testApp::saveCalibration() {
	string dirName = "calibration-" + ofGetTimestampString() + "/";
	ofDirectory dir(dirName);
	dir.create();
	
	FileStorage fs(ofToDataPath(dirName + "calibration-advanced.yml"), FileStorage::WRITE);
	
	Mat cameraMatrix = intrinsics_.getCameraMatrix();
	fs << "cameraMatrix" << cameraMatrix;
	
	double focalLength = intrinsics_.getFocalLength();
	fs << "focalLength" << focalLength;
	
	Point2d fov = intrinsics_.getFov();
	fs << "fov" << fov;
	
	Point2d principalPoint = intrinsics_.getPrincipalPoint();
	fs << "principalPoint" << principalPoint;
	
	cv::Size imageSize = intrinsics_.getImageSize();
	fs << "imageSize" << imageSize;
	
	fs << "translationVector" << translation_vector_;
	fs << "rotationVector" << rotation_vector_;
    
	Mat rotationMatrix;
	Rodrigues(rotation_vector_, rotationMatrix);
	fs << "rotationMatrix" << rotationMatrix;
	
	double rotationAngleRadians = norm(rotation_vector_, NORM_L2);
	double rotationAngleDegrees = ofRadToDeg(rotationAngleRadians);
	Mat rotationAxis = rotation_vector_ / rotationAngleRadians;
	fs << "rotationAngleRadians" << rotationAngleRadians;
	fs << "rotationAngleDegrees" << rotationAngleDegrees;
	fs << "rotationAxis" << rotationAxis;
	
	ofVec3f axis(rotationAxis.at<double>(0), rotationAxis.at<double>(1), rotationAxis.at<double>(2));
	ofVec3f euler = ofQuaternion(rotationAngleDegrees, axis).getEuler();
	Mat eulerMat = (Mat_<double>(3,1) << euler.x, euler.y, euler.z);
	fs << "euler" << eulerMat;
	
    //	ofFile basic("calibration-basic.txt", ofFile::WriteOnly);
    //	ofVec3f position( translation_vector_.at<double>(1), translation_vector_.at<double>(2));
    //	basic << "position (in world units):" << endl;
    //	basic << "\tx: " << ofToString(translation_vector_.at<double>(0), 2) << endl;
    //	basic << "\ty: " << ofToString(translation_vector_.at<double>(1), 2) << endl;
    //	basic << "\tz: " << ofToString(translation_vector_.at<double>(2), 2) << endl;
    //	basic << "axis-angle rotation (in degrees):" << endl;
    //	basic << "\taxis x: " << ofToString(axis.x, 2) << endl;
    //	basic << "\taxis y: " << ofToString(axis.y, 2) << endl;
    //	basic << "\taxis z: " << ofToString(axis.z, 2) << endl;
    //	basic << "\tangle: " << ofToString(rotationAngleDegrees, 2) << endl;
    //	basic << "euler rotation (in degrees):" << endl;
    //	basic << "\tx: " << ofToString(euler.x, 2) << endl;
    //	basic << "\ty: " << ofToString(euler.y, 2) << endl;
    //	basic << "\tz: " << ofToString(euler.z, 2) << endl;
    //	basic << "fov (in degrees):" << endl;
    //	basic << "\thorizontal: " << ofToString(fov.x, 2) << endl;
    //	basic << "\tvertical: " << ofToString(fov.y, 2) << endl;
    //	basic << "principal point (in screen units):" << endl;
    //	basic << "\tx: " << ofToString(principalPoint.x, 2) << endl;
    //	basic << "\ty: " << ofToString(principalPoint.y, 2) << endl;
    //	basic << "image size (in pixels):" << endl;
    //	basic << "\tx: " << ofToString(principalPoint.x, 2) << endl;
    //	basic << "\ty: " << ofToString(principalPoint.y, 2) << endl;
	
    ofstream ofs(ofToDataPath(dirName + "object-points.dat").c_str());
    boost::archive::text_oarchive oa(ofs);
    for (int i = 0; i < calibration_meshes_.size(); i++) {
        oa << *calibration_meshes_[i];
    }
    ofs.close();
}


void testApp::loadCalibration() {
    
    // retrieve advanced calibration folder
    
    ofFileDialogResult result = ofSystemLoadDialog("Select a calibration folder", true, ofToDataPath("", true));
    if (!result.bSuccess) return;
    string data_dir = result.getPath() + "/";
    
    // load objectPoints and imagePoints
    
    ifstream ifs(ofToDataPath(data_dir + "object-points.dat").c_str());
    boost::archive::text_iarchive ia(ifs);
    for (int i = 0; i < calibration_meshes_.size(); i++) {
        ia >> *calibration_meshes_[i];
    }
    ifs.close();
    
    // load the calibration-advanced yml
    
    FileStorage fs(ofToDataPath(data_dir + "calibration-advanced.yml", true), FileStorage::READ);
    
    Mat cameraMatrix;
    Size2i imageSize;
    fs["cameraMatrix"] >> cameraMatrix;
    fs["imageSize"][0] >> imageSize.width;
    fs["imageSize"][1] >> imageSize.height;
    fs["rotationVector"] >> rotation_vector_;
    fs["translationVector"] >> translation_vector_;
    
    intrinsics_.setup(cameraMatrix, imageSize);
    model_matrix_ = makeMatrix(rotation_vector_, translation_vector_);
    
    calibration_ready_ = true;
}


void testApp::resetCalibration() {
    for (auto it = calibration_meshes_.begin(); it != calibration_meshes_.end(); it++) {
        (*it)->reset();
        calibration_ready_ = false;
    }
}


//--------------------------------------------------------------------------------------------------
#pragma mark - Render

void testApp::render() {
    ofPushMatrix();
    ofDrawAxis(10);
    
	ofPushStyle();
	ofSetLineWidth(geti("lineWidth"));
    ofEnableSmoothing();
	ofSetColor(255);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_DEPTH_TEST);

	ofColor transparentBlack(0, 0, 0, 0);
	switch(geti("drawMode")) {
		case 0: // faces
//			glEnable(GL_CULL_FACE);
//			glCullFace(GL_BACK);
            CalibrationMesh::white = getf("white");
            CalibrationMesh::visibility = getf("visibility");
            for (int i = 0; i < calibration_meshes_.size(); i++) {
                calibration_meshes_[i]->draw();
            }
            ball_.draw();
            goal_.draw();
			break;
		case 1: // fullWireframe
            for (int i = 0; i < calibration_meshes_.size(); i++) {
                calibration_meshes_[i]->object_mesh.drawFaces();
            }
			break;
		case 2: // outlineWireframe
            for (int i = 0; i < calibration_meshes_.size(); i++) {
                LineArt::draw(calibration_meshes_[i]->object_mesh, true, transparentBlack, NULL);
            }
			break;
		case 3: // occludedWireframe
            for (int i = 0; i < calibration_meshes_.size(); i++) {
                calibration_meshes_[i]->transformGL();
                LineArt::draw(calibration_meshes_[i]->object_mesh, false, transparentBlack, NULL);
                calibration_meshes_[i]->restoreTransformGL();
            }
            ball_.draw();
//            goal_.draw();
			break;
	}

	glPopAttrib();
	ofPopStyle();

    ofPopMatrix();
}


void testApp::drawLabeledPoint(int label, ofVec2f position, ofColor color, ofColor bg, ofColor fg) {
	glPushAttrib(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	//glEnable(GL_DEPTH_TEST);
	ofVec2f tooltipOffset(5, -25);
	ofSetColor(color);
	float w = ofGetWidth();
	float h = ofGetHeight();
	ofSetLineWidth(1.5);
	ofLine(position - ofVec2f(w,0), position + ofVec2f(w,0));
	ofLine(position - ofVec2f(0,h), position + ofVec2f(0,h));
	ofCircle(position, geti("selectedPointSize"));
	drawHighlightString(ofToString(label), position + tooltipOffset, bg, fg);
	glPopAttrib();
}


void testApp::drawSelectionMode() {
    ofSetColor(255);
    
    camera_.begin(); {
        float scale = getf("scale");
        ofScale(scale, scale, scale);
        
        render();
        
        if (getb("setupMode")) {
            for (int i = 0; i < calibration_meshes_.size(); i++) {
                calibration_meshes_[i]->transformGL();
                calibration_meshes_[i]->project();
                calibration_meshes_[i]->restoreTransformGL();
            }
        }
    }
    camera_.end();
    
    if (getb("setupMode")) {
        // draw all points cyan small
        glPointSize(geti("screenPointSize"));
        glEnable(GL_POINT_SMOOTH);
        ofSetColor(cyanPrint);
        for (int i = 0; i < calibration_meshes_.size(); i++) {
            calibration_meshes_[i]->projected_mesh.drawVertices();
        }
        
        // draw all reference points cyan
        for (int j = 0; j < calibration_meshes_.size(); j++) {
            CalibrationMesh *m = calibration_meshes_[j];
            int n = m->points.size();
            for (int i = 0; i < n; i++) {
                if (m->points[i].enabled) {
                    drawLabeledPoint(i, m->projected_mesh.getVertex(i), cyanPrint);
                }
            }
        }
        
        // check to see if anything is selected
        // draw hover point magenta
        if (hovering_point_ >= 0) {
            drawLabeledPoint(hovering_point_, calibration_meshes_[hovering_mesh_]->getProjected(hovering_point_), magentaPrint);
        }
        // draw selected point yellow
        if (selected_point_ >= 0) {
            drawLabeledPoint(selected_point_, selectedMesh()->getProjected(selected_point_), yellowPrint, ofColor::white, ofColor::black);
        }
    }
}


void testApp::drawRenderMode() {
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	
	if (calibration_ready_) {
		intrinsics_.loadProjectionMatrix(10, 2000);
		applyMatrix(model_matrix_);
		render();
		if (getb("setupMode")) {
            for (int i = 0; i < calibration_meshes_.size(); i++) {
                calibration_meshes_[i]->transformGL();
                calibration_meshes_[i]->projected_mesh = getProjectedMesh(calibration_meshes_[i]->object_mesh);
                calibration_meshes_[i]->restoreTransformGL();
            }
		}
	}
	
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	
	if (getb("setupMode")) {
		// draw all reference points cyan
        for (int j = 0; j < calibration_meshes_.size(); j++) {
            CalibrationMesh *m = calibration_meshes_[j];
            int n = m->points.size();
            for (int i = 0; i < n; i++) {
                if (m->points[i].enabled) {
                    drawLabeledPoint(i, m->points[i].image, cyanPrint);
                }
            }
        }
		
		// move points that need to be dragged
		// draw selected yellow
        if (selected_point_ >= 0) {
            CalibrationPoint &p = selectedPoint();
            p.enabled = true;
            if (p.image == ofVec2f::zero()) {
                p.object = p.object * selectedMesh()->getGlobalTransformMatrix();
                if (calibration_ready_) {
                    p.image = ofVec2f(selectedMesh()->projected_mesh.getVertex(selected_point_));
                } else {
                    p.image = ofVec2f(mouseX, mouseY);
                }
            }
        }
		if (getb("dragging")) {
            ofVec2f &current = selectedPoint().image;
			float rate = ofGetMousePressed(0) ? getf("slowLerpRate") : getf("fastLerpRate");
			current = ofVec2f(ofLerp(current.x, mouseX, rate), ofLerp(current.y, mouseY, rate));
			drawLabeledPoint(selected_point_, current, yellowPrint, ofColor::white, ofColor::black);
			ofSetColor(ofColor::black);
			ofRect(current, 1, 1);
		} else if (getb("arrowing")) {
            ofVec2f &current = selectedPoint().image;
			drawLabeledPoint(selected_point_, current, yellowPrint, ofColor::white, ofColor::black);
			ofSetColor(ofColor::black);
			ofRect(current, 1, 1);
		} else if (hovering_point_ >= 0) {
            drawLabeledPoint(hovering_point_, calibration_meshes_[hovering_mesh_]->points[hovering_point_].image, magentaPrint);
        }
	}
}


//--------------------------------------------------------------------------------------------------
#pragma mark - Utils

inline void testApp::setb(string name, bool value) {
	panel.setValueB(name, value);
}
inline void testApp::seti(string name, int value) {
	panel.setValueI(name, value);
}
inline void testApp::setf(string name, float value) {
	panel.setValueF(name, value);
}
inline bool testApp::getb(string name) {
	return panel.getValueB(name);
}
inline int testApp::geti(string name) {
	return panel.getValueI(name);
}
inline float testApp::getf(string name) {
	return panel.getValueF(name);
}


float testApp::getClosestPointOnMeshes(float x, float y, int &mesh_index, int &point_index) {
	float distance = numeric_limits<float>::infinity();
    for (int j = 0; j < calibration_meshes_.size(); j++) {
        ofMesh &mesh = calibration_meshes_[j]->projected_mesh;
        for(int i = 0; i < mesh.getNumVertices(); i++) {
            const ofVec3f &v = mesh.getVerticesPointer()[i];
            float dx = x - v.x;
            float dy = y - v.y;
            float d = dx * dx + dy * dy;
            if(d < distance) {
                distance = d;
                mesh_index = j;
                point_index = i;
            }
        }
    }
	return sqrt(distance);
}


float testApp::getClosestImagePoint(float x, float y, int &mesh_index, int &point_index) {
	float distance = numeric_limits<float>::infinity();
    for (int j = 0; j < calibration_meshes_.size(); j++) {
        vector<CalibrationPoint> &p = calibration_meshes_[j]->points;
        for (int i = 0; i < p.size(); i++) {
            if (!p[i].enabled) continue;
            const ofVec2f &v = p[i].image;
            float dx = x - v.x;
            float dy = y - v.y;
            float d = dx * dx + dy * dy;
            if(d < distance) {
                distance = d;
                mesh_index = j;
                point_index = i;
            }
        }
    }
	return sqrt(distance);
}
