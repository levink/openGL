#include<windows.h>
#include<gl/gl.h>
#include<gl/glu.h>

#include"glext.h"
#include"CubeTexture.h"
#include"BmpLoad.h"

GLenum cubeSides[6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT
};

void Error(const char *message)
{
	::MessageBox(NULL,message,"Error",MB_OK|MB_ICONEXCLAMATION);
	exit(1);
}

GLuint LoadCubeTexture(const char *path[6])
{
	GLuint texture;
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP_EXT,texture);
	int prev_width = -1;
	int prev_height = -1;
	for(int i = 0;i<6;i++) {	// Создаем массив с цветами пикселов
		unsigned char *tex_bits;
		int tex_width;
		int tex_height;
		if((tex_bits = LoadTrueColorBMPFile(path[i],&tex_width,&tex_height)) == NULL) 
			Error("Unable to read texture file");
		if(prev_width != -1 && prev_height != -1)
			if(prev_width != tex_width || prev_height != tex_height)
				Error("Texture maps have different size");
		prev_width = tex_width;
		prev_height = tex_height;
		// Загружаем текстуру
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		gluBuild2DMipmaps(cubeSides[i],   
			              3,                     // Формат текстуры
			              tex_width,tex_height,  // Размер текстуры
			              GL_RGB,                // Формат исходных данных
			              GL_UNSIGNED_BYTE,      // Тип исходных данных
				          tex_bits);             // Указатель на исходные данные */
		delete[] tex_bits;
	}
    // Устанавливаем параметры текстуры
    glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Включаем перспективное текстурирования
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	return texture;
}
