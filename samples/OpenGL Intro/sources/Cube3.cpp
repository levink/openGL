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

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

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

void init() {
	// Задаем матрицу модельного преобразования
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,0,-2);
	glRotated(30,1,0,0);
	glRotated(15,0,1,0);
	// Рисуем только границы объекта
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	// Рисуем белый куб ...
	glColor3d(1.0,1.0,1.0);
	// ... на черном фоне
	glClearColor(0.0,0.0,0.0,0.0);
}

void idle() {
	// Nothing to do here
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
    glutCreateWindow("OpenGL cube (optimized & rotated)");
	init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
    glutMainLoop();
    return 0;          
}

