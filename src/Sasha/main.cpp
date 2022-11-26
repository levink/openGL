#include <windows.h>
#include <cmath>
#include <time.h>
#include <glut/glut.h>
#include <glut/glext.h>
#include "ext.h"
#include "bmp.h"
#include "data.h"
#include "headers.h"


int main(int argc, char *argv[])
{	//hello^^ML
	glutInit(&argc, argv);
	glutInitWindowSize(width_scene,height_scene);
	glutInitWindowPosition(100, 200);

	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL );
	glutCreateWindow("Компьютерная графика");
	
	//загрузка текстур, инициализация всего нужного и т.п.
	Init();
	
	//устанавливаем обработчики событий
	glutDisplayFunc(DisplayFunction);
	glutReshapeFunc(ReshapeFunction);
	glutKeyboardFunc(KeyboardFunction);
	glutMouseFunc(MouseClickFunction);
	glutMotionFunc(MouseMotionFunction);
	glutIdleFunc(IdleFunction);
	
	//и зацикливаемся
	glutMainLoop();
	return 0;
}

void ReshapeFunction(int w, int h)
{
	glViewport(0,0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,w/(GLdouble)h,0.1,150);
	glRotated(mouse_x_angle,1,0,0);	
	glRotated(mouse_y_angle,0,1,0);
	glTranslated(pos_x,pos_y,pos_z);

	width_scene = w;
	height_scene = h;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void KeyboardFunction(unsigned char key,int x,int y)
{
	double new_pos;
	double Pi = 3.1415;
	switch(key){
		case 'w':
			new_pos = pos_x - sin(mouse_y_angle*2*Pi/360);
			if (new_pos>-20 && new_pos<20) pos_x = new_pos;
			new_pos = pos_z + cos(mouse_y_angle*2*Pi/360);
			if (new_pos>-20 && new_pos<20) pos_z = new_pos;
			break;
		case 's':
			new_pos = pos_x + sin(mouse_y_angle*2*Pi/360);
			if (new_pos>-20 && new_pos<20) pos_x = new_pos;
			new_pos = pos_z - cos(mouse_y_angle*2*Pi/360);
			if (new_pos>-20 && new_pos<20) pos_z = new_pos;
			break;
		case 'a':
			new_pos = pos_x + cos(mouse_y_angle*2*Pi/360);
			if (new_pos>-20 && new_pos<20) pos_x = new_pos;
			new_pos = pos_z + sin(mouse_y_angle*2*Pi/360);
			if (new_pos>-20 && new_pos<20) pos_z = new_pos;
			break;
		case 'd':
			new_pos = pos_x - cos(mouse_y_angle*2*Pi/360);
			if (new_pos>-20 && new_pos<20) pos_x = new_pos;
			new_pos = pos_z - sin(mouse_y_angle*2*Pi/360);
			if (new_pos>-20 && new_pos<20) pos_z = new_pos;
			break;
		case ' ':
			if (pos_y>-12) pos_y -= 0.5;
			break;
		case 'c':
			if (pos_y<-0.5) pos_y += 0.5;
			break;
		default:
			break;
	}
}
void IdleFunction(void)
{
	GetSystemTime(&t2);
	WORD t;
	t=t2.wMilliseconds - t1.wMilliseconds;
	if (t>10)
	{
		t1 = t2;
		ang +=0.2;
		fog += 1;
		if (fog > 100) fog =100;
		if (ang > 360) ang = 0;

		glutPostRedisplay();
	}
}
void MouseClickFunction(int button,int state,int x,int y)
{
	mouse_button = button;
	mouse_state = state;
	mouse_x = x;
	mouse_y = y;
}
void MouseMotionFunction(int x,int y) // Поворачиваем объект при движении мыши
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

void Init(void)
{
	//fog
	typedef void (APIENTRYP PFNGLFOGCOORDFPROC) (GLfloat coord);
	glFogCoordfEXT = (PFNGLFOGCOORDFEXTPROC) wglGetProcAddress("glFogCoordfEXT");
	//shadowmap
	typedef void (APIENTRYP PFNGLACTIVETEXTUREARBPROC) (GLenum texture);
	glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");

	glGenTextures(1,&shadowmap);
	glBindTexture(GL_TEXTURE_2D,shadowmap);
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

	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32_ARB,size1,size1,0,
	GL_DEPTH_COMPONENT32_ARB,GL_UNSIGNED_BYTE,NULL );
	
	glGenTextures(1,&shadowmap1);
	glBindTexture(GL_TEXTURE_2D,shadowmap1);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32_ARB,size1,size1,0,
	GL_DEPTH_COMPONENT32_ARB,GL_UNSIGNED_BYTE,NULL );

	
	//cubemap
	unsigned char *tex_bits = NULL;
	glGenTextures(1,&cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP,cubemap);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	tex_bits = LoadTrueColorBMPFile("img/cubemap/pos_x.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);
	
	tex_bits = LoadTrueColorBMPFile("img/cubemap/pos_y.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);

	tex_bits = LoadTrueColorBMPFile("img/cubemap/pos_z.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);

	tex_bits = LoadTrueColorBMPFile("img/cubemap/neg_x.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);

	tex_bits = LoadTrueColorBMPFile("img/cubemap/neg_y.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);

	tex_bits = LoadTrueColorBMPFile("img/cubemap/neg_z.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST );
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST );

	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP  );
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP  );
	glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP  );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);


	//floor texture
	glGenTextures(1,&floor_texture);
	glBindTexture(GL_TEXTURE_2D,floor_texture);
	//glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	tex_bits = LoadTrueColorBMPFile("img/floor.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_REPLACE);

	
	//trees
	tex_bits = ConstructTexture(&tex_width,&tex_height);
	glGenTextures(1,&tree);
	glBindTexture(GL_TEXTURE_2D,tree);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage2D(GL_TEXTURE_2D,
		         0,
			     4,
			     tex_width,tex_height,
			     0,
			     GL_RGBA,
			     GL_UNSIGNED_BYTE,tex_bits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	

	delete[] tex_bits;

	cube = glGenLists(1);
	glNewList(cube,GL_COMPILE);
	glBegin(GL_QUADS);
	double x = 0.5; 
	double y = 2.5; 
	double z = 0.5; 
		for(int i=0;i<10;i++)
			for (int j=0; j<10;j++)
			{
				glNormal3d(-1,0,0);
				glFogCoordfEXT(i*y*0.1);
				glVertex3f(-x, i*y*0.1,   j*z*0.2-z);
				glFogCoordfEXT((i+1)*y*0.1);
				glVertex3f(-x,(i+1)*y*0.1,j*z*0.2-z);
				glVertex3f(-x,(i+1)*y*0.1,(j+1)*z*0.2-z);
				glFogCoordfEXT(i*y*0.1);
				glVertex3f(-x, i*y*0.1,   (j+1)*z*0.2-z);
			}
		for(int i=0;i<10;i++)
			for (int j=0; j<10;j++)
			{
				glNormal3d(0,0,1);
				glFogCoordfEXT(i*y*0.1);
				glVertex3f(-x, i*y*0.1,   z);
				glFogCoordfEXT((i+1)*y*0.1);
				glVertex3f(-x,(i+1)*y*0.1,z);			
				glVertex3f(x, (i+1)*y*0.1,z);
				glFogCoordfEXT(i*y*0.1);
				glVertex3f(x,  i*y*0.1,   z);
			}
		for(int i=0;i<10;i++)
			for (int j=0; j<10;j++)
			{
				glNormal3d(0,0,-1);
				glFogCoordfEXT(i*y*0.1);
				glVertex3f(-x, i*y*0.1,   -z);
				glVertex3f(x,  i*y*0.1,   -z);
				glFogCoordfEXT((i+1)*y*0.1);
				glVertex3f(x, (i+1)*y*0.1,-z);
				glVertex3f(-x,(i+1)*y*0.1,-z);
			}
		for(int i=0;i<10;i++)
			for (int j=0; j<10;j++)
			{
				glNormal3d(1,0,0);
				glFogCoordfEXT(i*y*0.1);
				glVertex3f(x, i*y*0.1,   j*z*0.2-z);
				glVertex3f(x, i*y*0.1,   (j+1)*z*0.2-z);
				glFogCoordfEXT((i+1)*y*0.1);
				glVertex3f(x,(i+1)*y*0.1,(j+1)*z*0.2-z);
				glVertex3f(x,(i+1)*y*0.1,j*z*0.2-z);
				
				
			}

		glNormal3d(0,1,0);
		glFogCoordfEXT(y);
		glVertex3f(-x, y, -z);
		glVertex3f( x, y, -z);
		glVertex3f( x, y,  z);
		glVertex3f(-x, y,  z);

		glNormal3d(0,-1,0);
		glFogCoordfEXT(0);
		glVertex3f(-x, 0, -z);
		glVertex3f(-x, 0,  z);
		glVertex3f( x, 0,  z);
		glVertex3f( x, 0, -z);

	glEnd();
	glEndList();

	int a =4;
	for (int i=0;i<maxparticles;i++)
	{
		particles[i].t = i;
		particles[i].flag = i%4;
		particles[i].x = -6;
		particles[i].y = -3;
	}

	//billboards
	trees[0].x = -4;  trees[1].x =-8; trees[2].x =-6; trees[3].x =-4;  trees[4].x =-8;
	trees[0].y =  -1;  trees[1].y = -1; trees[2].y = 2; trees[3].y = 5;  trees[4].y = 5;

	//ShadowMatrix (simplest shadows)
	plane[0] = 0; plane[1] = 1; plane [2] = 0; plane[3] = 0;
	ShadowMatrix(sm,plane,lpos);
}
void DisplayFunction(void)
{
	//сразу сделаем карту теней
	RenderShadows();
	//и установим нужные параметры окошка, и матриц преобразования
	ReshapeFunction(width_scene,height_scene);

	//начальные настройки
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	//установим освещение
	GLfloat scene_ambient[] = {0.15f,0.15f,0.15f,1.0f};
	//фоновое освещение всей сцены
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,scene_ambient);
	// локальная точка обзора
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	//Ну и другие параметры освещения
	GLfloat light_color[] = {1.0f,1.0f,1.0f,1.0f};
	glLightfv(GL_LIGHT0,GL_AMBIENT_AND_DIFFUSE, light_color);
	glLightfv(GL_LIGHT0,GL_SPECULAR, light_color);
	glLightfv(GL_LIGHT0,GL_POSITION,lpos);
	glShadeModel(GL_SMOOTH);
	//включим освещение
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	

	//начнем строить отражение (потом плоскость, а потом "нормальную" сцену)
	//пометим пикселы отражающей поверхности
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_ZERO,GL_ZERO,GL_REPLACE);
	glStencilFunc(GL_ALWAYS,1,1);
	//отключим буферы кадра и глубины
	glDepthMask(GL_FALSE);
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	
	//вот так помечаем
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glVertex3f(-10,0,-6);
	glVertex3f(10,0,-6);
	glVertex3f(10,0,6);
	glVertex3f(-10,0,6);
	glEnd();

	//возвращаем настройки обратно для нормального рисования
	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	glStencilFunc(GL_EQUAL,1,1);
	
	//и добавим плоскость отсечения Ay=0. Будем рисовать все, что под ней.
	GLdouble eqn[4] = {0,-1,0,0};
	glClipPlane(GL_CLIP_PLANE0,eqn);
	glEnable(GL_CLIP_PLANE0);

	//отразим пространство.
	glPushMatrix();	
	glTranslated(0,0,0); // =)
	glScaled(1,-1,1);
	//установим отраженный источник освещения
	glLightfv(GL_LIGHT0,GL_POSITION,lpos);
	//включим для текстурирования первый слой (на нем карта теней)
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D,shadowmap);
	GLfloat p1[4] = {1,0,0,0};
	glTexGenfv(GL_S,GL_EYE_PLANE,p1);
	//рисуем сцену
	rf = 1; 
	PaintAll();
	rf = 0;
	glPopMatrix();

	//восстанавливаем настройки
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_CLIP_PLANE0);
	
	
	//немного изменим фоновое освещение всей сцены
	//scene_ambient[0] = 0;
	//scene_ambient[1] = 0;
	//scene_ambient[2] = 0;
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,scene_ambient);
	//поставим на место источник света
	glLightfv(GL_LIGHT0,GL_POSITION,lpos);

	//настройки используемого далее материала
	GLfloat amb[4] = {1,1,1,0.8};
	GLfloat spec[4] = {0.2,0.2,0.2,0.2};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,100);
	
	//включим нулевой слой
	glActiveTextureARB(GL_TEXTURE0_ARB); 
	glBindTexture(GL_TEXTURE_2D,floor_texture);
	glEnable(GL_TEXTURE_2D);
	
	//и настроим параметры смешивания
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	
	//рисуем саму плоскость пола
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glTexCoord2d(-5,-3);
	glVertex3f(-10,0,-6);
	glTexCoord2d(5,-3);
	glVertex3f(10,0,-6);
	glTexCoord2d(5,3);
	glVertex3f(10,0,6);
	glTexCoord2d(-5,3);
	glVertex3f(-10,0,6);
	glEnd();

	//и восстанавливаем настройки
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

		

	//теперь нарисуем тени на пол от объектов,
	//которые не попали в теневую карту.
	//замечание: просто для порядка.
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	//в буфере трафарета плоскость уже помечена (при рисовании отражения)
	//так что просто настроим его должным образом
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
	glStencilFunc(GL_EQUAL,1,1);
	
	//это цвет тени
	glColor4d(0,0,0,0.6);

	//рисуем непосредственно сами объекты
	glPushMatrix();
	//умножаем их на специальную вырожденную матрицу
	glMultMatrixf((GLfloat *) sm);

	glPushMatrix();
	glTranslated(3,0.5,-3);
	glRotated(ang,0,1,0);
	glCallList(cube);
	glPopMatrix();
	glPushMatrix();
	glTranslated(3,0.5,0);
	glRotated(ang,0,1,0);
	glCallList(cube);
	glPopMatrix();
	glPushMatrix();
	glTranslated(3,0.5,3);
	glRotated(ang,0,1,0);
	glCallList(cube);
	glPopMatrix();
	glPopMatrix();

	//восстанавливаем все настройки
	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);		
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glColor4d(1,1,1,1);


	//тут рисуем сцену в нормальном режиме
	PaintAll();


	glFlush();
	glutSwapBuffers();
}
void RenderShadows(void)
{

	//буфер кадра не нужен
    glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
                                       
	//ставим камеру в позицию источника
    glViewport(0,0,size1,size1);
    glClear(GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40,1,2,25);
    gluLookAt(lpos[0],lpos[1],lpos[2], 0,0,0, 0,0,1 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //сохраним матрицы преобразования                  
    glGetFloatv(GL_MODELVIEW_MATRIX, mv);
    glGetFloatv(GL_PROJECTION_MATRIX,pr);

	//без смещения будет много артефактов
	glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(4,4);
	
	//рисуем сами объекты
	glPushMatrix();
	glTranslated(0,1.6,0.2);
	glRotated(ang,1,0,0);
	glRotated(ang+30,0,1,0);
	glutSolidTorus(0.4,1,100,100);
	glPopMatrix();

	double r = 0.25;
	glPushMatrix();
	glRotated(4*ang,0,1,0);
	glTranslated(1.2,4,0);
	glutSolidSphere(r,30,30);
	glPopMatrix();
	glPushMatrix();
	glRotated(4*ang+120,0,1,0);
	glTranslated(1.2,4,0);
	glutSolidSphere(r,30,30);
	glPopMatrix();
	glPushMatrix();
	glRotated(4*ang+240,0,1,0);
	glTranslated(1.2,4,0);
	glutSolidSphere(r,30,30);
	glPopMatrix();

	//сохраняем значения глубины в текстуру.
	//потом они нам понадобятся.
    glBindTexture(GL_TEXTURE_2D, shadowmap);
    glCopyTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32_ARB ,0,0,size1,size1,0);
	
	//восстановим параметры
	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_POLYGON_OFFSET_FILL);
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

	//и настроим теневую карту
	//она будет на первом текстурном слое.
	//на нулевом будут кубическая и обычные текстуры.
	glActiveTextureARB(GL_TEXTURE1_ARB);

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

	//Установка текстурных координат
	glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	//в буфере были значения в диапозоне [-1,1]*[-1,1]
	//а в текстуре [0,1]*[0,1], так что промасштабируем.
    glTranslatef(0.5,0.5,0.5);
	glScalef(0.5,0.5,0.5);
    //и домножим все на сохраненные ранее матрицы
	glMultMatrixf(pr);
    glMultMatrixf(mv); 
	glMatrixMode(GL_MODELVIEW );
}
void PaintAll(void)
{
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D); //на 0 слое
	glDisable(GL_LIGHTING);
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);//на 1 слое

	//активируем кубическую текстуру
	glActiveTextureARB(GL_TEXTURE0_ARB); 
	glBindTexture(GL_TEXTURE_CUBE_MAP,cubemap);
	glTexEnvf(GL_TEXTURE_ENV_COLOR,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glEnable(GL_TEXTURE_CUBE_MAP); //на 0 слое
	
	//и нарисуем боольшой куб
	glPushMatrix();
	double a = 0.9; 
	double b = 1;
	if (rf==1) b = 0.5;
	glTranslated(-pos_x*a,-pos_y*a,-pos_z*a);
	glBegin(GL_QUADS);
	int idx;
	int size=100;
	for(int i = 0;i<24;i++) 
	{
		idx = cubeVertices[i];
		glTexCoord3d(-v[idx].x,-v[idx].y,-v[idx].z);
		glVertex3d(b*size*v[idx].x,b*size*v[idx].y,b*size*v[idx].z);
	}
	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING); 
	glEnable(GL_DEPTH_TEST); 
	
	//у куба задавали координаты текстур вручную, теперь включим автоматическое задание (для тора)
	glTexEnvf(GL_TEXTURE_ENV_COLOR,GL_TEXTURE_ENV_MODE,GL_DECAL);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);		

	//активируем теневую карту на торе
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D,shadowmap);
	glEnable(GL_TEXTURE_2D);//на 1 слое
	
	
	//материал тора
	GLfloat amb[4] = {0.4,0.4,0.4,1.0};
	GLfloat dif[4] = {1.0,1.0,1.0,1.0};
	GLfloat spec[4] = {0.2,0.2,0.2,0.2};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,50);

	//рисуем
	glPushMatrix();
	glTranslated(0,1.6,0.2);
	glRotated(ang,1,0,0);
	glRotated(ang+30,0,1,0);
	glutSolidTorus(0.4,1,100,100);
	glPopMatrix();

	//а это три летающих шарика над тором
	double r = 0.25;
	glPushMatrix();
	glRotated(4*ang,0,1,0);
	glTranslated(1.2,4,0);
	glutSolidSphere(r,30,30);
	glPopMatrix();
	glPushMatrix();
	glRotated(4*ang+120,0,1,0);
	glTranslated(1.2,4,0);
	glutSolidSphere(r,30,30);
	glPopMatrix();
	glPushMatrix();
	glRotated(4*ang+240,0,1,0);
	glTranslated(1.2,4,0);
	glutSolidSphere(r,30,30);
	glPopMatrix();

	//отключим кубмэп и авто генерацию его текстурных координат
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_CUBE_MAP);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
	

	//настройки тумана
	GLfloat fc[4] = {1.0,1.0,1.0,1.0};
	glFogfv(GL_FOG_COLOR,fc);
	glFogi(GL_FOG_MODE,GL_LINEAR);
	glFogf(GL_FOG_START,2.5*fog/100); //при > этой величины - тумана нет 
	glFogf(GL_FOG_END,0); // при < этой величины - ничего не видно
	glFogf(GL_FOG_COORDINATE_SOURCE_EXT,GL_FOG_COORDINATE_EXT);
	glEnable(GL_FOG);
	
	
	//теперь нарисуем другие не менее важные в сцене объекты
	amb[0] = 0.0; amb[1] = 0.75; amb[2] = 1.0;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,130);
	//синий
	glPushMatrix();
	glTranslated(3,0.5,-3);
	glRotated(ang,0,1,0);
	glCallList(cube);
	glPopMatrix();
	//желтый
	amb[0] = 0.93; amb[1] = 0.93; amb[2] = 0.0;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, amb);
	glPushMatrix();
	glTranslated(3,0.5,0);
	glRotated(ang,0,1,0);
	glCallList(cube);
	glPopMatrix();
	//зеленый
	amb[0] = 0.56; amb[1] = 0.93; amb[2] = 0.56;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, amb);
	glPushMatrix();
	glTranslated(3,0.5,3);
	glRotated(ang,0,1,0);
	glCallList(cube);
	glPopMatrix();
	glDisable(GL_FOG);
	
	
	//подправим фоновое глобальное освещение
	GLfloat scene_ambient[] = {0.3f,0.3f,0.3f,1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,scene_ambient);

	//и займемся системой частиц
	amb[0]=0; amb[1] = 0.6; amb[2]=0;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, amb);
	ViewParticle();
	
	//а потом деревьями-биллбордами
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D,shadowmap);
	glEnable(GL_TEXTURE_2D);
	SortAndView();
}
void ViewParticle(void)
{
	int livetime = 225;
	double r;
	for(int i=0;i<maxparticles;i++)
	{
		if (particles[i].t>livetime) particles[i].t = 0;
		else
		{
			if (particles[i].t <100) r = 0.02 + particles[i].t*0.002;
			else r = 0.35 - particles[i].t*0.0015;
			double h = particles[i].t*0.02;
			double Pi = 3.14;
			double t = particles[i].t *2*Pi/(100);
			double live1 = livetime *2*Pi/(100);

			glPushMatrix();
			if (particles[i].flag==0)glTranslated(particles[i].x+((live1-t)/(live1))*sin(t),h,
												  particles[i].y+((live1-t)/(live1))*cos(t));
			if (particles[i].flag==1)glTranslated(particles[i].x+((live1-t)/(live1))*sin(t+Pi/2),h,
												  particles[i].y+((live1-t)/(live1))*cos(t+Pi/2));
			if (particles[i].flag==2)glTranslated(particles[i].x+((live1-t)/(live1))*sin(t+Pi),h,
												  particles[i].y+((live1-t)/(live1))*cos(t+Pi));
			if (particles[i].flag==3)glTranslated(particles[i].x+((live1-t)/(live1))*sin(t+Pi*1.5),h,
												  particles[i].y+((live1-t)/(live1))*cos(t+Pi*1.5));
			glutSolidSphere(r,20,20);
			glPopMatrix();
			particles[i].t ++;
		}
	}
}
void SortAndView(void)
{
	//сортировка пузырьком
	for(int i=0;i<5;i++)
	{
		trees[i].r = (pos_x+trees[i].x)*(pos_x+trees[i].x) +
					 (pos_z+trees[i].y)*(pos_z+trees[i].y);
	}

	double x;
	double y;
	double r;
	for(int i=0; i < 5; i++) 
	{ 
		for(int j = 0; j < i; j++ ) 
		{
			if ( trees[j].r < trees[j+1].r ) 
			{
				x = trees[j].x;
				y = trees[j].y;
				r = trees[j].r;

				trees[j].x = trees[j+1].x;
				trees[j].y = trees[j+1].y;
				trees[j].r = trees[j+1].r;


				trees[j+1].x = x;
				trees[j+1].y = y;
				trees[j+1].r = r;
				
			}
		}
	}


	//выводим на экран в порядке back-to-front
	double treesize = 1.5;
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D,0);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D,tree);
	glTexEnvf(GL_TEXTURE_ENV_COLOR,GL_TEXTURE_ENV_MODE,GL_BLEND);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);


	for (int i=0;i<5;i++)
	{
		glPushMatrix();
		glTranslated(trees[i].x,0,trees[i].y);
		glRotated(-mouse_y_angle,0,1,0);
		glBegin(GL_QUADS);
			glTexCoord2d(0,0);
			glVertex3d(treesize,0,0);
			glTexCoord2d(0,1);
			glVertex3d(treesize,2*treesize,0);
			glTexCoord2d(1,1);
			glVertex3d(-treesize,2*treesize,0);
			glTexCoord2f(1,0);
			glVertex3d(-treesize,0,0);
		glEnd();
		glPopMatrix();
	}

	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D,shadowmap);
}
void ShadowMatrix(GLfloat matrix[4][4], GLfloat plane[4],GLfloat lightpos[4])
{
  GLfloat dot;

    dot = plane[0]     *   lightpos[0] +
          plane[1]     *   lightpos[1] +
          plane[2]     *   lightpos[2] +
          plane[3]     *   lightpos[3];

    matrix[0][0]   =   dot   -   lightpos[0]   *   plane[0];
    matrix[1][0]   =   0.f   -   lightpos[0]   *   plane[1];
    matrix[2][0]   =   0.f   -   lightpos[0]   *   plane[2];
    matrix[3][0]   =   0.f   -   lightpos[0]   *   plane[3];

    matrix[0][1]   =   0.f   -   lightpos[1]   *   plane[0];
    matrix[1][1]   =   dot   -   lightpos[1]   *   plane[1];
    matrix[2][1]   =   0.f   -   lightpos[1]   *   plane[2];
    matrix[3][1]   =   0.f   -   lightpos[1]   *   plane[3];

    matrix[0][2]   =   0.f   -   lightpos[2]   *   plane[0];
    matrix[1][2]   =   0.f   -   lightpos[2]   *   plane[1];
    matrix[2][2]   =   dot   -   lightpos[2]   *   plane[2];
    matrix[3][2]   =   0.f   -   lightpos[2]   *   plane[3];

    matrix[0][3]   =   0.f   -   lightpos[3]   *   plane[0];
    matrix[1][3]   =   0.f   -   lightpos[3]   *   plane[1];
    matrix[2][3]   =   0.f   -   lightpos[3]   *   plane[2];
    matrix[3][3]   =   dot   -   lightpos[3]   *   plane[3];
}