//
//  MotorManager.h
//  MovingProjection
//
//  Created by Saqoosha on 2013/10/27.
//
//

#pragma once

#include "ofMain.h"
#include "pystring.h"



class Motor {
public:
    Motor() : current(0), destination(0) {}
    int current;
    int destination;
};


class MotorManager {
public:
    MotorManager() : is_ready_(false) {
    }
    
    void setup(string port) {
        buffer_[0] = NULL;
        serial_.listDevices();
        serial_.setup(port, 57600);
        
        motors_.push_back(new Motor());
        motors_.push_back(new Motor());
        motors_.push_back(new Motor());
    }
    
    void update() {
        if (!serial_.isInitialized()) return;
        
        while (serial_.available() > 0) {
            unsigned char c = serial_.readByte();
            if (c == '\n') {
                vector<string> tokens;
                pystring::split(string((char *)buffer_), tokens);
                if (tokens.size() && tokens[0] == "P") {
                    int id;
                    istringstream(tokens[1]) >> id;
                    int pos;
                    istringstream(tokens[2]) >> pos;
                    motors_[id]->current = pos;
                    ofLog(OF_LOG_VERBOSE, "Motor(%d) = %d", id, pos);
                    is_ready_ = true;
                } else {
                    ofLog(OF_LOG_VERBOSE, "Unknown command: %s", buffer_);
                }
                bufp_ = 0;
                buffer_[0] = NULL;
            } else if (isprint(c)) {
                buffer_[bufp_++] = c;
                buffer_[bufp_] = NULL;
            }
        }
    }
    
    void sendCommand(string command) {
        cout << "COMMAND: " << command << endl;
        serial_.writeBytes((unsigned char *)command.c_str(), command.length());
        serial_.writeByte('\n');
    }
    
    void setPosition(int index, float millimeter) {
        Motor *motor = motors_[index];
        millimeter = ofClamp(millimeter, -1000, 1000);
        if (motor->destination == millimeter) return;
        motor->destination = millimeter;
        
        int position = -millimeter / (50.0f * M_PI) * 200 * 128;
        stringstream str;
        str << "P " << index << " " << position;
        sendCommand(str.str());
    }
    
    void move(int motor_id, float amount) {
        setPosition(motor_id, motors_[motor_id]->destination + amount);
    }
    
    void sync() {
        ofVec3f p = ofVec3f(220, 0, 179.3) * matrix_;
        setPosition(1, p.y);
        p = ofVec3f(-220, 0, 179.3) * matrix_;
        setPosition(2, p.y);
        p = ofVec3f(0, 0, -230.7) * matrix_;
        setPosition(0, p.y);
    }
    
    void initOrigin() {
        matrix_.makeIdentityMatrix();
        sendCommand("SETHOME");
    }
    
    void reset() {
        matrix_.makeIdentityMatrix();
        sync();
    }
    
    void setTransformMatrix(const ofMatrix4x4 matrix) {
        matrix_ = matrix;
        sync();
    }
    
    
    bool is_ready_;
    ofSerial serial_;
    unsigned char buffer_[1024];
    int bufp_ = 0;
    
    vector<Motor*> motors_;

    ofMatrix4x4 matrix_;
};
