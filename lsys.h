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

class lsys{
    
public:
    
    std::string result;
    
    lsys();
    
    void setPosition(Vector3 position);
    
    void draw();
    
    
private:
    
    std::map<char,std::string> rule;
    
    Vector3 start;
        
    void forward(float);
    
    void f_angle();
    
    
    float theta;
    float current_angle;
    Vector3 current_location;


};


#endif /* lsys_h */
