//
//  CameraColorSampler.cpp
//  vase
//
//  Created by kikko on 11/16/12.
//
//

#include "CameraColorSampler.h"



CameraColorSampler::CameraColorSampler() {
    debugDraw.cam = &ps3eye;
}

CameraColorSampler::~CameraColorSampler() {
    
}

void CameraColorSampler::setup() {
    
    ps3eye.setDesiredFrameRate(60);
	ps3eye.initGrabber(640,480);
	ps3eye.setAutoGainAndShutter(false); // otherwise we can't set gain or shutter
}

void CameraColorSampler::setupControlTab() {
    
    panel.addPanel("Camera Color Sampler");
    panel.addDrawableRect("cam", &debugDraw, 220, 165);
    
//    listen( panel.addSlider("gain", 0.5, 0.0, 1.0) );
//    listen( panel.addSlider("shutter", 1.0, 0.0, 1.0) );
//    listen( panel.addSlider("gamma", 0.5, 0.0, 1.0) );
//    listen( panel.addSlider("brightness", 0.6, 0.0, 1.0) );
//    listen( panel.addSlider("contrast", 1.0, 0.0, 1.0) );
//    listen( panel.addSlider("hue", 0.0, 0.0, 1.0) );
//    listen( panel.addMultiToggle("flicker", 0, variadic("mode 0")("mode 1")("mode 2")) );
//    listen( panel.addMultiToggle("white_balance", 0, variadic("mode 0")("mode 1")("mode 2")) );
//    
//    panel.addSlider("roi_x", 0.4, 0.f, 1.f);
//    panel.addSlider("roi_y", 0.4, 0.f, 1.f);
//    panel.addSlider("roi_width", 0.2, 0.f, 0.5f);
//    panel.addSlider("roi_height", 0.2, 0.f, 0.5f);
    
    initValues();
}

void CameraColorSampler::listen(guiBaseObject *item) {
    ofAddListener(item->guiEvent, this, &CameraColorSampler::panelValueChanged);
}

void CameraColorSampler::initValues() {
    ps3eye.setGain( getf("gain") );
	ps3eye.setShutter( getf("shutter") );
	ps3eye.setGamma( getf("gamma") );
	ps3eye.setBrightness( getf("brightness") );
	ps3eye.setContrast( getf("contrast") );
	ps3eye.setHue( getf("hue") );
	ps3eye.setFlicker( geti("flicker") );
	ps3eye.setWhiteBalance( geti("white_balance") );
}

void CameraColorSampler::panelValueChanged(guiCallbackData& data) {
    string name = data.getXmlName();
    if(name=="gain")            ps3eye.setGain(data.getFloat(0));
    else if(name=="shutter")    ps3eye.setShutter(data.getFloat(0));
    else if(name=="gamma")      ps3eye.setGamma(data.getFloat(0));
    else if(name=="brightness") ps3eye.setBrightness(data.getFloat(0));
    else if(name=="contrast")   ps3eye.setContrast(data.getFloat(0));
    else if(name=="hue")        ps3eye.setHue(data.getFloat(0));
    else if(name=="flicker")    ps3eye.setFlicker(data.getInt(0));
    else if(name=="white_balance") ps3eye.setWhiteBalance(data.getInt(0)+1);
}

void CameraColorSampler::update() {
    
    ps3eye.update();
	if(ps3eye.isFrameNew()){
		Mat map = toCv(ps3eye);
        IplImage img = map;
        cvSetImageROI(&img, cvRect(getf("roi_x")*640, getf("roi_y")*480, getf("roi_width")*640-1, getf("roi_height")*480-1));
        CvScalar c = cvAvg(&img);
        avg_color[0] = c.val[0] / 255;
        avg_color[1] = c.val[1] / 255;
        avg_color[2] = c.val[2] / 255;
	}
}
