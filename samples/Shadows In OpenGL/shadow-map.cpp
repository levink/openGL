//
// Sample for shadow maps in OpenGL
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#include    "libExt.h"

#include    <glut.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <assert.h>

#include    "libTexture.h"
#include	"Vector3D.h"
#include	"Vector4D.h"

Vector3D    eye    ( 7, 5, 8 );       	// camera position
Vector3D    light  ( 5, 0, 4 );      	// light position
Vector3D	center ( 0, 0, 0 );			// lookAt position
Vector3D	rot    ( 0, 0, 0 );
int			mouseOldX = 0;
int			mouseOldY = 0;
unsigned    ambMap;                     // ambient texture
unsigned    diffuseMap;                 // diffuse texture
unsigned	gridMap;
unsigned	shadowMap;
float       angle = 0;
int			shadowMapSize = 512;

int	width, height;

float   mv [16];                        // to hold modelview matrix used when rendering from light
float   pr [16];                        // to hold projection matrix used when rendering from light

/////////////////////////// Some useful shadowing operations ////////////////////
/////////////////////////////////////////////////////////////////////////////////

void	renderScene ();
void 	reshape     ( int w, int h );

void init ()
{
    glClearColor   ( 0.0, 0.0, 0.0, 1.0 );
    glClearStencil ( 0 );
    glEnable       ( GL_DEPTH_TEST );
    glDepthFunc    ( GL_LESS );

    glHint ( GL_POLYGON_SMOOTH_HINT,         GL_NICEST );
    glHint ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

void	renderToShadowMap ()
{
	glColorMask ( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
	glDisable   ( GL_TEXTURE_2D );

	glEnable        ( GL_POLYGON_OFFSET_FILL );
	glPolygonOffset ( 4, 4 );

									// setup projection
	glViewport ( 0, 0, shadowMapSize, shadowMapSize );
	glClear    ( GL_DEPTH_BUFFER_BIT );

	glMatrixMode   ( GL_PROJECTION );
	glLoadIdentity ();

  	gluPerspective ( 120, 1, 0.1, 60 );
   	gluLookAt      ( light.x, light.y, light.z,		// eye
					 center.x, center.x, center.z,	// center
					 0, 0, 1 );						// up

	glMatrixMode   ( GL_MODELVIEW );
	glLoadIdentity ();

                                        // get modelview and projections matrices
    glGetFloatv ( GL_MODELVIEW_MATRIX,  mv );
    glGetFloatv ( GL_PROJECTION_MATRIX, pr );

										// now render scene from light position
	renderScene ();

										// copy depth map into texture
	glBindTexture    ( GL_TEXTURE_2D, shadowMap );
	glCopyTexImage2D ( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, shadowMapSize, shadowMapSize, 0 );

										// restore state
	glDisable        ( GL_POLYGON_OFFSET_FILL );
	glColorMask      ( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
	glEnable         ( GL_TEXTURE_2D );
}

void	renderScene ()
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

                                                // draw unlit geometry
    glMatrixMode       ( GL_MODELVIEW );
    glColor3f          ( 0.5, 0.5, 0.5 );
    glActiveTextureARB ( GL_TEXTURE0_ARB );
    glEnable           ( GL_TEXTURE_2D );
    glBindTexture      ( GL_TEXTURE_2D, diffuseMap );

	glMatrixMode ( GL_MODELVIEW );
	glPushMatrix ();

	glRotatef       ( 90, 1, 0, 0 );
	glutSolidTeapot ( 1.5 );

	glTranslatef    ( 4, 0, 0 );
	glutSolidTeapot ( 1.5 );

	glTranslatef    ( -4, 0, -3 );
	glutSolidTeapot ( 1.5 );

	glPopMatrix     ();
												// draw grid
    glActiveTextureARB ( GL_TEXTURE0_ARB );
    glEnable           ( GL_TEXTURE_2D );
    glBindTexture      ( GL_TEXTURE_2D, gridMap );

	glEnable    ( GL_ALPHA_TEST );
	glAlphaFunc ( GL_GEQUAL, 0.5 );

    glBegin ( GL_QUADS );

    glTexCoord2f ( 0, 0 );
    glVertex3f   ( -4, -2, 0 );

    glTexCoord2f ( 2, 0 );
    glVertex3f   ( 4,  -2, 0 );

    glTexCoord2f ( 2, 2 );
    glVertex3f   ( 4,  -2, 3 );

    glTexCoord2f ( 0, 2 );
    glVertex3f   ( -4, -2, 3 );

    glEnd   ();
	glDisable ( GL_ALPHA_TEST );

                                                // draw unlit floor
    glActiveTextureARB ( GL_TEXTURE0_ARB );
    glEnable           ( GL_TEXTURE_2D );
    glBindTexture      ( GL_TEXTURE_2D, ambMap );

    glBegin ( GL_QUADS );

    glTexCoord2f ( 0, 0 );
    glVertex3f   ( -7, -7, -1 );

    glTexCoord2f ( 1, 0 );
    glVertex3f   ( 7, -7, -1 );

    glTexCoord2f ( 1, 1 );
    glVertex3f   ( 7, 7, -1 );

    glTexCoord2f ( 0, 1 );
    glVertex3f   ( -7, 7, -1 );

    glEnd   ();
}

void display ()
{
	renderToShadowMap ();						// compute shadow map

												// clear buffers
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	reshape ( width, height );					// setup modelview and projection

	glMatrixMode   ( GL_MODELVIEW );
	glPushMatrix   ();

    glRotatef    ( rot.x, 1, 0, 0 );
    glRotatef    ( rot.y, 0, 1, 0 );
    glRotatef    ( rot.z, 0, 0, 1 );

												// setup shadowing
	glActiveTextureARB ( GL_TEXTURE1_ARB );
	glBindTexture      ( GL_TEXTURE_2D, shadowMap );

	glEnable ( GL_TEXTURE_2D    );
	glEnable ( GL_TEXTURE_GEN_S );
	glEnable ( GL_TEXTURE_GEN_T );
	glEnable ( GL_TEXTURE_GEN_R );
	glEnable ( GL_TEXTURE_GEN_Q );

	glTexGenfv ( GL_S, GL_EYE_PLANE, Vector4D ( 1, 0, 0, 0 ) );
	glTexGenfv ( GL_T, GL_EYE_PLANE, Vector4D ( 0, 1, 0, 0 ) );
	glTexGenfv ( GL_R, GL_EYE_PLANE, Vector4D ( 0, 0, 1, 0 ) );
	glTexGenfv ( GL_Q, GL_EYE_PLANE, Vector4D ( 0, 0, 0, 1 ) );

	glTexEnvi ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

										// set correct texcoord transform
    glMatrixMode  ( GL_TEXTURE );
    glPushMatrix  ();

    glLoadIdentity ();
    glTranslatef   ( 0.5, 0.5, 0.5 );     // remap from [-1,1]^2 to [0,1]^2
    glScalef       ( 0.5, 0.5, 0.5 );
    glMultMatrixf  ( pr );
    glMultMatrixf  ( mv );

	glActiveTextureARB ( GL_TEXTURE0_ARB );

	renderScene ();

	glActiveTextureARB ( GL_TEXTURE1_ARB );
	glDisable          ( GL_TEXTURE_2D   );
	glActiveTextureARB ( GL_TEXTURE0_ARB );

                                                // draw the light
    glMatrixMode ( GL_MODELVIEW );
    glPopMatrix  ();
    glPushMatrix ();

    glTranslatef       ( light.x, light.y, light.z );
    glActiveTextureARB ( GL_TEXTURE0_ARB );
    glDisable          ( GL_TEXTURE_2D );
    glutSolidSphere    ( 0.1f, 15, 15 );
    glPopMatrix        ();

    glMatrixMode ( GL_TEXTURE );
    glPopMatrix  ();

    glMatrixMode ( GL_MODELVIEW );

    glutSwapBuffers ();
}

void reshape ( int w, int h )
{
    glViewport     ( 0, 0, (GLsizei)w, (GLsizei)h );
   	glMatrixMode   ( GL_PROJECTION );
	glLoadIdentity ();

    gluPerspective ( 60.0, (GLfloat)w/(GLfloat)h, 0.1, 60.0 );
    gluLookAt      ( eye.x, eye.y, eye.z,    		// eye
                     center.x, center.y, center.z,  // center
                     0, 0, 1 );              		// up

	glMatrixMode   ( GL_MODELVIEW );
    glLoadIdentity ();

    width  = w;
    height = h;
}

void motion ( int x, int y )
{
    rot.y -= ((mouseOldY - y) * 180.0f) / 200.0f;
    rot.z -= ((mouseOldX - x) * 180.0f) / 200.0f;
    rot.x  = 0;

    if ( rot.z > 360 )
		rot.z -= 360;

	if ( rot.z < -360 )
		rot.z += 360;

    if ( rot.y > 360 )
		rot.y -= 360;

	if ( rot.y < -360 )
        rot.y += 360;

    mouseOldX = x;
    mouseOldY = y;

	glutPostRedisplay ();
}

void mouse ( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN )
    {
        mouseOldX = x;
        mouseOldY = y;
	}
}

void key ( unsigned char key, int x, int y )
{
    if ( key == 27 || key == 'q' || key == 'Q' )        // quit requested
        exit ( 0 );
}

void    animate ()
{
    angle  = 0.002f * glutGet ( GLUT_ELAPSED_TIME );

    light.x = 3*cos ( angle );
    light.y = 3*sin ( angle );
    light.z = 5 + 0.9 * sin ( angle / 3 );

    glutPostRedisplay ();
}

int main ( int argc, char * argv [] )
{
                                // initialize glut
    glutInit            ( &argc, argv );
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL );
    glutInitWindowSize  ( 500, 500 );


                                // create window
    glutCreateWindow ( "OpenGL shadow maps demo" );

                                // register handlers
    glutDisplayFunc  ( display );
    glutReshapeFunc  ( reshape );
    glutKeyboardFunc ( key     );
	glutMouseFunc    ( mouse   );
	glutMotionFunc   ( motion  );
    glutIdleFunc     ( animate );

    init           ();
    initExtensions ();

	assertExtensionsSupported ( "GL_ARB_shadow GL_ARB_depth_texture" );

    ambMap      = createTexture2D ( true,  "../../Textures/oak.bmp" );
    diffuseMap  = createTexture2D ( true,  "../../Textures/Oxidated.jpg" );
    gridMap     = createTexture2D ( true,  "block-with-alpha.tga" );

								// create depth texture
	glGenTextures   ( 1, &shadowMap );
	glBindTexture   ( GL_TEXTURE_2D, shadowMap );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glTexParameteri  ( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB );
	glTexParameteri  ( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL );

	glTexGeni        ( GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
	glTexGeni        ( GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
	glTexGeni        ( GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
	glTexGeni        ( GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );

	glTexImage2D     ( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize, 0,
                       GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL );

    glutMainLoop ();

    return 0;
}
