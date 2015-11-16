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
    this->vertices = new std::vector<Vector3*>();
    this->normals = new std::vector<Vector3*>();
    this->faces = new std::vector<Face*>();
    
    parse(filename);
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
    
    glBegin(GL_TRIANGLES);
    
    
    //Loop through the faces
    //For each face:
    //  Look up the vertices, normals (if they exist), and texcoords (if they exist)
    //  Draw them as triplets:
    
    //      glNorm(normals->at(face.normalIndices[0]))
    //      glVert(vertices->at(face.vertexIndices[0]))
    //      Etc.
    //
    
    
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
    
    int lineNum = 0;
    
    
    std::cout << "Starting parse..." << std::endl;
    
    //While all your lines are belong to us
    while (std::getline(infile, line))
    {
        //Progress
        if(++lineNum % 10000 == 0)
            std::cout << "At line " << lineNum << std::endl;
        
        //Split a line into tokens by delimiting it on spaces
        //"Er Mah Gerd" becomes ["Er", "Mah", "Gerd"]
        tokens.clear();
        tokens = split(line, ' ', tokens);
        
        //If first token is a v then it gots to be a vertex
        if(tokens.at(0).compare("v") == 0)
        {
            //Parse the vertex line
            float x = std::stof(tokens.at(1));
            float y = std::stof(tokens.at(2));
            float z = std::stof(tokens.at(3));
            
            vertices->push_back(new Vector3(x, y, z));
        }
        else if(tokens.at(0).compare("vn") == 0)
        {
            //Parse the normal line
        }
        else if(tokens.at(0).compare("f") == 0)
        {
            Face* face = new Face;
            
            //Parse the face line
            
            faces->push_back(face);
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
