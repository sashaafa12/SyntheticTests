//
// Simple utilities
//
#include "stdafx.h"
#include	"libExt\libExt.h"

//OpenGL
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

void	drawQuad ( int w, int h )
{
    glBegin       ( GL_QUADS );
        glTexCoord2f ( 0, 0 );
        glVertex2f   ( 0, 0 );

        glTexCoord2f ( 1, 0 );
        glVertex2f   ( w, 0 );

        glTexCoord2f ( 1, 1 );
        glVertex2f   ( w, h );

        glTexCoord2f ( 0, 1 );
        glVertex2f   ( 0, h );
    glEnd   ();
}

void	drawRect ( int w, int h )
{
    glBegin       ( GL_QUADS );
        glTexCoord2f ( 0, 0 );
        glVertex2f   ( 0, 0 );

        glTexCoord2f ( w, 0 );
        glVertex2f   ( w, 0 );

        glTexCoord2f ( w, h );
        glVertex2f   ( w, h );

        glTexCoord2f ( 0, h );
        glVertex2f   ( 0, h );
    glEnd   ();
}

void    startOrtho ( int w, int h )
{
    glMatrixMode   ( GL_PROJECTION );                   // select the projection matrix
    glPushMatrix   ();                                  // store the projection matrix
    glLoadIdentity ();                                  // reset the projection matrix
                                                        // set up an ortho screen
    glOrtho        ( 0, w, 0, h, -1, 1 );
    glMatrixMode   ( GL_MODELVIEW );                    // select the modelview matrix
    glPushMatrix   ();                                  // store the modelview matrix
    glLoadIdentity ();                                  // reset the modelview matrix

	glDisable   ( GL_DEPTH_TEST );
	glDepthMask ( GL_FALSE );
}

void    endOrtho ()
{
    glMatrixMode ( GL_PROJECTION );                     // select the projection matrix
    glPopMatrix  ();                                    // restore the old projection matrix
    glMatrixMode ( GL_MODELVIEW );                      // select the modelview matrix
    glPopMatrix  ();                                    // restore the old projection matrix

	glEnable    ( GL_DEPTH_TEST );
	glDepthMask ( GL_TRUE );
}

void init ()
{
	glClearColor ( 0, 0, 0, 1 );
	glEnable     ( GL_DEPTH_TEST );
	glDepthFunc  ( GL_LEQUAL );

	glHint ( GL_POLYGON_SMOOTH_HINT,         GL_NICEST );
	glHint ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

