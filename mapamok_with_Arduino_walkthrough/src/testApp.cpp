#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	ofSetVerticalSync(true);
	calibrationReady = false;
	setupMesh();
    
#ifdef USE_CAMERA_COLOR_SAMPLER
    camSampler.setup();
#endif
    
    motor.setup();
	setupControlPanel();
}

void testApp::exit() {
    motor.exit();
}

void testApp::update() {
	ofSetWindowTitle("Vase 1.0");
    
#ifdef USE_CAMERA_COLOR_SAMPLER
    camSampler.update();
#endif
    
    motor.update();
    
	if(getb("selectionMode")) {
		cam.enableMouseInput();
	} else {
		updateRenderMode();
		cam.disableMouseInput();
	}
}

void testApp::draw() {
	ofBackground(geti("backgroundColor"));
	if(getb("saveCalibration")) {
		saveCalibration();
		setb("saveCalibration", false);
	}
	if(getb("selectionMode")) {
		drawSelectionMode();
	} else {
		drawRenderMode();
	}
	if(!getb("validShader")) {
		ofPushStyle();
		ofSetColor(magentaPrint);
		ofSetLineWidth(8);
		ofLine(0, 0, ofGetWidth(), ofGetHeight());
		ofLine(ofGetWidth(), 0, 0, ofGetHeight());
		string message = "Shader failed to compile.";
		ofVec2f center(ofGetWidth(), ofGetHeight());
		center /= 2;
		center.x -= message.size() * 8 / 2;
		center.y -= 8;
		drawHighlightString(message, center);
		ofPopStyle();
	}
}

void testApp::keyPressed(int key) {
	if(key == OF_KEY_LEFT || key == OF_KEY_UP || key == OF_KEY_RIGHT|| key == OF_KEY_DOWN){
		int choice = geti("selectionChoice");
		setb("arrowing", true);
		if(choice > 0){
			Point2f& cur = imagePoints[choice];
			switch(key) {
				case OF_KEY_LEFT: cur.x -= 1; break;
				case OF_KEY_RIGHT: cur.x += 1; break;
				case OF_KEY_UP: cur.y -= 1; break;
				case OF_KEY_DOWN: cur.y += 1; break;
			}
		}
	} else {
		setb("arrowing",false);
	}
	if(key == OF_KEY_BACKSPACE) { // delete selected
		if(getb("selected")) {
			setb("selected", false);
			int choice = geti("selectionChoice");
			referencePoints[choice] = false;
			imagePoints[choice] = Point2f();
		}
	}
	if(key == '\n') { // deselect
		setb("selected", false);
	}
	if(key == ' ') { // toggle render/select mode
		setb("selectionMode", !getb("selectionMode"));
	}
}

void testApp::mousePressed(int x, int y, int button) {
	setb("selected", getb("hoverSelected"));
	seti("selectionChoice", geti("hoverChoice"));
	if(getb("selected")) {
		setb("dragging", true);
	}
}

void testApp::mouseReleased(int x, int y, int button) {
	setb("dragging", false);
}

void testApp::setupMesh() {
    ofLogLevel(OF_LOG_VERBOSE);
	model.loadModel("model.dae");
	objectMesh = model.getMesh(0);
	int n = objectMesh.getNumVertices();
	objectPoints.resize(n);
	imagePoints.resize(n);
	referencePoints.resize(n, false);
	for(int i = 0; i < n; i++) {
		objectPoints[i] = toCv(objectMesh.getVertex(i));
	}
}

