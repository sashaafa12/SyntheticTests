//
// Object wrapper for Cg programs
//
// Author: Alex V. Boreskoff <steps3d@narod.ru>, <alexboreskoff@mtu-net.ru>
//

#ifndef	__CG_PROGRAM__
#define	__CG_PROGRAM__

#ifdef	_WIN32
	#include	<windows.h>

	#pragma	warning (disable:4786)
#endif

#include	<string>

using namespace std;

#ifdef	MACOSX
	#include	<OpenGL/gl.h>
#else
	#include	<GL/gl.h>
#endif

#include	<Cg/cgGL.h>
#include	"Vector4D.h"
#include	"Vector3D.h"
#include	"Vector2D.h"

class	CgProgram
{
protected:
	static	CGcontext	context;
	static	CGprofile	vertexProfile;
	static	CGprofile	fragmentProfile;

	CGprogram	program;
	int			type;
	string		listing;
	string		errorDesc;

public:
	CgProgram  ( int theType );
	~CgProgram ();

	bool	load ( const char * fileName );

	const string&	getListing () const
	{
		return listing;
	}

	const string&	getErrorDescription () const
	{
		return errorDesc;
	}

	bool	isVertex () const
	{
		return type == CgVertexProgram;
	}

	bool	isFragment () const
	{
		return type == CgFragmentProgram;
	}

	CGprofile	getProfile () const
	{
		return isVertex () ? vertexProfile : fragmentProfile;
	}

	CGparameter	parameterForName ( const char * name ) const
	{
		return cgGetNamedParameter ( program, name );
	}

	bool	setTexture ( const char * name, unsigned texId )
	{
		CGparameter param = parameterForName ( name );

		if ( !param )
			return false;

		cgGLSetTextureParameter ( param, texId );

		return true;
	}

	bool	setVector  ( const char * name, const Vector4D& value )
	{
		CGparameter param = parameterForName ( name );

		if ( !param )
			return false;

		cgGLSetParameter4f ( param, value.x, value.y, value.z, value.w );

		return true;
	}

	bool	setVector ( CGparameter param, const Vector4D& value )
	{
		if ( !param )
			return false;

		cgGLSetParameter4f ( param, value.x, value.y, value.z, value.w );

		return true;
	}

	bool	enable  ()
	{
		cgGLEnableProfile ( getProfile () );

		return true;
	}

	bool	disable ()
	{
		cgGLDisableProfile ( getProfile () );

		return true;
	}

	bool	bind ()
	{
		cgGLBindProgram ( program );

		return true;
	}

	bool	unbind ()
	{
		cgGLBindProgram ( 0 );

		return true;
	}

	void	setParameter ( CGparameter param, float x )
	{
		cgGLSetParameter1f ( param, x );
	}

	void	setParameter ( CGparameter param, const Vector2D& v )
	{
		cgGLSetParameter2fv ( param, v );
	}

	void	setParameter ( CGparameter param, const Vector3D& v )
	{
		cgGLSetParameter3fv ( param, v );
	}

	void	setParameter ( CGparameter param, const Vector4D& v )
	{
		cgGLSetParameter4fv ( param, v );
	}

	void	setMatrixParameter ( CGparameter param, CGGLenum matrix, CGGLenum transform )
	{
		cgGLSetStateMatrixParameter ( param, matrix, transform );
	}

	enum CgProgramType
	{
		CgVertexProgram   = 1,
		CgFragmentProgram = 2
	};

protected:
	bool	checkCgError ();
};

#endif
