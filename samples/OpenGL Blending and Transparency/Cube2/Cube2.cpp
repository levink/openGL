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

GLuint cube = 0;

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

	// Устанавливаем материал
	GLfloat m_emissive[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat m_diffuse[4] = { 0.4f, 0.4f, 0.4f, 0.1f };   // Blending alpha is here
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
	glFrontFace(GL_CW);

	// Записываем команды для рисования куба
	cube = glGenLists(1);
	glNewList(cube,GL_COMPILE);
	glBegin(GL_QUADS);
	
	glNormal3d(0,0,-1);
	glVertex3d(v[0].x,v[0].y,v[0].z);
	glVertex3d(v[1].x,v[1].y,v[1].z);
	glVertex3d(v[3].x,v[3].y,v[3].z);
	glVertex3d(v[2].x,v[2].y,v[2].z);

	glNormal3d(0,1,0);
	glVertex3d(v[2].x,v[2].y,v[2].z);
	glVertex3d(v[3].x,v[3].y,v[3].z);
	glVertex3d(v[7].x,v[7].y,v[7].z);
	glVertex3d(v[6].x,v[6].y,v[6].z);

	glNormal3d(1,0,0);
	glVertex3d(v[1].x,v[1].y,v[1].z);
	glVertex3d(v[5].x,v[5].y,v[5].z);
	glVertex3d(v[7].x,v[7].y,v[7].z);
	glVertex3d(v[3].x,v[3].y,v[3].z);

	glNormal3d(0,-1,0);
	glVertex3d(v[4].x,v[4].y,v[4].z);
	glVertex3d(v[5].x,v[5].y,v[5].z);
	glVertex3d(v[1].x,v[1].y,v[1].z);
	glVertex3d(v[0].x,v[0].y,v[0].z);

	glNormal3d(0,0,1);
	glVertex3d(v[4].x,v[4].y,v[4].z);
	glVertex3d(v[6].x,v[6].y,v[6].z);
	glVertex3d(v[7].x,v[7].y,v[7].z);
	glVertex3d(v[5].x,v[5].y,v[5].z);

	glNormal3d(-1,0,0);
	glVertex3d(v[4].x,v[4].y,v[4].z);
	glVertex3d(v[0].x,v[0].y,v[0].z);
	glVertex3d(v[2].x,v[2].y,v[2].z);
	glVertex3d(v[6].x,v[6].y,v[6].z);

	glEnd();
	glEndList();

	return true; // Success
}

void finalize() 
{
	if(bk_tex != 0)
		glDeleteTextures(1,&bk_tex);
	if(bk_bits != NULL)
		delete[] bk_bits;
	if(cube != 0)
		glDeleteLists(cube,1);
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
	glEnable(GL_BLEND);
	glColor4d(1.0,1.0,1.0,0.5);

	// Рисуем куб
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,0,-3);
	glRotated(y_angle,0,1,0);
	glRotated(x_angle,1,0,0);

	glCullFace(GL_FRONT);
	glCallList(cube);
	glCullFace(GL_BACK);
	glCallList(cube);

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
