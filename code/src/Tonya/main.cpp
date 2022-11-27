#include <windows.h>
#include <cmath>
#include <time.h>
#include <glut/glut.h>
#include <glut/glext.h>
#include "data.h"
#include "functions.h"
#include "bmp.h"


void reshape(int w, int h)
{
	glViewport(0,0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,w/(GLdouble)h,0.1,150);
	glRotated(mouse_x_angle,1,0,0);	
	glRotated(mouse_y_angle,0,1,0);
	glTranslated(pos[0],pos[1],pos[2]);

	width_scene = w;
	height_scene = h;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key,int x,int y)
{
	double np;
	double Pi = 3.1415;
	double speed = 0.2;
	switch(key){
		case 'w':
			np = pos[0] - speed*sin(mouse_y_angle*2*Pi/360);
			if (np>-29 && np<29) pos[0] = np;
			np = pos[2] + speed*cos(mouse_y_angle*2*Pi/360);
			if (np>-29 && np<29) pos[2] = np;
			break;
		case 's':
			np = pos[0] + speed*sin(mouse_y_angle*2*Pi/360);
			if (np>-29 && np<29) pos[0] = np;
			np = pos[2] - speed*cos(mouse_y_angle*2*Pi/360);
			if (np>-29 && np<29) pos[2] = np;
			break;
		case 'a':
			np = pos[0] + speed*cos(mouse_y_angle*2*Pi/360);
			if (np>-29 && np<29) pos[0] = np;
			np = pos[2] + speed*sin(mouse_y_angle*2*Pi/360);
			if (np>-29 && np<29) pos[2] = np;
			break;
		case 'd':
			np = pos[0] - speed*cos(mouse_y_angle*2*Pi/360);
			if (np>-29 && np<29) pos[0] = np;
			np = pos[2] - speed*sin(mouse_y_angle*2*Pi/360);
			if (np>-29 && np<29) pos[2] = np;
			break;
		case ' ':
			if (pos[1]>-25) pos[1] -= 0.25;
			break;
		case 'c':
			if (pos[1]<-0.5) pos[1] += 0.25;
			break;
		default:
			break;
		}
}
void idle(void)
{
	GetSystemTime(&t2);
	WORD t;
	t=t2.wMilliseconds - t1.wMilliseconds;
	if (t>10)
	{
		t1 = t2;
		glutPostRedisplay();
	}
}
void mouseclick(int button,int state,int x,int y)
{
	mouse_button = button;
	mouse_state = state;
	mouse_x = x;
	mouse_y = y;
}
void mousemotion(int x,int y) // Поворачиваем объект при движении мыши
{
	int dx = x - mouse_x;
	int dy = y - mouse_y;	
	if(mouse_state == GLUT_DOWN) {
		if(mouse_button == GLUT_LEFT_BUTTON) {
			
			mouse_y_angle += dx/3;
			if(mouse_y_angle < 0)  mouse_y_angle += 360;
			if(mouse_y_angle >360) mouse_y_angle -= 360;
			
			mouse_x_angle += dy/3;
			if(mouse_x_angle < -90)  mouse_x_angle = -90;
			if(mouse_x_angle > 90) mouse_x_angle = 90;
			glutPostRedisplay();
		}
	}
	mouse_x = x;
	mouse_y = y;
}
void init(void)
{
	//fog
	typedef void (APIENTRYP PFNGLFOGCOORDFPROC) (GLfloat coord);
	glFogCoordfEXT = (PFNGLFOGCOORDFEXTPROC) wglGetProcAddress("glFogCoordfEXT");


	//textures
	unsigned char *tex_bits = NULL;
	tex_bits = ConstructTexture(&tex_width,&tex_height);
	glGenTextures(1,&tree);
	glBindTexture(GL_TEXTURE_2D,tree);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage2D(GL_TEXTURE_2D,0,4,tex_width,tex_height,0,GL_RGBA,GL_UNSIGNED_BYTE,tex_bits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glGenTextures(1,&ice);
	glBindTexture(GL_TEXTURE_2D,ice);
	tex_bits = LoadTrueColorBMPFile("img/ice.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	delete[] tex_bits;


	//shadows
	plane[0] = 0; plane[1] = 1; plane [2] = 0; plane[3] = 0;
	ShadowMatrix(matrix,plane,lpos);

	//billboards
	elka[0].x = 20;   elka[1].x = 13;   elka[2].x =22;    elka[3].x = 13;  elka[4].x =7;     elka[5].x = 20;
	elka[0].y = 6;    elka[1].y = 6;    elka[2].y = 6;    elka[3].y = 4.5; elka[4].y =4.5;   elka[5].y = 5.5;
	elka[0].z = 20;   elka[1].z = 22;   elka[2].z = 13;   elka[3].z = 13;  elka[4].z =18;    elka[5].z = 5;
	elka[0].size=4;   elka[1].size=3;   elka[2].size=3.5; elka[3].size=2;  elka[4].size=1.5; elka[5].size = 3.2;


	
}
void display (void)
{
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	reshape(width_scene,height_scene);
	glEnable(GL_DEPTH_TEST);

	//lighting
	GLfloat global_light[] = {0.2,0.2,0.2,1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,global_light);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	
	
	//light0
	GLfloat ambient[] = {1.0f,1.0f,1.0f,1.0f};
	glLightfv(GL_LIGHT0,GL_AMBIENT_AND_DIFFUSE, ambient);
	glLightfv(GL_LIGHT0,GL_POSITION,lpos);
	glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION ,0.0);
	glEnable(GL_LIGHT0);

	
	
	//отражение
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_ZERO,GL_ZERO,GL_REPLACE);
	glStencilFunc(GL_ALWAYS,1,1);
	glDepthMask(GL_FALSE);
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	
	glBegin(GL_QUADS);
	glVertex3f(-30,0,-30);
	glVertex3f(30,0,-30);
	glVertex3f(30,0,30);
	glVertex3f(-30,0,30);
	glEnd();

	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	glStencilFunc(GL_EQUAL,1,1);
	
	GLdouble p0[4] = {0,-1,0,0};
	GLdouble p1[4] = {-1,0,0,30}; //x<30
	GLdouble p2[4] = {1,0,0,30};  //x>-30
	GLdouble p3[4] = {0,0,1,30};  //z>-30
	GLdouble p4[4] = {0,0,-1,30};  //z<30

	glClipPlane(GL_CLIP_PLANE0,p0);
	glClipPlane(GL_CLIP_PLANE1,p1);
	glClipPlane(GL_CLIP_PLANE2,p2);
	glClipPlane(GL_CLIP_PLANE3,p3);
	glClipPlane(GL_CLIP_PLANE4,p4);
	
	//рисуем отражения
	glPushMatrix();	
	glScaled(1,-1,1);
	glLightfv(GL_LIGHT0,GL_POSITION,lpos);
	snowman();
	bspline();
	glEnable(GL_CLIP_PLANE0);
	glEnable(GL_CLIP_PLANE0);
	glEnable(GL_CLIP_PLANE1);
	glEnable(GL_CLIP_PLANE2);
	glEnable(GL_CLIP_PLANE3);
	glEnable(GL_CLIP_PLANE4);
	landscape();
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
	glDisable(GL_CLIP_PLANE2);
	glDisable(GL_CLIP_PLANE3);
	glDisable(GL_CLIP_PLANE4);

	trees();

	glPopMatrix();
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_CLIP_PLANE0);
	glLightfv(GL_LIGHT0,GL_POSITION,lpos);
	

	//рисуем лед
	GLfloat mat[4] = {1,1,1,0.6};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,mat);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D,ice);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glTexCoord2d(0,0);
	glVertex3f(-30,0,-30);
	glTexCoord2d(0,1);
	glVertex3f(30,0,-30);
	glTexCoord2d(1,1);
	glVertex3f(30,0,30);
	glTexCoord2d(1,0);
	glVertex3f(-30,0,30);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//тень от снеговика
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
	glStencilFunc(GL_EQUAL,1,1);
	glColor4d(0,0,0,0.3);
	glPushMatrix();
	glMultMatrixf((GLfloat *) matrix);
	shadowflag = 1;
	snowman();
	bspline();
	shadowflag = 0;
	glPopMatrix();
	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);		
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	
	
	//снеговика
	snowman();
	

	//сугробы
	p0[1] = 1;
	glClipPlane(GL_CLIP_PLANE0,p0);
	glEnable(GL_CLIP_PLANE0);
	glEnable(GL_CLIP_PLANE0);
	glEnable(GL_CLIP_PLANE1);
	glEnable(GL_CLIP_PLANE2);
	glEnable(GL_CLIP_PLANE3);
	glEnable(GL_CLIP_PLANE4);
	landscape();
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
	glDisable(GL_CLIP_PLANE2);
	glDisable(GL_CLIP_PLANE3);
	glDisable(GL_CLIP_PLANE4);

	//фон.
	GLfloat fc[4] = {0.7,0.7,0.7,1.0};
	glFogfv(GL_FOG_COLOR,fc);
	glFogi(GL_FOG_MODE,GL_LINEAR);
	glFogf(GL_FOG_START,0); //при < этой величины - тумана нет 
	glFogf(GL_FOG_END,25); // при > этой величины - ничего не видно
	glFogf(GL_FOG_COORDINATE_SOURCE_EXT,GL_FOG_COORDINATE_EXT);
	glEnable(GL_FOG);
	background();
	glDisable(GL_FOG);


	bspline();
	particles();
	trees();


	glFlush();
	glutSwapBuffers();
}
int main(int argc, char *argv[])
{	
	glutInit(&argc, argv);
	glutInitWindowSize(width_scene,height_scene);
	glutInitWindowPosition(100, 200);

	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL );
	glutCreateWindow("OpenGL");
	
	init();
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseclick);
	glutMotionFunc(mousemotion);
	glutIdleFunc(idle);
	
	glutMainLoop();
	return 0;
}
void snowman(void)
{

	if (!shadowflag)
	{
		GLfloat amb[4] = {1,1,1,1};
		GLfloat dif[4] = {0.8,0.8,0.8,1};
		GLfloat spec[4] = {0.1,0.1,0.1,0.1};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,amb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,dif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,10);
	}
	glPushMatrix();
	glTranslated(0,2,0);
	glutSolidSphere(3,50,50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0,5.9,0);
	glutSolidSphere(2.1,50,50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0,8.7,0);
	glutSolidSphere(1.4,50,50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(2,6.7,0);
	glutSolidSphere(0.7,50,50);
	glPopMatrix();
	glPushMatrix();
	glTranslated(-2,6.7,0.3);
	glutSolidSphere(0.7,50,50);
	glPopMatrix();
	
	if (!shadowflag)
	{
		GLfloat amb[4] = {0.0,0.0,0.1};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,amb);
	}
	glPushMatrix();
	glTranslated(0.4,9.2,1.2);
	glutSolidSphere(0.12,15,15);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.4,9.2,1.2);
	glutSolidSphere(0.12,15,15);
	glPopMatrix();

	if (!shadowflag)
	{
		GLfloat amb[4] = {1,0.0,0.1};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,amb);
	}
	glPushMatrix();
	glTranslated(0,8.8,1.0);
	glRotated(-10,1,0,0);
	glScaled(1,1,12);
	glutSolidSphere(0.13,50,50);
	glPopMatrix();
}
void landscape(void)
{
	GLfloat amb[4] = {0.75,0.75,0.85,1};
	GLfloat dif[4] = {0.5,0.6,0.6,1};
	GLfloat spec[4] = {0,0,0,1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,100);

	glPushMatrix();
	glTranslated(30,-43,10);
	glutSolidSphere(50,50,50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(22,-63,30);
	glutSolidSphere(70,50,50);
	glPopMatrix();
	

	glPushMatrix();
	glTranslated(-29,-24,12);
	glutSolidSphere(30,50,50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-40,-65,-10);
	glutSolidSphere(70,50,50);
	glPopMatrix();
}
void background(void)
{
	GLfloat amb[4] = {1,1,1,1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,amb);

	glBegin(GL_QUADS);
	for(int i=0;i<30;i++)
	{
		glNormal3d(1,0,0);
		glFogCoordfEXT(i);
		glVertex3f(-30, i,-30);
		glFogCoordfEXT(i+1);
		glVertex3f(-30,i+1,-30);
		glVertex3f(-30,i+1,30);
		glFogCoordfEXT(i);
		glVertex3f(-30,i,30);
	}
	for(int i=0;i<30;i++)
	{
		glNormal3d(0,0,-1);
		glFogCoordfEXT(i);
		glVertex3f(-30,i,30);
		glFogCoordfEXT(i+1);
		glVertex3f(-30,i+1,30);			
		glVertex3f(30,i+1,30);
		glFogCoordfEXT(i);
		glVertex3f(30,i,30);
		}
	for(int i=0;i<30;i++)
	{
		glNormal3d(0,0,1);
		glFogCoordfEXT(i);
		glVertex3f(-30,i,-30);
		glVertex3f(30, i,-30);
		glFogCoordfEXT(i+1);
		glVertex3f(30,i+1,-30);
		glVertex3f(-30,i+1,-30);
	}
	for(int i=0;i<30;i++)
	{
		glNormal3d(-1,0,0);
		glFogCoordfEXT(i);
		glVertex3f(30, i,-30);
		glVertex3f(30, i, 30);
		glFogCoordfEXT(i+1);
		glVertex3f(30,i+1, 30);
		glVertex3f(30,i+1,-30);
	}

		glNormal3d(0,1,0);
		glFogCoordfEXT(30);
		glVertex3f(-30, 30, -30);
		glVertex3f( 30, 30, -30);
		glVertex3f( 30, 30,  30);
		glVertex3f(-30, 30,  30);

	glEnd();
}
void trees(void)
{
	glColor4d(0.6,0.8,0.6,1);

	for(int i=0;i<6;i++)
	{
		elka[i].r = (pos[0]+elka[i].x)*(pos[0]+elka[i].x) +
					(pos[1]+elka[i].y)*(pos[1]+elka[i].y) + 
					(pos[2]+elka[i].z)*(pos[2]+elka[i].z);
	}

	double x;
	double y;
	double r;
	for(int i=0; i < 6; i++) 
	{ 
		for(int j = 0; j < i; j++ ) 
		{
			My_tree a;
			if ( elka[j].r < elka[j+1].r ) 
			{
				a = elka[j];
				elka[j] = elka[j+1];
				elka[j+1] = a;
			}
		}
	}



	//выводим на экран в порядке back-to-front
	glBindTexture(GL_TEXTURE_2D,tree);
	glTexEnvf(GL_TEXTURE_ENV_COLOR,GL_TEXTURE_ENV_MODE,GL_BLEND);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);

	for (int i=0;i<6;i++)
	{
		glPushMatrix();
		glTranslated(elka[i].x,elka[i].y,elka[i].z);
		glRotated(-mouse_y_angle,0,1,0);
		glBegin(GL_QUADS);
			glTexCoord2d(0,0);
			glVertex3d(elka[i].size,0,0);
			glTexCoord2d(0,1);
			glVertex3d(elka[i].size,3*elka[i].size,0);
			glTexCoord2d(1,1);
			glVertex3d(-elka[i].size,3*elka[i].size,0);
			glTexCoord2f(1,0);
			glVertex3d(-elka[i].size,0,0);
		glEnd();
		glPopMatrix();
	}

	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}
