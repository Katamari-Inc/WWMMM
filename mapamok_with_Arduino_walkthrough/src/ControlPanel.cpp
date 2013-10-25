//
//  ControlPanel.cpp
//  vase
//
//  Created by kikko on 11/16/12.
//
//

#include "ControlPanel.h"

ofxAutoControlPanel panel;

void setb(string name, bool value) {
	panel.setValueB(name, value);
}
void seti(string name, int value) {
	panel.setValueI(name, value);
}
void setf(string name, float value) {
	panel.setValueF(name, value);
}
bool getb(string name) {
	return panel.getValueB(name);
}
int geti(string name) {
	return panel.getValueI(name);
}
float getf(string name) {
	return panel.getValueF(name);
}