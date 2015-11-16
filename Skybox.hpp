//
//  Skybox.hpp
//  CSE167 Spring 2015 Starter Code
//
//  Created by Li Yujia on 15/11/10.
//  Copyright © 2015年 RexWest. All rights reserved.
//

#ifndef Skybox_hpp
#define Skybox_hpp

#include <stdio.h>
#include "Texture.h"
#include "Matrix4.h"
class Skybox
{
    
protected:
    
    
public:
    Matrix4 toWorld;
    Texture front, back, top, bot, left, right;
    
    Skybox(void);
    virtual ~Skybox(void);
    
    void draw(void);    
};


#endif /* Skybox_hpp */
