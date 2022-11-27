#include"FPSCounter.h"

#include<stdio.h>

FPSCounter::FPSCounter(double updateInterval) : updateInterval(updateInterval)
{
	isUpdated = false; // ������� FPS �� ��������	
	fps = 0;
	isInited = false;
}

void FPSCounter::NextFrame()
{
	if(!isInited) { // NextFrame ������� ������ ���
		frameCount = 0;
		prevTime = SystemTimer.GetTime();
		fps = 0;
		isUpdated = false;
		isInited = true;
	} else {
		frameCount++;
		double currTime = SystemTimer.GetTime();
		double delta = ticks2ms(currTime - prevTime);
		if(delta >= 1000*updateInterval) {
			fps = 1000.0*frameCount/delta;
			prevTime = currTime;
			frameCount = 0;
			isUpdated = true;
		} else
			isUpdated = false;
	}
}


FPSDisplay::FPSDisplay(const char *fmt, double updatePeriod) : FPSCounter(updatePeriod)
{
	if(fmt == NULL) // ��������� �� ���������
		strncpy(formatString,"FPS = %.2lf",MaxFPSMessageLength);
	else
		strncpy(formatString,fmt,MaxFPSMessageLength);
	strcpy(text,"");
}

void FPSDisplay::NextFrame()
{
	FPSCounter::NextFrame();
	if(IsUpdated())
		_snprintf(text,MaxFPSMessageLength,formatString,GetFPS());
	Paint();
}

void GlutFPSDisplay::Paint()
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,glutGet(GLUT_WINDOW_WIDTH),0,glutGet(GLUT_WINDOW_HEIGHT),0,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	double fontScale = fontSize/(119.05 + 33.33);
	glColor3d(r,g,b);
	glTranslated(glutGet(GLUT_WINDOW_WIDTH) - glutStrokeLength(GLUT_STROKE_MONO_ROMAN,
		(const unsigned char*)text)*fontScale,120*fontScale,0);
    glScalef(fontScale,fontScale,fontScale);
	for(const char *p = text; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}
