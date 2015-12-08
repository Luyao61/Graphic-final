#ifndef CSE167_Window_h
#define CSE167_Window_h
#include "Skybox.hpp"
#include <vector>
#include "OBJObject.h"
#include <list>
using namespace std;

class Window	  // OpenGL output window related routines
{

public:
    static int width, height; 	            // window size
    
    static void initialize(void);
    static void idleCallback(void);
    static void reshapeCallback(int, int);
    static void displayCallback(void);
    
    static void processNormalKeys(unsigned char key, int x, int y);
    static void processSpecialKeys(int key, int x, int y);

	static void processMouse(int button, int state, int x, int y);
	static void processMotion(int x, int y);

	static Vector3 trackObjMapping(int x, int y);
    static void insertionSortList(vector<OBJObject*>);
    static void findOverlapPair(vector<OBJObject*>);
    static void testCollision(void);
};

#endif

