//
// Proxy classes for accessing ARB program parameters
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>,<steps3d@narod.ru>
//

#ifndef	__PARAM_PROXY__
#define	__PARAM_PROXY__

#include	"libExt.h"
#include	"Vector3D.h"
#include	"Vector4D.h"

#ifndef	_WIN32
    #define	__stdcall
#endif

										// wrapper for parameters
class	ParamProxy
{
public:
  	typedef	void ( __stdcall *PutFunc)( GLenum target, GLuint index, const float * vector );
	typedef void ( __stdcall *GetFunc)( GLenum target, GLuint index, float * params );

protected:
	GetFunc		getter;
	PutFunc		putter;
	int			index;
	GLint		target;
	GLint		id;

public:
	ParamProxy ( PutFunc put, GetFunc get, GLenum theTarget, int theIndex, GLuint theId );

										// put-method
	void	operator = ( const Vector4D& v );
	void	operator = ( const Vector3D& v );

										// get method
	operator Vector4D () const;
};

class	ParamArray
{
protected:
	ParamProxy :: GetFunc	getter;
	ParamProxy :: PutFunc	putter;
	int						target;
	GLuint					id;

public:
	ParamArray ( GLenum theTarget );

	void	setId       ( GLuint newId )
	{
		id = newId;
	}

	void	setPointers ( ParamProxy :: PutFunc put, ParamProxy :: GetFunc get );

	ParamProxy operator [] ( int index )
	{
		return ParamProxy ( putter, getter, target, index, id );
	}
};

#endif
