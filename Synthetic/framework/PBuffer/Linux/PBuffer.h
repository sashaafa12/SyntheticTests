//
// Simple p-buffer encapsulation for Linux.
//
// Author: Alex V. Boreskoff
//

#ifndef	__P_BUFFER__
#define	__P_BUFFER__

#include	"libExt.h"
#include	<GL/glxext.h>
#include	<stdio.h>

class	PBuffer
{
protected:
	Display    * display;
    GLXContext   hGLContext;
    GLXPbuffer   hBuffer;
    GLXDrawable  hPreviousDrawable;
    GLXContext   hPreviousContext;
	int			 width;
	int			 height;
	int			 mode;
	bool		 shareObjects;
	unsigned	texture;

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
	bool	bindToTexture     ();

										// unbind from texture (release)
	bool	unbindFromTexture ();

/*
										// set specific cubemap side, call glFlush after side is done
	bool	setCubemapSide    ( int side );
*/
	void	printLastError () {}

	bool	checkExtensions ();
	
	enum	Mode
	{
		modeAlpha          = 1,
		modeDepth          = 2,
		modeStencil        = 4,
		modeAccum          = 8,
		modeDouble         = 16,
//		modeTexture1D      = 32,
		modeTexture2D      = 64,
//		modeTextureCubeMap = 128,
		modeTextureMipmap  = 256
	};
};

#endif
