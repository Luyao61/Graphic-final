#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Vector3.h"
#include "Matrix4.h"
class BezierCurve
{
public:
	Vector3 p0, p1, p2, p3;
	GLfloat ctrlpoints[4][3];
	Matrix4 m;

	BezierCurve(Vector3, Vector3, Vector3, Vector3);
	virtual ~BezierCurve(void);

	void draw(void);
};

