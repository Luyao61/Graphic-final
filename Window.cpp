#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif
#import <OpenAL/alc.h>
#import <OpenAL/al.h>
#include <OpenAL/OpenAL.h>
#include <OpenAL/MacOSX_OALExtensions.h>
#include <cstdlib>
#include "Window.h"
#include "Cube.h"
#include "Matrix4.h"
#include "Globals.h"
#include "Skybox.hpp"
#include "BezierPatch.h"
#include "Shader.h"
#include "Texture.h"
#include <math.h>
#include <iostream>
#define TEST_XMAS			"src/merryXmas.wav"
#define SNOW_COUNT	1000	// particle count
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


bool cloudAnimantion = false;

vector<OBJObject*> list_x;
//vector<OBJObject*> list_y;
//vector<OBJObject*> list_z;
OBJObject * OBJptr = &Globals::cloud1;


//SNOW FLAKEs
unsigned char* snowflake;
GLuint snowTexture;
GLfloat yrot;
Texture * snowFlake;
int snowFrameCount = 0;
//openAL
ALuint      uiBuffer[3];
ALuint      uiSource[3];
ALint       iState, iState2, iState3;

struct SSnow
{
    float x, y, z;
    float xrot, yrot, zrot;
    float Dspeed;
    float Rspeed;
};
SSnow sSnow[SNOW_COUNT];

#define NUM_BUFFERS 3
#define BUFFER_SIZE 4096


