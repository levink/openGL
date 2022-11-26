//Используемые функции

unsigned char *LoadTrueColorBMPFile(const char *path,int *width,int *height);
void initialize(void);
void display(void);
void RenderToShadowMap(void);
void RenderScene(int mode);
void RenderLamp(GLfloat pos[4]);
void InitLists(void);
void frame(double vs);
void HandleParticle(void);
void ViewParticle(void);