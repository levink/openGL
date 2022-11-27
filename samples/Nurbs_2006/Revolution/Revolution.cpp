#include<gl/glut.h>

#include"BSpline.h"

const double PI = 3.14159265358979323846;

/*
double2 ControlPoints[] = {
	double2(0,-0.9),
	double2(0.2,-0.9),
	double2(0.5,-0.8),
//	double2(0.5,-0.8),
	double2(0.1,0.3),
	double2(0.5,0.5),
	double2(0.5,0.5),
	double2(0.5,0.5),
//	double2(0.5,0.5),
//	double2(0.5,0.5),
	double2(0.0,0.8),
	double2(0,0.9)
};


*/
double2 ControlPoints[] = {
	double2(0,0.684),
	double2(0.302,0.684),
	double2(0.302,0.684),
	double2(1.598,-0.288),
	double2(1.088,0.405),
	double2(0.374,0.773),
	double2(0.848,0.993),
	double2(1.232,1.446),
	double2(1.451,1.875),
	double2(1.502,2.631),
	double2(1.448,3.308),
	double2(1.226,4.076),
	double2(1.397,4.449),
	double2(1.398,4.592),
	double2(1.268,4.572),
	double2(1.241,4.485),
	double2(1.109,4.170),
	double2(1.214,3.669),
	double2(1.313,2.889),
	double2(1.310,2.280),
	double2(1.172,1.575),
	double2(0.629,1.119),
	double2(0.281,0.882),
	double2(0.278,0.891),
	double2(0,0.897)
};

// Количество точек в аппроксимации контура
int N = 25;

// Что именно рисуем
bool drawingObject = true;
bool drawingControlPoints = true;
bool drawingControlPolygon = true;

double x_angle = 30;
double y_angle = 60;

const double Dist = 6;
const double Scale = 1;

BSpline<double2> bsp(25,
	OpenBasis,sizeof(ControlPoints)/sizeof(ControlPoints[0]),3,0,1);

GLuint objectList;

// Параметры материала 
GLfloat torus_a[4] = { 0.1, 0.1, 0.1, 1.0 }; 
GLfloat torus_d[4] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat torus_s[4] = { 0.4, 0.4, 0.4, 1.0 };
GLfloat torus_r = 30;   

// Параметры источника освещения
GLfloat light_a[4] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat light_d[4] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat light_s[4] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat light_pos[4] = { 2.0, 0.0, 5.0, 1.0 };

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

	glCallList(objectList);
	
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
	bsp.SetTesselation(200);
	// Устанавливаем параметры материала
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, torus_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, torus_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, torus_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &torus_r);

	// Задаем позицию источника освещения
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	// Задаем параметры источника освещения
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_a);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_d);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_s);

	// Включаем освещение
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);

	// Рисуем объект
	objectList = glGenLists(1);
	glNewList(objectList,GL_COMPILE);
	if(drawingObject) {
		glEnable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glColor3f(1,1,1);
		// Нижняя розетка треугольников
		glBegin(GL_TRIANGLES);
		for(int i = 0;i<N;i++) {
			glNormal(RotateY(2*PI*(i + 0.5)/N,GetNormal(bsp,0)));
			glVertex3d(0,bsp.GetPoint(0).y,0);
			double phi = 2*PI*i/N;
			glNormal(RotateY(phi,GetNormal(bsp,1)));
			glVertex(RotateY(phi,bsp.GetPoint(1)));
			int i2 = (i < N-1)?(i+1):(0);
			double phi2 = 2*PI*i2/N;
			glNormal(RotateY(phi2,GetNormal(bsp,1)));
			glVertex(RotateY(phi2,bsp.GetPoint(1)));
		}
		glEnd();
		// Средняя часть объекта
		for(int j = 1;j<bsp.GetTesselation() - 2;j++) {
			glBegin(GL_QUAD_STRIP);
			for(int i = 0;i<=N;i++) {
				double phi = (i < N)?(2*PI*i/N):(0);
				glNormal(RotateY(phi,GetNormal(bsp,j)));
				glVertex(RotateY(phi,bsp.GetPoint(j)));
				glNormal(RotateY(phi,GetNormal(bsp,j+1)));
				glVertex(RotateY(phi,bsp.GetPoint(j+1)));
			}
			glEnd();
		}
		// Верхняя розетка треугольников
		glBegin(GL_TRIANGLES);
		int n2 = bsp.GetTesselation() - 1;
		for(int i = 0;i<N;i++) {
			double phi = 2*PI*i/N;
			glNormal(RotateY(phi,GetNormal(bsp,n2 - 1)));
			glVertex(RotateY(phi,bsp.GetPoint(n2 - 1)));
			int i2 = (i < N-1)?(i+1):(0);
			double phi2 = 2*PI*i2/N;
			glNormal(RotateY(phi2,GetNormal(bsp,n2 - 1)));
			glVertex(RotateY(phi2,bsp.GetPoint(n2 - 1)));
			glNormal(RotateY(2*PI*(i + 0.5)/N,GetNormal(bsp,n2)));
			glVertex3d(0,bsp.GetPoint(0).y,0);
		}
		glEnd();
	} else {
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		// Рисуем контрольные точки
		if(drawingControlPoints) {
			glBegin(GL_QUADS);
			glColor3d(1,0,0);
			for(int i = 0;i<bsp.GetNumPoints();i++) {
				double2 v = bsp.ControlPoint(i);
				glVertex2d(v.x, v.y - 0.025);
				glVertex2d(v.x + 0.025, v.y);
				glVertex2d(v.x, v.y + 0.025);
				glVertex2d(v.x - 0.025, v.y);
			}
			glEnd();
		}
		// Рисуем контрольный многоугольник
		if(drawingControlPolygon) {
			glBegin(GL_LINE_STRIP);
			glColor3d(0.5,0.5,0.5);
			for(int i = 0;i<bsp.GetNumPoints();i++) {
				double2 v = bsp.ControlPoint(i);
				glVertex2d(v.x, v.y);
			}
			glEnd();
		}
		// Рисуем нормали
		glBegin(GL_LINES);
		glColor3d(0,1,0);
		for(int i = 0;i<bsp.GetTesselation();i++) {
			double2 p = bsp.GetPoint(i);		
			double2 t = bsp.GetTangent(i);
			t.Normalize();
			t*=0.05;
			glVertex2d(p.x,p.y);
			glVertex2d(p.x + t.y, p.y - t.x);
		}		
		glEnd();
		// Рисуем сплайн
		glLineWidth(2);
		glBegin(GL_LINE_STRIP);
		glColor3d(1,1,1);
		for(int i = 0;i<bsp.GetTesselation();i++) 
			glVertex(bsp.GetPoint(i));
		glEnd();
		glLineWidth(1);
	}
	glEndList();
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
