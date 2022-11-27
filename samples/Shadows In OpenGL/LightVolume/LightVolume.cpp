#include<gl/glut.h>
#include<tchar.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>

double x_angle = 30; // ���� �������� ��������
double y_angle = 0;

double px_angle = 0;
double py_angle = 0;

int mouse_x = 0,mouse_y = 0,mouse_button = -1,mouse_state = GLUT_UP;

const int ObjectSize = 3;

void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	if(h > 0) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0,w/(GLdouble)h,1,3000);
	}
}

// �������� ����� � ������� ����������� �����������
void RenderLightSource()
{
	glPushMatrix();
	glTranslated(5,1,0);
	glTranslated(0.4,0,0);
	glRotated(px_angle,0,1,0);
	glRotated(py_angle,0,0,1);
	glTranslated(-0.4,0,0);
	glRotated(90,0,1,0);
	glutSolidCone(0.1, 0.4, 10,5);
	glPopMatrix();
}

// �������� ����� � ������� �����������
void RenderLightVolume()
{
	glPushMatrix();
	glTranslated(-4.6,1,0);
	glTranslated(10,0,0);
	glRotated(px_angle,0,1,0);
	glRotated(py_angle,0,0,1);
	glTranslated(-10,0,0);
	glRotated(90,0,1,0);
	glutSolidCone(2.5, 10, 40,5);
	glPopMatrix();
}

// ������� � ������� �����������
void RenderObjects()
{
	// ������������� �������� ����
	GLfloat c_emissive[4] = { 0.1f, 0.1f, 0.1f, 0.0f };
	GLfloat c_diffuse[4] = { 0.2f, 0.8f, 0.6f, 0.0f };
	GLfloat c_specular[4] = { 0.2f, 0.6f, 0.2f, 0.0f };
	GLfloat c_ambient[4] = { 0.1f, 0.1f, 0.1f, 0.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c_ambient);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,10);
	// ������ ���
	glPushMatrix();
	glTranslated(-1.0,0.5,-1.0);
	glutSolidCube(1.0);
	glPopMatrix();

	// ������������� �������� �����
	GLfloat s_emissive[4] = { 0.1f, 0.0f, 0.0f, 0.0f };
	GLfloat s_diffuse[4] = { 0.8f, 0.2f, 0.2f, 0.0f };
	GLfloat s_specular[4] = { 0.6f, 0.2f, 0.2f, 0.0f };
	GLfloat s_ambient[4] = { 0.1f, 0.1f, 0.1f, 0.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, s_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, s_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, s_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, s_ambient);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,10);
	// ������ �����
	glPushMatrix();
	glTranslated(1.0,0.5,-1.0);
	glutSolidSphere(0.5,15,10);
	glPopMatrix();

	// ������������� �������� 
	GLfloat t_emissive[4] = { 0.0f, 0.0f, 0.1f, 0.0f };
	GLfloat t_diffuse[4] = { 0.4f, 0.4f, 1.0f, 0.0f };
	GLfloat t_specular[4] = { 0.2f, 0.2f, 0.6f, 0.0f };
	GLfloat t_ambient[4] = { 0.1f, 0.1f, 0.1f, 0.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, t_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, t_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, t_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, t_ambient);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,10);
	// ������ ������
	glPushMatrix();
	glTranslated(0.0,0.5,1.0);
	glutSolidTeapot(0.8);
	glPopMatrix();

	// ������������� �������� 
	GLfloat p_emissive[4] = { 0.02f, 0.0f, 0.08f, 0.0f };
	GLfloat p_diffuse[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat p_specular[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat p_ambient[4] = { 0.2f, 0.0f, 0.8f, 0.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, p_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, p_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, p_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, p_ambient);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,10);
	// ������ ��������
	glBegin(GL_QUADS);
	glColor4d(0,0,0.9,0.5);
	glVertex3d(ObjectSize,0,ObjectSize);
	glVertex3d(-ObjectSize,0,ObjectSize);
	glVertex3d(-ObjectSize,0,-ObjectSize);
	glVertex3d(ObjectSize,0,-ObjectSize);
	glEnd();

}

