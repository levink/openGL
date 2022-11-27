#include<gl/glut.h>

#include"BSpline.h"

const double PI = 3.14159265358979323846;

double2 ControlPoints[] = {
	double2(0,0.7),
	double2(0.1,0.1),
	double2(0.7,0),
	double2(0.1,-0.1),
	double2(0.5,-0.7),
	double2(-0.1,-0.1),
	double2(-0.7,0),
	double2(-0.1,0.1),
	double2(0,0.7),
	double2(0.1,0.1),
	double2(0.7,0),
};

int N = 50; // Количество повторений кривой
const double R = 2;

bool drawingObject = true;
double x_angle = 30;
double y_angle = 60;

const double Dist = 7;
const double Scale = 1;

BSpline<double2> bsp(50,PeriodicalBasis,sizeof(ControlPoints)/sizeof(ControlPoints[0]),3,0,1);

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

// Дисплейный список с объектов
GLuint objectList;


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

inline double2 Rotate(double angle,double2 v)
{
	double c = cos(angle);
	double s = sin(angle);
	return double2(c*v.x - s*v.y,s*v.x + c*v.y);
}

void SetProjectionMatrix(int w,int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(drawingObject) 
		gluPerspective(60.0,w/(GLdouble)h,1,40);
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

	// Включаем освещение
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);

	objectList = glGenLists(1);
	glNewList(objectList,GL_COMPILE);
		if(drawingObject) {
		glEnable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glColor3f(1,1,1);
		int n2 = bsp.GetTesselation();
		for(int j = 0;j<n2;j++) {
			int j2 = (j<n2-1)?(j+1):(0);
			double psi1 = 2*j*PI/n2 + PI/4;
			double psi2 = 2*j2*PI/n2 + PI/4;	
			glBegin(GL_QUAD_STRIP);
			for(int i = 0;i<=N;i++) {
				double phi = 2*i*PI/N;
				double3 p = double3(R*cos(phi),0,R*sin(phi));
				glNormal(RotateY(phi,Rotate(phi,-GetNormal(bsp,j))));
				glVertex(p + RotateY(phi,Rotate(phi,bsp.GetPoint(j))));
				glNormal(RotateY(phi,Rotate(phi,-GetNormal(bsp,j2))));
				glVertex(p + RotateY(phi,Rotate(phi,bsp.GetPoint(j2))));
				/*glNormal(RotateY(phi,-GetNormal(bsp,j)));
				glVertex(p + RotateY(phi,bsp.GetPoint(j)));
				glNormal(RotateY(phi,-GetNormal(bsp,j2)));
				glVertex(p + RotateY(phi,bsp.GetPoint(j2)));*/
			}
			glEnd();
		}
	} else {
		glDisable(GL_LIGHTING);
		// Рисуем сплайн
		glBegin(GL_LINE_STRIP);
		glColor3d(1,1,1);
		for(int i = 0;i<bsp.GetTesselation();i++) 
			glVertex(bsp.GetPoint(i));
		glEnd();
		// Рисуем контрольные точки
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
		// Рисуем нормали
		glBegin(GL_LINES);
		glColor3d(0,1,0);
		for(int i = 0;i<bsp.GetTesselation();i++) {
			double2 p = bsp.GetPoint(i);		
			double2 t = bsp.GetTangent(i);
			t.Normalize();
			t*=-0.05;
			glVertex2d(p.x,p.y);
			glVertex2d(p.x + t.y, p.y - t.x);
		}		
		glEnd();
	}
	glEndList();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
    glutCreateWindow("Sweeping B-Spline along circle");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	init();
    glutMainLoop();
    return 0;          
}
