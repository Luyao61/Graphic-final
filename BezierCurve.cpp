#include "BezierCurve.h"

BezierCurve::BezierCurve(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3)
{
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;

	for (int i = 0; i < 3; i++){
		ctrlpoints[0][i] = *(p0.ptr() + i);
	}
	for (int i = 0; i < 3; i++){
		ctrlpoints[1][i] = *(p1.ptr() + i);
	}
    for (int i = 0; i < 3; i++){
		ctrlpoints[2][i] = *(p2.ptr() + i);
	}
    for (int i = 0; i < 3; i++){
		ctrlpoints[3][i] = *(p3.ptr() + i);
	}
    
    Matrix4 G(*p0.ptr(),*p1.ptr(), *p2.ptr(), *p3.ptr(),
              *(p0.ptr()+1),*(p1.ptr()+1),*(p2.ptr()+1),*(p3.ptr()+1),
              *(p0.ptr()+2),*(p1.ptr()+2),*(p2.ptr()+2),*(p3.ptr()+2),
              0.0f, 0.0f, 0.0f, 0.0f);
    
    Matrix4 B(-1.0f, 3.0f, -3.0f, 1.0f,
              3.0f, -6.0f, 3.0f, 0.0f,
              -3.0f, 3.0f, 0.0f, 0.0f,
              1.0f, 0.0f, 0.0f, 0.0f);
    
    this->m = G.multiply(B);
}

BezierCurve::~BezierCurve()
{
	/*	delete &p0;
	delete &p1;	delete &p2;	delete &p3;
	delete ctrlpoints;*/

}

void BezierCurve::draw()
{
    int i;
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);

    //glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_STRIP);
    for (i = 0; i <= 30; i++)
        glEvalCoord1f((GLfloat) i/30.0);
    glEnd();
    
    /* The following code displays the control points as dots. */
    glPointSize(5.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    for (i = 0; i < 4; i++)
        glVertex3fv(&ctrlpoints[i][0]);
    glEnd();
    
    glColor3f(0.0, 0.5, 1.0);
    glBegin(GL_LINE_STRIP);
    for (i = 0; i<4; i++) {
        glVertex3fv(&ctrlpoints[i][0]);
    }
    glEnd();
}
