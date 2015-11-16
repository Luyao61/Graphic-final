//
//  BezierPatch.cpp
//  CSE167 Spring 2015 Starter Code
//

#include "BezierPatch.h"
#include <iostream>
#include <math.h>
using namespace std;
BezierPatch::BezierPatch(){
    for (int i = 0; i <detail; ++i) {
        float u = (float)i / (float)(detail-1);
        for (int j = 0; j <detail; ++j) {
            float v = (float)j / (float)(detail-1);
            calculate( i, j, v, u);
        }
    }
    flag = *new Texture("ucsd-logo.ppm");
}

BezierPatch::~BezierPatch(void){
    
}

Vector3 BezierPatch::Bez(float t, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3) {
    Vector4 T;
    Vector4 G1, G2, G3;
    Matrix4 B;
    Matrix4 G;
    T.set(t*t*t, t*t, t, 1);
    B.set(-1.0f, 3.0f, -3.0f, 1.0f,
       3.0f, -6.0f, 3.0f, 0.0f,
       -3.0f, 3.0f, 0.0f, 0.0f,
       1.0f, 0.0f, 0.0f, 0.0f);

    G1.set(p0[0], p1[0], p2[0], p3[0]);
    G2.set(p0[1], p1[1], p2[1], p3[1]);
    G3.set(p0[2], p1[2], p2[2], p3[2]);
    float x = (B.multiply(G1)).dot(T);
    float y = (B.multiply(G2)).dot(T);
    float z = (B.multiply(G3)).dot(T);
    Vector3 rev;
    rev.set(x,y,z);
    return rev;
}

void BezierPatch::calculate(int i, int j, float t1, float t2){
    Vector3 q1,q2,q3,q4;
    
    q1 = Bez(t1, Vector3(ctrlpoints[0][0]), Vector3(ctrlpoints[0][1]), Vector3(ctrlpoints[0][2]), Vector3(ctrlpoints[0][3]));
    q2 = Bez(t1, Vector3(ctrlpoints[1][0]), Vector3(ctrlpoints[1][1]), Vector3(ctrlpoints[1][2]), Vector3(ctrlpoints[1][3]));
    q3 = Bez(t1, Vector3(ctrlpoints[2][0]), Vector3(ctrlpoints[2][1]), Vector3(ctrlpoints[2][2]), Vector3(ctrlpoints[2][3]));
    q4 = Bez(t1, Vector3(ctrlpoints[3][0]), Vector3(ctrlpoints[3][1]), Vector3(ctrlpoints[3][2]), Vector3(ctrlpoints[3][3]));
    
    Vector3 r1, r2, r3, r4;
    
    r1 = Bez(t1 + delta, Vector3(ctrlpoints[0][0]), Vector3(ctrlpoints[0][1]), Vector3(ctrlpoints[0][2]), Vector3(ctrlpoints[0][3]));
    r2 = Bez(t1 + delta, Vector3(ctrlpoints[1][0]), Vector3(ctrlpoints[1][1]), Vector3(ctrlpoints[1][2]), Vector3(ctrlpoints[1][3]));
    r3 = Bez(t1 + delta, Vector3(ctrlpoints[2][0]), Vector3(ctrlpoints[2][1]), Vector3(ctrlpoints[2][2]), Vector3(ctrlpoints[2][3]));
    r4 = Bez(t1 + delta, Vector3(ctrlpoints[3][0]), Vector3(ctrlpoints[3][1]), Vector3(ctrlpoints[3][2]), Vector3(ctrlpoints[3][3]));
    
    Vector3 u0 = Bez(t2, q1, q2, q3, q4);
    
    Vector3 u1 = Bez(t2 + delta, q1, q2, q3, q4);
    
    Vector3 tanu;
    tanu.set(u1[0]-u0[0], u1[1]-u0[1], u1[2]-u0[2]);
    
    Vector3 v0 = Bez(t2, r1, r2, r3, r4);
    
    Vector3 tanv;
    tanv.set(v0[0]-u0[0], v0[1]-u0[1], v0[2]-u0[2]);
    
    tanu = tanu.normalize();
    tanv = tanv.normalize();
    Vector3 normal;
    normal = tanu.cross(tanv);
    normal = normal.normalize();
    this->normals[i][j] = normal;
    this->indices[i][j] = u0;
}


void BezierPatch::draw(void){
    //glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
    glPushMatrix();
    flag.bind();
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    
    for (int i = 0; i <detail-1 ; ++i) {
        float u = 1 - (float)i / (float)(detail-1) - 1.0/(detail-1);
        for (int j = 0; j<detail-1 ; ++j) {
        float v = (float)j / (float)(detail-1);

            glNormal3fv(normals[i][j].ptr());
            
            glTexCoord2f(v, u);
            glVertex3fv( indices[i+1][j].ptr());
            
            glTexCoord2f(v, u+1.0/(detail-1));
            glVertex3fv( indices[i][j].ptr());
            
            glTexCoord2f(v+1.0/(detail-1), u+1.0/(detail-1));
            glVertex3fv( indices[i][j+1].ptr());
            
            glTexCoord2f(v+1.0/(detail-1), u);
            glVertex3fv( indices[i+1][j+1].ptr());
            
        }
    }
    glEnd();
    glPopMatrix();
}
void BezierPatch::update(float t){

    
    ctrlpoints[0][1][2] = -5 + 5*cos(t/100.0);
    ctrlpoints[1][1][2] = -5 + 5*cos(t/100.0);
    ctrlpoints[2][1][2] = -5 + 5*cos(t/100.0);
    ctrlpoints[3][1][2] = -5 + 5*cos(t/100.0);
/*
    ctrlpoints[0][3][2] = -5 + 5*cos(t/100.0);
    ctrlpoints[1][3][2] = -5 + 5*cos(t/100.0);
    ctrlpoints[2][3][2] = -5 + 5*cos(t/100.0);
    ctrlpoints[3][3][2] = -5 + 5*cos(t/100.0);
*/
    ctrlpoints[0][2][2] = -5 - 5*cos(t/100.0);
    ctrlpoints[1][2][2] = -5 - 5*cos(t/100.0);
    ctrlpoints[2][2][2] = -5 - 5*cos(t/100.0);
    ctrlpoints[3][2][2] = -5 - 5*cos(t/100.0);
    
    ctrlpoints[1][0][2] -= 5*cos(t/100.0);
    ctrlpoints[1][1][2] -= 5*cos(t/100.0);
    ctrlpoints[1][2][2] -= 5*cos(t/100.0);
    ctrlpoints[1][3][2] -= 5*cos(t/100.0);
    
    ctrlpoints[2][0][2] += 5*cos(t/100.0);
    ctrlpoints[2][1][2] += 5*cos(t/100.0);
    ctrlpoints[2][2][2] += 5*cos(t/100.0);
    ctrlpoints[2][3][2] += 5*cos(t/100.0);

 
    for (int i = 0; i <detail; ++i) {
        float u = (float)i / (float)(detail-1);
        for (int j = 0; j <detail; ++j) {
            float v = (float)j / (float)(detail-1);
            calculate( i, j, v, u);
        }
    }

}