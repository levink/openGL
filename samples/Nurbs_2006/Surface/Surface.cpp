#include"BSpline.h"

#include<windows.h>
#include<gl/gl.h>
#include<gl/glu.h>
#include<gl/glut.h>
#include<stdlib.h>

const double PI = 3.14159265358979323846;

double2 ControlPoints[] = {
	double2(0,0.7),
	double2(0.1,0.1),
	double2(0.7,0),
	double2(0.1,-0.1),
	double2(0,-0.7),
	double2(-0.1,-0.1),
	double2(-0.7,0),
	double2(-0.1,0.1),
	double2(0,0.7),
	double2(0.1,0.1),
	double2(0.7,0),
};


bool drawingMesh = true;
double x_angle = 30;
double y_angle = 60;

const double Dist = 3;

const int N1 = 7;
const int N2 = 7;

BSplineBasis uBasis(OpenBasis,N1,3,0,1);
BSplineBasis vBasis(OpenBasis,N2,3,0,1);

BSplineSurface<double3> bsp(&uBasis,&vBasis);

// Задаем материал объекта
GLfloat m_diffuse[4] = { 0.4f, 0.4f, 0.4f, 0.0f };
GLfloat m_specular[4] = { 0.6f, 0.6f, 0.6f, 0.0f };
GLfloat m_ambient[4] = { 0.1f, 0.1f, 0.1f, 0.0f };
GLfloat m_shininess = 10.0;

// Параметры источника освещения
GLfloat l_diffuse[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat l_specular[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat l_ambient[4] = { 1.0f, 1.0f, 1.0f, 0.0f };

// Координаты источника освещения
GLfloat light_pos[4] = { 4.0, 0.0, 10.0, 1.0 };

// Дисплейный список

GLuint list;

inline void glVertex(const double3 v)
{
	glVertex3d(v.x,v.y,v.z);
}

inline void glNormal(const double3 v)
{
	glNormal3d(v.x,v.y,v.z);
}

void SetProjectionMatrix(int w,int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0,w/(GLdouble)h,1,40);
}

void SetModelViewMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,0,-Dist);
	glRotated(y_angle,0,1,0);
	glRotated(x_angle,1,0,0);
}

void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	SetProjectionMatrix(w,h);
}

void display(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	SetModelViewMatrix();
	
	glCallList(list);

    glFlush();
	glutSwapBuffers();
}

void idle() {
	x_angle += 0.5;
	y_angle += 0.7;
	display();
}

void init() {
	for(int i =  0;i<N1;i++)
		for(int j = 0;j<N2;j++)
			bsp.ControlPoint(i,j) = double3(-1 + 2.0*i/(N1 - 1),
											-0.5 + 1.0*rand()/RAND_MAX,
			                                -1 + 2.0*j/(N2 - 1));
	
	list = glGenLists(1);
	glNewList(list,GL_COMPILE);
	glEnable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glColor3f(1,1,1);
	int n1 = bsp.GetUBasis()->GetTesselation();
	int n2 = bsp.GetVBasis()->GetTesselation();
	for(int i = 0;i<n1 - 1;i++) {
		glBegin(GL_QUAD_STRIP);
		for(int j = 0;j<n2;j++) {
			glNormal(bsp.GetNormal(i,j));
			glVertex(bsp.GetPoint(i,j));
			glNormal(bsp.GetNormal(i+1,j));
			glVertex(bsp.GetPoint(i+1,j));
		}
		glEnd();
	}
	if(drawingMesh) {
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glDisable(GL_LIGHTING);
		glColor3f(0,1,0);
		int n1 = bsp.GetUBasis()->GetNumPoints();
		int n2 = bsp.GetVBasis()->GetNumPoints();
		for(int i = 0;i<n1 - 1;i++) {
			glBegin(GL_QUAD_STRIP);
			for(int j = 0;j<n2;j++) {
				glVertex(bsp.ControlPoint(i,j));
				glVertex(bsp.ControlPoint(i+1,j));
			}
			glEnd();
		}

	}
	glEndList();

	// Устанавливаем параметры материала
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);

	// Задаем позицию источника освещения
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	// Задаем параметры источника освещения
	glLightfv(GL_LIGHT0, GL_AMBIENT, l_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l_specular);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,1);

	// Включаем освещение
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
    glutCreateWindow("B-Spline");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	init();
    glutMainLoop();
    return 0;          
}
