#include "Globals.h"

Camera Globals::camera;

Cube Globals::cube(10.0);

Light Globals::light;

DrawData Globals::drawData;
UpdateData Globals::updateData;

OBJObject Globals::charizard("OBJ/Charizard2.obj");
OBJObject Globals::cloudA1("OBJ/cloud1.obj");
OBJObject Globals::cloudA2("OBJ/cloud1.obj");
OBJObject Globals::cloudB1("OBJ/cloud2.obj");
OBJObject Globals::cloudB2("OBJ/cloud2.obj");
//OBJObject Globals::bunny("OBJ/bunny.obj");
//OBJObject Globals::dragon("dragon.obj");
//OBJObject Globals::bear("bear.obj");

//OBJObject Globals::bunny("");
//OBJObject Globals::dragon("");
//OBJObject Globals::bear("");

bool Globals::drawBoundingBox = false;

