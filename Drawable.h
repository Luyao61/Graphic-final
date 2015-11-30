#ifndef CSE167_Drawable_h
#define CSE167_Drawable_h

#include <iostream>
#include "Matrix4.h"
#include "DrawData.h"
#include "Vector4.h"
#include "UpdateData.h"
#include "Material.h"
#include <float.h>
#include <math.h>
#include <vector>


struct Face
{
    int vertexIndices[3];
    int normalIndices[3];
    //Add more members as necessary
};
class Drawable
{
    
public:
    
    std::vector<Vector3*>* vertices;
    std::vector<Vector3*>* normals;
    std::vector<Face*>* faces;
    std::vector<Vector3*>* colors;
    
    std::vector<Color*>* colorP3;  //added color for Project3, random color
    Vector4 center;
    int renderMode;
    
    
    
    Matrix4 toWorld;
    Material material;
    
    Drawable(void);
    ~Drawable(void);
    
    virtual void draw(DrawData&);
    virtual void update(UpdateData&);
    
    void moveX();
    void movex();
    void movey();
    void moveY();
    void movez();
    void moveZ();
    void scale(bool);
    void orbit(float);
    
    void spin(float);
    void reset();
    
    
};

#endif