void testApp::render() {
	
    ofPushMatrix();
    ofRotate(motor.getRotation()/* + getf("motor_speed")*/, 0, 0, 1);
    
    ofPushStyle();
	ofSetLineWidth(2);
	
	int shading = geti("shading");
	bool useLights = shading == 1;
	bool useShader = shading == 2;
    
	glShadeModel(GL_FLAT);
	ofSetColor(255);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_DEPTH_TEST);
	if(useShader) {
		ofFile fragFile("shaders/shader.frag"), vertFile("shaders/shader.vert");
		Poco::Timestamp fragTimestamp = fragFile.getPocoFile().getLastModified();
		Poco::Timestamp vertTimestamp = vertFile.getPocoFile().getLastModified();
		if(fragTimestamp != lastFragTimestamp || vertTimestamp != lastVertTimestamp) {
			bool validShader = shader.load("shaders/shader");
			setb("validShader", validShader);
		}
		lastFragTimestamp = fragTimestamp;
		lastVertTimestamp = vertTimestamp;
		
		shader.begin();
		shader.setUniform1f("time", ofGetElapsedTimef());
        shader.setUniform3f("color", getf("color_r"), getf("color_g"), getf("color_b"));
        //shader.setUniform3f("color", camSampler.avg_color[0], camSampler.avg_color[1], camSampler.avg_color[2]);
        shader.setUniform1f("mult", getf("mult"));
        shader.setUniform1f("offset", getf("offset"));
		shader.end();
	}
	ofColor transparentBlack(0, 0, 0, 0);
	switch(geti("drawMode")) {
		case 0: // faces
			if(useShader) shader.begin();
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			objectMesh.drawFaces();
			if(useShader) shader.end();
			break;
		case 1: // fullWireframe
			if(useShader) shader.begin();
			objectMesh.drawWireframe();
			if(useShader) shader.end();
			break;
		case 2: // outlineWireframe
			LineArt::draw(objectMesh, true, transparentBlack, useShader ? &shader : NULL);
			break;
		case 3: // occludedWireframe
			LineArt::draw(objectMesh, false, transparentBlack, useShader ? &shader : NULL);
			break;
	}
	glPopAttrib();
	
	ofPopStyle();
    
    ofPopMatrix();
}

void testApp::saveCalibration() {
	string dirName = "calibration-" + ofGetTimestampString() + "/";
	ofDirectory dir(dirName);
	dir.create();
	
	FileStorage fs(ofToDataPath(dirName + "calibration-advanced.yml"), FileStorage::WRITE);	
	
	Mat cameraMatrix = intrinsics.getCameraMatrix();
	fs << "cameraMatrix" << cameraMatrix;
	
	double focalLength = intrinsics.getFocalLength();
	fs << "focalLength" << focalLength;
	
	Point2d fov = intrinsics.getFov();
	fs << "fov" << fov;
	
	Point2d principalPoint = intrinsics.getPrincipalPoint();
	fs << "principalPoint" << principalPoint;
	
	cv::Size imageSize = intrinsics.getImageSize();
	fs << "imageSize" << imageSize;
	
	fs << "translationVector" << tvec;
	fs << "rotationVector" << rvec;

	Mat rotationMatrix;
	Rodrigues(rvec, rotationMatrix);
	fs << "rotationMatrix" << rotationMatrix;
	
	double rotationAngleRadians = norm(rvec, NORM_L2);
	double rotationAngleDegrees = ofRadToDeg(rotationAngleRadians);
	Mat rotationAxis = rvec / rotationAngleRadians;
	fs << "rotationAngleRadians" << rotationAngleRadians;
	fs << "rotationAngleDegrees" << rotationAngleDegrees;
	fs << "rotationAxis" << rotationAxis;
	
	ofVec3f axis(rotationAxis.at<double>(0), rotationAxis.at<double>(1), rotationAxis.at<double>(2));
	ofVec3f euler = ofQuaternion(rotationAngleDegrees, axis).getEuler();
	Mat eulerMat = (Mat_<double>(3,1) << euler.x, euler.y, euler.z);
	fs << "euler" << eulerMat;
	
	ofFile basic("calibration-basic.txt", ofFile::WriteOnly);
	ofVec3f position( tvec.at<double>(1), tvec.at<double>(2));
	basic << "position (in world units):" << endl;
	basic << "\tx: " << ofToString(tvec.at<double>(0), 2) << endl;
	basic << "\ty: " << ofToString(tvec.at<double>(1), 2) << endl;
	basic << "\tz: " << ofToString(tvec.at<double>(2), 2) << endl;
	basic << "axis-angle rotation (in degrees):" << endl;
	basic << "\taxis x: " << ofToString(axis.x, 2) << endl;
	basic << "\taxis y: " << ofToString(axis.y, 2) << endl;
	basic << "\taxis z: " << ofToString(axis.z, 2) << endl;
	basic << "\tangle: " << ofToString(rotationAngleDegrees, 2) << endl;
	basic << "euler rotation (in degrees):" << endl;
	basic << "\tx: " << ofToString(euler.x, 2) << endl;
	basic << "\ty: " << ofToString(euler.y, 2) << endl;
	basic << "\tz: " << ofToString(euler.z, 2) << endl;
	basic << "fov (in degrees):" << endl;
	basic << "\thorizontal: " << ofToString(fov.x, 2) << endl;
	basic << "\tvertical: " << ofToString(fov.y, 2) << endl;
	basic << "principal point (in screen units):" << endl;
	basic << "\tx: " << ofToString(principalPoint.x, 2) << endl;
	basic << "\ty: " << ofToString(principalPoint.y, 2) << endl;
	basic << "image size (in pixels):" << endl;
	basic << "\tx: " << ofToString(principalPoint.x, 2) << endl;
	basic << "\ty: " << ofToString(principalPoint.y, 2) << endl;
	
	saveMat(Mat(objectPoints), dirName + "objectPoints.yml");
	saveMat(Mat(imagePoints), dirName + "imagePoints.yml");
}

