#include "Vector3.h"
#include <math.h>
#include <iostream>
#include <cstring>
#include "Vector4.h"

Vector3::Vector3()
{
    std::memset(m, 0, sizeof(m));
}

Vector3::Vector3(float m0, float m1, float m2)
{
    m[0] = m0;
    m[1] = m1;
    m[2] = m2;
}
Vector3::Vector3(GLfloat gl[3]){
    m[0] = gl[0];
    m[1] = gl[1];
    m[2] = gl[2];
}

Vector3::~Vector3()
{
    //
}

float* Vector3::ptr()
{
    return &m[0];
}

void Vector3::set(float x, float y, float z)
{
    m[0] = x;
    m[1] = y;
    m[2] = z;
}

void Vector3::set(int index, float value)
{
    m[index] = value;
}

float& Vector3::operator [] (int loc)
{
    return m[loc];
}

Vector3 Vector3::add(Vector3 a)
{
    Vector3 b;
    float x = *this->ptr() + *a.ptr();
    float y = *(this->ptr()+1) + *(a.ptr()+1);
    float z = *(this->ptr()+2) + *(a.ptr()+2);

    
    b = *new Vector3(x, y, z);
    //
    
    return b;
}

Vector3 Vector3::operator + (Vector3 a)
{
    return add(a);
}

Vector3 Vector3::subtract(Vector3 a)
{
    Vector3 b;
    
    //
    float x = *this->ptr() - *a.ptr();
    float y = *(this->ptr()+1) - *(a.ptr()+1);
    float z = *(this->ptr()+2) - *(a.ptr()+2);
    
    
    b = *new Vector3(x, y, z);

    
    return b;
}

Vector3 Vector3::operator - (Vector3 a)
{
    return subtract(a);
}

Vector3 Vector3::negate(void)
{
    Vector3 b;
    
    //
    float x = -*this->ptr();
    float y = -*(this->ptr()+1);
    float z = -*(this->ptr()+2);
    
    
    b = *new Vector3(x, y, z);

    
    return b;
}

Vector3 Vector3::scale(float s)
{
    Vector3 b;
    
    //
    float x = *this->ptr() * s;
    float y = *(this->ptr()+1) * s;
    float z = *(this->ptr()+2) * s;
    
    
    b = *new Vector3(x, y, z);

    
    return b;
}

Vector3 Vector3::multiply(float a)
{
    Vector3 b;
    
    //
    float x = *this->ptr() * a;
    float y = *(this->ptr()+1) * a;
    float z = *(this->ptr()+2) * a;
    
    
    b = *new Vector3(x, y, z);
    
    return b;
}

Vector3 Vector3::operator * (float a)
{
    return multiply(a);
}

Vector3 Vector3::multiply(Vector3 a)
{
    Vector3 b;
    
    //Coomponent-wise multiplication
    float x = *this->ptr() * *a.ptr();
    float y = *(this->ptr()+1) * *(a.ptr()+1);
    float z = *(this->ptr()+2) * *(a.ptr()+2);
    
    
    b = *new Vector3(x, y, z);

    
    return b;
}

Vector3 Vector3::operator * (Vector3 a)
{
    return multiply(a);
}


float Vector3::dot(Vector3 a)
{
    //
    float x = *this->ptr() * *a.ptr();
    float y = *(this->ptr()+1) * *(a.ptr()+1);
    float z = *(this->ptr()+2) * *(a.ptr()+2);

    return x+y+z;
}

Vector3 Vector3::cross(Vector3 a)
{
    float ux,uy,uz,vx,vy,vz;
    Vector3 b;
    ux = *this->ptr();
    uy = *(this->ptr()+1);
    uz = *(this ->ptr()+2);
    
    vx = *a.ptr();
    vy = *(a.ptr()+1);
    vz = *(a.ptr()+2);
    
    float x = uy*vz-uz*vy;
    float y = uz*vx-ux*vz;
    float z = ux*vy-uy*vx;
    
    b = *new Vector3(x,y,z);
    
    return b;
}

float Vector3::angle(Vector3 a)
{
    //
    return acos( this->dot(a) / (this->magnitude() * a.magnitude()));
    
}

float Vector3::magnitude(void)
{
    float x,y,z;
    x = *this->ptr();
    y = *(this->ptr()+1);
    z = *(this ->ptr()+2);
    
    return sqrt(x*x+y*y+z*z);
    
}

Vector3 Vector3::normalize(void)
{
    Vector3 b;
    float x,y,z;
    if (this->magnitude() != 0) {
        x = *this->ptr() / this->magnitude();
        y = *(this->ptr()+1) / this->magnitude();
        z = *(this ->ptr()+2) / this->magnitude();
        b= *new Vector3(x,y,z);
    }
    else{
        b = *new Vector3(0.0,0.0,0.0);
    }
    
    return b;
}

Vector4 Vector3::toVector4(float w)
{
    Vector4 b(m[0], m[1], m[2], w);
    return b;
}

void Vector3::print(std::string comment)
{
    std::cout << comment << std::endl;
    std::cout << "<x:" << m[0] <<  ", y:" << m[1] << ", z:" << m[2] << ">" << std::endl;
}
