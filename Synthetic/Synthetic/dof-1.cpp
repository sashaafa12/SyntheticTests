//
// Example of depth-of-field
//
// Author: Alex V. Boreskoff <steps3d@narod.ru>,<steps3d@gmail.com>
//

#include "stdafx.h"


#include    "libExt/libExt.h"

#ifdef	MACOSX
	#include	<GLUT/glut.h>
#else
	#include	<glut.h>
#endif

#include    <stdio.h>
#include    <stdlib.h>

#include    "libTexture/libTexture.h"
#include    "libTexture/TypeDefs.h"
#include    "3D/Vector3D.h"
#include    "3D/Vector2D.h"
#include    "utils/boxes.h"
#include    "FrameBuffer/FrameBuffer.h"
#include	"Program/GlslProgram.h"
#include	"utils.h"
#include	"libCamera/Camera.h"
#include	"libCamera/SphericalCamera.h"
#include <iostream>

Vector3D    eye   ( 0, 0, 5 );  // camera position
unsigned    decalMap;                   // decal (diffuse) texture
unsigned    stoneMap;
unsigned    teapotMap;
unsigned	blockMap;
unsigned 	depthMap;

float   angle = 0;
float	yaw   = 1.0f, pitch = 0.0f,	roll  = 0.0f;
float	focalDistance = 4.5;
float	focalRange    = 20;
float	radiusScale   = 3.0 / 512.0;

//Camera		camera ( eye, 0, 0, 0 );	// camera to be used
//SphericalCamera		camera ( eye, Vector3D(0, 0, -1), Vector3D(0, 1, 0), Vector3D(1, 0, 0) );	// camera to be used
SphericalCamera		camera (5, 0, 0);	// camera to be used

FrameBuffer	buffer  ( 512, 512, FrameBuffer :: depth32 );
FrameBuffer	buffer2 ( 512/4, 512/4 );
FrameBuffer	buffer3 ( 512/4, 512/4 );

GlslProgram	program1;						// build G-buffer
GlslProgram	program2;						// test G-buffer
GlslProgram	program3;						// test G-buffer
GlslProgram	blur;

void displayBoxes ();
void reshape      ( int w, int h );

void displayBoxes ()
{
    glMatrixMode ( GL_MODELVIEW );
    glPushMatrix ();

    //drawBox  ( Vector3D ( 0, 0, 0 ),   Vector3D ( 1, 1, 1 ),   stoneMap,  false );
    
    glBindTexture   ( GL_TEXTURE_2D, teapotMap );
    //glRotatef       ( angle * 45.3, 0, 1, 0 );  
    glutSolidTeapot ( 1.5 );

    glPopMatrix     ();
}

void display ()
{
	buffer.bind   ();
	program1.bind ();
	
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	program1.bind            ();
	program1.setUniformFloat ( "focalDistance", focalDistance );
	program1.setUniformFloat ( "focalRange",    focalRange    );
	camera.apply             ();
	
	displayBoxes ();
	
	program1.unbind ();
	buffer.unbind   ( true );
	
	glBindTexture ( GL_TEXTURE_2D, buffer.getColorBuffer () );
	
	program2.bind   ();
	buffer2.bind    ();
	startOrtho      ( 512/4, 512/4 );
	drawQuad        ( 512/4, 512/4 );
	endOrtho        ();
	buffer2.unbind  ( true );
	program2.unbind ();
	
	glBindTexture ( GL_TEXTURE_2D, buffer2.getColorBuffer () );
	
	blur.bind       ();
	buffer3.bind    ();
	startOrtho      ( 512/4, 512/4 );
	drawQuad        ( 512/4, 512/4 );
	endOrtho        ();
	buffer3.unbind  ( true );
	blur.unbind    ();
	
	glActiveTextureARB ( GL_TEXTURE1_ARB );
	glBindTexture      ( GL_TEXTURE_2D, buffer2.getColorBuffer () );
	
	glActiveTextureARB ( GL_TEXTURE0_ARB );
	glBindTexture      ( GL_TEXTURE_2D, buffer.getColorBuffer () );
	
	program3.bind   ();
	program3.setUniformFloat ( "radiusScale", radiusScale );
	startOrtho      ( 512, 512 );
	drawQuad        ( 512, 512 );
	endOrtho        ();
	program3.unbind ();
	
    glutSwapBuffers ();
}

void reshape ( int w, int h )
{
	camera.setViewSize ( w, h, 60 );
	camera.apply       ();
}

void key ( unsigned char key, int x, int y )
{
 //   if ( key == 27 || key == 'q' || key == 'Q' )        // quit requested
 //       exit ( 0 );
 //   else
 //  	if ( key == 'w' || key == 'W' )
 //  		camera.moveBy ( camera.getViewDir () * 0.2 );
 //  	else
 //  	if ( key == 'x' || key == 'X' )
 //  		camera.moveBy ( -camera.getViewDir () * 0.2 );
 //  	else
 //  	if ( key == 'a' || key == 'A' )
 //  		camera.moveBy ( -camera.getSideDir () * 0.2 );
 //  	else
 //  	if ( key == 'd' || key == 'D' )
 //  		camera.moveBy ( camera.getSideDir () * 0.2 );
	//else
	//if ( key == '+' )
	//	focalDistance += 0.1;
	//else
	//if ( key == '-' )
	//	focalDistance -= 0.1;
	//else
	//if ( key == '*' )
	//	focalRange += 0.3;
	//else
	//if ( key == '/' )
	//	focalRange -= 0.3;

 //  	glutPostRedisplay ();
}

