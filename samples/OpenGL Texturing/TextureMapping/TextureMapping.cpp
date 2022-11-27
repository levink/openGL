#include<gl/glut.h>
#include<tchar.h>
#include<stdio.h>

#include"BmpLoad.h"
#include"glext.h"

double x_angle = 0;
double y_angle = 0;
double z_offset = -150;

int tex_width;
int tex_height;
unsigned char *tex_bits = NULL;

int mouse_x = 0,mouse_y = 0,mouse_button = -1,mouse_state = GLUT_UP;

GLuint texture = 0;

int max = 0; // Max anisotropy

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
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,0,z_offset);
	glRotated(x_angle,1,0,0);
	glRotated(y_angle,0,1,0);

	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	glTexCoord2d(0,0);
	glVertex3d(-100,-100,0);
	glTexCoord2d(25,0);
	glVertex3d(100,-100,0);
	glTexCoord2d(25,25);
	glVertex3d(100,100,0);
	glTexCoord2d(0,25);
	glVertex3d(-100,100,0);
	glEnd();
    glFlush();
	glutSwapBuffers();
}

void idle() {
	// ... Nothing to do here
}

void keypress(unsigned char key,int x,int y)
{
	if(key == '1' || key == '!') {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
		display();
	}
	if(key == '2' || key == '@') {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
		display();
	}
	if(key == '3' || key == '#') {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
		display();
	}
	if(key == '4' || key == '$') {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max);
	}
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
			if(x_angle < 0)
				x_angle += 360;
			if(x_angle >= 360)
				x_angle -= 360;
			display();
		}
		if(mouse_button == GLUT_RIGHT_BUTTON) {
			z_offset += dx/20.0;
			display();
		}
	}
	mouse_x = x;
	mouse_y = y;
}

bool Init()
{
	// Создаем массив с цветами пикселов
	if((tex_bits = LoadTrueColorBMPFile("colormap.bmp",&tex_width,&tex_height)) == NULL) 
		return false;
	// Создаем текстурный объект
	glGenTextures(1,&texture);
	// Активизируем текстурный  объект
	glBindTexture(GL_TEXTURE_2D,texture);
	// Загружаем текстуру
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	gluBuild2DMipmaps(GL_TEXTURE_2D,   
			          3,                     // Формат текстуры
			          tex_width,tex_height,  // Размер текстуры
			          GL_RGB,                // Формат исходных данных
			          GL_UNSIGNED_BYTE,      // Тип исходных данных
				      tex_bits);             // Указатель на исходные данные */
	// Освобождаем память, в которой хранилась текстура
	delete[] tex_bits;
	// Устанавливаем параметры текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// Включаем перспективное текстурирования
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    // Разрешаем текстурирования
	glEnable(GL_TEXTURE_2D);
	// Получаем максимальное значение для анизотропной фильтрации
	glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&max);
	char msg[80];
	sprintf(msg,"Max. anisotropy value: %d",max);
	MessageBox(NULL,msg,"Anisotropic filtering",MB_OK);
	return true;
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutCreateWindow("OpenGL cube with texture");
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