void testApp::setupControlPanel()
{
    panel.setup("test", 0, 0, 280, 1000);
	panel.addPanel("tab hides the panel, space toggles render/selection mode, 'f' toggles fullscreen.", 0 );
    
	panel.addPanel("Interaction");
	panel.addToggle("setupMode", "setupMode", true);
	panel.addSlider("scale", "scale", 1, .1, 50);
	panel.addSlider("backgroundColor", "backgroundColor", 0, 0, 255, true);
    vector<string> boxNames;
    boxNames.push_back("faces");
    boxNames.push_back("fullWireframe");
    boxNames.push_back("outlineWireframe");
    boxNames.push_back("occludedWireframe");
    panel.addMultiToggle("drawMode", "drawMode", 3, boxNames );
    vector<string> shaderNames;
    shaderNames.push_back("none");
    shaderNames.push_back("lights");
    shaderNames.push_back("shader");
	panel.addMultiToggle("shading", "shading", 0, shaderNames );
    panel.addToggle("saveCalibration", "saveCalibration", false);
	
#ifdef USE_CAMERA_COLOR_SAMPLER
    camSampler.setupControlTab();
#endif
    
    motor.setupControlTab();

    panel.addPanel("Vase");
    panel.addSlider("color_r", "color_r", 1.0, 0.0, 1.0);
    panel.addSlider("color_g", "color_g", 0.5, 0.0, 1.0);
    panel.addSlider("color_b", "color_b", 0.1, 0.0, 1.0);
    panel.addSlider("mult", "mult", 1.0, 0.0, 2.0);
    panel.addSlider("offset", "offset", 0.0, -2.0, 2.0);
    
    panel.addPanel("Internal");
	panel.addToggle("validShader", "validShader", true);
	panel.addToggle("selectionMode", "selectionMode", true);
	panel.addToggle("hoverSelected", "hoverSelected", false);
    
    int s = objectPoints.size();
	panel.addSlider("hoverChoice","hoverChoice", 0, 0, s, true);
	panel.addToggle("selected", "selected", false);
	panel.addToggle("dragging", "dragging", false);
	panel.addToggle("arrowing", "arrowing", false);
	panel.addSlider("selectionChoice","selectionChoice", 0, 0, s, true);
	panel.addSlider("slowLerpRate", "slowLerpRate", .001, 0, .01);
	panel.addSlider("fastLerpRate", "fastLerpRate", 1, 0, 1);
    
    panel.reloadSettings();
}

void testApp::updateRenderMode() {
	// generate camera matrix given aov guess
	float aov = 80;
	Size2i imageSize(ofGetWidth(), ofGetHeight());
	float f = imageSize.width * ofDegToRad(aov); // i think this is wrong, but it's optimized out anyway
	Point2f c = Point2f(imageSize) * (1. / 2);
	Mat1d cameraMatrix = (Mat1d(3, 3) <<
		f, 0, c.x,
		0, f, c.y,
		0, 0, 1);
		
	int flags = CV_CALIB_USE_INTRINSIC_GUESS  | CV_CALIB_FIX_ASPECT_RATIO | CV_CALIB_FIX_K1 |
		CV_CALIB_FIX_K2 | CV_CALIB_FIX_K3 | CV_CALIB_ZERO_TANGENT_DIST;
	
	vector<Mat> rvecs, tvecs;
	Mat distCoeffs;
	vector<vector<Point3f> > referenceObjectPoints(1);
	vector<vector<Point2f> > referenceImagePoints(1);
	int n = referencePoints.size();
	for(int i = 0; i < n; i++) {
		if(referencePoints[i]) {
			referenceObjectPoints[0].push_back(objectPoints[i]);
			referenceImagePoints[0].push_back(imagePoints[i]);
		}
	}
	const static int minPoints = 6;
	if(referenceObjectPoints[0].size() >= minPoints) {
		calibrateCamera(referenceObjectPoints, referenceImagePoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs, flags);
		rvec = rvecs[0];
		tvec = tvecs[0];
		intrinsics.setup(cameraMatrix, imageSize);
		modelMatrix = makeMatrix(rvec, tvec);
		calibrationReady = true;
	} else {
		calibrationReady = false;
	}
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
	ofCircle(position, 8);
	drawHighlightString(ofToString(label), position + tooltipOffset, bg, fg);
	glPopAttrib();
}
	
