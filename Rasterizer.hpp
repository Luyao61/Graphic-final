//
//  Rasterizer.hpp
//  CSE167 Spring 2015 Starter Code
//
//  Created by Luyao Zhou on 10/12/15.
//  Copyright © 2015 RexWest. All rights reserved.
//

#ifndef Rasterizer_hpp
#define Rasterizer_hpp

#include <stdio.h>
#include "Matrix4.h"
#include "Drawable.h"


class Rasterizer : public Drawable{
private:
    
protected:
    std::vector<std::string>& split(const std::string&, char, std::vector<std::string>&);
    std::vector<std::string> split(const std::string&, char);

    void parse(std::string&);

public:
    Matrix4 d;   //Viewport Matrix
    Matrix4 p;  //Projection Matrix
    
    Drawable *toDraw;
    
    Rasterizer(void);
    ~Rasterizer(void);
    
    void loadData(std::string);
    void clearBuffer(void);
    void drawPoint(int,int,float,float,float);
    void rasterizeTriangle();
    void rasterize();
    void reshape(int new_width, int new_height);
    void keyboard(unsigned char key, int, int);
    void display();
    void updateD(void);
    void updateP(void);
    
    Vector4 rasterizeVertex(Vector4);
    
    void draw(DrawData& data);
    
};

#endif /* Rasterizer_hpp */
