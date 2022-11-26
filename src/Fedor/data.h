//определение глобальных переменных

PFNGLFOGCOORDFEXTPROC glFogCoordfEXT;
PFNGLPOINTPARAMETERFVEXTPROC glPointParameterfvEXT;
PFNGLPOINTPARAMETERFEXTPROC glPointParameterfEXT;
PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
PFNGLBLENDCOLORPROC glBlendColor;


SYSTEMTIME curtime;
SYSTEMTIME time1;

double red = 0.0;
double green = 0.0;
double blue = 0.0;
double x_angle = 30;
double y_angle = 0;
double idle_ang1 = 0;
double fw = -19;
int mouse_x = 0,mouse_y = 0,mouse_button = -1,mouse_state = GLUT_UP;
double width,height;
int initialized_flag=0;
int swf=0;
int reflectionflag=0;


GLdouble anglamp;
GLdouble angle = 0.0;
GLfloat light_position[] = {0,10,4,1};
GLfloat light_direction[] = {0,-10,-4};

float   mv [16];
float   pr [16];     
unsigned shadowMap;
int shadowMapSize = 512;
int cubeMapSize = 100;


//списки отображения
GLuint theObj;
GLuint theSphere;

//вода
vertex vertices[128][128];
field A,B;
field *p=&A,*n=&B, *m;
double dx=0.0;
double dy=0.0;
int framerate = 0;

//текстуры
int tex_width;
int tex_height;
unsigned char *tex_bits = NULL;
GLuint texture[4] = {0,0,0,0}; 
GLuint cubemap = 0;
GLuint reflectioncubemap = 0;
const int ReflectionTexSize = 256;


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

//Система частиц
int maxparticles=400;
particle MP[400];
