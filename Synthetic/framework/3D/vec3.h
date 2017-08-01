//
// Basic class for 3d vectors.
// Supports all basic vector operations
//
// Author: Alex V. Boreskoff
//

#ifndef	__VEC3__
#define	__VEC3__

#include	<math.h>

#ifndef	EPS
	#define	EPS	0.0001f
#endif

#ifndef	M_PI								// since not all compilers define it
	#define M_PI        3.14159265358979323846f
#endif

class	vec3
{
public:
	float	x, y, z;

	vec3 () {}
	vec3 ( float v ) : x ( v ), y ( v ), z ( v ) {}
	vec3 ( float px, float py, float pz ) : x ( px ), y ( py ), z ( pz ) {}
	vec3 ( const vec3& v ) : x ( v.x ), y ( v.y ), z ( v.z ) {}

	vec3& operator = ( const vec3& v )
	{
		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}
	
	vec3 operator - () const
	{
		return vec3 ( -x, -y, -z );
	}

	vec3& operator += ( const vec3& v )
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	vec3& operator -= ( const vec3& v )
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	vec3& operator *= ( const vec3& v )
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;

		return *this;
	}

	vec3& operator *= ( float f )
	{
		x *= f;
		y *= f;
		z *= f;

		return *this;
	}

	vec3& operator /= ( const vec3& v )
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;

		return *this;
	}

	vec3& operator /= ( float f )
	{
		x /= f;
		y /= f;
		z /= f;

		return *this;
	}

	float& operator [] ( int index )
	{
		return * ( index + &x );
	}

	float operator [] ( int index ) const
	{
		return * ( index + &x );
	}

	int	operator == ( const vec3& v ) const
	{
		return x == v.x && y == v.y && z == v.z;
	}

	int	operator != ( const vec3& v ) const
	{
		return x != v.x || y != v.y || z != v.z;
	}

	operator float * ()
	{
		return &x;
	}

	operator const float * () const
	{
		return &x;
	}

	float	length () const
	{
		return (float) sqrt ( x * x + y * y + z * z );
	}

    float   lengthSq () const
    {
        return x * x + y * y + z * z;
    }

	vec3&	normalize ()
	{
		return (*this) /= length ();
	}

    float   maxLength () const
    {
        return max3 ( (float)fabs (x), (float)fabs (y), (float)fabs (z) );
    }

    float   distanceToSq ( const vec3& p ) const
    {
        return sqr ( x - p.x ) + sqr ( y - p.y ) + sqr ( z - p.z );
    }

    float   distanceTo ( const vec3& p ) const
    {
        return (float)sqrt ( sqr ( x - p.x ) + sqr ( y - p.y ) + sqr ( z - p.z ) );
    }

	float	distanceToAlongAxis ( const vec3& p, int axis ) const
	{
		return (float)fabs ( operator [] ( axis ) - p [axis] );
	}

	int	getMainAxis () const
	{
		int		axis = 0;
		float	val  = (float) fabs ( x );

		for ( register int i = 1; i < 3; i++ )
		{
			float	vNew = (float) fabs ( operator [] ( i ) );

			if ( vNew > val )
			{
				val  = vNew;
				axis = i;
			}
		}

		return axis;
	}

	vec3&	clamp ( float lower, float upper );

	static	vec3	getRandomVector ( float len = 1 );

	static	inline	vec3 vmin ( const vec3& v1, const vec3& v2 )
	{
		return vec3 ( v1.x < v2.x ? v1.x : v2.x, v1.y < v2.y ? v1.y : v2.y, v1.z < v2.z ? v1.z : v2.z );
	}

	static	inline	vec3 vmax ( const vec3& v1, const vec3& v2 )
	{
		return vec3 ( v1.x > v2.x ? v1.x : v2.x, v1.y > v2.y ? v1.y : v2.y, v1.z > v2.z ? v1.z : v2.z );
	}

    static	vec3	zero;
    static	vec3	one;

private:
    float   max3 ( float a, float b, float c ) const
    {
        return a > b ? (a > c ? a : (b > c ? b : c)) :
                       (b > c ? b : (a > c ? a : c));
    }

    float   min3 ( float a, float b, float c ) const
    {
        return a < b ? (a < c ? a : (b < c ? b : c)) :
                       (b < c ? b : (a < c ? a : c));
    }

    float   sqr ( float x ) const
    {
        return x*x;
    }
};

inline vec3 operator + ( const vec3& u, const vec3& v )
{
	return vec3 ( u.x + v.x, u.y + v.y, u.z + v.z );
}

inline vec3 operator - ( const vec3& u, const vec3& v )
{
	return vec3 ( u.x - v.x, u.y - v.y, u.z - v.z );
}

inline vec3 operator * ( const vec3& u, const vec3& v )
{
	return vec3 ( u.x*v.x, u.y*v.y, u.z * v.z );
}

inline vec3 operator * ( const vec3& v, float a )
{
	return vec3 ( v.x*a, v.y*a, v.z*a );
}

inline vec3 operator * ( float a, const vec3& v )
{
	return vec3 ( v.x*a, v.y*a, v.z*a );
}

inline vec3 operator / ( const vec3& u, const vec3& v )
{
	return vec3 ( u.x/v.x, u.y/v.y, u.z/v.z );
}

inline vec3 operator / ( const vec3& v, float a )
{
	return vec3 ( v.x/a, v.y/a, v.z/a );
}

inline vec3 operator / ( float a, const vec3& v )
{
	return vec3 ( a / v.x, a / v.y, a / v.z );
}

inline float operator & ( const vec3& u, const vec3& v )
{
	return u.x*v.x + u.y*v.y + u.z*v.z;
}

inline vec3 operator ^ ( const vec3& u, const vec3& v )
{
	return vec3 (u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.x);
}

inline float dot ( const vec3& a, const vec3& b )
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline vec3 cross ( const vec3& u, const vec3& v )
{
	return vec3 (u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x);
}

inline	vec3 lerp ( const vec3& a, const vec3& b, float t )
{
	return a + t * (b - a);
}

inline	float	mixedProduct ( const vec3& a, const vec3& b, const vec3& c )
{
	return ( a & ( b ^ c ) );
}

inline	bool	areCollinear ( const vec3& a, const vec3& b, const vec3& c )
{
	return ((b - a) ^ (c - a)).lengthSq () < EPS * EPS;
}

inline	bool	areComplanar ( const vec3& a, const vec3& b, const vec3& c, const vec3& d )
{
	return fabs ( mixedProduct ( b - a, c - a, d - a ) ) < EPS * EPS * EPS;
}

inline vec3 reflect ( const vec3& v, const vec3& n )	// for unit vectors only
{
	return v - (2.0f * dot (v, n )) * n;
}

#endif
