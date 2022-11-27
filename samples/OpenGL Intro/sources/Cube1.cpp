#include<gl/glut.h>

void reshape(int w, int h)
{
	glViewport(0,0,w,h); // Задаем размер окна
}

void display(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
	glColor3d(1.0,1.0,1.0);
	// Рисуем сторону 5-7-6-4
	glVertex3d(0.5, -0.5, 0.5);
	glVertex3d(0.5, 0.5, 0.5);
	glVertex3d(-0.5, 0.5, 0.5);
	glVertex3d(-0.5, -0.5, 0.5);
	// Рисуем сторону 4-6-2-0
	glVertex3d(-0.5, -0.5, 0.5);
	glVertex3d(-0.5, 0.5, 0.5);
	glVertex3d(-0.5, 0.5, -0.5);
	glVertex3d(-0.5, -0.5, -0.5);
	// Рисуем сторону 0-2-3-1
	glVertex3d(-0.5, -0.5, -0.5);
	glVertex3d(-0.5, 0.5, -0.5);
	glVertex3d(0.5, 0.5, -0.5);
	glVertex3d(0.5, -0.5, -0.5);
	// Рисуем сторону 1-3-7-5
	glVertex3d(0.5, -0.5, -0.5);
	glVertex3d(0.5, 0.5, -0.5);
	glVertex3d(0.5, 0.5, 0.5);
	glVertex3d(0.5, -0.5, 0.5);
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
}

void idle() {
	// Nothing to do here
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
    glutCreateWindow("OpenGL cube");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
    glutMainLoop();
    return 0;          
}
