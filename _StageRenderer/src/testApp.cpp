#include "testApp.h"

ofxEasingLinear SmallItems::Ripple::linear_easing;
ofxEasingCubic SmallItems::Ripple::cubic_easing;
float SmallItems::white = 0.5;

//--------------------------------------------------------------
void testApp::setup() {
//    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);

    if (!loader_.loadModel("stage.obj")) {
        ofExit(1);
    }
    ocean_ = loader_.getMesh(0);
    bridges_ = loader_.getMesh(1);
    elevators_ = loader_.getMesh(2);
    floors_ = loader_.getMesh(3);

    ofDisableArbTex();
    ofPixels pixs;
    ofLoadImage(pixs, "http-aid-dcc.png");
    floors_texture_.setCompression(OF_COMPRESS_ARB);
    floors_texture_.loadData(pixs);
//    floors_texture_.setCompression(OF_COMPRESS_ARB);
//    floors_texture_.loadImage("http-aid-dcc.png");
    ocean_texture_.loadImage("ocean.png");
    
//    goal_.setScaleNomalization(false);
//    goal_.loadModel("goal.dae");
//    cout << goal_.getNumMeshes() << endl;
//    cout << goal_.getSceneMin() << endl;
//    cout << goal_.getSceneMax() << endl;
//    cout << goal_.getModelMatrix() << endl;
//    cout << goal_.getMeshHelper(0).matrix << endl;
//    goal_mesh_ = goal_.getMeshHelper(0);
    
    
//    ball_.setRadius(2);
//    ball_.setResolution(0);
//    ball_.setPosition(0, 0, 0);
//    ball_.loadModel("ball.obj");
//    ofPoint sceneMax = ball_.getSceneMax();
//    ofPoint sceneMin = ball_.getSceneMin();
//    float size = MAX(sceneMax.x - sceneMin.x, MAX(sceneMax.y - sceneMin.y, sceneMax.z - sceneMin.z));
//    cout << "Size: " << size << endl;
//    ofVec3f min(FLT_MAX, FLT_MAX, FLT_MAX);
//    ofVec3f max(FLT_MIN, FLT_MIN, FLT_MIN);
//    for (int i = 0; i < ball_.getNumMeshes(); i++) {
//        ofMesh mesh = ball_.getMesh(i);
//        ofVec3f *v = mesh.getVerticesPointer();
//        for (int j = 0; j < mesh.getNumVertices(); j++, v++) {
//            if (v->x < min.x) min.x = v->x;
//            if (v->y < min.y) min.y = v->y;
//            if (v->z < min.z) min.z = v->z;
//            if (max.x < v->x) max.x = v->x;
//            if (max.y < v->y) max.y = v->y;
//            if (max.z < v->z) max.z = v->z;
//        }
//    }
//    cout << "MIN: " << min << endl;
//    cout << "MAX: " << max << endl;
////    ball_.setScale(0.025, 0.025, 0.025);
//    cout << ball_.getModelMatrix() << endl;
    
    ball_.setup();
    goal_.setup();
    ocean2_.setup();
    ripple_.setup();
    fireworks_.setup();
    
    ofxJSONElement result;
    result.open("http-aid-dcc.json");
    small_items_.setup(result["small_items"]);
    
    random_shader_.load("bridge");
    random_texture_.loadImage("random.png");
    random_texture_.getTextureReference().setTextureWrap(GL_REPEAT, GL_REPEAT);
    
//    camera_.setPosition(0, 400, 475);
//    camera_.lookAt(ofVec3f());
//    camera_.setPosition(0, 800, 0);
//    camera_.setOrientation(ofQuaternion(-0.5, 0.5, 0.5, 0.5));
    camera_.setNearClip(1);
    camera_.move_speed *= 0.2;
    
    receiver_.setup(8001);
    
    panel_.setup();
    panel_.add(rotation_.setup("Rotation", 0.2f, 0.0f, 1.0f));
    panel_.add(white_.setup("White", 1.0f, 0.0f, 1.0f));
    panel_.add(go_button_.setup("Go"));
    go_button_.addListener(this, &testApp::goClicked);
}

