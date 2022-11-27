#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <glut/glut.h>
#include <glut/glext.h>
#include "formats.h"
#include "func.h"

unsigned char *LoadTrueColorBMPFile(const char *path,int *width,int *height)
{	/*взято из примеров. спасибо =)*/
	unsigned char *result = NULL;
    FILE *in = fopen(path,"rb");
	if(!in)
		return NULL;
    BMPHeader hdr;
    fread(&hdr,sizeof(hdr),1,in);
    if(hdr.type != 0x4D42)           // Not a bitmap file at all
		return NULL;
	BMPInfoHeader	infoHdr;
	fread(&infoHdr,sizeof(infoHdr),1,in);
	if(infoHdr.bitsPerPixel != 24)   // Not a truecolor bitmap
		return NULL;
	if(infoHdr.compression)          // Compressed bitmap
		return NULL;
	if((result = new unsigned char[infoHdr.width*infoHdr.height*3]) == NULL)
		return NULL;
    fseek(in,hdr.offset-sizeof(hdr)-sizeof(infoHdr),SEEK_CUR);   
	unsigned char *dst = result;
	for(int y=0;y<infoHdr.height;y++) {
		for(int x=0;x<infoHdr.width;x++) {
			dst[2] = fgetc(in);
			dst[1] = fgetc(in);
			dst[0] = fgetc(in);
			dst += 3;
		}
        for(int x=0;x<((4-(infoHdr.width&3))&3);x++)  // Skip alignment bytes
           fgetc(in);
    }
    fclose(in);
	*width = infoHdr.width;
	*height = infoHdr.height;
	return result;
}

void reshape(int w, int h)
{
	/* преобразование окна тут*/
	glViewport(0,0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,w/(GLdouble)h,0.1,100);
}
void display(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(Cam.psy,1,0,0);
	glRotatef(Cam.phi,0,1,0);
	gluLookAt(Cam.pos[0],Cam.pos[1],Cam.pos[2], Cam.pos[0]+cos(Cam.phi),Cam.pos[1],Cam.pos[2]+sin(Cam.phi), 0,1,0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	//Помечаем в буфере трафарета пикселы отражающей поверхности
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_ZERO,GL_ZERO,GL_REPLACE);
	glStencilFunc(GL_ALWAYS,1,1);
	glDepthMask(GL_FALSE);
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	RenderFloor();
	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	// Рисуем отраженные объекты (только в отражающей поверхности)
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	glStencilFunc(GL_EQUAL,1,1);
	glPushMatrix();
	glScaled(1,-1,1);
			//опции для тумана
			GLfloat fog_color[4] = {0.5,0.5,0.5,0.7};
			glFogfv(GL_FOG_COLOR,fog_color);
			glFogi(GL_FOG_MODE,GL_LINEAR);
			glFogf(GL_FOG_START,0.5); //при f<0.5 затуманивание не происходит
			glFogf(GL_FOG_END,4); // при f>4 полное затуманивание
			glFogf(GL_FOG_COORDINATE_SOURCE_EXT,GL_FOG_COORDINATE_EXT);
	
	//if (!transform)
		glEnable(GL_FOG);//в зеркальной поверхности - всегда туман т.к. круто получается.
	RenderObjects(0);
	//if (!transform)
		glDisable(GL_FOG);

	glPopMatrix();
	glDisable(GL_STENCIL_TEST);
	// Рисуем  пол

	if (transform) glEnable(GL_FOG);
	RenderFloor();
	RenderObjects(1);
	if (transform) glDisable(GL_FOG);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

    glFlush();
	glutSwapBuffers();
}
void idle(void)
{
	/*анимация тут*/
	
	GetSystemTime(&curtime);
	WORD t;
	t=curtime.wMilliseconds - time1.wMilliseconds;
	if (t>10)
	{
		time1 = curtime;
		ang1+=0.5;
		if(ang1>180) ang1=-180;
		ang2+=1.0;
		if(ang2>=360) ang2=0;
		display();
	}
}
void keyboard(unsigned char key, int x, int y)
{
	double newposition;
	/*работа с клавиатурой*/
	switch (key) {
	case 'w':
		newposition = Cam.pos[0] + cos(Cam.phi) * v;
		if (newposition > 0.5 && newposition < 19.5) Cam.pos[0] = newposition;
		newposition = Cam.pos[2] + sin(Cam.phi) * v;
		if (newposition > 0.5 && newposition < 19.5) Cam.pos[2] = newposition;
		break;
	case 's':
		newposition = Cam.pos[0] - cos(Cam.phi) * v;
		if (newposition > 0.5 && newposition < 19.5) Cam.pos[0] = newposition;
		newposition = Cam.pos[2] - sin(Cam.phi) * v;
		if (newposition > 0.5 && newposition < 19.5) Cam.pos[2] = newposition;
		break;
	case 'a':
		Cam.phi -= Pi * 1 / 24;
		if (Cam.phi < -2 * Pi * 23 / 24) Cam.phi = 0;
		break;
	case 'd':
		Cam.phi += Pi * 1 / 24;
		if (Cam.phi > 2 * Pi * 23 / 24) Cam.phi = 0;
		break;
	case 'x':
		if (Cam.psy > -90) Cam.psy -= 15.0;
		break;
	case 'z':
		if (Cam.psy < 90) Cam.psy += 15.0;
		break;
	case 'c':
		if (Cam.pos[1] > 0.5) Cam.pos[1] -= 0.25;
		break;
	case ' ':
		if (Cam.pos[1] < 8.5) Cam.pos[1] += 0.25;
		break;
	case 'f':
		if (f) f = 0;
		else f = 1;
		break;
	case 't':
		if (!transform) transform = 1;
		else transform = 0;
		break;
		/*
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
		*/
	default:
		break;
	}
}

