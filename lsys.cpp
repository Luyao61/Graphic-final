//
//  lsys.cpp
//  CSE167 Spring 2015 Starter Code
//
//  Created by Li Yujia on 12/3/15.
//  Copyright © 2015 RexWest. All rights reserved.
//

#include <GLUT/glut.h>
#include "lsys.h"
#include <map>
#include "Vector3.h"
#include <stack>
#include <math.h>
#include "Color.h"

using namespace std;

bool xaxis = true;

lsys::lsys(Vector3 position, int rule_number): Drawable(){
    //default start point is origin
    this->start = position;
    this->rule_number = rule_number;
    if(rule_number == 1){
        this->rule['X'] = "F[+X][&X][^X][-X]F";
        this->rule['F'] = "F";
        result = *new std::string("X");
        j = 6;
    }
    else if(rule_number == 2){
        this->rule['X'] = "F[+X][^X]F[-X][&X]+X";
        this->rule['F'] = "F";
        result = *new std::string("X");
        j = 5;
    }
    else if(rule_number == 3){
        //this->rule['X'] = "F[+X]F[-X]+X";
        this->rule['F'] = "F[+F][&F]F[-F][&F][F]";
        result = *new std::string("F");
        j = 3;
    }
    for (int i = 0 ; i < j; i++) {
        string temp = result;
        int x = 0;
        for(int i=0; i<result.size(); i++){
            if (result.at(i) == 'F') {
                temp.replace(x, 1, rule['F']);
                x+=rule['F'].size()-1;
            }
            else if (result.at(i) == 'X') {
                temp.replace(x, 1, rule['X']);
                x+=rule['X'].size()-1;
            }
            x++;
        }
        result = temp;
    }
    theta = 25.7f/180.0f * M_PI;
    current_angle = 0;
    line_width = 2.0;
}

void lsys::setPosition(Vector3 position){
    this->start = position;
}

void lsys::draw(){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(toWorld.ptr());
    glLineWidth(line_width);
    if(rule_number == 1){
        glColor3f(0.0, 0.5, 0.0);
    }
    else if(rule_number == 2){
        glColor3f(0.8, 0.8, 0.0);
    }
    else if(rule_number == 3){
        glColor3f(0.7, 0.1, 0.1);
    }
    glBegin(GL_LINES);
    
    std::stack<Vector3> locations = *new stack<Vector3>();
    std::stack<float> angles = *new stack<float>();
    
    current_location = start;
    current_angle = 0.0f;
    for(int i = 0; i < result.length(); i++){
        switch(result[i]){
            case 'F':
                forward(2.5f);
                break;
            case 'X':
                forward(0.8f);
                break;
            case '+':
                current_angle += theta;
                xaxis = true;
                break;
            case '-':
                current_angle -= theta;
                xaxis = true;
                break;
            case '&':
                current_angle += theta;
                xaxis = false;
                break;
            case '^':
                current_angle -= theta;
                xaxis = false;
                break;
            case '[':
                locations.push(current_location);
                angles.push(current_angle);
                break;
            case ']':
                current_location = locations.top();
                current_angle = angles.top();
                locations.pop();
                angles.pop();
                break;
        }
    }
    glEnd();
    glPopMatrix();
}

void lsys::update(){
    Matrix4 rotation;
    if(rule_number == 3)
        rotation.makeRotateY(1.0/180.0*M_PI);
    else rotation.makeRotateY(-1.0/180.0*M_PI);
    toWorld = toWorld * rotation;
}
void lsys::forward(float length){
    Vector3 end_point;
    if (xaxis) {
        end_point.set(current_location[0] + length*sin(current_angle),
                      current_location[1] + length*cos(current_angle),
                      current_location[2]);
    }else{
        end_point.set(current_location[0],
                      current_location[1] + length*cos(current_angle),
                      current_location[2] + length*sin(current_angle));
    }
    glVertex3f(current_location[0], current_location[1], current_location[2]);
    glVertex3f(end_point[0], end_point[1], end_point[2]);
    
    current_location = end_point;
}


