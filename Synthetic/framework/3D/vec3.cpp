//
// Basic class for 3d vectors.
// Supports all basic vector operations
//
// Author: Alex V. Boreskoff
//

#include	<stdlib.h>
#include	"vec3.h"

vec3	vec3 :: zero ( 0, 0, 0 );
vec3	vec3 :: one  ( 1, 1, 1 );

static	inline	float	rnd ()		// return float random number in [-1,1]
{
	return 2.0f * float ( rand () ) / float ( RAND_MAX ) - 1.0f;
}

vec3	vec3 :: getRandomVector ( float len )
{
	vec3	v;

	for ( ; ; )
	{
		v.x = rnd ();
		v.y = rnd ();
		v.z = rnd ();

		if ( v.lengthSq () < EPS )
			continue;

		v *= len / v.length ();

		return v;
	}
}

vec3&	vec3 :: clamp ( float lower, float upper )
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

	return *this;
}