void init_light(void)
{
	//задание модели освещения
	GLfloat scene_ambient[] = {0.15f,0.15f,0.15f,1.0f};
	glShadeModel(GL_SMOOTH);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,scene_ambient); //фоновое освещение всей сцены
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE); // локальная точка обзора
}
bool init_tex()
{
	// Создаем массив с цветами пикселов
	if((tex_bits = LoadTrueColorBMPFile("pics/floor.bmp",&tex_width,&tex_height)) == NULL) 
		return false;
	// Создаем текстурный объект
	glGenTextures(1,&texture[0]);
	// Активизируем текстурный  объект
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	// Загружаем текстуру
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage2D(GL_TEXTURE_2D,
		         0,                     // Mip-level
			     GL_RGB,                // Формат текстуры
			     tex_width,tex_height,  // Размер текстуры
			     0,                     // Ширина границы
			     GL_RGB,                // Формат исходных данных
			     GL_UNSIGNED_BYTE,      // Тип исходных данных
				 tex_bits);             // Указатель на исходные данные 
	// Освобождаем память, в которой хранилась текстура
	delete[] tex_bits;
	// Устанавливаем параметры текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	if((tex_bits = LoadTrueColorBMPFile("pics/man.bmp",&tex_width,&tex_height)) == NULL) 
		return false;
	glGenTextures(1,&texture[1]);
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage2D(GL_TEXTURE_2D,
		         0,                     // Mip-level
			     GL_RGB,                // Формат текстуры
			     tex_width,tex_height,  // Размер текстуры
			     0,                     // Ширина границы
			     GL_RGB,                // Формат исходных данных
			     GL_UNSIGNED_BYTE,      // Тип исходных данных
				 tex_bits);             // Указатель на исходные данные 
	delete[] tex_bits;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	if((tex_bits = LoadTrueColorBMPFile("pics/water.bmp",&tex_width,&tex_height)) == NULL) 
		return false;
	glGenTextures(1,&texture[2]);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage2D(GL_TEXTURE_2D,
		         0,                     // Mip-level
			     GL_RGB,                // Формат текстуры
			     tex_width,tex_height,  // Размер текстуры
			     0,                     // Ширина границы
			     GL_RGB,                // Формат исходных данных
			     GL_UNSIGNED_BYTE,      // Тип исходных данных
				 tex_bits);             // Указатель на исходные данные 
	delete[] tex_bits;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if((tex_bits = LoadTrueColorBMPFile("pics/man2.bmp",&tex_width,&tex_height)) == NULL) 
		return false;
	glGenTextures(1,&texture[3]);
	glBindTexture(GL_TEXTURE_2D,texture[3]);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage2D(GL_TEXTURE_2D,
		         0,                     // Mip-level
			     GL_RGB,                // Формат текстуры
			     tex_width,tex_height,  // Размер текстуры
			     0,                     // Ширина границы
			     GL_RGB,                // Формат исходных данных
			     GL_UNSIGNED_BYTE,      // Тип исходных данных
				 tex_bits);             // Указатель на исходные данные 
	delete[] tex_bits;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	/*******************/

	glGenTextures(1,&cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP,cubemap);

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	tex_bits = LoadTrueColorBMPFile("pics/skybox/Right.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);

	tex_bits = LoadTrueColorBMPFile("pics/skybox/Left.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);

	tex_bits = LoadTrueColorBMPFile("pics/skybox/Front.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);

	tex_bits = LoadTrueColorBMPFile("pics/skybox/Back.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);
	
	tex_bits = LoadTrueColorBMPFile("pics/skybox/Down.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);

	tex_bits = LoadTrueColorBMPFile("pics/skybox/Up.bmp",&tex_width,&tex_height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,0,GL_RGB,tex_width,tex_height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_bits);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
	glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);

	delete[] tex_bits;

	// Включаем перспективное текстурирование
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	return true;
}
void init_lists(void)
{
	theWalls = glGenLists(1);
	glNewList(theWalls,GL_COMPILE);
	glBegin(GL_QUADS);
	for(int i=0;i<9;i++){
		for(int j=0;j<20;j++){
			glFogCoordfEXT(i);
			glNormal3f(0,0,1);
			glVertex3f(j,i,0);
			glFogCoordfEXT(i+1);
			glNormal3f(0,0,1);
			glVertex3f(j,i+1,0);
			glFogCoordfEXT(i+1);
			glNormal3f(0,0,1);
			glVertex3f(j+1,i+1,0);
			glFogCoordfEXT(i);
			glNormal3f(0,0,1);
			glVertex3f(j+1,i,0);
		}
	}
	for(int i=0;i<9;i++){
		for(int j=0;j<20;j++){
			glFogCoordfEXT(i);
			glNormal3f(0,0,-1);
			glVertex3f(j,i,20);
			glFogCoordfEXT(i+1);
			glNormal3f(0,0,-1);
			glVertex3f(j,i+1,20);
			glFogCoordfEXT(i+1);
			glNormal3f(0,0,-1);
			glVertex3f(j+1,i+1,20);
			glFogCoordfEXT(i);
			glNormal3f(0,0,-1);
			glVertex3f(j+1,i,20);
		}
	}
	for(int i=0;i<9;i++){
		for(int j=0;j<20;j++){
			glFogCoordfEXT(i);
			glNormal3f(1,0,0);
			glVertex3f(0,i,j);
			glFogCoordfEXT(i+1);
			glNormal3f(1,0,0);
			glVertex3f(0,i+1,j);
			glFogCoordfEXT(i+1);
			glNormal3f(1,0,0);
			glVertex3f(0,i+1,j+1);
			glFogCoordfEXT(i);
			glNormal3f(1,0,0);
			glVertex3f(0,i,j+1);
		}
	}
	for(int i=0;i<9;i++){
		for(int j=0;j<20;j++){
			glFogCoordfEXT(i);
			glNormal3f(-1,0,0);
			glVertex3f(20,i,j);
			glFogCoordfEXT(i+1);
			glNormal3f(-1,0,0);
			glVertex3f(20,i+1,j);
			glFogCoordfEXT(i+1);
			glNormal3f(-1,0,0);
			glVertex3f(20,i+1,j+1);
			glFogCoordfEXT(i);
			glNormal3f(-1,0,0);
			glVertex3f(20,i,j+1);
		}
	}
	//потолок - тоже стена
	for(int i=0;i<20;i++){
		for(int j=0;j<20;j++){
			glFogCoordfEXT(9);
			glNormal3f(0,-1,0);
			glVertex3f(i,9,j);
			glFogCoordfEXT(9);
			glNormal3f(0,-1,0);
			glVertex3f(i+1,9,j);
			glFogCoordfEXT(9);
			glNormal3f(0,-1,0);
			glVertex3f(i+1,9,j+1);	
			glFogCoordfEXT(9);
			glNormal3f(0,-1,0);
			glVertex3f(i,9,j+1);
		}
	}
	glEnd();
	glEndList();

	//список для аквариума
	theAqua = glGenLists(1);
	glNewList(theAqua,GL_COMPILE);	

	glFogCoordfEXT(0.7);
	glPushMatrix();
	glTranslatef(17,1.25,17);
	glScaled(3,0.25,3);
	glutSolidCube(2);
	glPopMatrix();

	glFogCoordfEXT(0.3);
	glPushMatrix();
	glTranslatef(14.25,0.5,19.75);
	glScaled(1,2,1);
	glutSolidCube(0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(19.75,0.5,19.75);
	glScaled(1,2,1);
	glutSolidCube(0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(14.25,0.5,14.25);
	glScaled(1,2,1);
	glutSolidCube(0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(19.75,0.5,14.25);
	glScaled(1,2,1);
	glutSolidCube(0.5);
	glPopMatrix();

	glBegin(GL_QUADS);
		glFogCoordfEXT(2.2);
		glNormal3f(0,0,-1);
		glVertex3f(20,2.2,14);
		glFogCoordfEXT(2.5);
		glNormal3f(0,0,-1);
		glVertex3f(20,2.5,14);
		glFogCoordfEXT(2.5);
		glNormal3f(0,0,-1);
		glVertex3f(14,2.5,14);
		glFogCoordfEXT(2.2);
		glNormal3f(0,0,-1);
		glVertex3f(14,2.2,14);

		glFogCoordfEXT(2.2);
		glNormal3f(0,0,-1);
		glVertex3f(20,2.2,19.99);
		glFogCoordfEXT(2.5);
		glNormal3f(0,0,-1);
		glVertex3f(20,2.5,19.99);
		glFogCoordfEXT(2.5);
		glNormal3f(0,0,-1);
		glVertex3f(14,2.5,19.99);
		glFogCoordfEXT(2.2);
		glNormal3f(0,0,-1);
		glVertex3f(14,2.2,19.99);

		glFogCoordfEXT(2.2);
		glNormal3f(-1,0,0);
		glVertex3f(14,2.2,14);
		glFogCoordfEXT(2.5);
		glNormal3f(-1,0,0);
		glVertex3f(14,2.5,14);
		glFogCoordfEXT(2.5);
		glNormal3f(-1,0,0);
		glVertex3f(14,2.5,20);
		glFogCoordfEXT(2.2);
		glNormal3f(-1,0,0);
		glVertex3f(14,2.2,20);

		glFogCoordfEXT(2.2);
		glNormal3f(-1,0,0);
		glVertex3f(19.99,2.2,14);
		glFogCoordfEXT(2.5);
		glNormal3f(-1,0,0);
		glVertex3f(19.99,2.5,14);
		glFogCoordfEXT(2.5);
		glNormal3f(-1,0,0);
		glVertex3f(19.99,2.5,20);
		glFogCoordfEXT(2.2);
		glNormal3f(-1,0,0);
		glVertex3f(19.99,2.2,20);
	glEnd();
	glEndList();

	//сложная поверхность
	theBottle = glGenLists(1);
	glNewList(theBottle,GL_COMPILE);
	int A=50;
	int B=50;
	int r=2.65;
	for (int i=0;i<=A;i++)
	{
		glBegin(GL_QUAD_STRIP);
		for(int j=0;j<=B;j++)
		{
			double V = j*2*Pi/B;
			double U = i*2*Pi/A;
			//считаем нормаль
			double xu = (-sin(U/2)*sin(V)/2 - cos(U/2)*sin(2*V)/2)*cos(U) - (r+ cos(U/2)*sin(V) - sin(U/2)*sin(2*V) )*sin(U); 
			double xv = (cos(U/2)*cos(V) - 2*sin(U/2)*cos(2*V))*cos(U);
			double yu = (-sin(U/2)*sin(V)/2 - cos(U/2)*sin(2*V)/2)*sin(U) + (r+ cos(U/2)*sin(V) - sin(U/2)*sin(2*V) )*cos(U); 
			double yv = (cos(U/2)*cos(V) - 2*sin(U/2)*cos(2*V))*sin(U);
			double zu = (cos(U/2)*sin(V) - sin(U/2)*sin(2*V))/2;
			double zv = sin(U/2)*cos(V) +2*cos(U/2)*cos(2*V);
			double nx = yu*zv - yv*zu;
			double ny = zu*xv - zv*xu;
			double nz = xu*yv - xv*yu;
			double norm = sqrt( nx*nx +ny*ny + nz*nz );
			if (V>0 && V<Pi)glNormal3f(-nx/norm,-ny/norm,-nz/norm);
			else glNormal3f(nx/norm,ny/norm,nz/norm);
			if (V>0 && V<Pi)
			{
				Nb[i][j][0]=-nx/norm;
				Nb[i][j][1]=-ny/norm;
				Nb[i][j][2]=-nz/norm;
			}
			else 
			{
				Nb[i][j][0]=nx/norm;
				Nb[i][j][1]=ny/norm;
				Nb[i][j][2]=nz/norm;
			}

			//считаем вершину
			double x = (r+cos(U/2)*sin(V) - sin(U/2)*sin(2*V))*cos(U);
			double y = (r+cos(U/2)*sin(V) - sin(U/2)*sin(2*V))*sin(U);
			double z = sin(U/2)*sin(V) + cos(U/2)*sin(2*V);

			b[i][j][0]=x;
			b[i][j][1]=y;
			b[i][j][2]=z;

			glVertex3f(x,y,z);
			
			U = (i+1)*2*Pi/A;
			//считаем нормаль
			xu = (-sin(U/2)*sin(V)/2 - cos(U/2)*sin(2*V)/2)*cos(U) - (r+ cos(U/2)*sin(V) - sin(U/2)*sin(2*V) )*sin(U); 
			xv = (cos(U/2)*cos(V) - 2*sin(U/2)*cos(2*V))*cos(U);
			yu = (-sin(U/2)*sin(V)/2 - cos(U/2)*sin(2*V)/2)*sin(U) + (r+ cos(U/2)*sin(V) - sin(U/2)*sin(2*V) )*cos(U); 
			yv = (cos(U/2)*cos(V) - 2*sin(U/2)*cos(2*V))*sin(U);
			zu = (cos(U/2)*sin(V) - sin(U/2)*sin(2*V))/2;
			zv = sin(U/2)*cos(V) +2*cos(U/2)*cos(2*V);
			nx = yu*zv - yv*zu;
			ny = zu*xv - zv*xu;
			nz = xu*yv - xv*yu;
			norm = sqrt( nx*nx +ny*ny + nz*nz );
			if (V>0 && V<Pi) glNormal3f(-nx/norm,-ny/norm,-nz/norm);
			else glNormal3f(nx/norm,ny/norm,nz/norm);
			
			//считаем вершину
			x = (r+cos(U/2)*sin(V) - sin(U/2)*sin(2*V))*cos(U);
			y = (r+cos(U/2)*sin(V) - sin(U/2)*sin(2*V))*sin(U);
			z = sin(U/2)*sin(V) + cos(U/2)*sin(2*V);
			glVertex3f(x,y,z);
			
		}	
		glEnd();
	}
	glEndList();

	double R=0.15; //размер шариков
	A=15; //качество шариков
	double r1=0.4; //радиус до центра
	double P = 0; //высотa
	double h = 1; //коэф. ухода вниз
	double angle = 30; //угол вращения
	double v=-1; //скорость вращения
	B=30;
	
	theBox = glGenLists(1);
	glNewList(theBox,GL_COMPILE);
	glFogCoordfEXT(9.0);
	glutSolidSphere(R,A,A);
	for (int i=0;i<=B;i++)
	{
		glPushMatrix();
		glRotated(angle,0,1,0);
		glTranslated(r1,P,0);
		glFogCoordfEXT(6-(GLfloat)5.0/B*i);
		glutSolidSphere(R,A,A);
		glPopMatrix();
		glPushMatrix();
		glRotated(angle,0,1,0);
		glTranslated(-r1,P,0);
		glutSolidSphere(R,A,A);
		glPopMatrix();
		if (!(i%5)&& (i!=0))
		{ 
			glPushMatrix();
			glTranslated(0,P,0);
			if (i<B) glutSolidSphere(R,A,A);
			else
			{
				R+=R;
				glutSolidSphere(R,A,A);
			}
			glPopMatrix();
		}
		P-=h*R;
		angle+=30;
	}
	glEndList();
}

int main(int argc, char *argv[])
{	
	glutInit(&argc, argv);
	glutInitWindowSize(scene_w,scene_h);
	glutInitWindowPosition(200, 200);

	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL );
	glutCreateWindow("OpenGL. Лёвин Константин, 302 гр.");
	
	init_extensions();
	if (!list_flag) { init_lists(); list_flag=1;}
	init_water();
	init_light();
	init_tex();
	init_ShadowMatrix();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	
	glutMainLoop();
	return 0;
}

void RenderObjects(int flag)
{	
	//рисуем объекты в 
	RenderLamp();
	RenderWalls();
	RenderPicture();
	RenderComplexSurface(flag);
	RenderSphere(flag);
	RenderAqua(flag);
}

void RenderFloor(void)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D,texture[0]);

	glTexEnvf(GL_TEXTURE_ENV_COLOR,GL_TEXTURE_ENV_MODE,GL_BLEND);
	
	if (f) glColor4d(0.4,0.4,0.35,0.8);
	else glColor4d(0.1,0.1,0.05,0.8);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glFogCoordfEXT(0);
	glTexCoord2d(0,0);
	glNormal3f(0,1,0);
	glVertex3d(0,0,0);
	glFogCoordfEXT(0);
	glTexCoord2d(20,0);
	glNormal3f(0,1,0);
	glVertex3d(20,0,0);
	glFogCoordfEXT(0);
	glTexCoord2d(20,20);
	glNormal3f(0,1,0);
	glVertex3d(20,0,20);
	glFogCoordfEXT(0);
	glTexCoord2d(0,20);
	glNormal3f(0,1,0);
	glVertex3d(0,0,20);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void RenderWalls(void)
{
//материал стен
	GLfloat w_emissive[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat w_amb_and_dif[4] = { 0.5f, 0.6f, 0.5f, 1.0f };
	GLfloat w_specular[4] = {0.1f, 0.1f, 0.1f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, w_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, w_amb_and_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, w_specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,50);
	
	//рисуем стены
	glCallList(theWalls);
}

void RenderPicture(void)
{
	GLfloat c_emissive[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat c_specular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,10);

	//картинка
	glPushMatrix();
	glTranslatef(19.9,2.5,6.5);
	glRotatef(-90,0,1,0);
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		glFogCoordfEXT(2.5);
		glNormal3f(0,0,-1);
		glTexCoord2d(0,0);
		glVertex3f(0,0,0);
		glFogCoordfEXT(7.5);
		glNormal3f(0,0,-1);
		glTexCoord2d(0,1);
		glVertex3f(0,5,0);
		glFogCoordfEXT(7.5);
		glNormal3f(0,0,-1);
		glTexCoord2d(1,1);
		glVertex3f(5,5,0);
		glFogCoordfEXT(2.5);
		glNormal3f(0,0,-1);
		glTexCoord2d(1,0);
		glVertex3f(5,0,0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	//рамка
	color[0] = 0.8; color[1] = 0; color[2] = 0; color[3] = 1.0;	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	glBegin(GL_QUADS);
		glFogCoordfEXT(2.5);
		glNormal3f(0,0,1);
		glVertex3f(0,0,0.01);
		glFogCoordfEXT(7.5);
		glVertex3f(0,5,0.01);
		glFogCoordfEXT(7.5);
		glVertex3f(0.25,5,0.01);
		glFogCoordfEXT(2.5);
		glVertex3f(0.25,0,0.01);

		glFogCoordfEXT(2.5);
		glVertex3f(4.75,0,0.01);
		glFogCoordfEXT(7.5);
		glVertex3f(4.75,5,0.01);
		glFogCoordfEXT(7.5);
		glVertex3f(5,5,0.01);
		glFogCoordfEXT(2.5);
		glVertex3f(5,0,0.01);

		glFogCoordfEXT(7.5);
		glVertex3f(0,5,0.01);
		glFogCoordfEXT(7.5);
		glVertex3f(5,5,0.01);
		glFogCoordfEXT(7.25);
		glVertex3f(5,4.75,0.01);
		glFogCoordfEXT(7.25);
		glVertex3f(0,4.75,0.01);

		glFogCoordfEXT(2.5);
		glVertex3f(0,0.25,0.01);
		glFogCoordfEXT(2.5);
		glVertex3f(5,0.25,0.01);
		glFogCoordfEXT(2.25);
		glVertex3f(5,0,0.01);
		glFogCoordfEXT(2.25);
		glVertex3f(0,0,0.01);
	glEnd();
	glPopMatrix();

	//вторая картинка
	color[0] = 1.0; color[1] = 1.0; color[2] =1.0; color[3] = 1.0;	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);

	glPushMatrix();
	glTranslatef(13,2.5,19.99);
	glRotatef(180,0,1,0);
	glBindTexture(GL_TEXTURE_2D,texture[3]);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		glFogCoordfEXT(2.5);
		glNormal3f(0,0,-1);
		glTexCoord2d(0,0);
		glVertex3f(0,0,0);
		glFogCoordfEXT(7.5);
		glNormal3f(0,0,-1);
		glTexCoord2d(0,1);
		glVertex3f(0,5,0);
		glFogCoordfEXT(7.5);
		glNormal3f(0,0,-1);
		glTexCoord2d(1,1);
		glVertex3f(5,5,0);
		glFogCoordfEXT(2.5);
		glNormal3f(0,0,-1);
		glTexCoord2d(1,0);
		glVertex3f(5,0,0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	//вторая рамка
	color[0] = 0.8; color[1] = 0; color[2] = 0; color[3] = 1.0;	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	glBegin(GL_QUADS);
		glFogCoordfEXT(2.5);
		glNormal3f(0,0,1);
		glVertex3f(0,0,0.01);
		glFogCoordfEXT(7.5);
		glVertex3f(0,5,0.01);
		glFogCoordfEXT(7.5);
		glVertex3f(0.25,5,0.01);
		glFogCoordfEXT(2.5);
		glVertex3f(0.25,0,0.01);

		glFogCoordfEXT(2.5);
		glVertex3f(4.75,0,0.01);
		glFogCoordfEXT(7.5);
		glVertex3f(4.75,5,0.01);
		glFogCoordfEXT(7.5);
		glVertex3f(5,5,0.01);
		glFogCoordfEXT(2.5);
		glVertex3f(5,0,0.01);

		glFogCoordfEXT(7.5);
		glVertex3f(0,5,0.01);
		glFogCoordfEXT(7.5);
		glVertex3f(5,5,0.01);
		glFogCoordfEXT(7.25);
		glVertex3f(5,4.75,0.01);
		glFogCoordfEXT(7.25);
		glVertex3f(0,4.75,0.01);

		glFogCoordfEXT(2.5);
		glVertex3f(0,0.25,0.01);
		glFogCoordfEXT(2.5);
		glVertex3f(5,0.25,0.01);
		glFogCoordfEXT(2.25);
		glVertex3f(5,0,0.01);
		glFogCoordfEXT(2.25);
		glVertex3f(0,0,0.01);
	glEnd();
	glPopMatrix();
}

void RenderComplexSurface(int flag)
{
	double angle = ang1;

	glEnable(GL_DEPTH_TEST);
	if (f && flag)
	{	//тут рисуем тени.
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
	
		//сначала тень на пол
		glClear(GL_STENCIL_BUFFER_BIT);
		glClearStencil(0x0);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc (GL_ALWAYS, 0x1, 0xffffffff);
		glStencilOp (GL_REPLACE, GL_REPLACE, GL_REPLACE);
		glDepthMask(GL_FALSE);
		glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
		glBegin(GL_QUADS);
			glVertex3d(0,0,0);
			glVertex3d(20,0,0);
			glVertex3d(20,0,20);
			glVertex3d(0,0,20);
		glEnd();
		glDepthMask(GL_TRUE);
		glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		glStencilFunc (GL_EQUAL, 0x1, 0xffffffff);
		glStencilOp (GL_KEEP, GL_KEEP, GL_INCR);	
	
		glColor4d(0,0,0,0.3);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		
		glPushMatrix();
		glMultMatrixf((GLfloat *) floorShadow);
		glTranslated(4.5,3.5,15.5);
		glRotatef(angle,0,1,0);
		glCallList(theBottle);
		glPopMatrix();

		//потом в стены
		glClear(GL_STENCIL_BUFFER_BIT);
		glClearStencil(0x0);
		glStencilFunc (GL_ALWAYS, 0x1, 0xffffffff);
		glStencilOp (GL_REPLACE, GL_REPLACE, GL_REPLACE);
		glDepthMask(GL_FALSE);
		glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
		glBegin(GL_QUADS);
			glVertex3d(0,0,0);
			glVertex3d(0,9,0);
			glVertex3d(0,9,20);
			glVertex3d(0,0,20);
		glEnd();
		glDepthMask(GL_TRUE);
		glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		glStencilFunc (GL_EQUAL, 0x1, 0xffffffff);
		glStencilOp (GL_KEEP, GL_KEEP, GL_INCR);	

		glDisable(GL_DEPTH_TEST);
		glPushMatrix();
		glTranslated(red,green,blue);
		glMultMatrixf((GLfloat *) right);
		glTranslated(4.5,3.5,15.5);
		glRotatef(angle,0,1,0);
		glCallList(theBottle);
		glPopMatrix();
		
		glClear(GL_STENCIL_BUFFER_BIT);
		glClearStencil(0x0);
		glStencilFunc (GL_ALWAYS, 0x1, 0xffffffff);
		glStencilOp (GL_REPLACE, GL_REPLACE, GL_REPLACE);
		glDepthMask(GL_FALSE);
		glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
		glBegin(GL_QUADS);
			glVertex3d(20,0,20);
			glVertex3d(20,9,20);
			glVertex3d(0,9,20);
			glVertex3d(0,0,20);
		glEnd();
		glDepthMask(GL_TRUE);
		glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		glStencilFunc (GL_EQUAL, 0x1, 0xffffffff);
		glStencilOp (GL_KEEP, GL_KEEP, GL_INCR);	

		glPushMatrix();
		glMultMatrixf((GLfloat *) forward);
		glTranslated(4.5,3.5,15.5);
		glRotatef(angle,0,1,0);
		glCallList(theBottle);
		glPopMatrix();

		glDisable(GL_STENCIL_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);

		//тени готовы, рисуем поверхность.
		GLfloat c_emissive[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat color[4] = { 0.9f, 0.6f, 0.3f, 1.0f };
		GLfloat c_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c_emissive);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,10);

		glDisable(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);
		glEnable(GL_TEXTURE_CUBE_MAP);

		glPushMatrix();
		glTranslated(4.5,3.5,15.5);
		glRotatef(angle,0,1,0);
		glCallList(theBottle);

		glDisable(GL_TEXTURE_CUBE_MAP);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
		glPopMatrix();
	}
	else if (!flag)
	{ //нужно рисовать отражение в полу
		
		glPushMatrix();
		glTranslated(4.5,3.5,15.5);
		glRotatef(angle,0,1,0);
	
		//запомним преобразования координат
		glPushMatrix();//для получения у-координаты
		glLoadIdentity();//тумана
		glTranslated(4.5,3.5,15.5);
		glRotatef(angle,0,1,0);
		float	MM[16]; //Модельно-видовая матрица.
		glGetFloatv ( GL_MODELVIEW_MATRIX, MM );
		glPopMatrix();

		
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);
		glEnable(GL_TEXTURE_CUBE_MAP);
		for(int i=0;i<50;i++)
		{	
			glBegin(GL_QUAD_STRIP);
			for(int j=0;j<50;j++)
			{	//получим высоту точки после матричных преобразований.
				double y = b[i][j][0]*MM[1] + b[i][j][1]*MM[5] + b[i][j][2]*MM[9] + MM[13];	
				glFogCoordfEXT(y);
				//и нарисуем ее.
				glNormal3f(Nb[i][j][0],Nb[i][j][1],Nb[i][j][2]);
				glVertex3f(b[i][j][0],b[i][j][1],b[i][j][2]);
				glNormal3f(Nb[i+1][j][0],Nb[i+1][j][1],Nb[i+1][j][2]);
				glVertex3f(b[i+1][j][0],b[i+1][j][1],b[i+1][j][2]);
			}
			glEnd();
		}
		glDisable(GL_TEXTURE_CUBE_MAP);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
		glPopMatrix();
	}

	else 
	{
		//тени готовы, рисуем саму поверхность.
		GLfloat c_emissive[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat color[4] = { 0.9f, 0.6f, 0.3f, 1.0f };
		GLfloat c_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c_emissive);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,10);

		glPushMatrix();
		glTranslated(4.5,3.5,15.5);
		glRotatef(angle,0,1,0);
	
		//запомним преобразования координат
		glPushMatrix();//для получения у-координаты
		glLoadIdentity();//тумана
		glTranslated(4.5,3.5,15.5);
		glRotatef(angle,0,1,0);
		float	MM[16]; //Модельно-видовая матрица.
		glGetFloatv ( GL_MODELVIEW_MATRIX, MM );
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);
		glEnable(GL_TEXTURE_CUBE_MAP);

		glCallList(theBottle);

		glDisable(GL_TEXTURE_CUBE_MAP);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
		glPopMatrix();
		}
}

void RenderLamp(void)
{
	//материал лампочки
	GLfloat emissive1[4] = { 1.0f, 1.0f, 0.5f, 1.0f };
	GLfloat emissive2[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat color[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat specular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	if (f) glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissive1);
	else glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissive2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,50);
	glFogCoordfEXT(light_position[1]);
	glPushMatrix();
	glTranslated(light_position[0],light_position[1],light_position[2]);
	glutSolidSphere(0.3,25,25);
	glPopMatrix();

	//свойства источника
	double power = 0.0;
	if (f) power = 0.9;

	GLfloat white_light[] = {0.1f+power,0.1f+power,0.1f+power,1.0};
	GLfloat l_specular[] = {0.4f,0.4f,0.4f,1.0f};
	glLightfv(GL_LIGHT0,GL_AMBIENT, white_light);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0,GL_SPECULAR, l_specular);
	glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.2);
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
}

void RenderAqua(int flag)
{
	glEnable(GL_DEPTH_TEST);
	if (flag & f)
	{
		//сначала тень
		glClear(GL_STENCIL_BUFFER_BIT);
		glClearStencil(0x0);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc (GL_ALWAYS, 0x1, 0xffffffff);
		glStencilOp (GL_REPLACE, GL_REPLACE, GL_REPLACE);
		glDepthMask(GL_FALSE);
		glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
		glBegin(GL_QUADS);
			glVertex3d(0,0,0);
			glVertex3d(20,0,0);
			glVertex3d(20,0,20);
			glVertex3d(0,0,20);
		glEnd();
		glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		glDepthMask(GL_TRUE);
		glStencilFunc (GL_EQUAL, 0x1, 0xffffffff);
		glStencilOp (GL_KEEP, GL_KEEP, GL_INCR);	
	
		glColor4d(0,0,0,0.4);
		glDisable(GL_LIGHTING);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		
		glPushMatrix();
		glMultMatrixf((GLfloat *) floorShadow);
		glCallList(theAqua);
		glPopMatrix();

		glClear(GL_STENCIL_BUFFER_BIT); //на будущее.
		glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);
		glDisable(GL_STENCIL_TEST);
	}

	GLfloat mat_emissive[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat mat_color[4] = { 0.0f, 0.4f, 0.4f, 1.0f };
	GLfloat mat_specular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,10);
	//рисуем подставку
	glCallList(theAqua);

	//тут рисуем воду
	GLfloat water_color[4] = { 0.3f, 0.7f, 0.6f, 0.7f};
	GLfloat water_specular[4] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat water_emissive[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, water_color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, water_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, water_emissive);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,70);
	

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);//включаем смешивание.
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D,texture[2]);//и рисуем саму водную поверхность.
	glEnable(GL_TEXTURE_2D);
	
	glPushMatrix();
	glEnable(GL_NORMALIZE);
	glTranslated(17,2.2,17);
	glRotated(90,1,0,0);
	step(0.004);
	glDisable(GL_NORMALIZE);
	glPopMatrix();

	//тут - стеклышки
	GLboolean fog = glIsEnabled(GL_FOG);
	
	glFogi(GL_FOG_MODE,GL_EXP2);
	glFogf(GL_FOG_DENSITY, 0.4);
	glEnable(GL_FOG);

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, water_color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, water_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, water_emissive);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,70);

	glBegin(GL_QUADS);
		int v=0;// при < тумана нет
		int n=3;// при > полный туман
		glFogCoordfEXT(n);
		glTexCoord2d(0,0.3);
		glNormal3f(0,0,-1);
		glVertex3f(20,1.5,14);
		glFogCoordfEXT(v);
		glTexCoord2d(0,0.5);
		glNormal3f(0,0,-1);
		glVertex3f(20,2.2,14);
		glFogCoordfEXT(v);
		glTexCoord2d(1,0.5);
		glNormal3f(0,0,-1);
		glVertex3f(14,2.2,14);
		glFogCoordfEXT(n);
		glTexCoord2d(1,0.3);
		glNormal3f(0,0,-1);
		glVertex3f(14,1.5,14);

		glFogCoordfEXT(n);
		glTexCoord2d(0,0.3);
		glNormal3f(-1,0,0);
		glVertex3f(14,1.5,14);
		glFogCoordfEXT(v);
		glTexCoord2d(0,0.5);
		glNormal3f(-1,0,0);
		glVertex3f(14,2.2,14);
		glFogCoordfEXT(v);
		glTexCoord2d(1,0.5);
		glNormal3f(-1,0,0);
		glVertex3f(14,2.2,20);
		glFogCoordfEXT(n);
		glTexCoord2d(1,0.3);
		glNormal3f(-1,0,0);
		glVertex3f(14,1.5,20);
	glEnd();
	//если тумана не было, а ведь мы его включили чтоб нарисовать стеклышки красиво, надо его выключить.
	if(!fog) glDisable(GL_FOG); 
	glFogi(GL_FOG_MODE,GL_LINEAR);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);//смешивание и картинки больше не нужны
}

