#ifndef CSE167_Drawable_h
#define CSE167_Drawable_h

#include <iostream>
#include "Matrix4.h"
#include "DrawData.h"
#include "Vector4.h"
#include "Vector3.h"
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
    

    //for bounding box;
    float min_x, min_y, min_z, max_x, max_y, max_z;
    bool hasBBox;
    

    std::vector<Vector3*>* vertices;
    std::vector<Vector3*>* normals;
    std::vector<Face*>* faces;
    std::vector<Vector3*>* colors;
    
    std::vector<Color*>* colorP3;  //added color for Project3, random color
    Vector4 center;
    Vector3 speed;
    Vector3 WallDirection;
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
    void scale(int);
    void orbit(float);
    
    void spin(float);
    void reset();

    
};

#endif
