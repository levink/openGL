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

int tex_width;
int tex_height;
unsigned char *tex_bits = NULL;
GLuint cube_tex = 0;

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

unsigned char *ConstructTexture(int *w,int *h)
{
	int width1,height1;
	unsigned char *tex1 = LoadIndexedBMPFile("colormap.bmp",&width1,&height1);
	if(!tex1)
		return NULL;
	int width2,height2;
	unsigned char *tex2 = LoadIndexedBMPFile("alpha.bmp",&width2,&height2);
	if(!tex2) {
		delete[] tex1;
		return NULL;
	}
	if(width1 != width2 || height1 != height2) {
		delete[] tex1;
		delete[] tex2;
		return NULL;
	}
	unsigned char *result = new unsigned char [width1*height1*4];
	if(result != NULL) {
		for(int i = 0;i<width1*height1;i++) {
			result[4*i] = tex1[3*i];
			result[4*i + 1] = tex1[3*i + 1];
			result[4*i + 2] = tex1[3*i + 2];
			result[4*i + 3] = (unsigned char)(((int)tex2[3*i] + (int)tex2[3*i + 1] + (int)tex2[3*i + 2])/3);
		}
	}	
	delete[] tex1;
	delete[] tex2;
	*w = width1;
	*h = height1;
	return result;
}

bool init() 
{
	// Текстура фона
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

	// Текстура куба
	if((tex_bits = ConstructTexture(&tex_width,&tex_height)) == NULL)
		return false;
	glGenTextures(1,&cube_tex);
	glBindTexture(GL_TEXTURE_2D,cube_tex);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage2D(GL_TEXTURE_2D,
		         0,
			     4,
			     tex_width,tex_height,
			     0,
			     GL_RGBA,
			     GL_UNSIGNED_BYTE,tex_bits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

	glFrontFace(GL_CW);

	glEnable(GL_TEXTURE_2D);

	// Записываем команды для рисования куба
	cube = glGenLists(1);
	glNewList(cube,GL_COMPILE);
	glBegin(GL_QUADS);
	
	glTexCoord2d(0,0);
	glVertex3d(v[0].x,v[0].y,v[0].z);
	glTexCoord2d(1,0);
	glVertex3d(v[1].x,v[1].y,v[1].z);
	glTexCoord2d(1,1);
	glVertex3d(v[3].x,v[3].y,v[3].z);
	glTexCoord2d(0,1);
	glVertex3d(v[2].x,v[2].y,v[2].z);

	glTexCoord2d(0,0);
	glVertex3d(v[2].x,v[2].y,v[2].z);
	glTexCoord2d(1,0);
	glVertex3d(v[3].x,v[3].y,v[3].z);
	glTexCoord2d(1,1);
	glVertex3d(v[7].x,v[7].y,v[7].z);
	glTexCoord2d(0,1);
	glVertex3d(v[6].x,v[6].y,v[6].z);

	glTexCoord2d(0,0);
	glVertex3d(v[1].x,v[1].y,v[1].z);
	glTexCoord2d(1,0);
	glVertex3d(v[5].x,v[5].y,v[5].z);
	glTexCoord2d(1,1);
	glVertex3d(v[7].x,v[7].y,v[7].z);
	glTexCoord2d(0,1);
	glVertex3d(v[3].x,v[3].y,v[3].z);

	glTexCoord2d(0,0);
	glVertex3d(v[4].x,v[4].y,v[4].z);
	glTexCoord2d(1,0);
	glVertex3d(v[5].x,v[5].y,v[5].z);
	glTexCoord2d(1,1);
	glVertex3d(v[1].x,v[1].y,v[1].z);
	glTexCoord2d(0,1);
	glVertex3d(v[0].x,v[0].y,v[0].z);

	glTexCoord2d(0,0);
	glVertex3d(v[4].x,v[4].y,v[4].z);
	glTexCoord2d(1,0);
	glVertex3d(v[6].x,v[6].y,v[6].z);
	glTexCoord2d(1,1);
	glVertex3d(v[7].x,v[7].y,v[7].z);
	glTexCoord2d(0,1);
	glVertex3d(v[5].x,v[5].y,v[5].z);

	glTexCoord2d(0,0);
	glVertex3d(v[4].x,v[4].y,v[4].z);
	glTexCoord2d(1,0);
	glVertex3d(v[0].x,v[0].y,v[0].z);
	glTexCoord2d(1,1);
	glVertex3d(v[2].x,v[2].y,v[2].z);
	glTexCoord2d(0,1);
	glVertex3d(v[6].x,v[6].y,v[6].z);

	glEnd();

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
	if(cube_tex !=  0)
		glDeleteTextures(1,&cube_tex);
	if(tex_bits != NULL)
		delete[] tex_bits;
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
	//glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D,bk_tex);
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
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_LESS,0.5);


	// Рисуем куб
	glEnable(GL_CULL_FACE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,0,-3);
	glRotated(y_angle,0,1,0);
	glRotated(x_angle,1,0,0);

	glBindTexture(GL_TEXTURE_2D,cube_tex);
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