void init_water(void)
{	/*создаем прямоугольную область, расставляем вершины по (х,у) и нормали по z*/
	/*не трогаем z-компоненту координат и (x,y) - у нормалей*/
	
	for(int i=0;i<128;i++)
	{
		for(int j=0;j<128;j++)
		{
			P[i][j].x = 3.0f-6.0f*i/127.0f;
			P[i][j].y = 3.0f-6.0f*j/127.0f;

			vertices[i][j].coo[0]=3.0f-6.0f*i/127.0f;
			vertices[i][j].coo[1]=3.0f-6.0f*(j+1)/127.0f;
			vertices[i][j].nor[2]=-4.0f/127.0f;
		}
	}
}

void step(double vs)
{
	/*vs - вязкость. если vs=0, то волны не будут затухать*/
	//работаем с 2 слоями,т.к. строим один слой на основе другого.
	int i,j,i1,j1; 
	i1=rand()%128;  //генерируем (х,у) координату возмущаемой точки
	j1=rand()%128;

    if((rand()% 25)==0) //возмущаем поверхность.
	for(i=-4;i<5;i++)	//с вероятностью 1/25
	{
		for(j=-4;j<5;j++)
		{
			float v=6.0f-i*i-j*j;
			if(v<0.0f)v=0.0f;
			n->U[i+i1+3][j+j1+3]-=v*0.005f+ 0.01;

			MakeParticle(i1,j1);	
		}
	}
	//считаем высоты точек и недостающие компоненты вектора нормали
	for(i=1;i<127;i++)//нормализовать за нас будет программа.
	{
		for(j=1;j<127;j++)
		{
			vertices[i][j].coo[2]=n->U[i][j];
			vertices[i][j].nor[0]=n->U[i-1][j]-n->U[i+1][j];
			vertices[i][j].nor[1]=n->U[i][j-1]-n->U[i][j+1];
			
			float krest=(n->U[i-1][j]+ 
				          n->U[i+1][j]+
						  n->U[i][j+1]+
						  n->U[i][j-1])*0.25f-n->U[i][j];
			//krest - есть в некотором смысле приближение оператора Лапласа.
			//считаем новое значение высоты поверхности воды.
			p->U[i][j]=((2.0f-vs)*n->U[i][j]-p->U[i][j]*(1.0f-vs)+krest);
		}
	}
	//пора нарисовать.
	//нижний слой
	dx+=0.005;
	dy+=0.005;
	glDisable(GL_BLEND);
	/*for(i=0;i<127;i++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for(j=0;j<127;j++)
		{	int k = 6;
			double m = 0.705;
			glTexCoord2d((3+vertices[i][j].coo[0])/k + dx,vertices[i][j].coo[1]/k+dy);
			glNormal3fv(vertices[i][j].nor);
			glVertex3f(vertices[i][j].coo[0],vertices[i][j].coo[1],vertices[i][j].coo[2]+m);
			glTexCoord2d((3+vertices[i+1][j].coo[0])/k+dx,vertices[i+1][j].coo[1]/k+dy);
			glNormal3fv(vertices[i+1][j].nor);
			glVertex3f(vertices[i+1][j].coo[0],vertices[i+1][j].coo[1],vertices[i+1][j].coo[2]+m);
		}
		glEnd();
	}
	*/
	glEnable(GL_BLEND);

	//верхний слой
	for(i=1;i<127;i++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for(j=1;j<127;j++)
		{	int k = 6;
			//glFogCoordfEXT(-4);
			glTexCoord2d((3+vertices[i][j].coo[0])/k,vertices[i][j].coo[1]/k);
			glNormal3fv(vertices[i][j].nor);
			glVertex3fv(vertices[i][j].coo);
			glTexCoord2d((3+vertices[i+1][j].coo[0])/k,vertices[i+1][j].coo[1]/k);
			glNormal3fv(vertices[i+1][j].nor);
			glVertex3fv(vertices[i+1][j].coo);
		}
		glEnd();
	}
	HandleParticle();
	
	//меняем местами слои.
	field *sw=p;
	p=n;
	n=sw;
	sw=0;
	delete[] sw;
}

