#include "Camera.h"

Camera::Camera()
{
    c.identity();
    e.set(0.0, 0.0, 20.0);
    d.set(0.0, 0.0, 0.0);
    up.set(0.0, 1.0, 0.0);
    
    update();
    
    //Pre-define a camera matrix (and its inverse) that are shifted 'e' from the origin
    //This is used as a default camera position for Project 1
    //c.set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, e[0], e[1], e[2], 1);
    //ci.set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -e[0], -e[1], -e[2], 1);
}

Camera::~Camera()
{
    //Delete and dynamically allocated memory/objects here
}

Matrix4& Camera::getMatrix()
{
    update();
    return c;
}

Matrix4& Camera::getInverseMatrix()
{
    update();
    return ci;
}

void Camera::update()
{
    //Update the Camera Matrix using d, e, and up
    //Solve for the z, x, and y axes of the camera matrix
    //Use these axes and the e vector to create a camera matrix c
    
    Vector3 z_c = e.subtract(d).multiply(1/e.subtract(d).magnitude());
    Vector3 x_c = up.cross(z_c).multiply(1/(up.cross(z_c).magnitude()));
    Vector3 y_c = up.multiply(1/up.magnitude());
    c.set( *x_c.ptr(), *(x_c.ptr()+1), *(x_c.ptr()+2), 0.0,
          *y_c.ptr(), *(y_c.ptr()+1), *(y_c.ptr()+2), 0.0,
          *z_c.ptr(), *(z_c.ptr()+1), *(z_c.ptr()+2), 0.0,
          *e.ptr(), *(e.ptr()+1), *(e.ptr()+2), 1.0);
    
    //Use c to solve for an inverse camera matrix ci
    ci = c.rigidInverse();
    
}

void Camera::set(Vector3& e, Vector3& d, Vector3& up)
{
    this->e = e;
    this->d = d;
    this->up = up;
    update();
}

void Camera::reset(){
    c.identity();
    e.set(0.0, 0.0, 20.0);
    d.set(0.0, 0.0, 0.0);
    up.set(0.0, 1.0, 0.0);
    update();
    
}
