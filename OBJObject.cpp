#include "OBJObject.h"

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "Window.h"
#include "math.h"
#include <sstream>
#include <fstream>
#include "Vector3.h"
#include "Globals.h"


#define deleteVector(__type__, __vect__) do {\
                                   std::vector<__type__>::iterator iter; \
                                   std::vector<__type__>::iterator end; \
                                   iter = __vect__->begin();\
                                   end = __vect__->end();\
                                   while(iter != end) delete (*(iter++));\
                                   delete __vect__;\
                               } while(false)


OBJObject::OBJObject(std::string filename) : Drawable()
{
    center = * new Vector4(0,0,0,1);
    this->vertices = new std::vector<Vector3*>();
    this->normals = new std::vector<Vector3*>();
    this->faces = new std::vector<Face*>();
    this->colors = new std::vector<Vector3*>();
    try {
        parse(filename);
    } catch (const std::out_of_range& oor) {
        std::cerr << "Out of Range error: " << oor.what() << '\n';
        exit(-1);
    }
    //this->hasBBox = false;
}

OBJObject::OBJObject(OBJObject const &obj) : Drawable(){
    center = * new Vector4(0,0,0,1);
    this->vertices = obj.vertices;
    this->normals = obj.normals;
    this->faces = obj.faces;
    this->colors = obj.colors;
    showRedBBox = false;
}

OBJObject::~OBJObject()
{
    //Delete any dynamically allocated memory/objects here
    
    deleteVector(Vector3*, vertices);
    deleteVector(Vector3*, normals);
    deleteVector(Face*, faces);
}

void OBJObject::draw(DrawData& data)
{
    material.apply();
    
    glMatrixMode(GL_MODELVIEW);
    
    glPushMatrix();
    glMultMatrixf(toWorld.ptr());
    
    if (hasBBox) {
        drawBoundingBox();
    }
    
    glBegin(GL_TRIANGLES);
    
    
    //Loop through the faces
    //For each face:
    //  Look up the vertices, normals (if they exist), and texcoords (if they exist)
    //  Draw them as triplets:
    
    //      glNorm(normals->at(face.normalIndices[0]))
    //      glVert(vertices->at(face.vertexIndices[0]))
    //      Etc.
    //
    int i = 0;
    try {
        for ( i = 0; i < faces->size(); i++) {
            
            Vector3 vn1 = *normals -> at(faces->at(i)->normalIndices[0]);
            Vector3 vn2 = *normals -> at(faces->at(i)->normalIndices[1]);
            Vector3 vn3 = *normals -> at(faces->at(i)->normalIndices[2]);
            
            Vector3 v1 = *vertices -> at(faces->at(i)->vertexIndices[0]);
            Vector3 v2 = *vertices -> at(faces->at(i)->vertexIndices[1]);
            Vector3 v3 = *vertices -> at(faces->at(i)->vertexIndices[2]);
            
            // if it is bunny file, add color
            if (colors->size() == vertices ->size()) {
                Vector3 color1 = *colors -> at(faces->at(i)->vertexIndices[0]);
                Vector3 color2 = *colors -> at(faces->at(i)->vertexIndices[1]);
                Vector3 color3 = *colors -> at(faces->at(i)->vertexIndices[2]);
                
                
                glNormal3f(*vn1.ptr(), *(vn1.ptr()+1), *(vn1.ptr()+2));
                glColor3f(*color1.ptr(), *(color1.ptr()+1), *(color1.ptr()+2));
                glVertex3f(*v1.ptr(), *(v1.ptr()+1), *(v1.ptr()+2));
                
                glNormal3f(*vn2.ptr(), *(vn2.ptr()+1), *(vn2.ptr()+2));
                glColor3f(*color2.ptr(), *(color2.ptr()+1), *(color2.ptr()+2));
                glVertex3f(*v2.ptr(), *(v2.ptr()+1), *(v2.ptr()+2));
                
                glNormal3f(*vn3.ptr(), *(vn3.ptr()+1), *(vn3.ptr()+2));
                glColor3f(*color3.ptr(), *(color3.ptr()+1), *(color3.ptr()+2));
                glVertex3f(*v3.ptr(), *(v3.ptr()+1), *(v3.ptr()+2));
            }
            else{
                glNormal3f(*vn1.ptr(), *(vn1.ptr()+1), *(vn1.ptr()+2));
                glVertex3f(*v1.ptr(), *(v1.ptr()+1), *(v1.ptr()+2));
                glNormal3f(*vn2.ptr(), *(vn2.ptr()+1), *(vn2.ptr()+2));
                glVertex3f(*v2.ptr(), *(v2.ptr()+1), *(v2.ptr()+2));
                glNormal3f(*vn3.ptr(), *(vn3.ptr()+1), *(vn3.ptr()+2));
                glVertex3f(*v3.ptr(), *(v3.ptr()+1), *(v3.ptr()+2));
            }
        }
    } catch (const std::out_of_range& oor) {
        
        std::cerr << "Out of Range error: " << oor.what() <<" :" << i  <<"face.size:" << faces->size()<< '\n';
        exit(-1);
        
    }

    glEnd();
    
    glPopMatrix();
}

void OBJObject::update(UpdateData& data)
{
    //
}

