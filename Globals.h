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
    static OBJObject cloudA1;
    static OBJObject cloudA2;
    static OBJObject cloudB1;
    static OBJObject cloudB2;
    
    static OBJObject ft_wall;
    static OBJObject bk_wall;
    static OBJObject lt_wall;
    static OBJObject rt_wall;
    
    
//    static OBJObject dragon;
//    static OBJObject bear;
//    static OBJObject bunny;

    
    static bool drawBoundingBox;


};

#endif
