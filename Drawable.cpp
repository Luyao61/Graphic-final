#include "Drawable.h"
#include "Window.h"

Drawable::Drawable()
{
    this->toWorld.identity();
    center = *new Vector4(0,0,0,1);
    renderMode = 0;
    
    this->vertices = new std::vector<Vector3*>();
    this->normals = new std::vector<Vector3*>();
    this->faces = new std::vector<Face*>();
    this->colors = new std::vector<Vector3*>();
    
    //added for project 3
    this->colorP3 = new std::vector<Color*>();
    
}

Drawable::~Drawable()
{
    //
}

void Drawable::draw(DrawData& data)
{
    std::cout << "Warning: draw() is not implemented" << std::endl;
}

void Drawable::update(UpdateData& data)
{
    std::cout << "Warning: update() is not implemented" << std::endl;
}



//self Centered!;
void Drawable::spin(float radians)
{
    Matrix4 rotation;
    rotation.makeRotateY(radians);
    
    toWorld = toWorld  * rotation;
}
void Drawable::orbit(float radians){
    Matrix4 rotation;
    rotation.makeRotateZ(radians);
    
    toWorld = rotation  * toWorld;
    
    center = rotation * center;
    
}

void Drawable::moveX(){
    Matrix4 move;
    move = move.makeTranslate(0.5, 0, 0);
    
    toWorld = move * toWorld;
    
    center = move * center;
    
}
void Drawable::movex(){
    Matrix4 move;
    move = move.makeTranslate(-0.5, 0, 0);
    
    toWorld = move * toWorld;
    center = move * center;
    
}

void Drawable::moveY(){
    Matrix4 move;
    move = move.makeTranslate(0, 0.5, 0);
    
    toWorld = move * toWorld;
    center = move * center;
    
}
void Drawable::movey(){
    Matrix4 move;
    move = move.makeTranslate(0, -0.5, 0);
    
    toWorld = move * toWorld;
    center = move * center;
}
void Drawable::moveZ(){
    Matrix4 move;
    move = move.makeTranslate(0, 0, 0.5);
    
    toWorld = move * toWorld;
    center = move * center;
    
}
void Drawable::movez(){
    Matrix4 move;
    move = move.makeTranslate(0, 0, -0.5);
    
    toWorld = move * toWorld;
    center = move * center;
}


void Drawable::scale(bool x){
    Matrix4 scale;
    if (x) {
        scale.makeScale(1.1);
    }
    else{
        scale.makeScale(0.9);
    }
    toWorld = toWorld * scale;
}

void Drawable::reset(){
    toWorld.identity();
    center.set(0, 0, 0, 1);
}