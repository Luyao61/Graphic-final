#ifdef __APPLE__

#include <GLUT/glut.h>

#else
    #include <GL/glut.h>
#endif

#include "Window.h"
#include "Cube.h"
#include "Matrix4.h"
#include "Globals.h"
#include "Skybox.hpp"
#include "BezierPatch.h"
#include "Shader.h"
#include <math.h>
#include <iostream>
using namespace std;

int Window::width  = 512;   //Set window width in pixels here
int Window::height = 512;   //Set window height in pixels here
Skybox *sky;
Shader *toon_Shader;
Shader *light_Shader;
Shader *ucsd_Shader;

BezierPatch *platform;

int t;

float deltaAngleX = 0.0f;
float deltaAngleY = 0.0f;
int xOrigin = -1;
int yOrigin = -1;
float angle_Horizontal = 0.0f;
float angle_Vertical = 0.0f;
float x_d = 0.0f;
float y_d = 0.0f;
float z_d = 0.0f;



struct OBJPair {
    OBJObject* o1;
    OBJObject* o2;
};
vector<OBJObject*> list_x;
vector<OBJObject*> list_y;
vector<OBJObject*> list_z;
vector<OBJPair> possibleCollisionPair;

void Window::initialize(void)
{
    //Setup the light
    Vector4 lightPos(10.0, 10.0, 15.0, 1.0);
    Globals::light.position = lightPos;
    Globals::light.quadraticAttenuation = 0.00;
    Globals::light.constantAttenuation = 1.0;
    Globals::light.linearAttenuation = 0.0;
    
    sky= new Skybox();
    platform = new BezierPatch();
    toon_Shader = new Shader("Toon.vert", "Toon.frag", true);
    light_Shader = new Shader("dirLightAmbDiffSpec.vert", "dirLightAmbDiffSpec.frag", true);
    ucsd_Shader = new Shader("ucsd.vert", "ucsd.frag", true);
    
    for (int i = 0; i<40; i++) {
        Globals::charizard.scale(false);
    }
    for (int i = 0; i<10; i++) {
        Globals::charizard.movey();
    }
    for (int i = 0; i<5; i++) {
        Globals::charizard.moveZ();
    }
    list_x = *new vector<OBJObject*>;
    possibleCollisionPair = *new vector<OBJPair>;
    list_x.push_back(&Globals::cloudA1);
    list_x.push_back(&Globals::cloudB1);
    //list_y.push_back(&Globals::cloudA1);
    //list_y.push_back(&Globals::cloudB1);
    //list_z.push_back(&Globals::cloudA1);
    //list_z.push_back(&Globals::cloudB1);
    insertionSortList(list_x);
    findOverlapPair(list_x);
    testCollision();
}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
// This is called at the start of every new "frame" (qualitatively)
void Window::idleCallback()
{
    //Call the display routine to draw the cube
    displayCallback();
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{
    width = w;                                                       //Set the window width
    height = h;                                                      //Set the window height
    glViewport(0, 0, w, h);                                          //Set new viewport size
    glMatrixMode(GL_PROJECTION);                                     //Set the OpenGL matrix mode to Projection
    glLoadIdentity();                                                //Clear the projection matrix by loading the identity
    gluPerspective(60.0, double(width)/(double)height, 1.0, 1000.0); //Set perspective projection viewing frustum
    
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{

    t=glutGet(GLUT_ELAPSED_TIME);
    //Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Set the OpenGL matrix mode to ModelView
    glMatrixMode(GL_MODELVIEW);
    
    //Push a matrix save point
    //This will save a copy of the current matrix so that we can
    //make changes to it and 'pop' those changes off later.
    glPushMatrix();

    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Reset transformations
    glLoadIdentity();

    //Replace the current top of the matrix stack with the inverse camera matrix
    //This will convert all world coordiantes into camera coordiantes
    glLoadMatrixf(Globals::camera.getInverseMatrix().ptr());
    
    //Bind the light to slot 0.  We do this after the camera matrix is loaded so that
    //the light position will be treated as world coordiantes
    //(if we didn't the light would move with the camera, why is that?)
    //
    Globals::light.bind(0);
    
    
    //sky->draw();

    //light_Shader->bind();
    //toon_Shader -> bind();
    
    Globals::cloudA1.draw(Globals::drawData);
    Globals::cloudB1.draw(Globals::drawData);
    
    //Globals::charizard.draw(Globals::drawData );
    //toon_Shader -> unbind();
    
    //DrawScene(0);
    
    //ucsd_Shader->bind();
    platform->draw();
    //ucsd_Shader->unbind();
    //light_Shader->unbind();
    
    

    //Pop off the changes we made to the matrix stack this frame
    glPopMatrix();

    //Tell OpenGL to clear any outstanding commands in its command buffer
    //This will make sure that all of our commands are fully executed before
    //we swap buffers and show the user the freshly drawn frame
    glFlush();
    
    //Swap the off-screen buffer (the one we just drew to) with the on-screen buffer
    glutSwapBuffers();
}


//TODO: Keyboard callbacks!
void Window::processNormalKeys(unsigned char key, int x, int y) {
    if (key == 27) {
        exit(0);
    }
    else if (key == 'b'){
        Globals::drawBoundingBox = !Globals::drawBoundingBox;
    }
    else if (key == 's') {
        //Globals::pokemon.keyboard(key, x, y);
    }
    else if (key == 'S'){
        //Globals::pokemon.keyboard(key, x, y);
    }
    else if (key == 'x'){
        Globals::charizard.movex();
    }
    else if (key == 'X'){
        Globals::charizard.moveX();
    }
    else if (key == 'y'){
    }
    else if (key == 'Y'){
        //Globals::charizard.moveY();
    }
    else if (key == 'z'){
        Globals::charizard.movez();
    }
    else if (key == 'Z'){
        Globals::charizard.moveZ();
    }
    else if (key == 'r' || key == 'R'){
        Globals::camera.reset();
        angle_Horizontal = angle_Vertical = 0.0f;
    }
}
//TODO: Function Key callbacks!
void Window::processSpecialKeys(int key, int x, int y) {
    
    switch(key)
    {
        case GLUT_KEY_UP:
            Globals::camera.forward();
            break;
        case GLUT_KEY_DOWN:
            Globals::camera.backward();
            break;
        case GLUT_KEY_LEFT:
            Globals::camera.leftward();
            break;
        case GLUT_KEY_RIGHT:
            Globals::camera.rightward();
            break;
    }
}
//TODO: Mouse callbacks!
void Window::processMouse(int button, int state, int x, int y) {
    // only start motion if the left button is pressed
    if (button == GLUT_LEFT_BUTTON) {
        
        // when the button is released
        if (state == GLUT_UP) {
            angle_Horizontal += deltaAngleX;
            angle_Vertical += deltaAngleY;
            xOrigin = -1;
        }
        else  {// state = GLUT_DOWN
            xOrigin = x;
            yOrigin = y;
        }
    }

}
//TODO: Mouse Motion callbacks!
void Window::processMotion(int x, int y) {
    // this will only be true when the left button is down
    if (xOrigin >= 0) {
        
        // update deltaAngle
        deltaAngleX = (x - xOrigin) * 0.01f;
        deltaAngleY = (yOrigin - y) * 0.01f;
        
        x_d = 20.0f * sin(angle_Horizontal + deltaAngleX) * cos(angle_Vertical + deltaAngleY);
        z_d = 20.0f * cos(angle_Horizontal + deltaAngleX) * cos(angle_Vertical + deltaAngleY);
        y_d = 20.0f * sin(angle_Vertical + deltaAngleY);

        Globals::camera.d.set(Globals::camera.e[0] + x_d, Globals::camera.e[1] + y_d, Globals::camera.e[2] - z_d);
        
        //y_v = 20.f * cos(angle_Vertical + deltaAngleY);

        Globals::camera.update();
        
    }
}

void Window::insertionSortList(vector<OBJObject*> list){
    for (int j = 1; j < list.size(); j++)
    {
        Vector4 keyelementVector, listelementVector;
        OBJObject *keyelement = list.at(j);
        keyelementVector.set(keyelement->min_x, keyelement->min_y, keyelement->min_z, 1);
        keyelementVector = keyelement->toWorld * keyelementVector;
        //float key = keyelement->min_x;
        float key = keyelementVector[0];

        int i = j - 1;
        
        while (i >= 0 )
        {
            listelementVector.set(list.at(i)->min_x,list.at(i)->min_y,list.at(i)->min_z,1);
            listelementVector = list.at(i)->toWorld * listelementVector;

            if (listelementVector[0] > key) {
                OBJObject *swapper = list.at(i);
                
                /*
                 if (keyelement.Begin && !swapper.Begin)
                 {
                 if (CheckBoundingBoxes(swapper.Body, keyelement.Body))
                 {
                 lock (fullOverlaps) fullOverlaps.Add(new BroadphasePair(swapper.Body, keyelement.Body));
                 }
                 }
                 
                 if (!keyelement.Begin && swapper.Begin)
                 {
                 lock (fullOverlaps) fullOverlaps.Remove(new BroadphasePair(swapper.Body, keyelement.Body));
                 }
                 */
                
                list.at(i + 1) = swapper;
                i = i - 1;
            }

        }
        list.at(i + 1) = keyelement;
    }
}
void Window::findOverlapPair(vector<OBJObject*> list){
    vector<OBJObject*> activeList = * new vector<OBJObject*>;
    activeList.push_back(list.at(0));
    Vector4 list_item, activeList_item;
    
    for (int i = 1; i<list.size(); i++) {
        //transfer local coordinate to world coordinate
        list_item.set(list.at(i)->min_x, list.at(i)->min_y, list.at(i)->min_z, 1);
        list_item = list.at(i)->toWorld * list_item;
        for (int j=0; j<activeList.size(); j++) {
            //transfer local coordinate to world coordinate
            activeList_item.set(activeList.at(j)->min_x, activeList.at(j)->min_y, activeList.at(j)->min_z, 1);
            activeList_item = activeList.at(j)->toWorld * activeList_item;
            
            //if (list.at(i)->min_x > activeList.at(i)->max_x) {
            if (list_item[0] > activeList_item[0]) {
                activeList.erase(activeList.begin() + j);
            }
            else{
                activeList.push_back(list.at(i));
                OBJPair pair;
                pair.o1 = list.at(i);
                pair.o2 = activeList.at(j);
                possibleCollisionPair.push_back(pair);
                break;
            }
        }
    }
}

void Window::testCollision(){
    for (int i = 0; i < possibleCollisionPair.size(); i++) {
        Vector4 o1VectorMin, o1VectorMax, o2VectorMin, o2VectorMax;
        o1VectorMin.set(possibleCollisionPair.at(i).o1->min_x, possibleCollisionPair.at(i).o1->min_y, possibleCollisionPair.at(i).o1->min_z, 1);
        o2VectorMin.set(possibleCollisionPair.at(i).o2->min_x, possibleCollisionPair.at(i).o2->min_y, possibleCollisionPair.at(i).o2->min_z, 1);
        o1VectorMax.set(possibleCollisionPair.at(i).o1->max_x, possibleCollisionPair.at(i).o1->max_y, possibleCollisionPair.at(i).o1->max_z, 1);
        o2VectorMax.set(possibleCollisionPair.at(i).o2->max_x, possibleCollisionPair.at(i).o2->max_y, possibleCollisionPair.at(i).o2->max_z, 1);
        o1VectorMin = possibleCollisionPair.at(i).o1->toWorld * o1VectorMin;
        o2VectorMin = possibleCollisionPair.at(i).o2->toWorld * o2VectorMin;
        o1VectorMax = possibleCollisionPair.at(i).o1->toWorld * o1VectorMax;
        o2VectorMax = possibleCollisionPair.at(i).o2->toWorld * o2VectorMax;
        
        if (o1VectorMin[1] < o2VectorMax[1] && o2VectorMin[1] < o1VectorMax[1]) {
            if (o1VectorMin[2] < o2VectorMax[2] && o2VectorMin[2] < o1VectorMax[2]) {
                cout << "Colision" << endl;
            }
        }
    }
}


//map 2D coordinate to real world 3D coordinate
Vector3 Window::trackObjMapping(int x, int y) {
	Vector3 v;
	float d;
	v.set((2.0*x - width) / width, (height - 2.0*y) / height, 0.0);
	d = v.magnitude();
	d = (d<1.0) ? d : 1.0;

	v.set((2.0*x - width) / width, (height - 2.0*y) / height, sqrtf(1.001 - d*d));
	v = v.normalize();
	return v;
}