#include<gl/glut.h>
#include<math.h>

const double Pi = 3.1415;

double y_angle = 0;
double z_angle = 0;

const int Side = 8.0;
const int N = 200;

GLuint MakeSquare(double side,double n) {
	GLuint list = glGenLists(1);
	glNewList(list,GL_COMPILE);
	glNormal3d(0,1,0);
	for(int j = 0;j<n;j++) {
		glBegin(GL_QUAD_STRIP);
		for(int i = 0;i<=n;i++) {
			glVertex3d(side*(i/(double)n - 0.5),0,side*(j/(double)n - 0.5));
			glVertex3d(side*(i/(double)n - 0.5),0,side*((j+1)/(double)n - 0.5));
		}
		glEnd();
	}
	glEndList();
	return list;
}

GLuint square;

void Init() 
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	square = MakeSquare(Side,N);
	// Устанавливаем материал
	GLfloat m_emissive[4] = { 0.2f, 0.2f, 0.2f, 0.0f };
	GLfloat m_diffuse[4] = { 0.2f, 0.2f, 0.2f, 0.0f };
	GLfloat m_specular[4] = { 0.8f, 0.8f, 0.8f, 0.0f };
	GLfloat m_ambient[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambient);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,20);
	
	// Устанавливаем цвет
	GLfloat l_diffuse[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat l_specular[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat l_ambient[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	glLightfv(GL_LIGHT0,GL_DIFFUSE,l_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,l_specular);
	glLightfv(GL_LIGHT0,GL_AMBIENT,l_ambient);

	// Прожектор
	glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,30);

	glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,10);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glColor3d(1,1,1);
}

void Finalize() 
{
	glDeleteLists(square,1);
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
	gluLookAt(-5,7,-5,0,0,0,0,1,0);

	glPushMatrix();

	// Устанавливаем источник освещения
	GLfloat l_position[4] = { 0, 3, 0, 1 };
	glLightfv(GL_LIGHT0,GL_POSITION,l_position);

	GLfloat spot_dir[3] = {0,-1,0};
	glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,spot_dir);


	// Включаем освещение
	glEnable(GL_LIGHTING);

	// Рисуем площадку
	glTranslated(0,-2,0);
	glCallList(square);
	glPopMatrix();

	// Обозначаем положение источника освещения
	glPushMatrix();
	glTranslated(0,3,0);
	glRotated(-90,1,0,0);
	glDisable(GL_LIGHTING);
	glColor3d(0,1,0);
	glutSolidCone(0.2, 0.4, 10,5);
	glPopMatrix();
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