void OBJObject::parse(std::string& filename)
{
    std::ifstream infile(filename);
    std::string line;
    std::vector<std::string> tokens;
    std::string token;
    
    std::vector<std::string> faceToken;
    
    int lineNum = 0;
    
    std::cout << "Starting parse..." << "Filename: " << filename << std::endl;
    
    //While all your lines are belong to us
    while (std::getline(infile, line))
    {
        //Progress
        if(++lineNum % 1000 == 0)
            std::cout << "At line " << lineNum << "  Filename: " << filename << std::endl;
        
        //Split a line into tokens by delimiting it on spaces
        //"Er Mah Gerd" becomes ["Er", "Mah", "Gerd"]
        tokens.clear();
        tokens = split(line, ' ', tokens);
        
        //If first token is a v then it gots to be a vertex
        if (tokens.size() == 0) {
            continue;
        }
        else if(tokens.at(0).compare("v") == 0)
        {
            //Parse the vertex line
            float x = std::stof(tokens.at(1));
            float y = std::stof(tokens.at(2));
            float z = std::stof(tokens.at(3));
            vertices->push_back(new Vector3(x, y, z));
            
            // only bunny file has color vertex
            if (tokens.size() == 7){
                float r = std::stof(tokens.at(4));
                float g = std::stof(tokens.at(5));
                float b = std::stof(tokens.at(6));
                colors->push_back(new Vector3(r, g, b));
            }
            
        }
        else if(tokens.at(0).compare("vn") == 0)
        {
            //Parse the normal line
            float x = std::stof(tokens.at(1));
            float y = std::stof(tokens.at(2));
            float z = std::stof(tokens.at(3));
            
            normals->push_back(new Vector3(x,y,z));

        }
        else if(tokens.at(0).compare("f") == 0)
        {
            Face* face = new Face;
            
            //Parse the face line
            faceToken.clear();
            faceToken = split(tokens.at(1), '/', faceToken);
            face->vertexIndices[0] = std::stoi(faceToken.at(0)) - 1;
            face->normalIndices[0] = std::stoi(faceToken.at(2)) - 1;
            
            faceToken.clear();
            faceToken = split(tokens.at(2), '/', faceToken);
            face->vertexIndices[1] = std::stoi(faceToken.at(0)) - 1;
            face->normalIndices[1] = std::stoi(faceToken.at(2)) - 1;

            faceToken.clear();
            faceToken = split(tokens.at(3), '/', faceToken);
            face->vertexIndices[2] = std::stoi(faceToken.at(0)) - 1;
            face->normalIndices[2] = std::stoi(faceToken.at(2)) - 1;
            
            faces->push_back(face);
        }
        else if(tokens.at(0).compare("min") == 0){
            min_x = std::stof(tokens.at(1));
            min_y = std::stof(tokens.at(2));
            min_z = std::stof(tokens.at(3));
            hasBBox = true;
        }
        else if(tokens.at(0).compare("max") == 0){
            max_x = std::stof(tokens.at(1));
            max_y = std::stof(tokens.at(2));
            max_z = std::stof(tokens.at(3));
            hasBBox = true;
        }
        else if(tokens.at(0).compare("center") == 0){
            center.set(std::stof(tokens.at(1)), std::stof(tokens.at(2)), std::stof(tokens.at(3)), 1);
        }
        else if(tokens.at(0).compare("direction") == 0){
            WallDirection.set(std::stof(tokens.at(1)), std::stof(tokens.at(2)), std::stof(tokens.at(3)));
        }
        else if(tokens.at(0).compare("How does I are C++?!?!!") == 0)
        {
            //Parse as appropriate
            //There are more line types than just the above listed
            //See the Wavefront Object format specification for details
        }
        
    }
    
    std::cout << "Done parsing." << std::endl;
}

//Split functions from the interwebs
//http://stackoverflow.com/questions/236129/split-a-string-in-c
std::vector<std::string>& OBJObject::split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> OBJObject::split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


void OBJObject::drawBoundingBox(){
    if (Globals::drawBoundingBox) {

        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        if (showRedBBox) {
            glColor3f(1.0, 0, 0);
        }
        else{
            glColor3f(1.0, 1.0, 1.0);
        }
    glBegin(GL_QUADS);
 
 // Draw front face:
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(min_x,  min_y,  max_z);
    glVertex3f(min_x,  max_y,  max_z);
    glVertex3f(max_x,  max_y,  max_z);
    glVertex3f(max_x,  min_y,  max_z);
 
 // Draw left side:
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(min_x,  min_y,  max_z);
    glVertex3f(min_x,  min_y,  min_z);
    glVertex3f(min_x,  max_y,  min_z);
    glVertex3f(min_x,  max_y,  max_z);
 
 // Draw right side:
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f(max_x,  min_y,  max_z);
    glVertex3f(max_x,  min_y,  min_z);
    glVertex3f(max_x,  max_y,  min_z);
    glVertex3f(max_x,  max_y,  max_z);

 // Draw back face:
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(min_x,  min_y,  min_z);
    glVertex3f(min_x,  max_y,  min_z);
    glVertex3f(max_x,  max_y,  min_z);
    glVertex3f(max_x,  min_y,  min_z);
 
 // Draw top side:
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(min_x,  max_y,  max_z);
    glVertex3f(min_x,  max_y,  min_z);
    glVertex3f(max_x,  max_y,  min_z);
    glVertex3f(max_x,  max_y,  max_z);

 
 // Draw bottom side:
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(min_x,  min_y,  max_z);
    glVertex3f(min_x,  min_y,  min_z);
    glVertex3f(max_x,  min_y,  min_z);
    glVertex3f(max_x,  min_y,  max_z);

    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
}

void OBJObject::animate(){
   
    /*if ( !this->speed ) {
        std::cout << "this object speed not initialized" << std::endl;
        return;
    }*/
    Matrix4 displacement;
    displacement.makeTranslate(speed);
    toWorld = toWorld * displacement;
}

void OBJObject::setSpeed(Vector3 v){
    this->speed.set(v[0], v[1], v[2]);
}






