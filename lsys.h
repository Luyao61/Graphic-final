//
//  lsys.hpp
//  CSE167 Spring 2015 Starter Code
//
//  Created by Li Yujia on 12/3/15.
//  Copyright © 2015 RexWest. All rights reserved.
//

#ifndef lsys_h
#define lsys_h

#include <stdio.h>
#include "Vector3.h"
#include <iostream>
#include <map>
#include <stack>
#include "Drawable.h"

class lsys : public Drawable{
    
public:
    
    lsys(Vector3 position, int rule_number);
    void setPosition(Vector3 position);
    void draw();
    void update();
    std::string result;
    
private:
    std::map<char,std::string> rule;
    Vector3 start;
    void forward(float);
    void f_angle();
    float theta;
    float current_angle;
    Vector3 current_location;
    float line_width;
    int rule_number;
    int j;
};


#endif /* lsys_h */