void    specialKey ( int key, int x, int y )
{
    if ( key == GLUT_KEY_UP )
        camera.changeTheta(M_PI / 50);
    else
    if ( key == GLUT_KEY_DOWN )
        camera.changeTheta(-M_PI / 50);
	else
    if ( key == GLUT_KEY_RIGHT )
        camera.changePhi(M_PI / 50);
    else
		if ( key == GLUT_KEY_LEFT )
			camera.changePhi(-M_PI / 50);
//
//	camera.setEulerAngles ( yaw, pitch, roll );
//
//    glutPostRedisplay ();
}

//void	mouseFunc ( int x, int y );

void    animate ()
{
	static	float	lastTime = 0.0;
	float			time     = 0.001f * glutGet ( GLUT_ELAPSED_TIME );

    angle += 1.0 * (time - lastTime);

    lastTime = time;

    glutPostRedisplay ();
}

int main ( int argc, char * argv [] )
{
                                // initialize glut
    glutInit            ( &argc, argv );
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize  ( buffer.getWidth (), buffer.getHeight () );


                                // create window
    glutCreateWindow ( "OpenGL Depth Of Field Demo" );

                                // register handlers
    glutDisplayFunc  ( display    );
    glutReshapeFunc  ( reshape    );
    glutKeyboardFunc ( key        );
    glutSpecialFunc  ( specialKey );
    //glutPassiveMotionFunc ( mouseFunc );
    glutIdleFunc     ( animate    );

    init           ();
    initExtensions ();

    assertExtensionsSupported ( "EXT_framebuffer_object" );

    decalMap  = createTexture2D ( true, "wood.png" );
    stoneMap  = createTexture2D ( true, "brick.tga" );
    teapotMap = createTexture2D ( true, "../../Textures/hindi.png" );
    blockMap  = createTexture2D ( true, "../../Textures/block.bmp" );
	
	buffer.create ();
	buffer.bind   ();
	
	if ( !buffer.attachColorTexture ( GL_TEXTURE_2D, buffer.createColorTexture ( GL_RGBA, GL_RGBA8 ), 0 ) )
		printf ( "buffer error with color attachment\n");

	if ( !buffer.isOk () )
		printf ( "Error with framebuffer\n" );
					
	buffer.unbind ();

	buffer2.create ();
	buffer2.bind   ();
	
	if ( !buffer2.attachColorTexture ( GL_TEXTURE_2D, buffer2.createColorTexture ( GL_RGBA, GL_RGBA8 ), 0 ) )
		printf ( "buffer2 error with color attachment\n");

	if ( !buffer2.isOk () )
		printf ( "Error with framebuffer2\n" );
					
	buffer2.unbind ();

	buffer3.create ();
	buffer3.bind   ();
	
	if ( !buffer3.attachColorTexture ( GL_TEXTURE_RECTANGLE_ARB, buffer3.createColorRectTexture ( GL_RGBA, GL_RGBA8 ), 0 ) )
		printf ( "buffer3 error with color attachment\n");

	if ( !buffer3.isOk () )
		printf ( "Error with framebuffer3\n" );
					
	buffer3.unbind ();

	if ( !program1.loadShaders ( "dof1-p1.vsh", "dof1-p1.fsh" ) )
	{
		printf ( "Error loading shaders:\n%s\n", program1.getLog ().c_str () );

		return 3;
	}
	
	if ( !program2.loadShaders ( "dof1-p2.vsh", "dof1-p2.fsh" ) )
	{
		printf ( "Error loading shaders2:\n%s\n", program2.getLog ().c_str () );

		return 3;
	}

	if ( !program3.loadShaders ( "dof1-p3.vsh", "dof1-p3.fsh" ) )
	{
		printf ( "Error loading shaders3:\n%s\n", program3.getLog ().c_str () );

		return 3;
	}

	if ( !blur.loadShaders ( "blur.vsh", "blur.fsh" ) )
	{
		printf ( "Error loading shaders3:\n%s\n", program3.getLog ().c_str () );

		return 3;
	}

	program1.bind        ();
	program1.setTexture  ( "tex", 0 );
	program1.unbind      ();

	program2.bind        ();
	program2.setTexture  ( "tex",    0 );
	program2.unbind      ();

	program3.bind        ();
	program3.setTexture  ( "tex",    0 );
	program3.setTexture  ( "texLow", 1 );
	program3.unbind      ();
	
	blur.bind        ();
	blur.setTexture  ( "tex", 0 );
	blur.unbind      ();

	//camera.setRightHanded ( false );
	//camera.setEulerAngles ( 0, 0, -1.1);


	printf ( "Depth of Field demo.\n\tUse + and - to change focal distance.\n\tUse * and / to change focal range.\n\tUse mouse and wsad to control camera.\n" );
	
    glutMainLoop ();

    return 0;
}
