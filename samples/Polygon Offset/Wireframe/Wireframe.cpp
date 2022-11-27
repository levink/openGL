#include<gl/glut.h>
#include<gl/glut.h>

GLuint sphere_list = 0;
GLuint teapot_list = 0;
GLuint cube_list = 0;

GLfloat sphere_emissive[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat sphere_diffuse[4] = { 0.7f, 0.0f, 0.0f, 0.0f };
GLfloat sphere_specular[4] = { 0.3f, 0.0f, 0.0f, 0.0f };
GLfloat sphere_ambient[4] = { 0.1f, 0.0f, 0.0f, 0.0f };
GLfloat sphere_shine = 10;

GLfloat teapot_emissive[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat teapot_diffuse[4] = { 0.0f, 0.4f, 0.4f, 0.0f };
GLfloat teapot_specular[4] = { 0.0f, 0.6f, 0.6f, 0.0f };
GLfloat teapot_ambient[4] = { 0.0f, 0.1f, 0.0f, 0.0f };
GLfloat teapot_shine = 8;

GLfloat cube_emissive[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat cube_diffuse[4] = { 0.2f, 0.2f, 0.2f, 0.0f };
GLfloat cube_specular[4] = { 0.8f, 0.8f, 0.8f, 0.0f };
GLfloat cube_ambient[4] = { 0.1f, 0.1f, 0.1f, 0.0f };
GLfloat cube_shine = 12;

int angle = -140;

void display()
{
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslated(0,0,-6);
	glRotated(30,1,0,0);
	glRotated(angle,0,1,0);

	// Рисуем закрашенные и освещенные объекты
	glEnable(GL_LIGHTING);
	//glEnable(GL_POLYGON_OFFSET_FILL);
	//glPolygonOffset(0.3,10);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPushMatrix();
	glTranslated(1,0,-1);
	glCallList(sphere_list);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1,0.2f,-1);
	glCallList(teapot_list);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0,0,1);
	glCallList(cube_list);
	glPopMatrix();
	//glDisable(GL_POLYGON_OFFSET_FILL);

	// Рисуем каркасные объекты
	glDisable(GL_LIGHTING);
	//glEnable(GL_POLYGON_OFFSET_LINE);
	//glPolygonOffset(-0.3,-10);
	glColor3f(0.0f, 1.0f, 0.0f);

	glPushMatrix();
	glTranslated(1,0,-1);
	glutWireSphere(0.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1,0.2f,-1);
	glutWireTeapot(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0,0,1);
	glutWireCube(1);
	glPopMatrix();
	//glDisable(GL_POLYGON_OFFSET_LINE);

	// Рисуем подложку
	glDisable(GL_LIGHTING);

	glTranslated(0,-0.5,0);
	glBegin(GL_QUADS);
	glColor3f(0.0f,0.0f,1.0f);
	glVertex3f(-2,0,2);
	glVertex3f(2,0,2);
	glVertex3f(2,0,-2);
	glVertex3f(-2,0,-2);
	glEnd();


    glFlush();
	glutSwapBuffers();

}

void Init()
{
	// Создаем список команд для сферы
	sphere_list = glGenLists(1);
	glNewList(sphere_list,GL_COMPILE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sphere_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, sphere_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sphere_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, sphere_ambient);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, sphere_shine);
	glutSolidSphere(0.5,20,20);
	glEndList();

	// Создаем список команд для чайника
	teapot_list = glGenLists(1);
	glNewList(teapot_list,GL_COMPILE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, teapot_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, teapot_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, teapot_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, teapot_ambient);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, teapot_shine);
	glutSolidTeapot(1);
	glEndList();

	// Создаем список команд для куба
	cube_list = glGenLists(1);
	glNewList(cube_list,GL_COMPILE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, cube_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cube_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, cube_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cube_ambient);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, cube_shine);
	glutSolidCube(1);
	glEndList();


	// Задаем режимы освещения
	// Устанавливаем цвет источника освещения
	GLfloat l_diffuse[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat l_specular[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat l_ambient[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	glLightfv(GL_LIGHT0,GL_DIFFUSE,l_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,l_specular);
	glLightfv(GL_LIGHT0,GL_AMBIENT,l_ambient);
	// Устанавливаем координаты освещения
	GLfloat l_position[4] = { 4.0f, 0.0f, 4.0f, 1.0f };
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0,GL_POSITION,l_position);
	
	// Включаем освещение
	glEnable(GL_LIGHT0);

	// Включаем закраску по Гуро
	glShadeModel(GL_SMOOTH);

	// Включаем буфер глубины
	glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	if(h > 0) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0,w/(GLdouble)h,1,20);
	}
}

void idle() {
	angle += 1;
	display();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGB);
    glutCreateWindow("Wireframe with polygon offset");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	Init();
    glutMainLoop();
    return 0;          
}
