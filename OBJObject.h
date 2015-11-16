#ifndef CSE167_OBJObject_h
#define CSE167_OBJObject_h

#include <iostream>
#include <vector>
#include <string>
#include "Vector3.h"
#include "Drawable.h"

struct Face
{
    int vertexIndices[3];
    int normalIndices[3];
    //Add more members as necessary
};

class OBJObject : public Drawable
{
    
protected:
    
    //Storage vectors
    std::vector<Vector3*>* vertices;
    std::vector<Vector3*>* normals;
    std::vector<Face*>* faces;
    
    //Helper functions
    std::vector<std::string>& split(const std::string&, char, std::vector<std::string>&);
    std::vector<std::string> split(const std::string&, char);
    
    //Parse
    void parse(std::string&);
    
public:
    
    OBJObject(std::string);
    virtual ~OBJObject(void);
    
    virtual void draw(DrawData&);
    virtual void update(UpdateData&);
    
};

#endif
