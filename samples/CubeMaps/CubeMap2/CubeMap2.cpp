#include<windows.h>
#include<gl/glut.h>
#include<math.h>

#include"glext.h"
#include"CubeTexture.h"

double x_angle = 0;
double y_angle = 0;

const double Pi = 3.1415;

inline void TorusVertex(double r1,double r2,double phi,double psi)
{
	double nx = cos(phi)*cos(psi);
	double ny = sin(psi);
	double nz = sin(phi)*cos(psi);
	glNormal3d(nx,ny,nz);
	glVertex3d(r1*cos(phi) + r2*nx,r2*ny,r1*sin(phi) + r2*nz);
}

GLuint MakeTorus(double r1,double r2,int n1,int n2) {
	GLuint list = glGenLists(1);
	glNewList(list,GL_COMPILE);
	glBegin(GL_QUADS);
	glColor3f(1,1,1);
	for(int i = 0;i<n1;i++) {
		int i2 = (i<n1-1)?(i+1):(0);
		double phi1 = 2*i*Pi/n1;
		double phi2 = 2*i2*Pi/n1;
		for(int j = 0;j<n2;j++) {
			int j2 = (j<n2-1)?(j+1):(0);
			double psi1 = 2*j*Pi/n2;
			double psi2 = 2*j2*Pi/n2;
			TorusVertex(r1,r2,phi1,psi1);
			TorusVertex(r1,r2,phi1,psi2);
			TorusVertex(r1,r2,phi2,psi2);
			TorusVertex(r1,r2,phi2,psi1);
		}
	}
	glEnd();
	glEndList();
	return list;
}

GLuint torus;

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

const int BoxSize = 30.0;

void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	if(h > 0) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0,w/(GLdouble)h,0.1,45);
	}
}

void display(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,0,-5);
	glRotated(y_angle,0,1,0);
	glRotated(x_angle,1,0,0);
	
	glEnable(GL_TEXTURE_CUBE_MAP_EXT);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP_EXT);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP_EXT);
	glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP_EXT);
	
	// Поворачиваем кубическую текстуру в соответсвии с объектов
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glRotated(-x_angle,1,0,0);
	glRotated(-y_angle,0,1,0);

	glMatrixMode(GL_MODELVIEW);
	glCallList(torus);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_RESCALE_NORMAL);
	glutSolidTeapot(1.0);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	// Рисуем окружающую среду
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_CUBE_MAP_EXT);

	glRotated(y_angle,0,1,0);
	glRotated(x_angle,1,0,0);


	glColor3d(1,1,1);
	glBegin(GL_QUADS);	
	for(int i = 0;i<24;i++) {
		int idx = cubeVertices[i];
		glTexCoord3d(v[idx].x,v[idx].y,v[idx].z);
		glVertex3d(BoxSize*v[idx].x,BoxSize*v[idx].y,BoxSize*v[idx].z);
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
	torus = MakeTorus(2.0,0.3,40,20);
}

void finalize()
{
	glDeleteLists(torus,1);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);	
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
    glutCreateWindow("Environment cube map");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	init();
    glutMainLoop();
    return 0;          
}
