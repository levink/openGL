#include<gl/glut.h>

void idle() {
	// Nothing to do here
}

void reshape(int w, int h)
{
    glViewport(0,0,w,h); // Область рисования -- все окно
}

void display(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT);
   // glFlush(); // Гарантируем выполнение всех операций: попробуйте закомментировать :)
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);	
	glutInitDisplayMode(GLUT_RGB);
    glutCreateWindow("Самая простая OpenGL программа");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
    glutMainLoop();
    return 0;          
}
