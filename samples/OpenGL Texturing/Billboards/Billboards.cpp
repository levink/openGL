#include<gl/glut.h>
#include<tchar.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>

#include"BmpLoad.h"

double x_angle = 30;
double y_angle = 0;

int mouse_x = 0,mouse_y = 0,mouse_button = -1,mouse_state = GLUT_UP;

const int GroundSize = 5;
const double TreeSize = 1.0;

double trees[100];

GLuint tree_tex;

unsigned char *ConstructTexture(int *w,int *h)
{
	int width1,height1;
	unsigned char *tex1 = LoadTrueColorBMPFile("tree_color.bmp",&width1,&height1);
	if(!tex1)
		return NULL;
	int width2,height2;
	unsigned char *tex2 = LoadTrueColorBMPFile("tree_alpha.bmp",&width2,&height2);
	if(!tex2) {
		delete[] tex1;
		return NULL;
	}
	if(width1 != width2 || height1 != height2) {
		delete[] tex1;
		delete[] tex2;
		return NULL;
	}
	unsigned char *result = new unsigned char [width1*height1*4];
	if(result != NULL) {
		for(int i = 0;i<width1*height1;i++) {
			result[4*i] = tex1[3*i];
			result[4*i + 1] = tex1[3*i + 1];
			result[4*i + 2] = tex1[3*i + 2];
			result[4*i + 3] = (unsigned char)(((int)tex2[3*i] + (int)tex2[3*i + 1] + (int)tex2[3*i + 2])/3);
		}
	}	
	delete[] tex1;
	delete[] tex2;
	*w = width1;
	*h = height1;
	return result;
}


void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	if(h > 0) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0,w/(GLdouble)h,1,3000);
	}
}

void display(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glTranslated(0,0,-3*GroundSize);
	glRotated(x_angle,1,0,0);
	glRotated(y_angle,0,1,0);

	glBegin(GL_QUADS);
	glColor3d(0,1,0);
	glVertex3d(GroundSize,0,GroundSize);
	glVertex3d(-GroundSize,0,GroundSize);
	glVertex3d(-GroundSize,0,-GroundSize);
	glVertex3d(GroundSize,0,-GroundSize);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	int treeCount = sizeof(trees)/sizeof(trees[0]);
	for(int i = 0;i<treeCount;i++) {
		int index = (y_angle > 180)?(i):(treeCount - 1 - i);
		double x = -GroundSize + TreeSize + 2*(GroundSize - TreeSize)*index/(treeCount - 1);
		double z = trees[index];
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.05);
		glPushMatrix();

		glLoadIdentity();
		glTranslated(0,0,-3*GroundSize);
		glRotated(x_angle,1,0,0);
		glRotated(y_angle,0,1,0);
		glTranslated(x,0,z);
		glRotated(-y_angle,0,1,0);
		glRotated(-x_angle,1,0,0);
		glBegin(GL_QUADS);
		glTexCoord2d(1.0,0);
		glVertex3d(TreeSize/2,0,0);
		glTexCoord2d(1.0,1.0);
		glVertex3d(TreeSize/2,TreeSize,0);
		glTexCoord2d(0.0,1.0);
		glVertex3d(-TreeSize/2,TreeSize,0);
		glTexCoord2f(0.0,0.0);
		glVertex3d(-TreeSize/2,0,0);
		glEnd();
		glPopMatrix();
	}
	glDisable(GL_TEXTURE_2D);

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
}

void mouseClick(int button,int state,int x,int y)
{
	mouse_button = button;
	mouse_state = state;
	mouse_x = x;
	mouse_y = y;
}

void mouseMotion(int x,int y)
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
	// Текстура куба
	unsigned char *tex_bits = NULL;
	int tex_width,tex_height;
	if((tex_bits = ConstructTexture(&tex_width,&tex_height)) == NULL)
		return false;
	glGenTextures(1,&tree_tex);
	glBindTexture(GL_TEXTURE_2D,tree_tex);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage2D(GL_TEXTURE_2D,
		         0,
			     4,
			     tex_width,tex_height,
			     0,
			     GL_RGBA,
			     GL_UNSIGNED_BYTE,tex_bits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Расставляем деревья
	for(int i = 0;i<sizeof(trees)/sizeof(trees[0]);i++)
		trees[i] = -GroundSize + TreeSize + 2*rand()*(GroundSize - TreeSize)/RAND_MAX;
	return true;
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutCreateWindow("Billboard Demo");
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
