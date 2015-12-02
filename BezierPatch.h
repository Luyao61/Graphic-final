//
//  BezierPatch.hpp
//  CSE167 Spring 2015 Starter Code
//

#ifndef BezierPatch_hpp
#define BezierPatch_hpp

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include "Vector3.h"
#include "Matrix4.h"
#include "Vector4.h"
#include "Texture.h"
#include "Globals.h"
class BezierPatch
{
public:
    
    //GLfloat ctrlpoints[4][4][3];
    Texture flag;
    
    GLfloat ctrlpoints[4][4][3] = {
        {
            { -3, -5, 3 },
            { -1, -5, 3 },
            { 1, -5, 3 },
            { 3, -5, 3 }
        },
        {
            { -3, -5, 1 },
            { -1, -5, 1 },
            { 1, -5, 1 },
            { 3, -5, 1 }
        },
        {
            { -3, -5, -1 },
            { -1, -5, -1 },
            { 1, -5, -1 },
            { 3, -5, -1 }
        },
        {
            { -3, -5, -3 },
            { -1, -5, -3 },
            { 1, -5, -3 },
            { 3, -5, -3 }
        }
     
    };
    
    float delta = 0.00001f;
    static const int detail = 100;
    Vector3 indices[detail][detail];
    Vector3 normals[detail][detail];
    //constructors
    //BezierPatch(GLfloat[4][4][3]);
    BezierPatch();
    //delete
    virtual ~BezierPatch(void);
    //methods
    Vector3 Bez(float t, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3);
    void calculate(int i, int j, float t1, float t2);
    //Vector3 calculateIndices(float t1, float t2);
    void draw(void);
    
    void update(float);
    
};
#endif