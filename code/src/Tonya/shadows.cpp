#include <glut/glut.h>

void ShadowMatrix(GLfloat matrix[4][4], GLfloat plane[4],GLfloat lightpos[4])
{
  GLfloat dot;

    dot = plane[0]     *   lightpos[0] +
          plane[1]     *   lightpos[1] +
          plane[2]     *   lightpos[2] +
          plane[3]     *   lightpos[3];

    matrix[0][0]   =   dot   -   lightpos[0]   *   plane[0];
    matrix[1][0]   =   0.f   -   lightpos[0]   *   plane[1];
    matrix[2][0]   =   0.f   -   lightpos[0]   *   plane[2];
    matrix[3][0]   =   0.f   -   lightpos[0]   *   plane[3];

    matrix[0][1]   =   0.f   -   lightpos[1]   *   plane[0];
    matrix[1][1]   =   dot   -   lightpos[1]   *   plane[1];
    matrix[2][1]   =   0.f   -   lightpos[1]   *   plane[2];
    matrix[3][1]   =   0.f   -   lightpos[1]   *   plane[3];

    matrix[0][2]   =   0.f   -   lightpos[2]   *   plane[0];
    matrix[1][2]   =   0.f   -   lightpos[2]   *   plane[1];
    matrix[2][2]   =   dot   -   lightpos[2]   *   plane[2];
    matrix[3][2]   =   0.f   -   lightpos[2]   *   plane[3];

    matrix[0][3]   =   0.f   -   lightpos[3]   *   plane[0];
    matrix[1][3]   =   0.f   -   lightpos[3]   *   plane[1];
    matrix[2][3]   =   0.f   -   lightpos[3]   *   plane[2];
    matrix[3][3]   =   dot   -   lightpos[3]   *   plane[3];
}