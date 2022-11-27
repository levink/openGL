#include<gl/glut.h>
#include<tchar.h>

#include"BmpLoad.h"

double x_angle = 0;
double y_angle = 0;

int tex_width;
int tex_height;
unsigned char *tex_bits = NULL;

GLuint texture = 0;

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
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,0,-2);
	glRotated(x_angle,1,0,0);
	glRotated(y_angle,0,1,0);

	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	glColor3d(1.0,1.0,1.0);

	// ������ ������� 5-7-6-4
	glTexCoord2d(0,0);
	glVertex3d(0.5, -0.5, 0.5);
	glTexCoord2d(1,0);
	glVertex3d(0.5, 0.5, 0.5);
	glTexCoord2d(1,1);
	glVertex3d(-0.5, 0.5, 0.5);
	glTexCoord2d(0,1);
	glVertex3d(-0.5, -0.5, 0.5);
	// ������ ������� 4-6-2-0
	glTexCoord2d(0,0);
	glVertex3d(-0.5, -0.5, 0.5);
	glTexCoord2d(1,0);
	glVertex3d(-0.5, 0.5, 0.5);
	glTexCoord2d(1,1);
	glVertex3d(-0.5, 0.5, -0.5);
	glTexCoord2d(0,1);
	glVertex3d(-0.5, -0.5, -0.5);
	// ������ ������� 0-2-3-1
	glTexCoord2d(0,0);
	glVertex3d(-0.5, -0.5, -0.5);
	glTexCoord2d(1,0);
	glVertex3d(-0.5, 0.5, -0.5);
	glTexCoord2d(1,1);
	glVertex3d(0.5, 0.5, -0.5);
	glTexCoord2d(0,1);
	glVertex3d(0.5, -0.5, -0.5);
	// ������ ������� 1-3-7-5
	glTexCoord2d(0,0);
	glVertex3d(0.5, -0.5, -0.5);
	glTexCoord2d(5,0);
	glVertex3d(0.5, 0.5, -0.5);
	glTexCoord2d(5,5);
	glVertex3d(0.5, 0.5, 0.5);
	glTexCoord2d(0,5);
	glVertex3d(0.5, -0.5, 0.5);
	// ������ ������� 7-3-2-6
	glTexCoord2d(0,0);
	glVertex3d(0.5, 0.5, 0.5);
	glTexCoord2d(1,0);
	glVertex3d(0.5, 0.5, -0.5);
	glTexCoord2d(1,1);
	glVertex3d(-0.5, 0.5, -0.5);
	glTexCoord2d(0,1);
	glVertex3d(-0.5, 0.5, 0.5);
	// ������ ������� 5-4-0-1
	glTexCoord2d(0,0);
	glVertex3d(0.5, -0.5, 0.5);
	glTexCoord2d(1,0);
	glVertex3d(-0.5, -0.5, 0.5);
	glTexCoord2d(1,1);
	glVertex3d(-0.5, -0.5, -0.5);
	glTexCoord2d(0,1);
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

bool Init()
{
	// ������� ������ � ������� ��������
	if((tex_bits = LoadTrueColorBMPFile("colormap.bmp",&tex_width,&tex_height)) == NULL) 
		return false;
	// ������� ���������� ������
	glGenTextures(1,&texture);
	// ������������ ����������  ������
	glBindTexture(GL_TEXTURE_2D,texture);
	// ��������� ��������
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage2D(GL_TEXTURE_2D,
		         0,                     // Mip-level
			     GL_RGB,                // ������ ��������
			     tex_width,tex_height,  // ������ ��������
			     0,                     // ������ �������
			     GL_RGB,                // ������ �������� ������
			     GL_UNSIGNED_BYTE,      // ��� �������� ������
				 tex_bits);             // ��������� �� �������� ������ 
	// ����������� ������, � ������� ��������� ��������
	delete[] tex_bits;
	// ������������� ��������� ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// �������� ������������� ���������������
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    // ��������� ���������������
	glEnable(GL_TEXTURE_2D);

	glShadeModel(GL_FLAT);
	glEnable(GL_CULL_FACE);

	return true;
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutCreateWindow("OpenGL cube with texture");
	if(!Init())
		return 1;
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
    glutMainLoop();
    return 0;          
}
