//Здесь содержатся объявления всех глобальных переменных
SYSTEMTIME t1;
SYSTEMTIME t2;
int width_scene=1024;
int height_scene=768;


double ang=0; //анимация с помощью этой величины
int flag1 = 1; //shadows - 0, normal - 1;
int rf = 0; //reflection - 1, normal - 0;

//мышь
int mouse_x = 0;
int mouse_y = 0;
int mouse_button = -1;
int mouse_state = GLUT_UP;
double mouse_x_angle = 30;
double mouse_y_angle = 0;
double fog = 0;


//наблюдатель и источник освещения
double pos_x = 0.0;
double pos_y = -5.0;
double pos_z = -10.0;
GLfloat lpos[4] ={ 2, 10, 2, 1 };

//2D-текстуры
int tex_width;
int tex_height;
GLuint floor_texture = 0; 
GLuint cubemap = 0;
GLuint tree = 0;

//кубическая текстура
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


//тени
double size1 = 512;
float mv [16];
float pr [16];
float mv1 [16];
float pr1 [16];
GLuint shadowmap = 0;
GLuint shadowmap1 = 0;
GLfloat sm[4][4];
GLfloat plane[4];



//список ("cube" - это условно)
GLuint cube;

//Система частиц
struct particle_fomat {
	int t;
	double flag;
	double x;
	double y;
} particles[100];
int maxparticles = 100;

struct my_tree {
	double r; //расстояние до наблюдателя
	double x; //расстояние по оси х
	double y; //расстояние по оси z
} trees[5];
