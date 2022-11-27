
#include<windows.h>
#include<gl/glut.h>

#include"glext.h"
#include"CubeTexture.h"

double x_angle = 0;
double y_angle = 0;

struct Vertex {
	double x,y,z;
} v[8] = { 
	{ -0.5,-0.5,-0.5 }, 
	{ 0.5,-0.5,-0.5 },
	{ -0.5,0.5,-0.5 },
	{ 0.5,0.5,-0.5 },
	{ -0.5,-0.5,0.5 },
	{ 0.5,-0.5,0.5 },
	{ -0.5,0.5,0.5 },
	{ 0.5,0.5,0.5 }
};

int cubeVertices[24] = {
	5,4,6,7,
	6,2,3,7,
	3,1,5,7,
	4,5,1,0,
	1,3,2,0,
	2,6,4,0
};

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

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,0,-2);
	glRotated(x_angle,1,0,0);
	glRotated(y_angle,0,1,0);
	
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	
	glColor3d(1.0,1.0,1.0);

	for(int i = 0;i<24;i++) {
		int idx = cubeVertices[i];
		glTexCoord3d(v[idx].x,v[idx].y,v[idx].z);
		glVertex3d(v[idx].x,v[idx].y,v[idx].z);
	}

	glEnd();
    glFlush();
	glutSwapBuffers();
}

void idle() {
	x_angle += 0.3;
	y_angle += 0.45;
    display();
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	static const char *texture[6] = {
		"pos_x.bmp",
		"neg_x.bmp",
		"pos_y.bmp",
		"neg_y.bmp",
		"pos_z.bmp",
		"neg_z.bmp"
	};
	LoadCubeTexture(texture);
	glEnable(GL_TEXTURE_CUBE_MAP_EXT);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);	
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
    glutCreateWindow("Simple cube map");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	init();
	MessageBox(NULL,(char*)glGetString(GL_EXTENSIONS),"Info",MB_OK);
    glutMainLoop();
    return 0;          
}
