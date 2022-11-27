//All needed variables
SYSTEMTIME t1;
SYSTEMTIME t2;
int width_scene=800;
int height_scene=600;


//extension
PFNGLFOGCOORDFEXTPROC glFogCoordfEXT;


//mouse
int mouse_x = 0;
int mouse_y = 0;
int mouse_button = -1;
int mouse_state = GLUT_UP;
double mouse_x_angle = 30;
double mouse_y_angle = 60;


//camera & light
double pos[4] ={ 20, -20,-20, 1};
GLfloat lpos[4] ={ 20, 30, 5, 1 };


//textures
int tex_width;
int tex_height;
GLuint ice = 0; 
GLuint tree = 0;


//тени
GLfloat matrix[4][4];
GLfloat plane[4];
int shadowflag = 0;

struct My_tree {
	double r; //расстояние до наблюдателя
	double x; //расстояние по оси х
	double y; //расстояние по оси y
	double z; //расстояние по оси z
	double size;
} elka[6];

struct particle
{
	double h;
	int time;
	int flag;
	double phi;

} P[60][60];


float BSP[5][5][3] =
{ 
	{{-5,0,0}, {5,10, 0},  {10,-12,0},  {15,10,0},  {25,0,0},  },
	{{2,0,5},  {6,10,5},   {10,-12,5},  {14,10,5},  {18,0,5},  },
	{{5,-3,10},{7,10,10},  {10,-10,10}, {13,10,10}, {15,-3,10}, },
	{{7,9,16}, {8,17,16},  {10, 1,16},  {12,17,16}, {13,9,16}, },
	{{9,7,20}, {9.5,7,20}, {10, 7,20},  {10.5,7,20},{11,7,20}, },
	
};

float TexBSP[2][2][2] = {{{0, 0},{0, 1}},{{1, 1},{1, 0}}};