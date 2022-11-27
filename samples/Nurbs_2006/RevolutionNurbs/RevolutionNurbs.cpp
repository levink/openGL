#include<gl/glut.h>

#include"BSpline.h"

const double PI = 3.14159265358979323846;

double3 ControlPoints[] = {
	double3(0,-0.9,1.0),
	double3(0.2,-0.9,1.0),
	double3(0.5*5,-0.8*5,1.0*5),
	//double3(0.5,-0.8,1.0),
	double3(0.2,0.3,1.0),
	//double3(0.5,0.5,1.0),
	double3(0.5,0.5,1.0),
	double3(0,0.8,1.0),
	double3(0,0.9,1.0)
};

int N = 25;

bool drawingObject = false;
double x_angle = 30;
double y_angle = 60;

const double Dist = 3;
const double Scale = 1;

BSpline<double3> bsp(25,OpenBasis,sizeof(ControlPoints)/sizeof(ControlPoints[0]),3,0,1);

inline void glVertex(const double2 v)
{
	glVertex2d(v.x,v.y);
}

inline void glVertex(const double3 v)
{
	glVertex3d(v.x,v.y,v.z);
}

inline void glNormal(const double3 v)
{
	glNormal3d(v.x,v.y,v.z);
}

inline double3 RotateY(double angle,const double2 v)
{
	return double3(cos(angle)*v.x,v.y,sin(angle)*v.x);
}

void SetProjectionMatrix(int w,int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(drawingObject) 
		gluPerspective(90.0,w/(GLdouble)h,1,40);
	else {
		if(w > h && h > 0)
			glOrtho(-Scale*w/(double)h,Scale*w/(double)h,-Scale,Scale,-1,1);
		else if(w > 0)
			glOrtho(-Scale,Scale,-Scale*h/(double)w,Scale*h/(double)w,-1,1);
	}
}

void SetModelViewMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(drawingObject) {
		glTranslated(0,0,-Dist);
		glRotated(y_angle,0,1,0);
		glRotated(x_angle,1,0,0);
	}
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
	
	if(drawingObject) {
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glColor3f(1,1,1);
		// Нижняя розетка треугольников
		glBegin(GL_TRIANGLES);
		for(int i = 0;i<N;i++) {
			glVertex3d(0,bsp.GetPoint(0).Perspective().y,0);
			double phi = 2*PI*i/N;
			glVertex(RotateY(phi,bsp.GetPoint(1).Perspective()));
			int i2 = (i < N-1)?(i+1):(0);
			double phi2 = 2*PI*i2/N;
			glVertex(RotateY(phi2,bsp.GetPoint(1).Perspective()));
		}
		glEnd();
		// Средняя часть объекта
		for(int j = 1;j<bsp.GetTesselation() - 2;j++) {
			glBegin(GL_QUAD_STRIP);
			for(int i = 0;i<=N;i++) {
				double phi = (i < N)?(2*PI*i/N):(0);
				glVertex(RotateY(phi,bsp.GetPoint(j).Perspective()));
				glVertex(RotateY(phi,bsp.GetPoint(j+1).Perspective()));
			}
			glEnd();
		}
		// Верхняя розетка треугольников
		glBegin(GL_TRIANGLES);
		int n2 = bsp.GetTesselation() - 1;
		for(int i = 0;i<N;i++) {
			double phi = 2*PI*i/N;
			glVertex(RotateY(phi,bsp.GetPoint(n2 - 1).Perspective()));
			int i2 = (i < N-1)?(i+1):(0);
			double phi2 = 2*PI*i2/N;
			glVertex(RotateY(phi2,bsp.GetPoint(n2 - 1).Perspective()));
			glVertex3d(0,bsp.GetPoint(0).Perspective().y,0);
		}
		glEnd();
	} else {
		// Рисуем сплайн
		glBegin(GL_LINE_STRIP);
		glColor3d(1,1,1);
		for(int i = 0;i<bsp.GetTesselation();i++) 
			glVertex(bsp.GetPoint(i).Perspective());
		glEnd();
		// Рисуем контрольные точки
		glBegin(GL_QUADS);
		glColor3d(1,0,0);
		for(int i = 0;i<bsp.GetNumPoints();i++) {
			double2 v = bsp.ControlPoint(i).Perspective();
			glVertex2d(v.x, v.y - 0.025);
			glVertex2d(v.x + 0.025, v.y);
			glVertex2d(v.x, v.y + 0.025);
			glVertex2d(v.x - 0.025, v.y);
		}
		glEnd();
		// Рисуем нормали
		glBegin(GL_LINES);
		glColor3d(0,1,0);
		for(int i = 0;i<bsp.GetTesselation();i++) {
			double2 p = bsp.GetPoint(i).Perspective();		
			double3 t3 = bsp.GetTangent(i);
			double2 t = double2(t3.x,t3.y);
			t.Normalize();
			t *= 0.05;
			glVertex2d(p.x,p.y);
			glVertex2d(p.x - t.y, p.y + t.x);
		}		
		glEnd();
	}

    glFlush();
	glutSwapBuffers();
}

void idle() {
	if(drawingObject) {
		x_angle += 0.5;
		y_angle += 0.7;
		display();
	}
}

void init() {
	for(int i = 0;i<sizeof(ControlPoints)/sizeof(ControlPoints[0]);i++)
		bsp.ControlPoint(i) = ControlPoints[i];
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
