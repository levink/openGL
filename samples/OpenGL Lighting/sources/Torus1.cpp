#include<gl/glut.h>
#include<math.h>

const double Pi = 3.1415;

double y_angle = 0;
double z_angle = 0;

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

void Init() 
{
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	const double r1 = 2.0;
	const double r2 = 0.5;
	const int n1 = 25;
	const int n2 = 15;
	torus = MakeTorus(r1,r2,n1,n2);
}

void Finalize() 
{
	glDeleteLists(torus,1);
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
	glRotated(z_angle,0,0,1);
	glRotated(y_angle,1,0,0);
	glCallList(torus);
    glFlush();
	glutSwapBuffers();
}

void idle() {
	y_angle += 0.5;
	z_angle += 0.2;
	display();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH|GLUT_RGB);
    glutCreateWindow("Torus");
	Init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
    glutMainLoop();
	Finalize();
    return 0;          
}
