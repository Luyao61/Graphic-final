#ifndef CSE167_Light_h
#define CSE167_Light_h

#include "Matrix4.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Color.h"
#include <vector>

class Light
{
    
protected:
    
    int bindID = -1;
    
public:
    
    Vector4 position;
    
    Vector3 spotdirection;
    
    Color ambientColor;
    Color diffuseColor;
    Color specularColor;
    
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    
    float cutoff;
    float exponent;
    
public:
    
    Light();
    virtual ~Light(void);
    
    void bind(int);
    void unbind(void);
    
    
};

#endif