void shadowmatrix(GLfloat matrix[4][4], GLfloat plane[4],GLfloat lightpos[4])
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

void init_ShadowMatrix(void)
{
	//y=0
	plane[0] = 0; plane[1] = 1; plane [2] = 0; plane[3] = 0;
	shadowmatrix(floorShadow,plane,light_position);
	//x=20
	plane[0] = -1; plane[1] = 0; plane [2] = 0; plane[3] = 20;
	shadowmatrix(left,plane,light_position);
	//x=0
	plane[0] = 1; plane[1] = 0; plane [2] = 0; plane[3] = 0;
	shadowmatrix(right,plane,light_position);
	//z=0
	plane[0] = 0; plane[1] = 0; plane [2] = 1; plane[3] = 0;
	shadowmatrix(back,plane,light_position);
	//z=20
	plane[0] = 0; plane[1] = 0; plane [2] = -1; plane[3] = 20;
	shadowmatrix(forward,plane,light_position);
}

void init_extensions(void)
{
	//для тумана
	typedef void (APIENTRYP PFNGLFOGCOORDFPROC) (GLfloat coord);
	glFogCoordfEXT = (PFNGLFOGCOORDFEXTPROC) wglGetProcAddress("glFogCoordfEXT");
	
	

	//ну и время сюда же
	GetSystemTime(&time1);
}

