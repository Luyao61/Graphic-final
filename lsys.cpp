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

lsys::lsys(){
    //default start point is origin
    this->start = *new Vector3(0,0,0);
    
    //set the rules
    this->rule['X'] = "F[+X][&X][^X][-X]F";
    //this->rule['F'] = "FF-[-F+F+F]+[+F-F-F]";
    this->rule['F'] = "F";

    result = *new std::string("X");
    //get the final result
    for (int j = 0 ; j < 5; j++) {
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

    
}

void lsys::setPosition(Vector3 position){
    this->start = position;
}

void lsys::draw(){
    std::stack<Vector3> locations = *new stack<Vector3>();
    std::stack<float> angles = *new stack<float>();
    //locations.push(start);
    //angles.push(0.0f);
    
    current_location = start;
    current_angle = 0.0f;
    for(int i = 0; i < result.length(); i++){
        switch(result[i]){
            case 'F':
                forward(1.0f);
                break;
            case 'X':
                forward(0.5f);
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
}

void lsys::forward(float length){
    Vector3 end_point;
    if (xaxis) {
        end_point.set(current_location[0] + length*sin(current_angle), current_location[1]+length*cos(current_angle), current_location[2]);
    }else{
        end_point.set(current_location[0], current_location[1]+length*cos(current_angle), current_location[2]  + length*sin(current_angle));
    }
    glLineWidth(0.5);
    
    //cout<<current_angle<<endl;
    glColor3f(1.0, 1.0, 1.0);
    //Color rand;
    //rand = rand.randomPastel();
    //glColor3fv(rand.ptr());

    glBegin(GL_LINES);
    glVertex3f(current_location[0], current_location[1], current_location[2]);
    glVertex3f(end_point[0], end_point[1], end_point[2]);
    glEnd();
    
    
    current_location = end_point;
}


