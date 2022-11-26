#include <windows.h>
#include <time.h>
#include <math.h>
#include <glut/glut.h>
#include <glut/glext.h>
#include "formats.h"
#include "functions.h"
#include "data.h"



void reshape(int w, int h)
{
	glViewport(0,0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,w/(GLdouble)h,0.1,150);
	width = w;
	height = h;
}
void idle(void)
{
	GetSystemTime(&curtime);
	WORD t = 0;
	t=curtime.wMilliseconds - time1.wMilliseconds;	
	if (t>10)
	{
		time1 = curtime;
		idle_ang1 +=0.5;
		if (idle_ang1>360) idle_ang1 = 0;
		glutPostRedisplay();
	}
	
}

void keyboard(unsigned char key,int x,int y)
{
	/*������ � �����������*/
	switch(key){		
		case '1':
			red+=0.1;
			break;
		case '2':
			red-=0.1;
			break;
		case '3':
			green+=0.1;
			break;
		case '4':
			green-=0.1;
			break;
		case '5':
			blue+=0.1;
			break;
		case '6':
			blue-=0.1;
			break;
		case 'w':
			if (fw<0) 
				fw+=0.5;
			break;
		case 's':
			if (fw>-19) 
				fw-=0.5;
			break;
		case 'f':
			swf=(swf+1)%2;
			break;
		default:
			break;


	}
	glutPostRedisplay();
}
void mouseClick(int button,int state,int x,int y)
{
	mouse_button = button;
	mouse_state = state;
	mouse_x = x;
	mouse_y = y;
}
void mouseMotion(int x,int y) // ������������ ������ ��� �������� ����
{
	int dx = x - mouse_x;
	int dy = y - mouse_y;	
	if(mouse_state == GLUT_DOWN) {
		if(mouse_button == GLUT_LEFT_BUTTON) {
			y_angle += dx/3;
			if(y_angle < 0)
				y_angle += 360;
			if(y_angle >360)
				y_angle -= 360;
			x_angle += dy/3;
			if(x_angle < 0)
				x_angle = 0;
			if(x_angle > 90)
				x_angle = 90;
			display();
		}
	}
	mouse_x = x;
	mouse_y = y;
}

