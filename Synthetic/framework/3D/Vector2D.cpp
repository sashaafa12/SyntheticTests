//
// Basic class for 2d vectors.
// Supports all basic vector operations
//
// Author: Alex V. Boreskoff
//

#include	<stdlib.h>
#include	"Vector2D.h"

Vector2D	Vector2D :: zero ( 0, 0 );
Vector2D	Vector2D :: one  ( 1, 1 );

static	inline	float	rnd ()		// return float random number in [-1,1]
{
	return 2.0f * float ( rand () ) / float ( RAND_MAX ) - 1.0f;
}

Vector2D	Vector2D :: getRandomVector ( float len )
{
	Vector2D	v;

	for ( ; ; )
	{
		v.x = rnd ();
		v.y = rnd ();

		if ( v.lengthSq () < EPS )
			continue;

		v *= len / v.length ();

		return v;
	}
}

