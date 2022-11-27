#ifndef __CUBE_TEXTURE_H

#define __CUBE_TEXTURE_H

#include<gl/gl.h>

extern GLenum cubeSides[6];

GLuint LoadCubeTexture(const char *path[6]);

#endif // __CUBE_TEXTURE_H