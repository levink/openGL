PFNGLFOGCOORDFEXTPROC glFogCoordfEXT;
PFNGLPOINTPARAMETERFVEXTPROC glPointParameterfvEXT;
PFNGLPOINTPARAMETERFEXTPROC glPointParameterfEXT;

int scene_w=800;
int scene_h=600;
SYSTEMTIME curtime;
SYSTEMTIME time1;


//вспомогательные переменные
int transform = 0;
GLfloat ang1 = -180;
GLfloat ang2 = 0;
GLfloat ang3 = 0;
int list_flag = 0;
double b[51][51][3];
double Nb[51][51][3];
double ball[3] = {4,4,4};

//текстуры
int tex_width;
int tex_height;
unsigned char *tex_bits = NULL;
GLuint texture[4] = {0,0,0,0}; 
GLuint cubemap = 0;

//списки отображения
GLuint theWalls;
GLuint theAqua;
GLuint theBottle;
GLuint theBox;


//свет
int f=1; //выключатель
GLfloat light_position[] = {10,8.5,10,1}; 

//тени
GLfloat floorShadow[4][4];
GLfloat left[4][4];
GLfloat right[4][4];
GLfloat forward[4][4];
GLfloat back[4][4];
GLfloat plane[4];


//без нее никак =)
double Pi = 3.141592;

//цвета
double red = 0.0;
double green = 0.0;
double blue = 0.0;

//наблюдатель
double v=0.5;//скорость движения камеры
struct cam Cam = {
	1.0,7.5,2.0,// положение камеры
	0.0, //угол по горизонтали (в радианах)
	0.0 //угол по вертикали (в градусах)
};

//вода
vertex vertices[128][128];
field A,B;
field *p=&A,*n=&B;
double dx=0.0;
double dy=0.0;
particle P[128][128];


//необходимые функции
unsigned char *LoadTrueColorBMPFile(const char *path,int *width,int *height);
void init_water(void);
void RenderObjects(int flag);
void RenderFloor(void);
void RenderPicture(void);
void RenderAqua(int flag);
void RenderComplexSurface(int flag);
void RenderLamp(void);
void RenderWalls(void);
void step(double vs);
void init_lists(void);
bool init_tex();
void init_light(void);
void shadowmatrix(GLfloat matrix[4][4], GLfloat plane[4],GLfloat lightpos[4]);
void init_ShadowMatrix(void);
void init_extensions(void);
void RenderSphere(int flag);
void MakeParticle(int x,int y);
void HandleParticle(void);
