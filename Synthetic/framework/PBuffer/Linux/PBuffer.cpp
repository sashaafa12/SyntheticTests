//
// Simple p-buffer encapsulation for Linux.
//
// Author: Alex V. Boreskoff
//

#include	<stdio.h>
#include	"libExt.h"
#include	"PBuffer.h"
#include	"../AttrList.h"

PBuffer :: PBuffer  ( int theWidth, int theHeight, int theMode, bool theShareObjects )
{
	width             = theWidth;
	height            = theHeight;
	mode              = theMode;
	display           = 0;
	shareObjects      = theShareObjects;
	hBuffer           = 0;
	hGLContext        = NULL;
	hPreviousContext  = NULL;
	hPreviousDrawable = 0;
	texture           = 0;
}

PBuffer :: ~PBuffer ()
{
	clear ();
}

void	PBuffer :: clear ()
{
	if ( hBuffer != 0 )
	{
		if ( glXGetCurrentContext () == hGLContext )
			glXMakeCurrent ( display, hBuffer, 0 );
			
		glXDestroyContext ( display, hGLContext );
		glXDestroyPbuffer ( display, hBuffer );
		
		hBuffer = 0;
	}
	
	glDeleteTextures ( 1, &texture );
}

bool	PBuffer :: create ()
{
	if ( !checkExtensions () )
	    return false;
		
	display = glXGetCurrentDisplay ();
	
	GLXContext  context = glXGetCurrentContext ();
	int			screen  = DefaultScreen ( display );
	

	AttrList <int>		intAttrs;
	
	intAttrs.add ( GLX_RENDER_TYPE_SGIX,   GLX_RGBA_BIT_SGIX );
	intAttrs.add ( GLX_DRAWABLE_TYPE_SGIX, GLX_PBUFFER_BIT_SGIX );
	intAttrs.add ( GLX_RED_SIZE,        8 );
	intAttrs.add ( GLX_GREEN_SIZE,      8 );
	intAttrs.add ( GLX_BLUE_SIZE,       8 );

	if ( mode & modeAlpha )
		intAttrs.add ( GLX_ALPHA_SIZE, 8 );
	else
		intAttrs.add ( GLX_ALPHA_SIZE, 0 );

	if ( mode & modeStencil )
		intAttrs.add ( GLX_STENCIL_SIZE, 8 );
	else
		intAttrs.add ( GLX_STENCIL_SIZE, 0 );

	if ( mode & modeDepth )
		intAttrs.add ( GLX_DEPTH_SIZE, 24 );

	if ( mode & modeAccum )
	{
		intAttrs.add ( GLX_ACCUM_RED_SIZE,        8 );
		intAttrs.add ( GLX_ACCUM_GREEN_SIZE,      8 );
		intAttrs.add ( GLX_ACCUM_BLUE_SIZE,       8 );

		if ( mode & modeAlpha )
			intAttrs.add ( GLX_ACCUM_ALPHA_SIZE, 8 );
	}

	if ( mode & modeDouble )
		intAttrs.add ( GLX_DOUBLEBUFFER, GL_TRUE );
	else
		intAttrs.add ( GLX_DOUBLEBUFFER, GL_FALSE );
		

	GLXFBConfig * fbConfigs;
	int           count;
	
	fbConfigs = glXChooseFBConfig ( display, screen, intAttrs.getAttrs (), &count );
	
	if ( fbConfigs == NULL || count < 1 )
	    return false;
											// pick first format for which we can create pbuffer
	for ( int i = 0; i < count; i++ )									
	{
		hBuffer = glXCreateGLXPbufferSGIX ( display, fbConfigs [i], width, height, NULL );
		
		if ( hBuffer != 0 )
		{
			hGLContext = glXCreateContextWithConfigSGIX ( display, fbConfigs [i], GLX_RGBA_TYPE, shareObjects ? context : NULL, GL_TRUE );
			
			break;
		}
	}
	
	if ( hBuffer == 0 )
		return false;
		
	if ( hGLContext == NULL )
		return false;
		
	glXQueryGLXPbufferSGIX ( display, hBuffer, GLX_WIDTH_SGIX,  (unsigned *)&width  );
	glXQueryGLXPbufferSGIX ( display, hBuffer, GLX_HEIGHT_SGIX, (unsigned *)&height );

	if ( (mode & modeTexture2D) == 0 )	// no texture creation required
	    return true;
	    
						// create associated texture
	glGenTextures   ( 1, &texture );
	glBindTexture   ( GL_TEXTURE_2D, texture );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	
	if ( mode & modeTextureMipmap )
	{
    	    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,   GL_LINEAR_MIPMAP_LINEAR );
    	    glTexParameteri ( GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE );	    
	}

	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
	
	return true;
}

bool	PBuffer :: makeCurrent ()
{
	hPreviousContext  = glXGetCurrentContext  ();
	hPreviousDrawable = glXGetCurrentDrawable ();
	
	bool res = glXMakeCurrent ( display, hBuffer, hGLContext );
	
	glDrawBuffer ( GL_FRONT );
	glReadBuffer ( GL_FRONT );
	
	return res;
}

bool	PBuffer :: restoreCurrent ()
{
	if ( hPreviousContext == NULL || hPreviousDrawable == 0 )
		return false;

	glBindTexture       ( GL_TEXTURE_2D, texture );
	glCopyTexSubImage2D ( GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height );
	
	bool	result = glXMakeCurrent ( display, hPreviousDrawable, hPreviousContext );

	hPreviousContext  = NULL;
	hPreviousDrawable = 0;

	return result;
}

bool	PBuffer :: isLost () const
{
	return false;
}

										// bind pbuffer to previously bound texture
bool	PBuffer :: bindToTexture ()
{
	return true;
}

										// unbind from texture (release)
bool	PBuffer :: unbindFromTexture ()
{
	return true;
}
	
bool	PBuffer :: checkExtensions ()
{
	return isExtensionSupported ( "GLX_SGIX_pbuffer"  ) &&
	       isExtensionSupported ( "GLX_SGIX_fbconfig" ) &&
	       isExtensionSupported ( "GL_SGIS_generate_mipmap" );
}