void testApp::drawSelectionMode() {
	ofSetColor(255);
	cam.begin();
	float scale = getf("scale");
	ofScale(scale, scale, scale);
	
	render();
	
	if(getb("setupMode")) {
		imageMesh = getProjectedMesh(objectMesh);
	}
	cam.end();
	
	if(getb("setupMode")) {
		// draw all points cyan small
		glPointSize(2);
		glEnable(GL_POINT_SMOOTH);
		ofSetColor(cyanPrint);
		imageMesh.drawVertices();

		// draw all reference points cyan
		int n = referencePoints.size();
		for(int i = 0; i < n; i++) {
			if(referencePoints[i]) {
				drawLabeledPoint(i, imageMesh.getVertex(i), cyanPrint);
			}
		}
		
		// check to see if anything is selected
		// draw hover point magenta
		int choice;
		float distance;
		ofVec3f selected = getClosestPointOnMesh(imageMesh, mouseX, mouseY, &choice, &distance);
		if(!ofGetMousePressed() && distance < 12) {
			seti("hoverChoice", choice);
			setb("hoverSelected", true);
			drawLabeledPoint(choice, selected, magentaPrint);
		} else {
			setb("hoverSelected", false);
		}
		
		// draw selected point yellow
		if(getb("selected")) {
			int choice = geti("selectionChoice");
			ofVec2f selected = imageMesh.getVertex(choice);
			drawLabeledPoint(choice, selected, yellowPrint, ofColor::white, ofColor::black);
		}
	}
}

void testApp::drawRenderMode() {
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	
	if(calibrationReady) {
		intrinsics.loadProjectionMatrix(10, 2000);
		applyMatrix(modelMatrix);
		render();
		if(getb("setupMode")) {
			imageMesh = getProjectedMesh(objectMesh);	
		}
	}
	
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	
	if(getb("setupMode")) {
		// draw all reference points cyan
		int n = referencePoints.size();
		for(int i = 0; i < n; i++) {
			if(referencePoints[i]) {
				drawLabeledPoint(i, toOf(imagePoints[i]), cyanPrint);
			}
		}
		
		// move points that need to be dragged
		// draw selected yellow
		int choice = geti("selectionChoice");
		if(getb("selected")) {
			referencePoints[choice] = true;	
			Point2f& cur = imagePoints[choice];	
			if(cur == Point2f()) {
				if(calibrationReady) {
					cur = toCv(ofVec2f(imageMesh.getVertex(choice)));
				} else {
					cur = Point2f(mouseX, mouseY);
				}
			}
		}
		if(getb("dragging")) {
			Point2f& cur = imagePoints[choice];
			float rate = ofGetMousePressed(1) ? getf("slowLerpRate") : getf("fastLerpRate");
			cur = Point2f(ofLerp(cur.x, mouseX, rate), ofLerp(cur.y, mouseY, rate));
			drawLabeledPoint(choice, toOf(cur), yellowPrint, ofColor::white, ofColor::black);
			ofSetColor(ofColor::black);
			ofRect(toOf(cur), 1, 1);
		} else if(getb("arrowing")) {
			Point2f& cur = imagePoints[choice];
			drawLabeledPoint(choice, toOf(cur), yellowPrint, ofColor::white, ofColor::black);
			ofSetColor(ofColor::black);
			ofRect(toOf(cur), 1, 1);
        } else {
			// check to see if anything is selected
			// draw hover magenta
			float distance;
			ofVec2f selected = toOf(getClosestPoint(imagePoints, mouseX, mouseY, &choice, &distance));
			if(!ofGetMousePressed() && referencePoints[choice] && distance < 12) {
				seti("hoverChoice", choice);
				setb("hoverSelected", true);
				drawLabeledPoint(choice, selected, magentaPrint);
			} else {
				setb("hoverSelected", false);
			}
		}
	}
}
