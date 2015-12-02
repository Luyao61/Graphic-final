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

//    static OBJObject dragon;
//    static OBJObject bear;
    static OBJObject bunny;
    static Rasterizer pokemon;
};

#endif
