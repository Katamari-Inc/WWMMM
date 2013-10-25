//
//  MotorControl.h
//  vase
//
//  Created by kikko on 11/17/12.
//
//

#ifndef __vase__MotorControl__
#define __vase__MotorControl__

#include "ofMain.h"
#include "ofxSerialLink.h"
#include "ControlPanel.h"

class MotorControl {
    
public:
    MotorControl();
    virtual ~MotorControl() {};
    
    void setup();
    void setupControlTab();
    void update();
    void exit();
    
    void setSpeed(int speed);
    void reset();
    
    int getRotation() { return rotation; }
    
protected:
    
    SerialLink serial;
    bool bAvailable, bExiting, bExitOk;
    
    int rotation;
    
    void panelValueChanged(guiCallbackData& data);
    void serialLinkEventHandler(SerialLinkEventArgs& e);
};

#endif /* defined(__vase__MotorControl__) */
