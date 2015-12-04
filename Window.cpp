#include <iostream>
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
#include "scene.h"


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

Vector4 lightPos(10.0, 10.0, 15.0, 1.0);
GLuint shadowMapTexture; // 纹理名字
Matrix4 lightProjectionMatrix; // 光源视角的投影矩阵
Matrix4 lightViewMatrix; // 光源视角的视图矩阵
Matrix4 cameraProjectionMatrix;
Matrix4 cameraViewMatrix;
const int shadowMapSize=512;

// Shadow Map
namespace SM{
    const int RATIO = 2;
    
    const GLfloat WIDTH = 3000;
    const GLfloat HEIGHT = 3000;
    //const GLfloat HEIGHT = 2048.0;
    
    GLhandleARB shaderID;
    GLuint uniform;
    GLuint uniformX;
    GLuint uniformY;
    
    GLuint shadowing;
    int mode = OFF;
    
    // Depth texture
    GLuint texture;
    
    GLuint matrix;
}


void Window::initialize(void)
{
    //Setup the light
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


    glGenTextures(1, &shadowMapTexture);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize, 0,
                 GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    updateMatrix();

}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
// This is called at the start of every new "frame" (qualitatively)
void Window::idleCallback()
{
    //Call the display routine to draw the cube
    displayCallback();
    
    //firstPass();
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
    Globals::charizard.draw(Globals::drawData );
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
    else if (key == 's') {
        Globals::charizard.scale(true);
        //Globals::pokemon.keyboard(key, x, y);
    }
    else if (key == 'S'){
        Globals::charizard.scale(false);
        //Globals::pokemon.keyboard(key, x, y);
    }
    else if (key == 'x'){
        Globals::charizard.movex();
    }
    else if (key == 'X'){
        Globals::charizard.moveX();
    }
    else if (key == 'y'){
        Globals::charizard.movey();
    }
    else if (key == 'Y'){
        Globals::charizard.moveY();
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


void Window::firstPass(){
    //First pass - from light's point of view
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(lightProjectionMatrix.ptr());
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(lightViewMatrix.ptr());
    //Use viewport the same size as the shadow map
    glViewport(0, 0, shadowMapSize, shadowMapSize);
     //Draw back faces into the shadow map
    glCullFace(GL_FRONT);
    //Disable color writes, and use flat shading for speed
    glShadeModel(GL_FLAT);
    glColorMask(0, 0, 0, 0);
    
    //Draw the scene
    Globals::charizard.draw(Globals::drawData );
    platform->draw();
    //DrawScene(0.0);
    
    //Read the depth buffer into the shadow map texture
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, shadowMapSize, shadowMapSize);
    //restore states
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);
    glColorMask(1, 1, 1, 1);
    
    secondPass();
}

void Window::secondPass(){
    glClear(GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(cameraProjectionMatrix.ptr());
    glMatrixMode(GL_MODELVIEW);
    //glLoadMatrixf(cameraViewMatrix.ptr());
    glLoadMatrixf(Globals::camera.getInverseMatrix().ptr());

    glViewport(0, 0, width, height);
    
    //Use dim light to represent shadowed areas
    GLfloat white[] = {0.2, 0.2, 0.2, 0.2};
    GLfloat black[] = {0.0, 0.0, 0.0, 0.0};
    GLfloat position[] = {10.0,10.0,15.0,1.0};
    glLightfv(GL_LIGHT1, GL_POSITION, position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, white);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, black);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
    
    thirdPass();
}

void Window::thirdPass(){
    //Draw with bright light
    GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    
    static Matrix4 biasMatrix(0.5f, 0.0f, 0.0f, 0.0f,
                                0.0f, 0.5f, 0.0f, 0.0f,
                                0.0f, 0.0f, 0.5f, 0.0f,
                                0.5f, 0.5f, 0.5f, 1.0f); //bias from [-1, 1] to [0, 1]
    Matrix4 textureMatrix=biasMatrix*lightProjectionMatrix*lightViewMatrix;
    //Set up texture coordinate generation.
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_S, GL_EYE_PLANE, textureMatrix.getRow(0).ptr());
    glEnable(GL_TEXTURE_GEN_S);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_T, GL_EYE_PLANE, textureMatrix.getRow(1).ptr());
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_R, GL_EYE_PLANE, textureMatrix.getRow(2).ptr());
    glEnable(GL_TEXTURE_GEN_R);
    glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_Q, GL_EYE_PLANE, textureMatrix.getRow(3).ptr());
    glEnable(GL_TEXTURE_GEN_Q);
    
    
    //Bind & enable shadow map texture
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glEnable(GL_TEXTURE_2D);
    //Enable shadow comparison
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);
    //Shadow comparison should be true (ie not in shadow) if r<=texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
    //Shadow comparison should generate an INTENSITY result
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
    
    //Set alpha test to discard false comparisons
    glAlphaFunc(GL_GEQUAL, 0.99f);
    glEnable(GL_ALPHA_TEST);
    
    //Draw the scene
    //Globals::charizard.draw(Globals::drawData );
    //platform->draw();
    Globals::charizard.draw(Globals::drawData);
    platform->draw();
    
    
    //Disable textures and texgen
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);
    //Restore other states
    glDisable(GL_LIGHTING);
    glDisable(GL_ALPHA_TEST);
    
    
    //Set matrices for ortho
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    //reset matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glFinish();
    glutSwapBuffers();
    glutPostRedisplay();
}

void Window::updateMatrix(){
    
    
    //Calculate & save matrices
    glPushMatrix();
    
    cameraProjectionMatrix.makePerspectiveProjection(60.0f, (float)width, (float)height, 1.0f, 1000.0f);
    
    glLoadIdentity();
    gluLookAt(Globals::camera.e[0], Globals::camera.e[1], Globals::camera.e[2],
              Globals::camera.d[0], Globals::camera.d[1], Globals::camera.d[2],
              0.0f, 1.0f, 0.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, cameraViewMatrix.ptr());

    lightProjectionMatrix.makePerspectiveProjection(90.0f, 1.0f,1.0f, 1.0f, 1000.0f);
    
    glLoadIdentity();
    gluLookAt( Globals::light.position[0], Globals::light.position[1], Globals::light.position[2],
              Globals::camera.d[0], Globals::camera.d[1], Globals::camera.d[2],
              0.0f, 1.0f, 0.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix.ptr());
    
    glPopMatrix();
}

