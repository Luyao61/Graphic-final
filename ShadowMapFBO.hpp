//
//  ShadowMapFBO.hpp
//  CSE167 Final Project
//
//  Created by Luyao Zhou on 12/3/15.
//  Copyright © 2015 RexWest. All rights reserved.
//

#ifndef ShadowMapFBO_hpp
#define ShadowMapFBO_hpp
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>

class ShadowMapFBO
{
public:
    ShadowMapFBO();
    
    ~ShadowMapFBO();
    
    bool Init(unsigned int WindowWidth, unsigned int WindowHeight);
    
    void BindForWriting();
    
    void BindForReading(GLenum TextureUnit);
    
private:
    GLuint m_fbo;
    GLuint m_shadowMap;
};

#endif /* ShadowMapFBO_hpp */
