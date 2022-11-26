void IdleFunction(void);
void DisplayFunction(void);
void ReshapeFunction(int w, int h);
void MouseMotionFunction(int x,int y);
void KeyboardFunction(unsigned char key,int x,int y);
void MouseClickFunction(int button,int state,int x,int y);

void Init(void);
void PaintAll(void);
void RenderShadows(void);
void ViewParticle(void);
void SortAndView(void);
void ShadowMatrix(GLfloat matrix[4][4], GLfloat plane[4],GLfloat lightpos[4]);