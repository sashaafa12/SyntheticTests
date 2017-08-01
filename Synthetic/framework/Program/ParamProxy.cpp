//
// Proxy classes for accessing ARB program parameters
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>,<steps3d@narod.ru>
//

#include	"ParamProxy.h"
#include	<stdlib.h>

///////////////////////////// ParamProxy methods /////////////////////////////

ParamProxy :: ParamProxy ( PutFunc put, GetFunc get, GLenum theTarget, int theIndex, GLuint theId )
{
	getter = get;
	putter = put;
	target = theTarget;
	index  = theIndex;
	id     = theId;
}

void ParamProxy :: operator = ( const Vector4D& v )
{
	GLint	idCur;
												// we should ensure that this program is currently bound
	glGetProgramivARB ( target, GL_PROGRAM_BINDING_ARB, &idCur );

	if ( idCur != id )
		glBindProgramARB ( target, id );

	putter ( target, index, v );

	if ( idCur != id )
		glBindProgramARB ( target, idCur );
}

void ParamProxy :: operator = ( const Vector3D& v )
{
	Vector4D	v1 ( v );
	GLint		idCur;

	glGetProgramivARB ( target, GL_PROGRAM_BINDING_ARB, &idCur );

	if ( idCur != id )
		glBindProgramARB ( target, id );

	putter ( target, index, v1 );

	if ( idCur != id )
		glBindProgramARB ( target, idCur );
}

ParamProxy :: operator Vector4D	() const
{
	Vector4D	v;
	GLint		idCur;

	glGetProgramivARB ( target, GL_PROGRAM_BINDING_ARB, &idCur );

	if ( idCur != id )
		glBindProgramARB ( target, id );


	getter ( target, index, v );

	if ( idCur != id )
		glBindProgramARB ( target, idCur );

	return v;
}

///////////////////////////// ParamArray methods /////////////////////////////

ParamArray :: ParamArray ( GLenum theTarget )
{
	getter = NULL;
	putter = NULL;
	target = theTarget;
	id     = 0;
}

void	ParamArray :: setPointers ( ParamProxy :: PutFunc put, ParamProxy :: GetFunc get )
{
	getter = get;
	putter = put;
}
