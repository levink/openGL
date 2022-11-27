#include<gl/glut.h>
#include<math.h>

#include"Torus.h"

double y_angle = 0;
double z_angle = 0;
GLuint torus;

void Init() 
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	const double r1 = 2.0;
	const double r2 = 0.5;
	const int n1 = 25;
	const int n2 = 15;
	torus = MakeTorus(r1,r2,n1,n2);
	// Устанавливаем материал
	GLfloat m_emissive[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat m_diffuse[4] = { 0.4f, 0.4f, 0.4f, 0.0f };
	GLfloat m_specular[4] = { 0.6f, 0.6f, 0.6f, 0.0f };
	GLfloat m_ambient[4] = { 0.3f, 0.3f, 0.3f, 0.0f };
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
	GLfloat l_position[4] = { 0.0f, 20.0f, -12.0f, 1.0f };
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0,GL_POSITION,l_position);
	// Включаем освещение
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
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

const double D1 = 4;
const double D2 = 10;

const double ObjectSize = 0.5;
const double ObjectSize2 = (D1 + D2)*ObjectSize/D1;

// Рисование теневого объема
void RenderShadowVolume()
{
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0,D1,0);
	glVertex3d(-ObjectSize2,-D2,-ObjectSize2);
	glVertex3d(-ObjectSize2,-D2,ObjectSize2);
	glVertex3d(ObjectSize2,-D2,ObjectSize2);
	glVertex3d(ObjectSize2,-D2,-ObjectSize2);
	glVertex3d(-ObjectSize2,-D2,-ObjectSize2);
	glEnd();
}

void display(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

	// Рисуем фон
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);
	// Установливаем режимы OpenGL
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	// Рисуем прямоугольльник
	glBegin(GL_QUADS);
	glColor3d(0,0,1);
	glVertex2d(-1,-1);
	glVertex2d(1,-1);
	glVertex2d(1,1);
	glVertex2d(-1,1);
	glVertex2d(-1,1);
	glEnd();
	// Восстанавливаем матрицы
	glPopMatrix();

	// Рисуем поверхность, отбрасывающую тень
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,4,-12);

	glBegin(GL_QUADS);
	glColor3d(0,0,0);
	glVertex3d(-ObjectSize,0,-ObjectSize);
	glVertex3d(-ObjectSize,0,ObjectSize);
	glVertex3d(ObjectSize,0,ObjectSize);
	glVertex3d(ObjectSize,0,-ObjectSize);
	glEnd();

	// Рисуем тор - освещенную поверхность
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,-2,-12);
	glRotated(z_angle,0,0,1);
	glRotated(y_angle,1,0,0);
	// Установливаем режимы OpenGL для тора
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCallList(torus);


// Рисуем теневую область
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,4,-12);


// Рисуем теневую область -- нелицевые грани
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	glDepthMask(GL_FALSE);
	
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS,1,1);
	glStencilOp(GL_KEEP,GL_KEEP,GL_ONE);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	RenderShadowVolume();

// Рисуем теневую область -- лицевые грани

	glStencilFunc(GL_ALWAYS,1,1);
	glStencilOp(GL_KEEP,GL_KEEP,GL_INVERT);

	glCullFace(GL_BACK);
	RenderShadowVolume();

	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

// Рисуем теневую область -- проволочную модель

	glDisable(GL_LIGHTING);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glColor3f(0,1,0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0,D1,0);
	glVertex3d(-ObjectSize2,-D2,-ObjectSize2);
	glVertex3d(-ObjectSize2,-D2,ObjectSize2);
	glVertex3d(ObjectSize2,-D2,ObjectSize2);
	glVertex3d(ObjectSize2,-D2,-ObjectSize2);
	glVertex3d(-ObjectSize2,-D2,-ObjectSize2);
	glEnd();


	// Рисуем тор - затененные области
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,-2,-12);
	glRotated(z_angle,0,0,1);
	glRotated(y_angle,1,0,0);
	// Установливаем режимы OpenGL для тора

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL,1,1);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glColor3f(0.2,0.2,0.2);
	glCallList(torus);

	glDisable(GL_STENCIL_TEST);


    glFlush();
	glutSwapBuffers();
}

void idle() {
	y_angle += 0.5;
	z_angle += 0.2;
	display();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH|GLUT_STENCIL|GLUT_RGB);
    glutCreateWindow("Torus");
	Init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
    glutMainLoop();
    return 0;          
}
