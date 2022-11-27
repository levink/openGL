#include<windows.h>
#include<gl/glut.h>
#include<math.h>

#include"glext.h"

#pragma pack(1)

struct RGB {
	unsigned char r,g,b;
};

#pragma pack()

const int TexSize = 256;

RGB texbits[6][TexSize][TexSize];

struct Normal {
	double x,y,z;
	Normal()
		{ x = y = z = 0; }
	Normal(double x,double y,double z) : x(x),y(y),z(z)
		{ }
	double GetLength()
		{ return sqrt(x*x + y*y + z*z); }
	void Normalize();
};

void Normal::Normalize()
{
	double length = GetLength();
	if(length > 1e-6) {
		x /= length;
		y /= length;
		z /= length;
	}
}

GLenum cubeSides[6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT
};

Normal GetNormal(int s,int j,int i)
{
	double sc = 2.0*i/TexSize - 1;
	double tc = 2.0*j/TexSize - 1;
	Normal n;
	if(cubeSides[s] == GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT) 
		n = Normal(1,-tc,sc);
	else if(cubeSides[s] == GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT)
		n = Normal(-1,tc,-sc);
	else if(cubeSides[s] == GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT)
		n = Normal(sc,1,tc);
	else if(cubeSides[s] == GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT)
		n = Normal(sc,-1,-tc);
	else if(cubeSides[s] == GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT)
		n = Normal(sc,-tc,1);
	else // cubeSides[s] == GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT
		n = Normal(-sc,-tc,-1);
	n.Normalize();
	return n;
}

RGB Light(Normal n)
{
	Normal h(0.4,0.1,2);
	h.Normalize();
	double d = fabs(n.z);
	double s = h.x*n.x + h.y*n.y + h.z*n.z;
	if(s < 0)
		s = -s;
	s = pow(s,20);
	RGB result;
	result.r = 100*d + 150*s;
	result.g = 100*d + 150*s;
	result.b = 100*d + 150*s;
	return result;
}

void InitTexture()
{
	for(int s = 0;s<6;s++)
		for(int j = 0;j<TexSize;j++)
			for(int i = 0;i<TexSize;i++)
				texbits[s][j][i] = Light(GetNormal(s,j,i));
}

void LoadCubeTexture()
{
	for(int i = 0;i<6;i++) {	// Создаем массив с цветами пикселов
		// Загружаем текстуру
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		gluBuild2DMipmaps(cubeSides[i],   
			              3,                     // Формат текстуры
			              TexSize,TexSize,       // Размер текстуры
			              GL_RGB,                // Формат исходных данных
			              GL_UNSIGNED_BYTE,      // Тип исходных данных
				          texbits[i]);             // Указатель на исходные данные */
	}
    // Устанавливаем параметры текстуры
    glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Включаем перспективное текстурирования
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
}


double x_angle = 0;
double y_angle = 0;

const double Pi = 3.1415;

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
	glColor3f(1,1,1);
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

struct Vertex {
	double x,y,z;
} v[8] = { 
	{ -0.5,-0.5,-0.5 }, 
	{ 0.5,-0.5,-0.5 },
	{ -0.5,0.5,-0.5 },
	{ 0.5,0.5,-0.5 },
	{ -0.5,-0.5,0.5 },
	{ 0.5,-0.5,0.5 },
	{ -0.5,0.5,0.5 },
	{ 0.5,0.5,0.5 }
};

int cubeVertices[24] = {
	5,4,6,7,
	6,2,3,7,
	3,1,5,7,
	4,5,1,0,
	1,3,2,0,
	2,6,4,0
};

const int BoxSize = 1.0;

void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	if(h > 0) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0,w/(GLdouble)h,0.1,45);
	}
}

void display(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,0,-5);
	glRotated(y_angle,0,1,0);
	glRotated(x_angle,1,0,0);
	
	glEnable(GL_TEXTURE_CUBE_MAP_EXT);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_NORMAL_MAP_EXT);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_NORMAL_MAP_EXT);
	glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_NORMAL_MAP_EXT);
	glColor3d(1,1,1);

	glCallList(torus);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_RESCALE_NORMAL);
	glutSolidTeapot(1.0);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
/*
	glColor3d(1,1,1);
	glBegin(GL_QUADS);	
	for(int i = 0;i<24;i++) {
		int idx = cubeVertices[i];
		glTexCoord3d(v[idx].x,v[idx].y,v[idx].z);
		glVertex3d(BoxSize*v[idx].x,BoxSize*v[idx].y,BoxSize*v[idx].z);
	}
	glEnd(); 
*/
	glutSwapBuffers();
}

void idle() {
	x_angle += 0.3;
	y_angle += 0.45;
    display();
}

void init()
{

/*	// Параметры материала 
	GLfloat torus_a[4] = { 0.1, 0.1, 0.1, 1.0 }; 
	GLfloat torus_d[4] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat torus_s[4] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat torus_r = 20;   

	// Параметры источника освещения
	GLfloat light_a[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_d[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_s[4] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat light_pos[4] = { 2.0, 0.0, 0.0, 1.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, torus_a);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, torus_d);
	glMaterialfv(GL_FRONT, GL_SPECULAR, torus_s);
	glMaterialfv(GL_FRONT, GL_SHININESS, &torus_r);

	// Задаем позицию источника освещения
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	// Задаем параметры источника освещения
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_a);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_d);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_s);

	// Включаем освещение
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);*/


	glEnable(GL_DEPTH_TEST);
	InitTexture();
	LoadCubeTexture();
	torus = MakeTorus(2.0,0.3,40,20);
}

void finalize()
{
	glDeleteLists(torus,1);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);	
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
    glutCreateWindow("Environment cube map");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	init();
    glutMainLoop();
    return 0;          
}
