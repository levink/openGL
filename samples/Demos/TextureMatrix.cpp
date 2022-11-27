#include<windows.h>
#include<gl/gl.h>
#include<gl/glu.h>
#include<gl/glaux.h>

#include<stdio.h>
#include<math.h>

double pi = 3.1415;

void RenderTorus(double r0,       // Main radius
				 int n,           // Number of subdivisions along main radius
				 double u_repeat, // How many times texture repeats along main radius
				 double start,    // Start angle
				 double end,      // End angle
				 double r1,       // Small radius
				 int m,           // Number of subdivisions along small radius
				 double v_repeat, // How many time texture repeats along small radius
				 bool ccw)        // Counter clockwise or clockwise oriented polygons
{
	bool closed = false;
	if(end == start) {
		start = 0;
		end = 2*pi;
		closed = true;
	}
	for(int i = 0;i<n;i++) {
		int i2;
		if(closed)
			i2 = (i < n-1)?(i+1):(0);
		else
			i2  = i+1;
		double xv = cos(start + (end-start)*i/n);
		double yv = sin(start + (end-start)*i/n);
		double x = r0*xv;
		double y = r0*yv;
		double xv2 = cos(start + (end-start)*i2/n);
		double yv2 = sin(start + (end-start)*i2/n);
		double x2 = r0*xv2;
		double y2 = r0*yv2;
		glBegin(GL_TRIANGLE_STRIP);
		glColor3d(0,1,0);
		for(int j = 0;j<=m;j++) {
			int j2 = (j == m)?(0):(j);
			double tv = r1*cos(2*j2*pi/m);
			double zv = r1*sin(2*j2*pi/m);
			if(ccw) {
				glTexCoord2d(u_repeat*i/n,v_repeat*j/m);
				glVertex3d(x + xv*tv,y + yv*tv,zv);
				glVertex3d(x2 + xv2*tv,y2 + yv2*tv,zv);
			} else {
				glTexCoord2d(u_repeat*(i+1)/n,v_repeat*j/m);
				glVertex3d(x2 + xv2*tv,y2 + yv2*tv,zv);
				glTexCoord2d(u_repeat*i/n,v_repeat*j/m);
				glVertex3d(x + xv*tv,y + yv*tv,zv);
			}
		}
		glEnd();
	}
}

void Init(void);
void CALLBACK Reshape(GLsizei w, GLsizei h);
void CALLBACK Display(void);

const double R0 = 1.5;
const double R1 = 0.4;

const int TexWidth = 256;
const int TexHeight = 256;

void CreateTexture()
{
	FILE *fp = fopen("fractal.raw","rb");
	if(!fp)
		return;
    byte *tex = new byte [TexWidth*TexHeight*3];
	if(!tex)
		return;
    for(int i = 0;i<TexWidth;i++)
		for(int j = 0;j<TexHeight;j++) {
			byte *p = tex+(i+j*TexWidth)*3;
			byte c = fgetc(fp);
			*p++ = 0;
			*p++ = c;
			*p++ = 0;
		}
	glBindTexture(GL_TEXTURE_2D,0);
	glTexImage2D(GL_TEXTURE_2D,
				 0,              // Mipmap level
				 GL_RGB8,        // Components
				 TexWidth,
				 TexHeight,
				 0,
				 GL_RGB,
                 GL_UNSIGNED_BYTE,
				 tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	delete[] tex;
	fclose(fp);
}


void Init(void)
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	CreateTexture();
}

double x_angle = 0;
double y_angle = 0;

void CALLBACK Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);   
// Render background
	glEnable(GL_TEXTURE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	glRotatef(15.0f,1,0,0);
	glTranslated(0,-R0,0);
	glRotatef(90.0f,0,1,0);
	RenderTorus(R0,40,3,-pi/20,pi/2,R1,32,3,false);
    glFlush();
    auxSwapBuffers();
}

void CALLBACK Reshape(GLsizei w, GLsizei h)
{
    if(!h) 
		return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0,(GLfloat)w/(GLfloat)h,0.1,R0 + 2*R1);
}

void CALLBACK Idle(void)
{
	static double du = 0;
	static double dv = 0;
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glTranslated(du,dv,0);
    du += 0.009;
	if(du > 1)
		du -= 1;
	dv += 0.009;
	if(dv > 1)
		dv -= 1;
	Display();
}

static void CALLBACK KeyUP(void)
{
	// Handle up here
}

static void CALLBACK KeyDOWN(void)
{
	// Handle down here
}

static void CALLBACK KeyLEFT(void)
{
	// Handle left here
}

static void CALLBACK KeyRIGHT(void)
{
	// Handle right here
}

int main(int argc, char** argv)
{
    auxInitDisplayMode(AUX_DOUBLE|AUX_RGB|AUX_DEPTH);
    auxInitPosition(0,0,300,300);
    auxInitWindow ("Texture matrix demo");
    Init();
    auxReshapeFunc(Reshape);
    auxKeyFunc(AUX_UP,KeyUP);
    auxKeyFunc(AUX_DOWN,KeyDOWN);
    auxKeyFunc(AUX_LEFT,KeyLEFT);
    auxKeyFunc(AUX_RIGHT,KeyRIGHT);
	auxIdleFunc(Idle);
    auxMainLoop(Display);
    return 0;
}
