#include<gl/glut.h>

void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	if(h > 0) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0,w/(GLdouble)h,1,3);
	}
}

double a1 = 60; 
double a2 = 30;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();       // M = E * T * Rx(a1) * Ry(a2)
	glTranslated(0,0,-2);
	glRotated(a1,1,0,0);
	glRotated(a2,0,1,0);

	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	glBegin(GL_QUAD_STRIP);
	// Рисуем сторону 5-7-6-4
	glVertex3d(0.5, -0.5, 0.5);
	glVertex3d(0.5, 0.5, 0.5);
	glVertex3d(-0.5, -0.5, 0.5);
	glVertex3d(-0.5, 0.5, 0.5);
	// Рисуем сторону 4-6-2-0
	glVertex3d(-0.5, -0.5, -0.5);
	glVertex3d(-0.5, 0.5, -0.5);
	// Рисуем сторону 0-2-3-1
	glVertex3d(0.5, -0.5, -0.5);
	glVertex3d(0.5, 0.5, -0.5);
	// Рисуем сторону 1-3-7-5
	glVertex3d(0.5, -0.5, 0.5);
	glVertex3d(0.5, 0.5, 0.5);
	glEnd();

	glBegin(GL_QUADS);
	// Рисуем сторону 7-3-2-6
	glVertex3d(0.5, 0.5, 0.5);
	glVertex3d(0.5, 0.5, -0.5);
	glVertex3d(-0.5, 0.5, -0.5);
	glVertex3d(-0.5, 0.5, 0.5);
	// Рисуем сторону 5-4-0-1
	glVertex3d(0.5, -0.5, 0.5);
	glVertex3d(-0.5, -0.5, 0.5);
	glVertex3d(-0.5, -0.5, -0.5);
	glVertex3d(0.5, -0.5, -0.5);
	glEnd();

    glFlush();
	glutSwapBuffers();
}

void idle() {
	a1 += 0.3;
	a2 += 0.5;
	display();
}

void init() {
	glClearColor(0.0,0.0,0.0,0.0);
	glColor3d(1.0,1.0,1.0);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|/*GLUT_DEPTH|*/GLUT_RGB);
    glutCreateWindow("OpenGL cube");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
    glutMainLoop();
    return 0;          
}
