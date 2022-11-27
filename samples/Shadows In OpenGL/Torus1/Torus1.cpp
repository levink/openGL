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
	glEnable(GL_CULL_FACE);
	const double r1 = 2.0;
	const double r2 = 0.5;
	const int n1 = 25;
	const int n2 = 15;
	torus = MakeTorus(r1,r2,n1,n2);
	// Устанавливаем материал
	GLfloat m_emissive[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat m_diffuse[4] = { 0.4f, 0.4f, 0.4f, 0.0f };
	GLfloat m_specular[4] = { 0.6f, 0.6f, 0.6f, 0.0f };
	GLfloat m_ambient[4] = { 0.3f, 0.3f, 0.3f, 0.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambient);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,10);
	// Устанавливаем цвет
	GLfloat l_diffuse[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat l_specular[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat l_ambient[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	glLightfv(GL_LIGHT0,GL_DIFFUSE,l_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,l_specular);
	glLightfv(GL_LIGHT0,GL_AMBIENT,l_ambient);
	// Устанавливаем источник освещения
	GLfloat l_position[4] = { 0.0f, 20.0f, -12.0f, 1.0f };
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0,GL_POSITION,l_position);
	// Включаем освещение
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
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

	// Рисуем фон
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);
	// Установливаем режимы OpenGL
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	// Рисуем прямоугольльник
	glBegin(GL_QUADS);
	glColor3d(0,1,0);
	glVertex2d(-1,-1);
	glVertex2d(1,-1);
	glVertex2d(1,0);
	glVertex2d(-1,0);
	glColor3d(0,0,1);
	glVertex2d(-1,0);
	glVertex2d(1,0);
	glVertex2d(1,1);
	glVertex2d(-1,1);
	glEnd();
	// Восстанавливаем матрицы
	glPopMatrix();

	// Рисуем тор
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,2,-12);
	glRotated(z_angle,0,0,1);
	glRotated(y_angle,1,0,0);
	// Установливаем режимы OpenGL для тора
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCallList(torus);

	// Рисуем тень тора
	GLdouble p[16] = {
		1,0,0,0,
        0,0,0,0,
		0,0,1,0,
		0,0,0,1
	};
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,-3,0);
	glMultMatrixd(p);
	glTranslated(0,2,-12);
	glRotated(z_angle,0,0,1);
	glRotated(y_angle,1,0,0);
	// Установливаем режимы OpenGL для тени тора
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glColor4d(0,0,0,0.5);
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
