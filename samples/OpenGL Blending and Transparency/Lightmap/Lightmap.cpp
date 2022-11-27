#include<gl/glut.h>
#include<tchar.h>

#include"BmpLoad.h"

double x_angle = 0;
double y_angle = 0;

int tex_width;
int tex_height;
unsigned char *tex_bits = NULL;

GLuint texture = 0;

int lightmap_width;
int lightmap_height;
unsigned char *lightmap_bits = NULL;

GLuint lightmap = 0;

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

	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D,texture);
	glBegin(GL_QUADS);	
	glTexCoord2d(0,0);
	glVertex3d(-0.5,-0.5,0);
	glTexCoord2d(1,0);
	glVertex3d(0.5,-0.5,0);
	glTexCoord2d(1,1);
	glVertex3d(0.5,0.5,0);
	glTexCoord2d(0,1);
	glVertex3d(-0.5,0.5,0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,lightmap);
	glBlendFunc(GL_ZERO,GL_SRC_COLOR);
	glEnable(GL_BLEND);

	glBegin(GL_QUADS);	
	glTexCoord2d(0,0);
	glVertex3d(-0.5,-0.5,0);
	glTexCoord2d(1,0);
	glVertex3d(0.5,-0.5,0);
	glTexCoord2d(1,1);
	glVertex3d(0.5,0.5,0);
	glTexCoord2d(0,1);
	glVertex3d(-0.5,0.5,0);
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
	// ��������� �������� �������

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
	// ������������� ��������� ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// ��������� ����� ������������ 

	// ������� ������ � ������� ��������
	if((lightmap_bits = LoadTrueColorBMPFile("lightmap.bmp",&lightmap_width,&lightmap_height)) == NULL) 
		return false;
	// ������� ���������� ������
	glGenTextures(1,&lightmap);
	// ������������ ����������  ������
	glBindTexture(GL_TEXTURE_2D,lightmap);
	// ��������� ��������
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage2D(GL_TEXTURE_2D,
		         0,                     // Mip-level
			     GL_RGB,                // ������ ��������
			     lightmap_width,lightmap_height,  // ������ ��������
			     0,                     // ������ �������
			     GL_RGB,                // ������ �������� ������
			     GL_UNSIGNED_BYTE,      // ��� �������� ������
				 lightmap_bits);             // ��������� �� �������� ������ 
	// ������������� ��������� ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// �������� ������������� ���������������
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    // ��������� ���������������
	glEnable(GL_TEXTURE_2D);
	return true;
}

void Finalize()
{
	if(texture != 0)
		glDeleteTextures(1,&texture);
	if(tex_bits != NULL)
		delete[] tex_bits;
	if(lightmap != 0)
		glDeleteTextures(1,&lightmap);
	if(lightmap_bits != NULL)
		delete[] lightmap_bits;
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutCreateWindow("OpenGL cube with texture");
	if(!Init())
		return 1;
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
    glutMainLoop();
	Finalize();
    return 0;          
}
