#include<gl/glut.h>
#include<tchar.h>
#include<math.h>

const double Pi = 3.14159265358979323846;

double x_angle = 30;
double y_angle = 30;

int mouse_x = 0,mouse_y = 0,mouse_button = -1,mouse_state = GLUT_UP;

const int GroundSize = 5;

const double RoadWidth = 0.8;
const int RoadSegments = 100;
const int RoadMarkers = 50;

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

//	glEnable(GL_POLYGON_OFFSET_FILL);
//	glPolygonOffset(-0.1,-3);
	
	// Рисуем дорогу
	glBegin(GL_QUAD_STRIP);
	glColor3f(0.6f,0.6f,0.6f);
	for(int i = 0;i<=RoadSegments;i++) {
		double r1 = GroundSize - 1 - RoadWidth/2;
		double r2 = GroundSize - 1 + RoadWidth/2;
		double phi = 2*i*Pi/RoadSegments;
		glVertex3d(r1*cos(phi),0,r1*sin(phi));
		glVertex3d(r2*cos(phi),0,r2*sin(phi));
	}
	glEnd();

	// Рисуем разметку
	
//	glPolygonOffset(-0.1,-10);
	glBegin(GL_QUADS);
	glColor3f(1.0f,1.0f,1.0f);
	for(int i = 0;i<=RoadMarkers;i++) {
		double r1 = GroundSize - 1 - RoadWidth/10;
		double r2 = GroundSize - 1 + RoadWidth/10;
		double phi = 2*i*Pi/RoadMarkers;
		double phi2 = 2*(i + 0.5)*Pi/RoadMarkers;
		glVertex3d(r1*cos(phi),0,r1*sin(phi));
		glVertex3d(r2*cos(phi),0,r2*sin(phi));
		glVertex3d(r2*cos(phi2),0,r2*sin(phi2));
		glVertex3d(r1*cos(phi2),0,r1*sin(phi2));
	}
	glEnd();
//	glDisable(GL_POLYGON_OFFSET_FILL);

/*	glPushMatrix();
	glTranslated(-GroundSize + 1,0.5,-GroundSize + 1);
	glColor3f(0.5f,0.5f,0.5f);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(GroundSize - 1,0.5,-GroundSize + 1);
	glColor3f(0.0f,0.8f,0.0f);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-GroundSize + 1,0.5,GroundSize - 1);
	glColor3f(0.8f,0.0f,0.0f);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(GroundSize - 1,0.5,GroundSize - 1);
	glColor3f(0.0f,0.0f,0.8f);
	glutSolidCube(1.0);
	glPopMatrix();*/

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
//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LEQUAL);
	return true;
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutCreateWindow("Polygon Offset Demo");
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
