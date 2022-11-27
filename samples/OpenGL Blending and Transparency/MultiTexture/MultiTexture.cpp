#include<windows.h>
#include<tchar.h>
#include<gl/glut.h>
#include<math.h>

#include"glext.h" // Файл с описаниями расширений OpenGL
#include"BmpLoad.h"
#include"FPSCounter.h"

const double Pi = 3.1415;

double x_angle = 0;
double y_angle = 0;

GlutFPSDisplay fpsDisplay;

// Указатель на функции из расширений OpenGL
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLMULTITEXCOORD2FPROC glMultiTexCoord2f;

// Проверка - поддерживатся ли данное расширение
bool IsExtensionSupported(const char *extension) 
{
	const GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;

	/* Extension names should not have spaces. */
	where = (GLubyte *) strchr(extension, ' ');
	if(where || *extension == '\0')
		return 0;
	extensions = glGetString(GL_EXTENSIONS);
	MessageBox(NULL,(LPCSTR)extensions,"Supported extensions",MB_OK);
	/* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
	start = extensions;
	for (;;) {
		where = (GLubyte *) strstr((const char *) start, extension);
		if(!where)
			break;
		terminator = where + strlen(extension);
		if (where == start || *(where - 1) == ' ')
			if(*terminator == ' ' || *terminator == '\0')
				return 1;
		start = terminator;
	}
	return 0;
}

// Инициализация необходимых расширений. В данном случае - GL_ARB_multitexture
bool InitExtensions()
{
	if(!IsExtensionSupported("GL_ARB_multitexture")) // Проверяем поддержку расширения
		return false;
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTextureARB");
	glMultiTexCoord2f = (PFNGLMULTITEXCOORD2FPROC)wglGetProcAddress("glMultiTexCoord2fARB");
	return true;
}

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
	if(!list)
		return 0;
	glNewList(list,GL_COMPILE);
	glBegin(GL_QUADS);
	for(int i = 0;i<n1;i++) {
		int i2 = (i<n1-1)?(i+1):(0);
		double phi1 = 2*i*Pi/n1;
		double phi2 = 2*i2*Pi/n1;
		for(int j = 0;j<n2;j++) {
			int j2 = (j<n2-1)?(j+1):(0);
			double psi1 = 2*j*Pi/n2;
			double psi2 = 2*j2*Pi/n2;
			glTexCoord2d(5*i/(double)n1,j/(double)n2);
			TorusVertex(r1,r2,phi1,psi1);
			glTexCoord2d(5*i/(double)n1,(j+1)/(double)n2);
			TorusVertex(r1,r2,phi1,psi2);
			glTexCoord2d(5*(i+1)/(double)n1,(j+1)/(double)n2);
			TorusVertex(r1,r2,phi2,psi2);
			glTexCoord2d(5*(i+1)/(double)n1,j/(double)n2);
			TorusVertex(r1,r2,phi2,psi1);
		}
	}
	glEnd();
	glEndList();
	return list;
}

GLuint torus;

int tex_width;
int tex_height;
unsigned char *tex_bits;
GLuint torus_tex;

int bk_width;
int bk_height;
unsigned char *bk_bits;
GLuint bk_tex;

bool init() 
{
	if(!InitExtensions())
		return false;
	if((bk_bits = LoadIndexedBMPFile("sunset.bmp",&bk_width,&bk_height)) == NULL)
		return false;
	
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glGenTextures(1,&bk_tex);
	glActiveTexture(GL_TEXTURE1); // Загружаем текстуру окружающей среды для для блока 1
	glBindTexture(GL_TEXTURE_2D,bk_tex);
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

	if((tex_bits = LoadIndexedBMPFile("colormap.bmp",&tex_width,&tex_height)) == NULL)
		return false;
	glGenTextures(1,&torus_tex);
	glActiveTexture(GL_TEXTURE0); // Загружаем текстуру тора для блока 0
	glBindTexture(GL_TEXTURE_2D,torus_tex);
	glTexImage2D(GL_TEXTURE_2D,
		         0,
			     GL_RGB,
			     tex_width,tex_height,
			     0,
			     GL_RGB,
			     GL_UNSIGNED_BYTE,tex_bits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	torus = MakeTorus(2.0,0.5,25,20); // Создаем тор
	return true; // Success
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
	glActiveTexture(GL_TEXTURE1);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE); 
	glEnable(GL_TEXTURE_2D);      // Используем текстуру с блока 1 (фон)
	// Рисуем прямоугольльник
	glBegin(GL_QUADS);
	glMultiTexCoord2f(GL_TEXTURE1,0,0);
	glVertex2d(-1,-1);
	glMultiTexCoord2f(GL_TEXTURE1,1,0);
	glVertex2d(1,-1);
	glMultiTexCoord2f(GL_TEXTURE1,1,1);
	glVertex2d(1,1);
	glMultiTexCoord2f(GL_TEXTURE1,0,1);
	glVertex2d(-1,1);
	glEnd();
	// Восстанавливаем матрицы
	glPopMatrix(); 

	// Рисуем тор c отражением
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,0,-8);
	glRotated(y_angle,0,1,0);
	glRotated(x_angle,1,0,0);

	// Рисуем тор с собственной текстурой
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glColor3f(0.5f,0.5f,0.5f); // Цвет тора
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE); // Смешение текстуры с цветом - умножение на 0.5
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_ADD); // Прибавляем текстуру со второго блока

	glEnable(GL_TEXTURE_GEN_S); // Задаем автоматическую генерацию текстурных координат для 0 текстуры
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);

	glCallList(torus);

	glDisable(GL_TEXTURE_GEN_S); // Отключаем автоматическую генерацию текстурных координат для 0 текстуры
	glDisable(GL_TEXTURE_GEN_T);

	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);     // Выключаем текстуру с блока 0 (тор)
	glActiveTexture(GL_TEXTURE1);

	fpsDisplay.NextFrame();

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
    glutCreateWindow("Torus");
	if(!init()) {
		::MessageBox(NULL,_T("Initialization failed"),_T("Error!"),MB_OK|MB_ICONEXCLAMATION);
		return 1;
	}
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
    glutMainLoop();
    return 0;          
}
