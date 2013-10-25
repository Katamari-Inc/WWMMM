//
//  ControlPanel.h
//  vase
//
//  Created by kikko on 11/16/12.
//
//

#ifndef __vase__ControlPanel__
#define __vase__ControlPanel__

#include "ofMain.h"
#include "ofxAutoControlPanel.h"

extern ofxAutoControlPanel panel;

void setb(string name, bool value);
void seti(string name, int value);
void setf(string name, float value);
bool getb(string name);
int geti(string name);
float getf(string name);

#endif /* defined(__vase__ControlPanel__) */