void initialize(void)
{
	//���������� ��� ������
	typedef void (APIENTRYP PFNGLFOGCOORDFPROC) (GLfloat coord);
	glFogCoordfEXT = (PFNGLFOGCOORDFEXTPROC) wglGetProcAddress("glFogCoordfEXT");
	//���������� ��� ������� �����
	typedef void (APIENTRYP PFNGLACTIVETEXTUREARBPROC) (GLenum texture);
	glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
	//��� ����������;
	typedef void (APIENTRYP PFNGLBLENDCOLORPROC) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
	glBlendColor = (PFNGLBLENDCOLORPROC) wglGetProcAddress("glBlendColor");

	//��������� ���� �����
	//GLfloat scene_ambient[] = {0.25f,0.25f,0.25f,1.0f};
	GLfloat scene_ambient[] = {0.1f,0.1f,0.1f,0.5f};
	glShadeModel(GL_SMOOTH);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,scene_ambient); //������� ��������� ���� �����
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE); // ��������� ����� ������

	//�������� ������� �����
	glGenTextures(1,&shadowMap);
	glBindTexture(GL_TEXTURE_2D,shadowMap);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE_ARB,GL_COMPARE_R_TO_TEXTURE_ARB);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_FUNC_ARB,GL_LEQUAL);

	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
	glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
	glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32_ARB,shadowMapSize,shadowMapSize,0,
		GL_DEPTH_COMPONENT32_ARB,GL_UNSIGNED_BYTE,NULL );

	//�������� ������� �����������
	InitLists();

	//������������� ����
	/*������� ������������� �������, ����������� ������� �� (�,�) � ������� �� z*/
	/*�� ������� z-���������� ��������� � (x,y) - � ��������*/
	for(int i=0;i<128;i++)
	{
		for(int j=0;j<128;j++)
		{
			vertices[i][j].coo[0]=10.0f-20.0f*i/127.0f;
			vertices[i][j].coo[1]=10.0f-20.0f*(j+1)/127.0f;
			vertices[i][j].nor[2]=-5.0f/127.0f;
		}
	}

	//������������� �������
	// ������� ������ � ������� ��������
	tex_bits = LoadTrueColorBMPFile("img/001.bmp",&tex_width,&tex_height);
	// ������� ���������� ������
	glGenTextures(1,&texture[0]);
	// ������������ ����������  ������
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	// ��������� ��������
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage2D(GL_TEXTURE_2D,
		         0,                     // Mip-level
			     GL_RGB,                // ������ ��������
			     tex_width,tex_height,  // ������ ��������
			     0,                     // ������ �������
			     GL_RGB,                // ������ �������� ������
			     GL_UNSIGNED_BYTE,      // ��� �������� ������
				 tex_bits);             // ��������� �� �������� ������ 
	// ����������� ������, � ������� ��������� ��������
	delete[] tex_bits;
	// ������������� ��������� ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	
	tex_bits = LoadTrueColorBMPFile("img/water.bmp",&tex_width,&tex_height);
	glGenTextures(1,&texture[1]);
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage2D(GL_TEXTURE_2D,
		         0,                     // Mip-level
			     GL_RGBA,                // ������ ��������
			     tex_width,tex_height,  // ������ ��������
			     0,                     // ������ �������
			     GL_RGB,                // ������ �������� ������
			     GL_UNSIGNED_BYTE,      // ��� �������� ������
				 tex_bits);             // ��������� �� �������� ������ 
	delete[] tex_bits;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	tex_bits = LoadTrueColorBMPFile("img/Vinni.bmp",&tex_width,&tex_height);
	glGenTextures(1,&texture[2]);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage2D(GL_TEXTURE_2D,
		         0,                     // Mip-level
			     GL_RGBA,                // ������ ��������
			     tex_width,tex_height,  // ������ ��������
			     0,                     // ������ �������
			     GL_RGB,                // ������ �������� ������
			     GL_UNSIGNED_BYTE,      // ��� �������� ������
				 tex_bits);             // ��������� �� �������� ������ 
	delete[] tex_bits;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	tex_bits = LoadTrueColorBMPFile("img/Pyatachok.bmp",&tex_width,&tex_height);
	glGenTextures(1,&texture[3]);
	glBindTexture(GL_TEXTURE_2D,texture[3]);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage2D(GL_TEXTURE_2D,
		         0,                     // Mip-level
			     GL_RGBA,                // ������ ��������
			     tex_width,tex_height,  // ������ ��������
			     0,                     // ������ �������
			     GL_RGB,                // ������ �������� ������
			     GL_UNSIGNED_BYTE,      // ��� �������� ������
				 tex_bits);             // ��������� �� �������� ������ 
	delete[] tex_bits;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);



	//cubemap
	glGenTextures(1,&cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP,cubemap);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	tex_bits = LoadTrueColorBMPFile("img/cubemap/positive_x.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);
	
	tex_bits = LoadTrueColorBMPFile("img/cubemap/positive_y.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);

	tex_bits = LoadTrueColorBMPFile("img/cubemap/positive_z.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);

	tex_bits = LoadTrueColorBMPFile("img/cubemap/negative_x.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);

	tex_bits = LoadTrueColorBMPFile("img/cubemap/negative_y.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);

	tex_bits = LoadTrueColorBMPFile("img/cubemap/negative_z.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST);


	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
	glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);

	delete[] tex_bits;

	// �������� ������������� ���������������
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	
	
	//������� ������
	for (int i=0;i<maxparticles;i++)
	{
		MP[i].time=0;
		MP[i].v=(1 + (rand()%maxparticles)*0.001);
		MP[i].h= 2 + (rand()%maxparticles)*0.004;
		if (i%2) MP[i].dir = -1;
		else MP[i].dir = 1;
	}

}
void renderToShadowMap ()
{
	//��������� ��������� � ����� �����
    glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
                                       
	//��������� ������ �� ���������
    glViewport(0,0,shadowMapSize,shadowMapSize);
    glClear(GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(120,1,2,25);
    gluLookAt(light_position[0],light_position[1],light_position[2], 0,0,0, 0,0,1 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //���������� ������ ��������������                  
    glGetFloatv(GL_MODELVIEW_MATRIX, mv);
    glGetFloatv(GL_PROJECTION_MATRIX,pr);

    //������ ����� � ����� "�� ��������" =)
	glEnable(GL_POLYGON_OFFSET_FILL);//�������� ����� ��� ����������� ����������� �������
    glPolygonOffset(2,2);
	RenderScene (1);

	//��������� ���������� �������� ������� � ��������.
	//�������: �� ���������, ��� ����� �� ��������� - ��������,
	//��������� - � ����.
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glCopyTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32_ARB ,0,0,shadowMapSize,shadowMapSize,0);

	//������� ����� �������
	glClear(GL_DEPTH_BUFFER_BIT);

	//��������������� ��������
	glDisable(GL_POLYGON_OFFSET_FILL);
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
    glEnable(GL_TEXTURE_2D);


	//��� ��������� ������� �����
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D,shadowMap);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);
	
	GLfloat p1[4] = {1,0,0,0};
	GLfloat p2[4] = {0,1,0,0};
	GLfloat p3[4] = {0,0,1,0};
	GLfloat p4[4] = {0,0,0,1};

	glTexGenfv(GL_S,GL_EYE_PLANE,p1);
	glTexGenfv(GL_T,GL_EYE_PLANE,p2);
	glTexGenfv(GL_R,GL_EYE_PLANE,p3);
	glTexGenfv(GL_Q,GL_EYE_PLANE,p4);

	//��������� ���������� ��������� ��� ������� �����
	glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
    glTranslatef(0.5,0.5,0.5);     // [-1,1]^2  ==> [0,1]^2
    glScalef       ( 0.5, 0.5, 0.5 );
    glMultMatrixf  ( pr );
    glMultMatrixf  ( mv );
	glMatrixMode ( GL_MODELVIEW );
    glPopMatrix();
}

void display(void)
{
	//������� ����� �����
	renderToShadowMap();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(0,0);//����������� ����� ��������, �.�. � renderToShadowMap �� ��� ��������.
	
	//����������� ��������� ���� 
	reshape(width,height);
	
	//��������� ��������� ����� (������� ������)
	glMatrixMode(GL_PROJECTION);
	glTranslated(0,-2,fw);
	glRotated(x_angle,1,0,0);
	glRotated(y_angle,0,1,0);
	
	//����� ��������� ��������� �������
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//��������� ���������
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_SMOOTH);
	
	//������� ������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	RenderLamp(light_position);
	RenderScene(0); //���������� ��������� �����
	RenderScene(2);//��������� ����� � �������

	
	//��������������� ��� �����, ��� ��������
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glutSwapBuffers ();
}

int main (int argc, char **argv)
{
	//������������� glut
	glutInit(&argc, argv);

	//������ � ��������� ����
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(200, 200);

	//��������� �����������
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH|GLUT_STENCIL);
    glutCreateWindow("CG.exe");
	if (!initialized_flag)
	{ 
		initialize(); 
		initialized_flag = 1;
	}

	//��������:
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMotion);
	glutDisplayFunc(display);//��������� ��������
	glutReshapeFunc(reshape);//��� ��������� �������� ����
	glutKeyboardFunc(keyboard);//��� ������� ������
	glutIdleFunc(idle);//�� ����� "��������"


	//���� � �������� ����
	glutMainLoop();
	return 0;
}
void RenderScene(int mode)
{
	//mode==0 ���������� ���������
	//mode==1 ��������� � ����� �����
	//mode==2 ��������� � �������

	if (mode==0)
	{
	//---cubemap---

	//��������� ���������
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D,0);
	
	//���������� ���������� ��������
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP,cubemap);
	glTexEnvf(GL_TEXTURE_ENV_COLOR,GL_TEXTURE_ENV_MODE,GL_DECAL);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glDisable(GL_LIGHTING);

	glBegin(GL_QUADS);
	int idx;
	int a=24;
	if (cubeMapSize<100) a = 20;
	for(int i = 0;i<a;i++) 
	{
		idx = cubeVertices[i];
		glTexCoord3d(v[idx].x,v[idx].y,v[idx].z);
		glVertex3d(cubeMapSize*v[idx].x,cubeMapSize*v[idx].y,cubeMapSize*v[idx].z);
	}
	cubeMapSize=100;
	glEnd();

	//��������������� �����
	glDisable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_LIGHTING);


	//---floor---
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D,shadowMap);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	//�������� ��������������� � ����� �������
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	//�������� �������� ���������
	GLfloat fl_emissive[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat amb_and_dif[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specular[4] = {0.1f, 0.1f, 0.1f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, amb_and_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, fl_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,10);
	//������ ���
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glTexCoord2d(0,0);
	glVertex3f(-10,0,-10);
	glTexCoord2d(1,0);
	glVertex3f(10,0,-10);
	glTexCoord2d(1,1);
	glVertex3f(10,0,10);
	glTexCoord2d(0,1);
	glVertex3f(-10,0,10);
	glEnd();
	//� ��������� �������� ��� - ��� ��� ������ �� �����
	glBindTexture(GL_TEXTURE_2D,0);
	
	
	//---water---
	//������������� ������ ����� ��������������� �� ���� ��� ����� �����
	//����� ���� "��������������" ���� �� ����
	glActiveTextureARB(GL_TEXTURE1_ARB);	
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE_ARB,GL_NONE);
	//glTexEnvf(GL_TEXTURE_ENV_COLOR,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	
	//�������� �������� ������ �����������
	amb_and_dif[0]=0.6; 
	amb_and_dif[1]=0.8; 
	amb_and_dif[2]=1; 
	amb_and_dif[3]=0.5;
	specular[0] = 1.0;
	specular[1]	= 0.5;
	specular[2]	= 0.1;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, amb_and_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,130);

	//�.�. ���� ����������, �������� ��������� ����������
	glBlendColor(0.0,0.0,0.0,0.65);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_CONSTANT_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_BLEND);
	glEnable(GL_NORMALIZE);
	glEnable(GL_FOG);
	//����� ��� ������
	GLfloat fog_color[4] = {0.3,0.7,0.5,1.0};
	glFogfv(GL_FOG_COLOR,fog_color);
	glFogi(GL_FOG_MODE,GL_LINEAR);
	glFogf(GL_FOG_START,-0.15); //��� f< ������������� �� ����������
	glFogf(GL_FOG_END,0.15); // ��� f> ������ �������������
	glFogf(GL_FOG_COORDINATE_SOURCE_EXT,GL_FOG_COORDINATE_EXT);

	//��� ������ ������ �����������
	glPushMatrix();
	glTranslated(0,1,0);
	glRotated(90,1,0,0);
	frame(0.0055);
	glPopMatrix();

	//����������� �������� �� ������ ����� ������
	glDisable(GL_FOG);
	glDisable(GL_NORMALIZE);
	glDisable(GL_BLEND);
	//� ������� ���������� ����� �������������� ��� ������� �����
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE_ARB,GL_COMPARE_R_TO_TEXTURE_ARB);
	glTexEnvf(GL_TEXTURE_ENV_COLOR,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	
		
	//---Some objects---
	amb_and_dif[0]=1;
	amb_and_dif[1]=1;
	amb_and_dif[2]=1;
	amb_and_dif[3]=0.2;
	specular[0] = 0.1;
	specular[1]	= 0.1;
	specular[2]	= 0.1;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, amb_and_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,10);

	if (reflectionflag==0 || ((fw*sin(y_angle*6.2830/360)) >-10))
	{
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	
	glPushMatrix();
	glTranslated(-3,2,-3);
	glRotated(idle_ang1+30,0,1,0);
	glCallList(theObj);
	glPopMatrix();
	
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPushMatrix();
	glTranslated(3,2,-3);
	glRotated(idle_ang1,0,1,0);
	glCallList(theObj);
	glPopMatrix();
	
	
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D,shadowMap);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP,cubemap);
	glTexEnvf(GL_TEXTURE_ENV_COLOR,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_NORMALIZE);

	glPushMatrix();
	glTranslated(0,3.7,0);
	glCallList(theSphere);
	glPopMatrix();

	glDisable(GL_NORMALIZE);
	glDisable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_LIGHTING);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glTexEnvf(GL_TEXTURE_ENV_COLOR,GL_TEXTURE_ENV_MODE,GL_DECAL);
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D,shadowMap);
	//��������� � ����������, ����� ��� � ������ ����
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE_ARB,GL_NONE);
	glTexEnvf(GL_TEXTURE_ENV_COLOR,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	glEnable(GL_TEXTURE_2D);
	ViewParticle();
	glDisable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE_ARB,GL_COMPARE_R_TO_TEXTURE_ARB);
	glTexEnvf(GL_TEXTURE_ENV_COLOR,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	}
	}


	if (mode==1)//���������� ����� �����
	{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glPushMatrix();
	glTranslated(-3,2,-3);
	glRotated(idle_ang1+30,0,1,0);
	glCallList(theObj);
	glPopMatrix();


	glPushMatrix();
	glTranslated(3,2,-3);
	glRotated(idle_ang1,0,1,0);
	glCallList(theObj);
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(0,3.7,0);
	glCallList(theSphere);
	glPopMatrix();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	}
	
	if (mode == 2)//������ ���������� �����������
	{
	//���������: ����� ������ ����������� ���������,
	//� ����� �������� ��������� � ���������� ������.
	
	//�������� � ������ ��������� ������� ���������� �����������
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_ZERO,GL_ZERO,GL_REPLACE);
	glStencilFunc(GL_ALWAYS,1,1);
	//��������� ������ � ������ ������� � �����
	glDepthMask(GL_FALSE);
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	
	glPushMatrix();
	glTranslated(0,1.1,0);//�������� �������� �������
	glBegin(GL_QUADS);
	int h = 5;
	glVertex3f(-10,0,10);
	glVertex3f(-10,0,-10);
	glVertex3f(-10,h,-10);
	glVertex3f(-10,h,10);
	glEnd();
	glPopMatrix();		
	//������ ��������� ������
	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	//������ � ���������� �����������
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	glStencilFunc(GL_EQUAL,1,1);
	
	//��������� ��������� ���������
	//�������� Ax+By+Cz+D => {A,B,C,D}
	GLdouble eqn[4] = {-1,0,0,-10};
	glClipPlane(GL_CLIP_PLANE0,eqn);
	glEnable(GL_CLIP_PLANE0);

	// � ������ ���������� ������� 
	glPushMatrix();	
	glTranslated(-20,0,0);
	//������� ��������� ������������ ��������� X=0
	glScaled(-1,1,1);
	//������������� �������� ��������� (����������)
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	//���������� ������� �����, ����� � ��������� �� ����� �����
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D,shadowMap);
	GLfloat p1[4] = {1,0,0,0};
	glTexGenfv(GL_S,GL_EYE_PLANE,p1);
	//������ �����
	cubeMapSize = 40;
	reflectionflag=1;
	RenderScene(0);
	reflectionflag=0;
	glPopMatrix();
	glDisable(GL_STENCIL_TEST);
	
	//������ ���� ����������� �������
	GLfloat emissive[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat amb_and_dif[4] = { 1.0f, 1.0f, 1.0f, 0.2}; //��� ���� �������
	GLfloat specular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, amb_and_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,50);
	//�������� ����������
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	glTranslated(0,1.1,0);
	glBegin(GL_QUADS);
	glVertex3f(-10,0,10);
	glVertex3f(-10,0,-10);
	glVertex3f(-10,h,-10);
	glVertex3f(-10,h,10);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glDisable(GL_CLIP_PLANE0);

	//���� ��������� �� ���������� ������� (��� ������ ���), ������ ������ ������ (������������)
	if ( (fw*sin(y_angle*6.2830/360)) <-10)
	{
		glPushMatrix();
		glTranslated(-0.001,1.1,0);
		glBegin(GL_QUADS);
		glNormal3f(-1,0,0);
		glVertex3f(-10,0,10);
		glVertex3f(-10,0,-10);
		glVertex3f(-10,h,-10);
		glVertex3f(-10,h,10);
		glEnd();
	}
	}


}
void RenderLamp(GLfloat pos[4])
{
	//�������� ���������
	GLfloat white_light[] = {1.0f,1.0f,1.0f,1.0};
	GLfloat l_specular[] = {1.0f,1.0f,1.0f,1.0f};
	glLightfv(GL_LIGHT0,GL_AMBIENT_AND_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0,GL_SPECULAR, l_specular);
	glLightfv(GL_LIGHT0,GL_POSITION,pos);
	glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION ,light_direction);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

