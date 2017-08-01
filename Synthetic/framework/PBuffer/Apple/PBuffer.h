//
// Simple p-buffer encapsulation for Mac OS X
//
// Author: Alex V. Boreskoff <steps3d@narod.ru>,<alexboreskoff@mtu-net.ru>
//

#ifndef	__P_BUFFER__
#define	__P_BUFFER__

#include	<OpenGL/gl.h>
#include	<OpenGL/glu.h>
#include	<OpenGL/OpenGL.h>
#include	<OpenGL/glext.h>
#include	<OpenGL/CGLContext.h>
#include	<OpenGL/CGLCurrent.h>
#include	<OpenGL/CGLRenderers.h>

class	PBuffer
{
protected:
	int				width;
	int				height;
	int				mode;
	CGLPBufferObj   buffer;
	CGLContextObj	context;
	CGLContextObj	saveContext;
	GLuint			texture;
	bool			shareObjects;
	CGLError		error;
	bool			needsClearing;
	bool			needsFlush;

public:
	PBuffer  ( int theWidth, int theHeight, int theMode = modeAlpha | modeDepth | modeStencil,
			   bool theShareObjects = true );
	~PBuffer ();

	int	getWidth () const
	{
		return width;
	}

	int	getHeight () const
	{
		return height;
	}

	bool	getShareObjects () const
	{
		return shareObjects;
	}

	int	getMode () const
	{
		return mode;
	}

	unsigned	getTexture () const
	{
		return texture;
	}
	
	void	clear  ();
	bool	create ();

										// set attribute (like WGL_TEXTURE_CUBE_MAP*_ARB)
	bool	setAttr ( int attr, int value );

	bool	makeCurrent ();				// direct rendering to this pbuffer

	bool	restoreCurrent ();			// direct rendering to prev. target

	bool	isLost () const;			// whether p-buffer is lost due to mode switch

										// bind pbuffer to previously bound texture
	bool	bindToTexture     ( int buf = 0 );

										// unbind from texture (release)
	bool	unbindFromTexture ( int buf = 0 );

										// set specific cubemap side, call glFlush after side is done
	bool	setCubemapSide    ( int side );

	void	printLastError ( const char * comment = "" );

	bool	checkExtensions ();			// check support of required extensions
	
	enum	Mode
	{
		modeAlpha          = 1,
		modeDepth          = 2,
		modeStencil        = 4,
		modeAccum          = 8,
		modeDouble         = 16,
		modeTexture1D      = 32,
		modeTexture2D      = 64,
		modeTextureCubeMap = 128,
		modeTextureMipmap  = 256
	};
};

#endif
