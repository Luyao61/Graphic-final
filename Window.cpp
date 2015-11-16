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
#include "BezierCurve.h"
#include "BezierPatch.h"
#include "Shader.h"
#include <math.h>

int Window::width  = 512;   //Set window width in pixels here
int Window::height = 512;   //Set window height in pixels here
Skybox *sky;

Vector3 oldPoint;
BezierCurve *b;
BezierPatch *p;
int oldX, oldY;
bool leftButton = false;
bool rightButton = false;
bool isDraging = false;

Shader *shader;

int t;

void Window::initialize(void)
{
    //Setup the light
    Vector4 lightPos(0.0, 10.0, 15.0, 1.0);
    Globals::light.position = lightPos;
    Globals::light.quadraticAttenuation = 0.02;
    
    //Initialize cube matrix:
    Globals::cube.toWorld.identity();
    
    //Setup the cube's material properties
    Color color(0x23ff27ff);
    Globals::cube.material.color = color;
    sky= new Skybox();
	Vector3 p0, p1, p2, p3;
	p0.set(-4, -4, 0);
	p1.set(-2, 4, 0);
	p2.set(2, -4, 0);
	p3.set(4, 4, 0);
	b = new BezierCurve(p0, p1, p2, p3);
    /*
    GLfloat ctrlpoints[4][4][3] = {
        {{-1.5, -1.5, 4.0}, {-0.5, -1.5, 2.0},
            {0.5, -1.5, -1.0}, {1.5, -1.5, 2.0}},
        {{-1.5, -0.5, 1.0}, {-0.5, -0.5, 3.0},
            {0.5, -0.5, 0.0}, {1.5, -0.5, -1.0}},
        {{-1.5, 0.5, 4.0}, {-0.5, 0.5, 0.0},
            {0.5, 0.5, 3.0}, {1.5, 0.5, 4.0}},
        {{-1.5, 1.5, -2.0}, {-0.5, 1.5, -2.0},
            {0.5, 1.5, 0.0}, {1.5, 1.5, -1.0}}
    };*/

    p = new BezierPatch();
    
    shader = new Shader("sample.vert", "sample.frag", true);
}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
// This is called at the start of every new "frame" (qualitatively)
void Window::idleCallback()
{
    //Set up a static time delta for update calls
    Globals::updateData.dt = 1.0/60.0;// 60 fps
    
    //Rotate cube; if it spins too fast try smaller values and vice versa
    //Globals::cube.spin(0.0005);
    
    //Call the update function on cube
    //Globals::cube.update(Globals::updateData);
    
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
    gluPerspective(60.0, double(width)/(double)height, 1.0, 10000.0); //Set perspective projection viewing frustum
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
    
    //Replace the current top of the matrix stack with the inverse camera matrix
    //This will convert all world coordiantes into camera coordiantes
    glLoadMatrixf(Globals::camera.getInverseMatrix().ptr());
    
    //Bind the light to slot 0.  We do this after the camera matrix is loaded so that
    //the light position will be treated as world coordiantes
    //(if we didn't the light would move with the camera, why is that?)
    Globals::light.bind(0);
    
    //Draw the cube!
    sky->draw();
    
    p->update(t);
    
    shader->bind();
    p->draw();
    shader->unbind();

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
    else if(key == 'a' || key == 'A'){
        Matrix4 r;
        r.makeRotateY(0.1);
        Globals::camera.e = r * Globals::camera.e;
        Globals::camera.update();
    }
    else if(key == 'd' || key == 'D'){
        Matrix4 r;
        r.makeRotateY(-0.1);
        Globals::camera.e = r * Globals::camera.e;
        Globals::camera.update();
    }
    else if(key == 'w' || key == 'W'){
        
        Matrix4 r;
        r.makeRotateX(-0.1);
        Globals::camera.e = r * Globals::camera.e;
        Globals::camera.update();
        
    }
    else if(key == 's' || key == 'S'){
        
        Matrix4 r;
        r.makeRotateX(0.1);
        Globals::camera.e = r * Globals::camera.e;
        Globals::camera.update();
        
    }
    else if(key == 'r' || key == 'R'){
        Globals::camera.e = Vector3(0,0,20);
        Globals::camera.update();
    }
}
//TODO: Function Key callbacks!

//TODO: Mouse callbacks!
void Window::processMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) { // left mouse button pressed
			isDraging = true;
			leftButton = true;
		}
		else { /* (state = GLUT_UP) */
			isDraging = false;
			leftButton = false;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) { // left mouse button pressed
			isDraging = true;
			rightButton = true;

			oldX = x;
			oldY = y;
		}
		else { /* (state = GLUT_UP) */
			isDraging = false;
			rightButton = false;
		}
	}
}
//TODO: Mouse Motion callbacks!
void Window::processMotion(int x, int y) {
	Vector3 currPoint;
	Vector3 direction;
	if (isDraging) {
		if (leftButton && !rightButton) {
			currPoint = trackObjMapping(x, y);
			direction = currPoint - oldPoint;
			Vector3 rotateAxis;
			float rotateAngle;
			rotateAxis = currPoint.cross(oldPoint);
			rotateAngle = oldPoint.angle(currPoint);

			//Globals::cube.makeRotateArbitrary(rotateAxis, rotateAngle);
		}
		else if (!leftButton && rightButton) {
			//Globals::cube.makeTranslate((x - oldX) / 10.0, (oldY - y) / 10.0, 0.0);
		}
	}
	oldPoint = currPoint;
	oldX = x;
	oldY = y;
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
