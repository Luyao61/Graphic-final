#ifndef CSE167_Globals_h
#define CSE167_Globals_h

#include "Camera.h"
#include "Cube.h"
#include "Light.h"
#include "DrawData.h"
#include "UpdateData.h"
#include "Skybox.hpp"
#include "OBJObject.h"
#include "Rasterizer.hpp"
#include "lsys.h"

class Globals
{
    
public:
    
    static Camera camera;
    static Cube cube;
    static Light light;
    static DrawData drawData;
    static UpdateData updateData;
    //Feel free to add more member variables as needed
    
    static OBJObject charizard;
    static OBJObject cloud1;
    static OBJObject cloud2;
    static OBJObject cloud3;
    static OBJObject cloud4;
    static OBJObject cloud5;
    static OBJObject cloud6;
    static OBJObject cloud7;
    static OBJObject cloud8;
    
    static OBJObject ft_wall;
    static OBJObject bk_wall;
    static OBJObject lt_wall;
    static OBJObject rt_wall;
    
    
//    static OBJObject dragon;
//    static OBJObject bear;
//    static OBJObject bunny;

    
    static bool drawBoundingBox;
    
    
    static OBJObject tree;
    //the red ones
    static lsys l1;
    static lsys l2;
    static lsys l3;
    static lsys l4;
    static lsys l5;
    static lsys l6;
    static lsys l7;
    static lsys l8;
    static lsys l9;
    static lsys l10;
    static lsys l11;
    static lsys l12;
    
    //the yellow ones
    static lsys y1;
    static lsys y2;
    static lsys y3;
    static lsys y4;
    static lsys y5;
    static lsys y6;
    static lsys y7;
    static lsys y8;

};

#endif
