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
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,0,-2);
	glRotated(x_angle,1,0,0);
	glRotated(y_angle,0,1,0);

	glBegin(GL_QUAD_STRIP);
	// Рисуем сторону 5-7-6-4
	glColor3d(0.0,0.0,1.0);     // Синий цвет
	glVertex3d(0.5, -0.5, 0.5);
	glVertex3d(0.5, 0.5, 0.5);
	glVertex3d(-0.5, -0.5, 0.5);
	glVertex3d(-0.5, 0.5, 0.5);
	// Рисуем сторону 4-6-2-0
	glColor3d(0.0,1.0,0.0);     // Зеленый цвет
	glVertex3d(-0.5, -0.5, -0.5);
	glVertex3d(-0.5, 0.5, -0.5);
	// Рисуем сторону 0-2-3-1
	glColor3d(0.0,1.0,0.0);     // Зеленый цвет
	glVertex3d(0.5, -0.5, -0.5);
	glVertex3d(0.5, 0.5, -0.5);
	// Рисуем сторону 1-3-7-5
	glColor3d(0.0,1.0,1.0);     // Cyan
	glVertex3d(0.5, -0.5, 0.5);
	glVertex3d(0.5, 0.5, 0.5);
	glEnd();

	glBegin(GL_QUADS);
	// Рисуем сторону 7-3-2-6
	glColor3d(1.0,0.0,1.0);     // Magenta
	glVertex3d(0.5, 0.5, 0.5);
	glVertex3d(0.5, 0.5, -0.5);
	glVertex3d(-0.5, 0.5, -0.5);
	glVertex3d(-0.5, 0.5, 0.5);
	// Рисуем сторону 5-4-0-1
	glColor3d(1.0,1.0,0.0);     // Yellow
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

void init() {
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutCreateWindow("OpenGL solid cube");
	init();
	glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
    glutMainLoop();
    return 0;          
}
