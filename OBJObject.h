#ifndef CSE167_OBJObject_h
#define CSE167_OBJObject_h

#include <iostream>
#include <vector>
#include <string>
#include "Vector3.h"
#include "Drawable.h"
#include "Vector4.h"



class OBJObject : public Drawable
{
    
protected:
    

    
    //Helper functions
    std::vector<std::string>& split(const std::string&, char, std::vector<std::string>&);
    std::vector<std::string> split(const std::string&, char);
    
    //Parse
    void parse(std::string&);
    void drawBoundingBox(void);

    
public:
    
    //Storage vectors

    
    OBJObject(std::string);
    OBJObject(OBJObject const &);
    virtual ~OBJObject(void);
    
    virtual void draw(DrawData&);
    virtual void update(UpdateData&);
    
    
    
};

#endif