//--------------------------------------------------------------
void testApp::update() {
    camera_.update();
    
    while (receiver_.hasWaitingMessages()) {
        ofxOscMessage message;
        receiver_.getNextMessage(&message);
        string address = message.getAddress();
        if (address == "/ball/position") {
            ball_.setPosition(message.getArgAsFloat(0) * .3, message.getArgAsFloat(1) * .3, message.getArgAsFloat(2) * .3);
        } else if (address == "/ball/orientation") {
            ball_.setOrientation(ofQuaternion(message.getArgAsFloat(0), message.getArgAsFloat(1), message.getArgAsFloat(2), message.getArgAsFloat(3)));
        } else if (address == "/world/orientation") {
            ofQuaternion q(message.getArgAsFloat(0), message.getArgAsFloat(1), message.getArgAsFloat(2), message.getArgAsFloat(3));
            q.slerp(rotation_, ofQuaternion(), q);
            stage_transform_matrix_.makeRotationMatrix(q);
        }
    }
    
    if (white_tween_.isRunning()) {
        white_ = white_tween_.update();
    }
}

//--------------------------------------------------------------
void testApp::draw() {
    ofBackground(64);
    
    ofVec3f p = ball_.getPosition() * stage_transform_matrix_;

    ofEnableDepthTest();
    camera_.begin();
    ofDrawAxis(100);
    
//    ocean_texture_.bind();
//    ocean_.drawFaces();
//    ocean_texture_.unbind();
    ocean2_.white = white_;
    ocean2_.draw();
//    ripple_.draw();
//    fireworks_.draw();
    
    ofPushMatrix();
    ofTranslate(0, -p.y + 50, 0);
    ofMultMatrix(stage_transform_matrix_);
    
    ofPushStyle();
    random_shader_.begin();
    random_shader_.setUniform1f("tick", ofGetElapsedTimef());
    random_shader_.setUniformTexture("random", random_texture_, 0);
    random_shader_.setUniform3f("base", 0.33, 0.56, 0.58);
    random_shader_.setUniform3f("variation", 0.0, 0.05, 0.08);
    bridges_.drawFaces();
    random_shader_.end();
    ofPopStyle();
    
    ofPushStyle();
    ofSetColor(219, 79, 79);
    random_shader_.begin();
    random_shader_.setUniform1f("tick", ofGetElapsedTimef());
    random_shader_.setUniformTexture("random", random_texture_, 0);
    random_shader_.setUniform3f("base", 0.0, 0.64, 0.86);
    random_shader_.setUniform3f("variation", 0.0, 0.05, 0.08);
    elevators_.drawFaces();
    random_shader_.end();
    ofPopStyle();
    
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);
    floors_texture_.bind();
    floors_.drawFaces();
    floors_texture_.unbind();
    glDisable(GL_POLYGON_OFFSET_FILL);
    
//    ofPushStyle();
//    ofNoFill();
//    ofSetColor(255, 0, 0, 128);
//    ofSetLineWidth(1);
//    ofPushMatrix();
//    ofTranslate(0, 20, 0);
//    ball_.draw();
//    ofPopMatrix();
//    ofPopStyle();

    SmallItems::white = white_;
    small_items_.draw();
    
//    goal_.draw();
    
    ofPopMatrix();
    
    camera_.end();
    
    ofDisableDepthTest();
    panel_.draw();

    
//    stringstream s;
//    ofVec3f p = ball_.getPosition();
//    s << "X: " << ofToString(p.x, 2) << endl
//    << "Y: " << ofToString(p.y, 2) << endl
//    << "Y: " << ofToString(p.z, 2);
//    ofDrawBitmapStringHighlight(s.str(), 10, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
    switch (key) {
        case '1': {
            camera_.setPosition(183.455, 61.2889, 243.16);
            camera_.setOrientation(ofQuaternion(-0.12015, 0.387389, 0.0510006, 0.91263));
            break;
        }
        case '2': {
            camera_.setPosition(0, 800, 0);
            camera_.setOrientation(ofQuaternion(-0.5, 0.5, 0.5, 0.5));
            break;
        }
        case '3': {
            camera_.setPosition(341.459, 437.179, 582.306);
            camera_.setOrientation(ofQuaternion(-0.354013, 0.24081, 0.0948576, 0.898716));
            break;
        }
        case ' ': {
            ofQuaternion q = camera_.getOrientationQuat();
            cout << "pos: " << camera_.getPosition() << endl;
            cout << "qua: " << q.x() << ", " << q.y() << ", " << q.z() << ", " << q.w() << endl;
            cout << "eul: " << camera_.getOrientationEuler() << endl;
            break;
        }
        case 'r':
            ocean2_.reloadShader();
            ripple_.reloadShader();
            random_shader_.unload();
            random_shader_.load("bridge");
            break;
            
        case 'q': {
            small_items_.remove(ofRandom(500));
            break;
        }
        case 'j': {
            small_items_.reset();
            break;
        }
    }
}


ofxEasingLinear linear_easing;

void testApp::goClicked() {
    white_tween_.setParameters(linear_easing, ofxTween::easeInOut, 0, 1, 3000, 0);
}
