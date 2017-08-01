//
// Basic class for 4d vectors.
// Supports all basic vector operations
//
// Author: Alex V. Boreskoff
//

#include	<stdlib.h>
#include	"Vector4D.h"

Vector4D	Vector4D :: zero ( 0, 0, 0, 0 );
Vector4D	Vector4D :: one  ( 1, 1, 1, 1 );

static	inline	float	rnd ()		// return float random number in [-1,1]
{
	return 2.0f * float ( rand () ) / float ( RAND_MAX ) - 1.0f;
}

Vector4D	Vector4D :: getRandomVector ( float len )
{
	Vector4D	v;

	for ( ; ; )
	{
		v.x = rnd ();
		v.y = rnd ();
		v.z = rnd ();
		v.w = rnd ();

		if ( v.lengthSq () < EPS )
			continue;

		v *= len / v.length ();

		return v;
	}
}

Vector4D&	Vector4D :: clamp ( float lower, float upper )
{
	if ( x < lower )
		x = lower;
	else
	if ( x > upper )
		x = upper;

	if ( y < lower )
		y = lower;
	else
	if ( y > upper )
		y = upper;
			
	if ( z < lower )
		z = lower;
	else
	if ( z > upper )
		z = upper;

	if ( w < lower )
		w = lower;
	else
	if ( w > upper )
		w = upper;

	return *this;
}
