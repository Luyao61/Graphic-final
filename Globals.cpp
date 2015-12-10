#include "Globals.h"

Camera Globals::camera;

Cube Globals::cube(10.0);

Light Globals::light;

DrawData Globals::drawData;
UpdateData Globals::updateData;

OBJObject Globals::charizard("OBJ/Charizard2.obj");
OBJObject Globals::cloud1("OBJ/cloud2.obj");
OBJObject Globals::cloud2("OBJ/cloud2.obj");
OBJObject Globals::cloud3("OBJ/cloud2.obj");
OBJObject Globals::cloud4("OBJ/cloud2.obj");
OBJObject Globals::cloud5("OBJ/cloud2.obj");
OBJObject Globals::cloud6("OBJ/cloud2.obj");
OBJObject Globals::cloud7("OBJ/cloud2.obj");
OBJObject Globals::cloud8("OBJ/cloud2.obj");


OBJObject Globals:: ft_wall("OBJ/Front Wall");
OBJObject Globals:: bk_wall("OBJ/Back Wall");
OBJObject Globals:: lt_wall("OBJ/Left Wall");
OBJObject Globals:: rt_wall("OBJ/Right Wall");

//OBJObject Globals::bunny("OBJ/bunny.obj");
//OBJObject Globals::dragon("dragon.obj");
//OBJObject Globals::bear("bear.obj");

//OBJObject Globals::bunny("");
//OBJObject Globals::dragon("");
//OBJObject Globals::bear("");

bool Globals::drawBoundingBox = false;

lsys Globals::l1(*new Vector3(72,-60,0), 3);
lsys Globals::l2(*new Vector3(0,-60,-72), 3);
lsys Globals::l3(*new Vector3(-72,-60,0), 3);
lsys Globals::l4(*new Vector3(0,-60,72), 3);
lsys Globals::l5(*new Vector3(36,-60,62), 3);
lsys Globals::l6(*new Vector3(62,-60,36), 3);
lsys Globals::l7(*new Vector3(36,-60,-62), 3);
lsys Globals::l8(*new Vector3(-36,-60,62), 3);
lsys Globals::l9(*new Vector3(-36,-60,62), 3);
lsys Globals::l10(*new Vector3(-62,-60,36), 3);
lsys Globals::l11(*new Vector3(62,-60,-36), 3);
lsys Globals::l12(*new Vector3(-62,-60,-36), 3);

lsys Globals::y1(*new Vector3(54,-60,0), 1);
lsys Globals::y2(*new Vector3(-54,-60,0), 1);
lsys Globals::y3(*new Vector3(0,-60,-54), 1);
lsys Globals::y4(*new Vector3(0,-60,54), 1);
lsys Globals::y5(*new Vector3(38,-60,38), 2);
lsys Globals::y6(*new Vector3(-38,-60,-38), 2);
lsys Globals::y7(*new Vector3(38,-60,-38), 2);
lsys Globals::y8(*new Vector3(-38,-60,38), 2);

OBJObject Globals::tree("OBJ/tree.obj");