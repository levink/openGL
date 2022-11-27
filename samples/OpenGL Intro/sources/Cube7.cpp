#include<gl/glut.h>

double x_angle = 0;
double y_angle = 0;

double y_angle2 = 0;
double z_angle2 = 0;

GLuint cube; // Номер дисплейного списка

void Init()
{
	cube = glGenLists(1);
	glNewList(cube,GL_COMPILE);

	glBegin(GL_QUAD_STRIP);
	// Рисуем сторону 1-2-3-4
	glColor3d(1.0,1.0,0.0);
	glVertex3d(0.5, -0.5, 0.5);
	glVertex3d(0.5, 0.5, 0.5);
	glVertex3d(-0.5, -0.5, 0.5);
	glVertex3d(-0.5, 0.5, 0.5);
	
	// Рисуем сторону 4-3-7-8
	glColor3d(1.0,0.0,0.0);
	glVertex3d(-0.5, -0.5, -0.5);
	glVertex3d(-0.5, 0.5, -0.5);
	
	// Рисуем сторону 8-7-6-5
	glColor3d(0.0,1.0,0.0);
	glVertex3d(0.5, -0.5, -0.5);
	glVertex3d(0.5, 0.5, -0.5);
	
	// Рисуем сторону 5-6-2-1
	glColor3d(0.0,0.0,1.0);
	glVertex3d(0.5, -0.5, 0.5);
	glVertex3d(0.5, 0.5, 0.5);
	glEnd();

	glBegin(GL_QUADS);
	// Рисуем сторону 2-6-7-3
	glColor3d(0.0,1.0,1.0);
	glVertex3d(0.5, 0.5, 0.5);
	glVertex3d(0.5, 0.5, -0.5);
	glVertex3d(-0.5, 0.5, -0.5);
	glVertex3d(-0.5, 0.5, 0.5);
	
	// Рисуем сторону 1-4-8-5
	glColor3d(1.0,0.0,1.0);
	glVertex3d(0.5, -0.5, 0.5);
	glVertex3d(-0.5, -0.5, 0.5);
	glVertex3d(-0.5, -0.5, -0.5);
	glVertex3d(0.5, -0.5, -0.5);
	glEnd();

	glEndList();
}


void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	if(h > 0) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0,w/(GLdouble)h,1,10);
	}
}

void display(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glShadeModel(GL_FLAT);
	
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,0,-5);
	glRotated(x_angle,1,0,0);
	glRotated(y_angle,0,1,0);

	glCallList(cube);


	for(int i = -1;i<=1;i++)
		for(int j = -1;j<=1;j++) {
			glPushMatrix();
			glTranslated(i*0.5,j*0.5,0);
			glScalef(0.2f,0.2f,0.2f);
			glCallList(cube);
			glPopMatrix();
		}

	glLoadIdentity();
	glTranslated(0,0,-5);
	glRotated(y_angle2,0,1,0);
	glRotated(z_angle2,0,0,1);
	glTranslated(2,0,0);
	glScaled(0.2,0.2,0.2);

	for(int i = -1;i<=1;i++)
		for(int j = -1;j<=1;j++) {
			glPushMatrix();
			glTranslated(i*0.5,j*0.5,0);
			glScalef(0.2f,0.2f,0.2f);
			glCallList(cube);
			glPopMatrix();
		}

//	glCallList(cube); 

    glFlush();
	glutSwapBuffers();
}

void idle() {
	x_angle += 0.3;
	y_angle += 0.5;
	y_angle2 += 0.2;
	z_angle2 += 0.4;
	display();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGB);
    glutCreateWindow("OpenGL cube");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	Init();
    glutMainLoop();
	//Finalize();
    return 0;          
}