void Window::initialize(void)
{
    //Setup the light
    Vector4 lightPos(100.0, 100.0, 100.0, 1.0);
    Globals::light.position = lightPos;
    Globals::light.quadraticAttenuation = 0.00;
    Globals::light.constantAttenuation = 1.0;
    Globals::light.linearAttenuation = 0.0;
    
    sky= new Skybox();
    platform = new BezierPatch();
    toon_Shader = new Shader("Toon.vert", "Toon.frag", true);
    light_Shader = new Shader("dirLightAmbDiffSpec.vert", "dirLightAmbDiffSpec.frag", true);
    ucsd_Shader = new Shader("ucsd.vert", "ucsd.frag", true);
    /*
    for (int i = 0; i<40; i++) {
        Globals::charizard.scale(false);
    }
    for (int i = 0; i<10; i++) {
        Globals::charizard.movey();
    }
    for (int i = 0; i<5; i++) {
        Globals::charizard.moveZ();
    }*/
    
    
    Globals::cloud1.translate(86, 70, 24);
    Globals::cloud2.translate(37, 70, 70);
    Globals::cloud3.translate(-12, 70, -40);
    Globals::cloud4.translate(10, 70, -66);
    Globals::cloud5.translate(0, 70, 0);
    Globals::cloud6.translate(-85, 70, -20);
    Globals::cloud7.translate(-52, 70, -85);
    Globals::cloud8.translate(-80, 70, -80);

    
    Globals::ft_wall.speed.set(0, 0, 0);
    Globals::bk_wall.speed.set(0, 0, 0);
    Globals::lt_wall.speed.set(0, 0, 0);
    Globals::rt_wall.speed.set(0, 0, 0);
    

    Globals::cloud1.speed.set(randomFloat(), 0, randomFloat());
    Globals::cloud2.speed.set(randomFloat(), 0, randomFloat());
    Globals::cloud3.speed.set(randomFloat(), 0, randomFloat());
    Globals::cloud4.speed.set(randomFloat(), 0, randomFloat());
    Globals::cloud5.speed.set(randomFloat(), 0, randomFloat());
    Globals::cloud6.speed.set(randomFloat(), 0, randomFloat());
    Globals::cloud7.speed.set(randomFloat(), 0, randomFloat());
    Globals::cloud8.speed.set(randomFloat(), 0, randomFloat());
    
    
    list_x = *new vector<OBJObject*>;

    list_x.push_back(&Globals::cloud1);
    list_x.push_back(&Globals::cloud2);
    list_x.push_back(&Globals::cloud3);
    list_x.push_back(&Globals::cloud4);
    list_x.push_back(&Globals::cloud5);
    list_x.push_back(&Globals::cloud6);
    list_x.push_back(&Globals::cloud7);
    list_x.push_back(&Globals::cloud8);
    list_x.push_back(&Globals::ft_wall);
    list_x.push_back(&Globals::bk_wall);
    list_x.push_back(&Globals::lt_wall);
    list_x.push_back(&Globals::rt_wall);


    insertionSortList(&list_x);
    findOverlapPair(list_x);
    
    srand(1);
    for (int i = 0; i<SNOW_COUNT; i++)
    {
        sSnow[i].x = float(rand() % 200 - 100);
        sSnow[i].z = float(rand() % 200 - 100);
        sSnow[i].y = 100.0f + float(rand() % 25);
        sSnow[i].xrot = sSnow[i].yrot = sSnow[i].zrot = 0;
        sSnow[i].Dspeed = 0.05f*(rand() % 10 + 2);
        sSnow[i].Rspeed = 0.1f*(rand() % 10 + 2);
    }
    
    snowFlake = new Texture("src/snowflakeppm");
    snowFlake->bind();
    
    Globals::tree.scale((float)0.6);
    Globals::tree.translate(0,-60,0);

    ALCdevice* openALDevice = alcOpenDevice(NULL);
    ALenum error = alGetError();
    
    ALCcontext* openALContext = alcCreateContext(openALDevice, NULL);
    alcMakeContextCurrent(openALContext);
    
    //generate output source
    ALuint outputSource;
    alGenSources(1, &outputSource);
    //create buffers to hold data
    ALuint outputBuffer;
    alGenBuffers(1, &outputBuffer);
    
    alSourcei(outputSource, AL_BUFFER, outputBuffer);
    
    FILE *fp = fopen("src/merryXmas.wav", "r");
    if (!fp) {
        fclose(fp);
    }
    char* ChunkID = new char[4];
    fread(ChunkID, 4, sizeof(char), fp);
    
    if (strcmp(ChunkID, "RIFF")) {
        delete [] ChunkID;
    }
    fseek(fp, 8, SEEK_SET);
    char *Format = new char[4];
    fread(Format, 4, sizeof(char), fp);
    if (strcmp(ChunkID, "RIFF")) {
        delete [] ChunkID;
        delete [] Format;
    }

    

    
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
    Globals::cloud1.showRedBBox = false;
    Globals::cloud2.showRedBBox = false;
    Globals::cloud3.showRedBBox = false;
    Globals::cloud4.showRedBBox = false;
    Globals::cloud5.showRedBBox = false;
    Globals::cloud6.showRedBBox = false;
    Globals::cloud7.showRedBBox = false;
    Globals::cloud8.showRedBBox = false;
    
    Globals::ft_wall.showRedBBox = false;
    Globals::bk_wall.showRedBBox = false;
    Globals::lt_wall.showRedBBox = false;
    Globals::rt_wall.showRedBBox = false;
    
    //collision detection;
    insertionSortList(&list_x);
    findOverlapPair(list_x);
    
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
    if (cloudAnimantion) {
        Globals::cloud1.animate();
        Globals::cloud2.animate();
        Globals::cloud3.animate();
        Globals::cloud4.animate();
        Globals::cloud5.animate();
        Globals::cloud6.animate();
        Globals::cloud7.animate();
        Globals::cloud8.animate();
    }

    
    Globals::l1.update();
    Globals::l1.draw();
    Globals::l2.update();
    Globals::l2.draw();
    Globals::l3.update();
    Globals::l3.draw();
    Globals::l4.update();
    Globals::l4.draw();
    
    Globals::l5.update();
    Globals::l5.draw();
    Globals::l6.update();
    Globals::l6.draw();
    Globals::l7.update();
    Globals::l7.draw();
    Globals::l8.update();
    Globals::l8.draw();
    
    Globals::l9.update();
    Globals::l9.draw();
    Globals::l10.update();
    Globals::l10.draw();
    Globals::l11.update();
    Globals::l11.draw();
    Globals::l12.update();
    Globals::l12.draw();
    
    Globals::y1.update();
    Globals::y1.draw();
    Globals::y2.update();
    Globals::y2.draw();
    Globals::y3.update();
    Globals::y3.draw();
    Globals::y4.update();
    Globals::y4.draw();
    
    Globals::y5.update();
    Globals::y5.draw();
    Globals::y6.update();
    Globals::y6.draw();
    Globals::y7.update();
    Globals::y7.draw();
    Globals::y8.update();
    Globals::y8.draw();
    
    //sky->draw();

    //light_Shader->bind();
    //toon_Shader -> bind();
    
    Globals::tree.draw(Globals::drawData);

    Globals::cloud1.draw(Globals::drawData);
    Globals::cloud2.draw(Globals::drawData);
    Globals::cloud3.draw(Globals::drawData);
    Globals::cloud4.draw(Globals::drawData);
    Globals::cloud5.draw(Globals::drawData);
    Globals::cloud6.draw(Globals::drawData);
    Globals::cloud7.draw(Globals::drawData);
    Globals::cloud8.draw(Globals::drawData);
    
    Globals::ft_wall.drawBoundingBox();
    Globals::bk_wall.drawBoundingBox();
    Globals::lt_wall.drawBoundingBox();
    Globals::rt_wall.drawBoundingBox();
    
    //Globals::charizard.draw(Globals::drawData );
    //toon_Shader -> unbind();
    
    //DrawScene(0);
    
    //ucsd_Shader->bind();
    //platform->draw();
    //ucsd_Shader->unbind();
    //light_Shader->unbind();
    
    
    
    glDisable(GL_TEXTURE_2D);

    if (snowFrameCount > 0)
    {
        if (snowFrameCount == 300)
        {
            //alSourcePause(uiSource[0]);
            //alSourcePlay(uiSource[2]);
            //alSourcePlay(uiSource[0]);
        }
        
        glClear(GL_DEPTH_BUFFER_BIT);
        glLoadMatrixf(Globals::camera.getInverseMatrix().ptr());
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);					// Set The Blending Function For Translucency
        glEnable(GL_BLEND);
        for (int i = 0; i<SNOW_COUNT; i++)
        {
            if(1)// (!(sSnow[i].x > width || sSnow[i].y > height || sSnow[i].z > 500))
            {
                glLoadIdentity();									// Reset The View
                glTranslatef(0.0f, -30.0f, -150.0f);
                glRotatef(yrot, 0.0f, 1.0f, 0.0f);
                glBindTexture(GL_TEXTURE_2D, snowTexture);
                glTranslatef(sSnow[i].x, sSnow[i].y, sSnow[i].z);
                glRotatef(sSnow[i].xrot, 1.0f, 0.0f, 0.0f);
                glRotatef(sSnow[i].yrot, 0.0f, 1.0f, 0.0f);
                glRotatef(sSnow[i].zrot, 0.0f, 0.0f, 1.0f);
                
                glScalef(0.11f, 0.09f, 0.096f);
                //glScalef(0.0812f,0.0851f,0.08512f);
                glBegin(GL_LINES);
                
                ///////////////////////////////////////////////////////////////////////////////
                // Front Face
                glNormal3f(0.0f, 0.0f, 0.5f);
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
                // Back Face
                glNormal3f(0.0f, 0.0f, -0.5f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
                glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
                // Top Face
                glNormal3f(0.0f, 0.5f, 0.0f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
                // Bottom Face
                glNormal3f(0.0f, -0.5f, 0.0f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
                glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
                // Right Face
                glNormal3f(0.5f, 0.0f, 0.0f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
                glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
                // Left Face
                glNormal3f(-0.5f, 0.0f, 0.0f);
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
                //////////////////////////////////////////////////////////////////////
                
                glEnd();
                /////////////////////////////////////////////////////////////end
                
                //std::cout << sSnow[i].x << "	" << sSnow[i].y << "	" << sSnow[i].z << std::endl;
                sSnow[i].y -= sSnow[i].Dspeed;
                if (sSnow[i].y < -33)
                    sSnow[i].y = 125.0f;
                sSnow[i].xrot += sSnow[i].Rspeed;
                sSnow[i].yrot += sSnow[i].Rspeed;
                sSnow[i].zrot += sSnow[i].Rspeed;
            }
        }
        yrot += 0.2f;
        snowFrameCount--;
        glDisable(GL_BLEND);
    }
    
    

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
    }
    else if (key == 'S'){
    }
    else if (key == 'x'){
        OBJptr->movex();
    }
    else if (key == 'X'){
        OBJptr->moveX();
    }
    else if (key == 'y'){
        OBJptr->movey();
    }
    else if (key == 'Y'){
        OBJptr->moveY();
    }
    else if (key == 'z'){
        OBJptr->movez();
    }
    else if (key == 'Z'){
        OBJptr->moveZ();
    }
    else if (key == 'r' || key == 'R'){
        Globals::camera.reset();
        angle_Horizontal = angle_Vertical = 0.0f;
        //OBJptr->toWorld.identity();
    }
    else if(key == '1'){
        OBJptr = &Globals::cloud1;
    }
    else if(key == '2'){
        OBJptr = &Globals::cloud2;
    }
    else if(key == '3'){
        OBJptr = &Globals::cloud3;
    }
    else if(key == '4'){
        OBJptr = &Globals::cloud4;
    }
    else if(key == 'p'){
        cloudAnimantion = !cloudAnimantion;
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
        float distance = (Globals::camera.e - Globals::camera.d).magnitude();
        x_d = distance * sin(angle_Horizontal + deltaAngleX) * cos(angle_Vertical + deltaAngleY);
        z_d = distance * cos(angle_Horizontal + deltaAngleX) * cos(angle_Vertical + deltaAngleY);
        y_d = distance * sin(angle_Vertical + deltaAngleY);

        Globals::camera.d.set(Globals::camera.e[0] + x_d, Globals::camera.e[1] + y_d, Globals::camera.e[2] - z_d);
        
        //y_v = 20.f * cos(angle_Vertical + deltaAngleY);

        Globals::camera.update();
        
    }
}

void Window::insertionSortList(vector<OBJObject*>* list){
    Vector4 keyelementVector, listelementVector;
    OBJObject *keyelement;
    OBJObject *swapper;
    for (int i =1; i < list->size(); i++){
        int j = i;
        keyelementVector.set(list->at(j)->min_x, list->at(j)->min_y, list->at(j)->min_z, 1);
        keyelementVector = list->at(j)->toWorld * keyelementVector;
        
        listelementVector.set(list->at(j-1)->min_x, list->at(j-1)->min_y, list->at(j-1)->min_z, 1);
        listelementVector = list->at(j-1)->toWorld * listelementVector;
        
        while (j > 0 && keyelementVector[0] < listelementVector[0]) {

                swapper = list->at(j);
                list->at(j) = list->at(j-1);
                list->at(j-1) = swapper;
                j--;
            if (j == 0) {
                break;
            }
            keyelementVector.set(list->at(j)->min_x, list->at(j)->min_y, list->at(j)->min_z, 1);
            keyelementVector = list->at(j)->toWorld * keyelementVector;
            
            listelementVector.set(list->at(j-1)->min_x, list->at(j-1)->min_y, list->at(j-1)->min_z, 1);
            listelementVector = list->at(j-1)->toWorld * listelementVector;
        }
    }

}
void Window::findOverlapPair(vector<OBJObject*> list){
    vector<OBJObject*> activeList = * new vector<OBJObject*>;
    Vector4 list_item_MIN, list_item_MAX, activeList_item_MAX, activeList_item_MIN;
    vector<OBJPair> possibleCollisionPair = *new vector<OBJPair>;
    bool push = true;
    
    activeList.push_back(list.at(0));

    for (int i = 1; i<list.size(); i++) {
        push = true;
        //if(activeList.size() == 0){
        //    activeList.push_back(list.at(i));
        //    continue;
        //}
        //transfer local coordinate to world coordinate
        list_item_MIN.set(list.at(i)->min_x, list.at(i)->min_y, list.at(i)->min_z, 1);
        list_item_MIN = list.at(i)->toWorld * list_item_MIN;
        list_item_MAX.set(list.at(i)->max_x, list.at(i)->max_y, list.at(i)->max_z, 1);
        list_item_MAX = list.at(i)->toWorld * list_item_MAX;
        int n = (int)activeList.size();
        for (int j=0; j < n; j++) {
            //transfer local coordinate to world coordinate
            //check the end point of active list item, because list is sorted;
            activeList_item_MAX.set(activeList.at(j)->max_x, activeList.at(j)->max_y, activeList.at(j)->max_z, 1);
            activeList_item_MAX = activeList.at(j)->toWorld * activeList_item_MAX;
            
            activeList_item_MIN.set(activeList.at(j)->min_x, activeList.at(j)->min_y, activeList.at(j)->min_z, 1);
            activeList_item_MIN = activeList.at(j)->toWorld * activeList_item_MIN;
            
            //if (list.at(i)->min_x > activeList.at(i)->max_x) {
            if (list_item_MIN[0] >= activeList_item_MAX[0] || list_item_MAX[0] <= activeList_item_MIN[0]) {
                activeList.erase(activeList.begin() + j);
                n=n-1;
                if (push) {
                    activeList.push_back(list.at(i));
                    push = false;
                }
            }
            else{
                OBJPair pair;
                pair.o1 = list.at(i);
                pair.o2 = activeList.at(j);
                possibleCollisionPair.push_back(pair);
                //break;
                if (push) {
                    activeList.push_back(list.at(i));
                    push = false;
                }
            }
        }
    }
    testCollision(possibleCollisionPair);
}

void Window::testCollision(vector<OBJPair> possibleCollisionPair){
    OBJObject * o1, * o2;
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
                //cout << "Colision" << " "<< t << endl;
                
                
                o1 = possibleCollisionPair.at(i).o1;
                o2 = possibleCollisionPair.at(i).o2;
                
                o1->showRedBBox = true;
                o2->showRedBBox = true;
                //handle ball wall collision
                if (o1->speed.magnitude() == 0) {
                    //Vec3f dir = (wallDirection(w)).normalize();
                    //b->v -= 2 * dir * b->v.dot(dir);
                    o2->setSpeed(o2 -> speed - o1->WallDirection * 2 * o2->speed.dot(o1->WallDirection));
                }
                else if(o2->speed.magnitude() == 0){
                    o1->setSpeed(o1 -> speed - o2->WallDirection * 2 * o1->speed.dot(o2->WallDirection));
                }else{
                    //Vector3 displacement = (o1->toWorld * o1->center - o2->toWorld * o2->center).toVector3().normalize();
                //possibleCollisionPair.at(i).o1->speed = o1 -> speed - (displacement * 2 * o1->speed.dot(displacement));
                //possibleCollisionPair.at(i).o2->speed = o2 -> speed - (displacement * 2 * o2->speed.dot(displacement));
                    //o1->setSpeed(o1 -> speed - (displacement * 2 * o1->speed.dot(displacement)));
                    //o2->setSpeed(o2 -> speed - (displacement * 2 * o2->speed.dot(displacement)));
                    o1->setSpeed(o1->speed.negate());
                    o2->setSpeed(o2->speed.negate());
                }
                snowFrameCount = 300;
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


float Window::randomFloat(){
    const float MIN_RAND = -1.0, MAX_RAND = 1.0;
    const float range = MAX_RAND - MIN_RAND;
    float random = range * ((((float) rand()) / (float) RAND_MAX)) + MIN_RAND ;
    return random;
}