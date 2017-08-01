//
// Header file to libExt
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#ifndef __LIB_EXT__
#define __LIB_EXT__

#ifdef	MACOSX

	#include	<OpenGL/gl.h>
	#include	<OpenGL/glu.h>
	#include	<OpenGL/OpenGL.h>
	#include	<OpenGL/glext.h>

	#define	GL_TEXTURE_3D_EXT	GL_TEXTURE_3D
	#define	glTexImage3DEXT		glTexImage3D
#endif

bool    isExtensionSupported ( const char * ext );
void    assertExtensionsSupported ( const char * extList );
void    initExtensions       ();
void    printfInfo           ();                // print info about card, driver, version & etc

const char * getGeneralExtensions  ();			// get list of platform-independant (GL) extensions
const char * getPlatformExtensions ();			// get list of extensions for current platform (WGL or GLX)

#define  glActiveTextureARB         glActiveTexture
#define  glClientActiveTextureARB   glClientActiveTexture

#ifndef	GL_VERSION_1_3							// ARB_transpose matrix
extern	PFNGLLOADTRANSPOSEMATRIXFPROC					glLoadTransposeMatrixfARB;
extern	PFNGLLOADTRANSPOSEMATRIXDPROC					glLoadTransposeMatrixdARB;
extern	PFNGLMULTTRANSPOSEMATRIXFPROC					glMultTransposeMatrixfARB;
extern	PFNGLMULTTRANSPOSEMATRIXDPROC					glMultTransposeMatrixdARB;
#endif

#endif