void RenderSphere(int flag)
{
	/*название функции условное... сначала хотел сферу рисовать.*/
	/*ну и пусть будет так )*/
	GLfloat c_emissive[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat c_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,50);
	
	double A =-1;
	double B = 0;

	if( f && flag)
	{	/*рисуем тень от объекта*/
		
		glColor4d(0,0,0,0.3);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		/*сначала на пол*/
		glClear(GL_STENCIL_BUFFER_BIT);
		glClearStencil(0x0);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc (GL_ALWAYS, 0x1, 0xffffffff);
		glStencilOp (GL_REPLACE, GL_REPLACE, GL_REPLACE);
		glDepthMask(GL_FALSE);
		glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
		glBegin(GL_QUADS);
			glVertex3d(0,0,0);
			glVertex3d(20,0,0);
			glVertex3d(20,0,20);
			glVertex3d(0,0,20);
		glEnd();
		glDepthMask(GL_TRUE);
		glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		glStencilFunc (GL_EQUAL, 0x1, 0xffffffff);
		glStencilOp (GL_KEEP, GL_KEEP, GL_INCR);	
	
		glPushMatrix();
		glMultMatrixf((GLfloat *) floorShadow);
		
		glTranslated(12+A,5,8+B);
		glRotated(ang2,0,1,0);
		glTranslated(1.5,0,0);
		glRotated(2*ang2,0,1,0);
		glCallList(theBox);
		glPopMatrix();

		glDisable(GL_STENCIL_TEST);		
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
	}
	
	glPushMatrix();
	glTranslated(12+A,5,8+B);
	glRotated(ang2,0,1,0);
	glTranslated(1.5,0,0);
	glRotated(2*ang2,0,1,0);
	glCallList(theBox);
	glPopMatrix();
	}

