//
// Simple p-buffer encapsulation for win32
//
// Author: Alex V. Boreskoff <steps3d@narod.ru>,<alexboreskoff@mtu-net.ru>
//

#include	<assert.h>
#include	<stdio.h>
#include	"libExt.h"
#include	"PBuffer.h"

/*
#ifdef	_WIN32
	#include	"..\AttrList.h"
#else
	#include	"../AttrList.h"
#endif
*/

PBuffer :: PBuffer  ( int theWidth, int theHeight, int theMode, bool theShareObjects )
{
	width         = theWidth;
	height        = theHeight;
	mode          = theMode;
	shareObjects  = theShareObjects;
	buffer        = NULL;
	texture       = 0;
	error         = (CGLError) 0;
	context       = NULL;
	saveContext   = NULL;
	needsClearing = true;
	needsFlush    = false;
}

PBuffer :: ~PBuffer ()
{
	clear ();
}

void	PBuffer :: clear ()
{
	CGLSetCurrentContext ( context ); 
	glDeleteTextures     ( 1, &texture );
	CGLDestroyPBuffer    ( buffer );
	CGLDestroyContext    ( context );
	
	texture = 0;
}

bool	PBuffer :: create ()
{
	saveContext = CGLGetCurrentContext ();

	CGLPixelFormatAttribute	attribs [] = 
	{ 
		(CGLPixelFormatAttribute) kCGLPFAAccelerated, 
//		(CGLPixelFormatAttribute) kCGLPFAWindow,
		(CGLPixelFormatAttribute) kCGLPFAPBuffer,
		(CGLPixelFormatAttribute) kCGLPFAColorSize,  (CGLPixelFormatAttribute) 24, 
		(CGLPixelFormatAttribute) kCGLPFADepthSize,  (CGLPixelFormatAttribute) 32,
		(CGLPixelFormatAttribute) kCGLPFAAlphaSize,  (CGLPixelFormatAttribute) 8,
		(CGLPixelFormatAttribute) NULL 
	}; 
	
	CGLPixelFormatObj		pixelFormat; 
	long					numPixelFormats = 0	; 
	unsigned long			fmt = GL_RGB;
	
	error = CGLChoosePixelFormat  ( attribs, &pixelFormat, &numPixelFormats );
	
	printLastError ( "ChoosePixelFormat" );
	
	error = CGLCreateContext      ( pixelFormat, saveContext, &context );
	
	printLastError ( "CreateContext" );
	
	CGLDestroyPixelFormat ( pixelFormat ); 

	if ( mode & modeAlpha )
		fmt = GL_RGBA;
		
	error = CGLCreatePBuffer ( width, height, GL_TEXTURE_2D, fmt, 0, &buffer );
	
	printLastError ( "CreatePBuffer" );
	
	saveContext = NULL;
	
	return true;
}

bool	PBuffer :: makeCurrent ()
{
	long		  screen;
	
	assert ( context != NULL );
	assert ( saveContext == NULL );
	assert ( buffer != NULL );
	
	saveContext = CGLGetCurrentContext ();

	CGLGetVirtualScreen  ( saveContext, &screen );
	CGLSetCurrentContext ( context );
	
	error = CGLSetPBuffer ( context, buffer, 0, 0, screen );
	
	printLastError ( "makeCurrent" );
	
	return true;
}

bool	PBuffer :: restoreCurrent ()
{
	glFlush ();

	needsFlush = false;
	
	assert ( context != NULL );
	assert ( saveContext != NULL );
	assert ( buffer != NULL );
	
	CGLSetCurrentContext ( saveContext );
	
	printLastError ( "restore saveContext" );
	
	saveContext = NULL;
	
	return true;
}

bool	PBuffer :: isLost () const
{
	return false;
}

bool	PBuffer :: bindToTexture ( int buf )
{
	assert ( context != NULL );
	assert ( saveContext == NULL );
	assert ( buffer != NULL );

	if ( needsFlush )
	{
		saveContext = CGLGetCurrentContext ();

		CGLSetCurrentContext ( context );
		
		glFlush ();
		
		CGLSetCurrentContext ( saveContext );
		
		needsFlush = false;
	}
	
	if ( texture == 0 )
	{
		saveContext = CGLGetCurrentContext ();

		glGenTextures ( 1, &texture );
		glBindTexture ( GL_TEXTURE_2D, texture );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
		error = CGLTexImagePBuffer ( saveContext, buffer, GL_FRONT_LEFT );
		
		printLastError ( "TexImagePBuffer" );
	}
	else
		glBindTexture ( GL_TEXTURE_2D, texture );

	glEnable ( GL_TEXTURE_2D );
	
	saveContext = NULL;
	
	return true;
}

bool	PBuffer :: unbindFromTexture ( int buf )
{
	glBindTexture ( GL_TEXTURE_2D, 0 );

	return true;	
}
/*

bool	PBuffer :: setCubemapSide ( int side )
{
	if ( (mode & modeTextureCubeMap) == 0 )
		return false;

	glFlush ();

	return setAttr ( WGL_CUBE_MAP_FACE_ARB, side );
}
*/

void PBuffer :: printLastError ( const char * comment )
{
	if ( error == 0 )
		return;
		
	const char * errString = CGLErrorString ( error );
	
	printf ( "%s: %d-%s\n", comment, error, errString );

}

bool	PBuffer :: checkExtensions ()
{
	return isExtensionSupported ( "GL_APPLE_pixel_buffer" );
}

