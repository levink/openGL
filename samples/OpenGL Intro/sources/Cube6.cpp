#include<gl/glut.h>

double x_angle = 0;
double y_angle = 0;

void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	if(h > 0) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0,w/(GLdouble)h,1,3);
	}
}

void display(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glShadeModel(GL_SMOOTH);
	
	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,0,-2);
	glRotated(x_angle,1,0,0);
	glRotated(y_angle,0,1,0);

	glBegin(GL_QUAD_STRIP);
	// Рисуем сторону 1-2-3-4
	glColor3d(1.0,1.0,0.0);
	glVertex3d(0.5, -0.5, 0.5);
	glVertex3d(0.5, 0.5, 0.5);
	glVertex3d(-0.5, -0.5, 0.5);
	glVertex3d(-0.5, 0.5, 0.5);
	
	// Рисуем сторону 4-3-7-8
	glColor3d(1.0,0.0,0.0);
	glVertex3d(-0.5, -0.5, -0.5);
	glColor3d(0.0,1.0,0.0);
	glVertex3d(-0.5, 0.5, -0.5);
	
	// Рисуем сторону 8-7-6-5
	glColor3d(0.0,1.0,0.0);
	glVertex3d(0.5, -0.5, -0.5);
	glVertex3d(0.5, 0.5, -0.5);
	
	// Рисуем сторону 5-6-2-1
	glColor3d(0.0,0.0,1.0);
	glVertex3d(0.5, -0.5, 0.5);
	glVertex3d(0.5, 0.5, 0.5);
	glEnd();

	glBegin(GL_QUADS);
	// Рисуем сторону 2-6-7-3
	glColor3d(0.0,1.0,1.0);
	glVertex3d(0.5, 0.5, 0.5);
	glVertex3d(0.5, 0.5, -0.5);
	glVertex3d(-0.5, 0.5, -0.5);
	glVertex3d(-0.5, 0.5, 0.5);
	
	// Рисуем сторону 1-4-8-5
	glColor3d(1.0,0.0,1.0);
	glVertex3d(0.5, -0.5, 0.5);
	glVertex3d(-0.5, -0.5, 0.5);
	glVertex3d(-0.5, -0.5, -0.5);
	glVertex3d(0.5, -0.5, -0.5);
	glEnd();

    glFlush();
	glutSwapBuffers();
}

void idle() {
	x_angle += 0.3;
	y_angle += 0.5;
	display();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGB);
    glutCreateWindow("OpenGL cube");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
    glutMainLoop();
    return 0;          
}