void MakeParticle(int x,int y)
{
	P[x][y].v = (0.02+rand()%5*0.01)*0.3;
	P[x][y].flag = 1;
	P[x][y].z = 0;
	P[x][y].time = 0;
}

void HandleParticle()
{
	int T=300; //максимальное время жизни
	double rad = 0.05;
	
	glDisable(GL_TEXTURE_2D);

	GLfloat water_color[4] = { 0.5f, 0.8f, 0.8f, 1.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, water_color);

	glPushMatrix();
	for(int i=0;i<127;i++)
	{	
		for(int j=0;j<127;j++)
		{
			if(P[i][j].flag)//если частица жива
			{
				if (P[i][j].time<T) //и если ее время меньше максимально возможного времени жизни
				{
					P[i][j].time++; //увеличим время
					if (P[i][j].time <T*4/5) //тут просто летит вверх
					{
						P[i][j].z=P[i][j].v*P[i][j].time; //и зададим координату(высоты) через V*T
						glPushMatrix();
						glTranslated(P[i][j].x, P[i][j].y, -P[i][j].z);
						glutSolidSphere(rad+0.01*P[i][j].z,10,10);	
						glPopMatrix();
					}
					else //а тут летит расширяясь. потом лопается.
					{
						double d = (-P[i][j].z+P[i][j].v*P[i][j].time)*0.1;
						glPushMatrix();
						glTranslated(P[i][j].x, P[i][j].y, -P[i][j].v*P[i][j].time);
						glutSolidSphere(rad+0.01*P[i][j].z+d,20,20);	
						glPopMatrix();
						
					}
				}
				else //у частицы наступило время Т ^^ 
				{
					P[i][j].flag = 0; //частицы не стало.
				}
			}
		}
	}
	glPopMatrix();
	glDisable(GL_POINT_SMOOTH);
	glEnable(GL_TEXTURE_2D);
}
