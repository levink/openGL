#include<gl/glut.h>

void idle() {
	// Nothing to do here
}

void reshape(int w, int h)
{
    glViewport(0,0,w,h); // ������� ��������� -- ��� ����
}

void display(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT);
   // glFlush(); // ����������� ���������� ���� ��������: ���������� ���������������� :)
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);	
	glutInitDisplayMode(GLUT_RGB);
    glutCreateWindow("����� ������� OpenGL ���������");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
    glutMainLoop();
    return 0;          
}