void InitLists(void)
{
	theObj = glGenLists(1);
	glNewList(theObj,GL_COMPILE);
	glBegin(GL_QUADS);
	double h = 0.3; //������
	double scale = 2; //�������
	double k = 1; //����������� ������ � ������
		for(int i=0;i<10;i++)
			for (int j=0; j<10;j++)
			{
				glNormal3d(-1,0,0);
				glTexCoord2d(i*0.1,j*0.1);
				glVertex3f(-h*scale,i*h*k,j*h - h*5);
				glTexCoord2d((i+1)*0.1,j*0.1);
				glVertex3f(-h*scale,(i+1)*h*k,j*h - h*5);
				glTexCoord2d((i+1)*0.1,(j+1)*0.1);
				glVertex3f(-h*scale,(i+1)*h*k,(j+1)*h - h*5);
				glTexCoord2d(i*0.1,(j+1)*0.1);
				glVertex3f(-h*scale,i*h*k,(j+1)*h - h*5);
			}
		for(int i=0;i<10;i++)
			for (int j=0; j<10;j++)
			{
				glNormal3d(1,0,0);
				glTexCoord2d(i*0.1,j*0.1);
				glVertex3f(h*scale,i*h*k,j*h - h*5);
				glTexCoord2d((i+1)*0.1,j*0.1);
				glVertex3f(h*scale,(i+1)*h*k,j*h - h*5);
				glTexCoord2d((i+1)*0.1,(j+1)*0.1);
				glVertex3f(h*scale,(i+1)*h*k,(j+1)*h - h*5);
				glTexCoord2d(i*0.1,(j+1)*0.1);
				glVertex3f(h*scale,i*h*k,(j+1)*h- h*5);
			}
		for(int i=0;i<10;i++)
			for (int j=0; j<10;j++)
			{
				glNormal3d(0,0,1);
				glVertex3f(-h*scale,i*h*k,10*h- h*5);
				glVertex3f(h*scale, i*h*k,10*h- h*5);
				glVertex3f(h*scale,(i+1)*h*k,10*h- h*5);
				glVertex3f(-h*scale,(i+1)*h*k,10*h- h*5);
			}
		for(int i=0;i<10;i++)
			for (int j=0; j<10;j++)
			{
				glNormal3d(0,0,-1);
				glVertex3f(-h*scale,i*h*k,0*h - h*5);
				glVertex3f(h*scale, i*h*k,0*h - h*5);
				glVertex3f(h*scale,(i+1)*h*k,0*h - h*5);
				glVertex3f(-h*scale,(i+1)*h*k,0*h - h*5);
			}

		glNormal3d(0,1,0);
		glVertex3f(-h*scale,10*h*k,0*h - h*5);
		glVertex3f(h*scale, 10*h*k,0*h - h*5);
		glVertex3f(h*scale,10*h*k,10*h - h*5);
		glVertex3f(-h*scale,10*h*k,10*h - h*5);

	glEnd();
	glEndList();


	theSphere = glGenLists(1);
	glNewList(theSphere,GL_COMPILE);
	glBegin(GL_QUADS);
	double r = 2;
	double A = 40;
	double B = 40;
	double Pi = 3.1415;
	for(int i=0;i<A;i++)
	{
		double phi = i*Pi*2/A;
		double phi1 = (i+1)*Pi*2/A;
		for(int j=0;j<B;j++)
		{
			double psy = j*Pi*2/B;
			double psy1 = (j+1)*Pi*2/B;
			double x=-0.3;
			double y=1;
			double z=0;

			glNormal3f(r*sin(phi)*cos(psy),r*sin(phi)*sin(psy),r*cos(phi));
			glTexCoord3f(r*sin(phi)*cos(psy)+x,r*sin(phi)*sin(psy)+y,r*cos(phi)+z);
			glVertex3f(r*sin(phi)*cos(psy),r*sin(phi)*sin(psy),r*cos(phi));

			glNormal3f(r*sin(phi1)*cos(psy),r*sin(phi1)*sin(psy),r*cos(phi1));
			glTexCoord3f(x+r*sin(phi1)*cos(psy),y+r*sin(phi1)*sin(psy),z+r*cos(phi1));
			glVertex3f(r*sin(phi1)*cos(psy),r*sin(phi1)*sin(psy),r*cos(phi1));
			
			glNormal3f(r*sin(phi1)*cos(psy1),r*sin(phi1)*sin(psy1),r*cos(phi1));
			glTexCoord3f(x+r*sin(phi1)*cos(psy1),y+r*sin(phi1)*sin(psy1),z+r*cos(phi1));
			glVertex3f(r*sin(phi1)*cos(psy1),r*sin(phi1)*sin(psy1),r*cos(phi1));

			glNormal3f(r*sin(phi)*cos(psy1),r*sin(phi)*sin(psy1),r*cos(phi));
			glTexCoord3f(x+r*sin(phi)*cos(psy1),y+r*sin(phi)*sin(psy1),z+r*cos(phi));
			glVertex3f(r*sin(phi)*cos(psy1),r*sin(phi)*sin(psy1),r*cos(phi));

			j++;
			}
			
	}
	glEnd();
	glEndList();

}