void RenderSurface()
{
	glBegin(GL_QUADS);
	glColor4d(0,0,0.9,0.5);
	glVertex3d(ObjectSize,0,ObjectSize);
	glVertex3d(-ObjectSize,0,ObjectSize);
	glVertex3d(-ObjectSize,0,-ObjectSize);
	glVertex3d(ObjectSize,0,-ObjectSize);
	glEnd();
}

// ��������� ��������� ��������� ��������� � ����������
GLfloat l_position[4] = { 5.0f, 1.0f, 0.0f, 1.0f };	

void display(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

	glDepthFunc(GL_LESS);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// ����� �������������� ��� ���� �����
	glTranslated(0,0,-3*ObjectSize);
	glRotated(x_angle,1,0,0);
	glRotated(y_angle,0,1,0);

	// ������ ���������
	glColor3d(1,1,0);
	RenderLightSource();
	//RenderLightVolume();

	glEnable(GL_LIGHTING);

	// ������������� �������� ���������
	glLightfv(GL_LIGHT0,GL_POSITION,l_position);
	// ������ ������� ��� ���������
	glDisable(GL_LIGHT0);
	RenderObjects();

// ������ ������� ������� -- ��������� �����
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	glDepthMask(GL_FALSE);
	
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS,1,1);
	glStencilOp(GL_KEEP,GL_KEEP,GL_ONE);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	RenderLightVolume();

// ������ ������� ������� -- ������� �����

	glStencilFunc(GL_ALWAYS,1,1);
	glStencilOp(GL_KEEP,GL_KEEP,GL_INVERT);

	glCullFace(GL_BACK);
	RenderLightVolume();

	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

// ������������� ������ OpenGL ��� ��������

	glStencilFunc(GL_EQUAL,1,1);
	
	glEnable(GL_LIGHT0);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	RenderObjects();

	glDisable(GL_STENCIL_TEST);
	glDisable(GL_LIGHTING);

	glFlush();
	glutSwapBuffers();
}

void idle() {
	// ... Nothing to do here
}

void keypress(unsigned char key,int x,int y)
{
	if(key == 27) {
		glutDestroyWindow(glutGetWindow());
		exit(1);
	}
	if(key == 'w') {
		py_angle += 1;
		display();
	}
	if(key == 's') {
		py_angle -= 1;
		display();
	}
	if(key == 'a') {
		px_angle -= 1;
		display();
	}
	if(key == 'd') {
		px_angle += 1;
		display();
	}
}

void mouseClick(int button,int state,int x,int y)
{
	mouse_button = button;
	mouse_state = state;
	mouse_x = x;
	mouse_y = y;
}

void mouseMotion(int x,int y) // ������������ ������ ��� �������� ����
{
	int dx = x - mouse_x;
	int dy = y - mouse_y;	
	if(mouse_state == GLUT_DOWN) {
		if(mouse_button == GLUT_LEFT_BUTTON) {
			y_angle += dx/3;
			if(y_angle < 0)
				y_angle += 360;
			if(y_angle >= 360)
				y_angle -= 360;
			x_angle += dy/3;
			if(x_angle < 5)
				x_angle = 10;
			if(x_angle > 90)
				x_angle = 90;
			display();
		}
	}
	mouse_x = x;
	mouse_y = y;
}

bool Init()
{
	// ������������� ���� ��������� ���������
	GLfloat l_diffuse[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat l_specular[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat l_ambient[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	glLightfv(GL_LIGHT0,GL_DIFFUSE,l_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,l_specular);
	glLightfv(GL_LIGHT0,GL_AMBIENT,l_ambient);

	// �������� ���������
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);

	// ������ ��������� OpenGL
	glEnable(GL_DEPTH_TEST);
	return true;
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH|GLUT_STENCIL);
    glutCreateWindow("Reflect Demo");
	if(!Init())
		return 1;
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMotion);
    glutDisplayFunc(display);
	glutKeyboardFunc(keypress);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
    glutMainLoop();
    return 0;          
}
