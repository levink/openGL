#include<gl/glut.h>
#include<math.h>
#include<windows.h>

#include"FPSCounter.h"

GlutFPSDisplay fpsDisplay;

const double Pi = 3.14159265358979323846;

const int N1 = 100; // Количество разбиений вдоль большого радиуса
const int N2 = 4;   // Количество разбиений вдоль малого радиуса
const double R1 = 2.0; // Большой радиус
const double R2 = 0.5; // Малый радиус

double y_angle = 0;
double z_angle = 0;

inline void TorusNormal(double phi,double psi)
{
	double nx = cos(phi)*cos(psi);
	double ny = sin(psi);
	double nz = sin(phi)*cos(psi);
	glNormal3d(nx,ny,nz);
}

inline void TorusVertex(double r1,double r2,double phi,double psi)
{
	double nx = cos(phi)*cos(psi);
	double ny = sin(psi);
	double nz = sin(phi)*cos(psi);
	glVertex3d(r1*cos(phi) + r2*nx,r2*ny,r1*sin(phi) + r2*nz);
}

void DrawTorus(double r1,double r2,int n1,int n2)
{
	for(int j = 0;j<n2;j++) {
		int j2 = (j<n2-1)?(j+1):(0);
		double psi1 = 2*j*Pi/n2 + Pi/4;
		double psi12 = 2*(j + 0.5)*Pi/n2 + Pi/4;
		double psi2 = 2*j2*Pi/n2 + Pi/4;	
		glBegin(GL_QUAD_STRIP);
		for(int i = 0;i<=n1;i++) {
			double phi = 2*i*Pi/n1;
			TorusNormal(phi,psi12 + phi/2);
			TorusVertex(r1,r2,phi,psi1 + phi/2);
			TorusVertex(r1,r2,phi,psi2 + phi/2);
		}
		glEnd();
	}
}

void Init() 
{
	glEnable(GL_DEPTH_TEST);
	// Задаем материал объект 
	GLfloat m_emissive[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat m_diffuse[4] = { 0.2f, 0.2f, 0.2f, 0.0f };
	GLfloat m_specular[4] = { 0.8f, 0.8f, 0.8f, 0.0f };
	GLfloat m_ambient[4] = { 0.1f, 0.1f, 0.1f, 0.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambient);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,10);
	// Задаем цвет источника освещения
	GLfloat l_diffuse[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat l_specular[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat l_ambient[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	glLightfv(GL_LIGHT0,GL_DIFFUSE,l_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,l_specular);
	glLightfv(GL_LIGHT0,GL_AMBIENT,l_ambient);
	// Задаем положение источника освещения 
	GLfloat l_position[4] = { 1.0f, 0.0f, 4.0f, 1.0f };
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0,GL_POSITION,l_position);
	// Включаем освещение
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	// Прочие настройки OpenGL
	glShadeModel(GL_SMOOTH);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_CULL_FACE);
//	glColor3f(1,1,1);
}

void reshape(int w, int h)
{
	if(h > 0) {
		glViewport(0,0,w,h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0,w/(GLdouble)h,1,20);
	}
}

void display(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,0,-8);
	glRotated(z_angle,0,0,1);
	glRotated(y_angle,1,0,0);
	DrawTorus(R1, R2, N1, N2);
	fpsDisplay.NextFrame();
    glFlush();
	glutSwapBuffers();
}

void idle() {
	y_angle += 0.05;
	z_angle += 0.02;
	display();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH|GLUT_RGB);
    glutCreateWindow("Torus with quad strips");
	Init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
    glutMainLoop();
    return 0;          
}