void particles(void)
{
	GLfloat amb[4] = {4,4,4,1};
	GLfloat dif[4] = {1,1,1,1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);


	int x = rand()%60;
	int y = rand()%60;

	P[x][y].flag = 1;
	P[x][y].h = 30;
	P[x][y].time = 0;
	P[x][y].phi = rand()%360;

	glEnable(GL_FOG);
	for(int i=0;i<60;i++)
		for(int j=0;j<60;j++)
		{
			if (P[i][j].flag)
			{
				P[i][j].time++;
				P[i][j].h = 30 - P[i][j].time*0.025;
				if (P[i][j].h < 0) 
				{ 
					P[i][j].time = 0; 
					P[i][j].flag = 0; 
				}
				else
				{
					double t = P[i][j].time;
					glFogCoordfEXT(P[i][j].h-15);
					glPushMatrix();
					glTranslated(i-30 + sin(P[i][j].phi+t*6.28/360), P[i][j].h, j-30 + sin(P[i][j].phi+t*6.28/360));
					glutSolidSphere(0.1,10,10);	
					glPopMatrix();
				}
			}
		}
	glDisable(GL_FOG);
}

void bspline(void)
{
	GLfloat amb[4] = {-2.5,1,-2.5,1};
	GLfloat dif[4] = {1,1,1,1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
	

	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_TEXTURE_COORD_2);
	glBindTexture(GL_TEXTURE_2D, ice);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslated(-10,0.3,-20.2);
		glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 5, 0, 1, 15, 5, &BSP[0][0][0]);
		glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 5, 2, 2, 0, 5, 2, 2, &TexBSP[0][0][0]);
		glMapGrid2f(20, 0, 1, 20, 0, 1);
		glEvalMesh2(GL_FILL, 0, 20, 0, 20);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}