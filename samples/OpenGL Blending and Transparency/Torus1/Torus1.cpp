#include<windows.h>
#include<tchar.h>
#include<gl/glut.h>
#include<math.h>

#include"BmpLoad.h"

const double Pi = 3.1415;

double x_angle = 0;
double y_angle = 0;

int bk_width;
int bk_height;
unsigned char *bk_bits = NULL;
GLuint bk_tex = 0;

GLuint torus = 0;

inline void TorusVertex(double r1,double r2,double phi,double psi)
{
	double nx = cos(phi)*cos(psi);
	double ny = sin(psi);
	double nz = sin(phi)*cos(psi);
	glNormal3d(nx,ny,nz);
	glVertex3d(r1*cos(phi) + r2*nx,r2*ny,r1*sin(phi) + r2*nz);
}

GLuint MakeTorus(double r1,double r2,int n1,int n2) {
	GLuint list = glGenLists(1);
	glNewList(list,GL_COMPILE);
	glBegin(GL_QUADS);
	glColor3f(1,1,1);
	for(int i = 0;i<n1;i++) {
		int i2 = (i<n1-1)?(i+1):(0);
		double phi1 = 2*i*Pi/n1;
		double phi2 = 2*i2*Pi/n1;
		for(int j = 0;j<n2;j++) {
			int j2 = (j<n2-1)?(j+1):(0);
			double psi1 = 2*j*Pi/n2;
			double psi2 = 2*j2*Pi/n2;
			TorusVertex(r1,r2,phi1,psi1);
			TorusVertex(r1,r2,phi1,psi2);
			TorusVertex(r1,r2,phi2,psi2);
			TorusVertex(r1,r2,phi2,psi1);
		}
	}
	glEnd();
	glEndList();
	return list;
}

bool init() 
{
	if((bk_bits = LoadIndexedBMPFile("sunset.bmp",&bk_width,&bk_height)) == NULL)
		return false;
	glGenTextures(1,&bk_tex);
	glBindTexture(GL_TEXTURE_2D,bk_tex);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage2D(GL_TEXTURE_2D,
		         0,
			     GL_RGB,
			     bk_width,bk_height,
			     0,
			     GL_RGB,
			     GL_UNSIGNED_BYTE,bk_bits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_REPLACE);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

	// Устанавливаем материал
	GLfloat m_emissive[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat m_diffuse[4] = { 0.4f, 0.4f, 0.4f, 0.5f };   // Blending alpha is here
	GLfloat m_specular[4] = { 0.6f, 0.6f, 0.6f, 0.0f };
	GLfloat m_ambient[4] = { 0.1f, 0.1f, 0.1f, 0.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambient);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,10);
	// Устанавливаем цвет
	GLfloat l_diffuse[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat l_specular[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat l_ambient[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	glLightfv(GL_LIGHT0,GL_DIFFUSE,l_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,l_specular);
	glLightfv(GL_LIGHT0,GL_AMBIENT,l_ambient);
	// Устанавливаем источник освещения
	GLfloat l_position[4] = { 1.0f, 0.0f, 4.0f, 1.0f };
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0,GL_POSITION,l_position);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
	// Включаем освещение
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	
	const double r1 = 2.0;
	const double r2 = 0.5;
	const int n1 = 25;
	const int n2 = 15;
	torus = MakeTorus(r1,r2,n1,n2);

	return true; // Success
}

void finalize() 
{
	if(bk_tex != 0)
		glDeleteTextures(1,&bk_tex);
	if(bk_bits != NULL)
		delete[] bk_bits;
	if(torus != 0)
		glDeleteLists(torus,1);
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

	// Рисуем фон
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);
	// Установливаем режимы OpenGL
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,bk_tex);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	// Рисуем прямоугольльник
	glBegin(GL_QUADS);
	glTexCoord2d(0,0);
	glVertex2d(-1,-1);
	glTexCoord2d(1,0);
	glVertex2d(1,-1);
	glTexCoord2d(1,1);
	glVertex2d(1,1);
	glTexCoord2d(0,1);
	glVertex2d(-1,1);
	glEnd();
	// Восстанавливаем матрицы
	glPopMatrix();

	// Включаем смешение!
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
	glEnable(GL_BLEND);
	glColor4d(1.0,1.0,1.0,0.5);

	// Рисуем тор
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	glTranslated(0,0,-8);
	glRotated(y_angle,0,1,0);
	glRotated(x_angle,1,0,0);

	glCallList(torus);

	glFlush();
	glutSwapBuffers();
}

void idle() {
	x_angle += 0.5;
	y_angle += 0.2;
	display();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH|GLUT_RGB);
    glutCreateWindow("Transparent Torus");
	if(!init()) {
		::MessageBox(NULL,_T("Initialization failed"),_T("Error!"),MB_OK|MB_ICONEXCLAMATION);
		return 1;
	}
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
    glutMainLoop();
	finalize();
    return 0;          
}
