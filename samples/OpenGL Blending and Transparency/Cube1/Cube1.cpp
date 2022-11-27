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


struct Vertex {
	double x,y,z;
} v[8] = { 
	{ -0.5,-0.5,-0.5 }, 
	{ 0.5,-0.5,-0.5 },
	{ -0.5,0.5,-0.5 },
	{ 0.5,0.5,-0.5 },
	{ -0.5,-0.5,0.5 },
	{ 0.5,-0.5,0.5 },
	{ -0.5,0.5,0.5 },
	{ 0.5,0.5,0.5 }
};

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
	return true; // Success
}

void finalize() 
{
	if(bk_tex != 0)
		glDeleteTextures(1,&bk_tex);
	if(bk_bits != NULL)
		delete[] bk_bits;
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
	// buf = buf * (1 - a) + src * a
	glEnable(GL_BLEND);
	//glColor4d(1.0,1.0,1.0,0.5);

	// Рисуем куб
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,0,-3);
	glRotated(y_angle,0,1,0);
	glRotated(x_angle,1,0,0);

	// Боковая сторона куба
	glBegin(GL_QUAD_STRIP);
	glColor4d(1,1,1,0.9);
	glVertex3d(v[1].x,v[1].y,v[1].z);
	glVertex3d(v[3].x,v[3].y,v[3].z);
	glVertex3d(v[0].x,v[0].y,v[0].z);
	glVertex3d(v[2].x,v[2].y,v[2].z);

	glVertex3d(v[4].x,v[4].y,v[4].z);
	glVertex3d(v[6].x,v[6].y,v[6].z);
	
	glVertex3d(v[5].x,v[5].y,v[5].z);
	glVertex3d(v[7].x,v[7].y,v[7].z);

	glVertex3d(v[1].x,v[1].y,v[1].z);
	glVertex3d(v[3].x,v[3].y,v[3].z);
	glEnd();

	// Верхняя и нижняя грани
	glBegin(GL_QUADS);
	glVertex3d(v[4].x,v[4].y,v[4].z);
	glVertex3d(v[5].x,v[5].y,v[5].z);
	glVertex3d(v[1].x,v[1].y,v[1].z);
	glVertex3d(v[0].x,v[0].y,v[0].z);

	glVertex3d(v[2].x,v[2].y,v[2].z);
	glVertex3d(v[3].x,v[3].y,v[3].z);
	glVertex3d(v[7].x,v[7].y,v[7].z);
	glVertex3d(v[6].x,v[6].y,v[6].z);
	glEnd();

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
    glutCreateWindow("Transparent Cube");
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