void frame(double vs)
{
	/*vs - ��������. ���� vs=0, �� ����� �� ����� ��������*/
	//�������� � 2 ������,�.�. ������ ���� ���� �� ������ �������.
	int i,j,i1,j1; 
	i1=rand()%128;  //���������� (�,�) ���������� ����������� �����
	j1 = 120;


    if((rand()% 100)==0) //��������� �����������.
	for(i=-4;i<5;i++)	//� ������������ 1/100
	{
		for(j=-4;j<5;j++)
		{
			int k = rand()%6;
			float v=k-i*i-j*j;
			if(v<0.0f)v=0.0f;
			n->U[i+i1+4][j+j1+4]-=v*0.005f + 0.06;	
		}
	}
	for (int k=0;k<128;k++)
	{
		n->U[k][127] = 0;
		n->U[k][0] = 0;
		n->U[0][k] = 0;
		n->U[127][k] = 0;
	}

	double A = 30;
	if (framerate>0)
	{	
	
		framerate -=1;
		for(i=1;i<127;i++)
		{
			glBegin(GL_QUAD_STRIP);
			for(j=1;j<127;j++)
			{	int k = 20;
				int l = 10;
				glFogCoordfEXT(vertices[i][j].coo[2]);
				if (j<30+A) glFogCoordfEXT(-(0.3-(j-10)*0.02*0.3));
				glTexCoord2d((l+vertices[i][j].coo[0])/k,(l+vertices[i][j].coo[1])/k);
				glNormal3fv(vertices[i][j].nor);
				glVertex3fv(vertices[i][j].coo);

				
				glFogCoordfEXT(vertices[i+1][j].coo[2]);
				if (j<30+A) glFogCoordfEXT(-(0.3-(j-10)*0.02*0.3));
				glTexCoord2d((l+vertices[i+1][j].coo[0])/k,(l+vertices[i+1][j].coo[1])/k);
				glNormal3fv(vertices[i+1][j].nor);
				glVertex3fv(vertices[i+1][j].coo);
			}
			glEnd();
		}
	}
	else
	{
	//������� ������ ����� � ����������� ���������� ������� �������
	for(i=1;i<127;i++)//������������� �� ��� ����� ���������.
	{
		for(j=1;j<127;j++)
		{
			vertices[i][j].coo[2]=n->U[i][j];
			vertices[i][j].nor[0]=(n->U[i-1][j]-n->U[i+1][j]);
			vertices[i][j].nor[1]=(n->U[i][j-1]-n->U[i][j+1]);
			
			float krest=(n->U[i-1][j]+ 
				          n->U[i+1][j]+
						  n->U[i][j+1]+
						  n->U[i][j-1])*0.25f-n->U[i][j];
			//krest - ���� � ��������� ������ ����������� ��������� �������.
			//������� ����� �������� ������ ����������� ����.
			p->U[i][j]=((2.0f-vs)*n->U[i][j]-p->U[i][j]*(1.0f-vs)+krest);
		}
	}


	
	for(i=1;i<127;i++)
	{
		glBegin(GL_QUAD_STRIP);
		for(j=1;j<127;j++)
		{	int k = 20;
			int l = 10;
			glFogCoordfEXT(vertices[i][j].coo[2]);
			if (j<30+A)  glFogCoordfEXT(-(0.3-(j-10)*0.02*0.3));
			glTexCoord2d((l+vertices[i][j].coo[0])/k,(l+vertices[i][j].coo[1])/k);
			glNormal3fv(vertices[i][j].nor);
			glVertex3fv(vertices[i][j].coo);

			glFogCoordfEXT(vertices[i+1][j].coo[2]);
			if (j<30+A)  glFogCoordfEXT(-(0.3-(j-10)*0.02*0.3));
			glTexCoord2d((l+vertices[i+1][j].coo[0])/k,(l+vertices[i+1][j].coo[1])/k);
			glNormal3fv(vertices[i+1][j].nor);
			glVertex3fv(vertices[i+1][j].coo);
		}
		glEnd();
	}
	glFogCoordfEXT(0);
	//������ ������� ����.
	field *sw=p;
	p=n;
	n=sw;
	sw=0;
	delete[] sw;
	framerate = 3;
	}
}

void ViewParticle(void)
{
	double size = 0.15;
	

	HandleParticle();
	glBegin(GL_QUADS);
	for (int i=0;i<maxparticles;i++)
	{
		double x = MP[i].coord[0]*MP[i].dir;
		double y = MP[i].coord[1];
		double z = -10 + i*0.001;
		glTexCoord2d(0,0);
		glVertex3f(x-size,y-size,z);
		glTexCoord2d(1,0);
		glVertex3f(x-size,y+size,z);
		glTexCoord2d(1,1);
		glVertex3f(x+size,y+size,z);
		glTexCoord2d(0,1);
		glVertex3f(x+size,y-size,z);
	}
	glEnd();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

}
void HandleParticle(void)
{
	for (int i=0;i<maxparticles;i++)
	{

		double k=1;
		MP[i].coord[0]= MP[i].v * MP[i].time *0.01;
		MP[i].coord[1]= MP[i].h * MP[i].h - k*k*(MP[i].coord[0]-MP[i].h)*(MP[i].coord[0]-MP[i].h);
		if (MP[i].coord[0] < (MP[i].h + MP[i].h/k) ) MP[i].time++;
		else MP[i].time = 0;	
	}
}
