#include<windows.h>
#include<tchar.h>
#include<gl/glut.h>
#include<math.h>

#include"BmpLoad.h"

const double Pi = 3.1415;

double x_angle = 0;
double y_angle = 0;

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
	if(!list)
		return 0;
	glNewList(list,GL_COMPILE);
	glBegin(GL_QUADS);
	glColor3f(0,1,0);
	for(int i = 0;i<n1;i++) {
		int i2 = (i<n1-1)?(i+1):(0);
		double phi1 = 2*i*Pi/n1;
		double phi2 = 2*i2*Pi/n1;
		for(int j = 0;j<n2;j++) {
			int j2 = (j<n2-1)?(j+1):(0);
			double psi1 = 2*j*Pi/n2;
			double psi2 = 2*j2*Pi/n2;
			glTexCoord2d(5*i/(double)n1,j/(double)n2);
			TorusVertex(r1,r2,phi1,psi1);
			glTexCoord2d(5*i/(double)n1,(j+1)/(double)n2);
			TorusVertex(r1,r2,phi1,psi2);
			glTexCoord2d(5*(i+1)/(double)n1,(j+1)/(double)n2);
			TorusVertex(r1,r2,phi2,psi2);
			glTexCoord2d(5*(i+1)/(double)n1,j/(double)n2);
			TorusVertex(r1,r2,phi2,psi1);
		}
	}
	glEnd();
	glEndList();
	return list;
}

GLuint torus;

int tex_width;
int tex_height;
unsigned char *tex_bits;
GLuint torus_tex;

unsigned char *ConstructTexture(int *w,int *h)
{
	int width1,height1;
	unsigned char *tex1 = LoadIndexedBMPFile("colormap.bmp",&width1,&height1);
	if(!tex1)
		return NULL;
	int width2,height2;
	unsigned char *tex2 = LoadIndexedBMPFile("alpha.bmp",&width2,&height2);
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

bool init() 
{
    if((tex_bits = ConstructTexture(&tex_width,&tex_height)) == NULL)
		return false;

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	glGenTextures(1,&torus_tex);
	glBindTexture(GL_TEXTURE_2D,torus_tex);
	glTexImage2D(GL_TEXTURE_2D,
		         0,
			     4,
			     tex_width,tex_height,
			     0,
			     GL_RGBA,
			     GL_UNSIGNED_BYTE,tex_bits);
	delete[] tex_bits;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

	glEnable(GL_TEXTURE_2D);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	GLfloat diffuse[4] = { 0.1f, 0.0f, 0.4f, 0.0f };
	GLfloat specular[4] = { 0.7f, 0.7f, 0.7f, 0.0f };
	GLfloat ambient[4] = { 0.07f, 0.3f, 0.3f, 0.0f };
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambient);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,10);

	GLfloat pos[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0,GL_POSITION,pos);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glShadeModel(GL_SMOOTH);

	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	torus = MakeTorus(2.0,0.5,25,15);
	return true; // Success
}

void reshape(int w, int h)
{
	if(h > 0) {
		glViewport(0,0,w,h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0,w/(GLdouble)h,1,20);
	}
}

void display(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,0,-8);
	glRotated(x_angle,1,0,0);
	glRotated(y_angle,0,1,0);
	glBindTexture(GL_TEXTURE_2D,torus_tex);
	glCallList(torus);
    glFlush();
	glutSwapBuffers();
}

void idle() {
	x_angle += 0.5;
	y_angle += 0.2;
	display();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH|GLUT_RGB);
    glutCreateWindow("Torus");
	if(!init()) {
		::MessageBox(NULL,_T("Initialization failed"),_T("Error!"),MB_OK|MB_ICONEXCLAMATION);
		return 1;
	}
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
    glutMainLoop();
    return 0;          
}
