//
// Basic class for 4d vectors.
// Supports all basic vector operations
//
// Author: Alex V. Boreskoff
//
#ifndef	__VECTOR4D__
#define	__VECTOR4D__

#include	<math.h>

#include	"Vector3D.h"

#ifndef	EPS
	#define	EPS	0.0001f
#endif

#ifndef	M_PI								// since not all compilers define it
	#define M_PI        3.14159265358979323846f
#endif

class	Vector4D
{
public:
	float	x, y, z, w;

	Vector4D () {}
	Vector4D ( float v ) : x ( v ), y ( v ), z ( v ), w ( v ) {}
	Vector4D ( float px, float py, float pz, float pw ) : x ( px ), y ( py ), z ( pz ), w ( pw ) {}
	Vector4D ( const Vector4D& v ) : x ( v.x ), y ( v.y ), z ( v.z ), w ( v.w ) {}
	Vector4D ( const Vector3D& v, float theW = 1 ) : x ( v.x ), y ( v.y ), z ( v.z ), w ( theW ) {}

	Vector4D& operator = ( const Vector4D& v )
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;

		return *this;
	}

	Vector4D operator + () const
	{
		return *this;
	}

	Vector4D operator - () const
	{
		return Vector4D ( -x, -y, -z, -w );
	}

	Vector4D& operator += ( const Vector4D& v )
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;

		return *this;
	}

	Vector4D& operator -= ( const Vector4D& v )
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;

		return *this;
	}

	Vector4D& operator *= ( const Vector4D& v )
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;

		return *this;
	}

	Vector4D& operator *= ( float f )
	{
		x *= f;
		y *= f;
		z *= f;
		w *= f;

		return *this;
	}

	Vector4D& operator /= ( const Vector4D& v )
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;

		return *this;
	}

	Vector4D& operator /= ( float f )
	{
		x /= f;
		y /= f;
		z /= f;
		w /= f;

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

	int	operator == ( const Vector4D& v ) const
	{
		return x == v.x && y == v.y && z == v.z && w == v.w;
	}

	int	operator != ( const Vector4D& v ) const
	{
		return x != v.x || y != v.y || z != v.z || w != v.w;
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
		return (float) sqrt ( x * x + y * y + z * z + w * w);
	}

    float   lengthSq () const
    {
        return x * x + y * y + z * z + w * w;
    }

    float   maxLength () const
    {
        return max2 ( max2 ( (float)fabs (x), (float)fabs (y) ), max2 ( (float)fabs (z), (float)fabs ( w ) ) );
    }

	Vector4D&	normalize ()
	{
		return (*this) /= length ();
	}

    float   distanceToSq ( const Vector4D& p ) const
    {
        return sqr ( x - p.x ) + sqr ( y - p.y ) + sqr ( z - p.z ) + sqr ( w - p.w );
    }

    float   distanceTo ( const Vector4D& p ) const
    {
        return (float)sqrt ( sqr ( x - p.x ) + sqr ( y - p.y ) + sqr ( z - p.z ) + sqr ( w - p.w ) );
    }

	float	distanceToAlongAxis ( const Vector4D& p, int axis ) const
	{
		return (float)fabs ( operator [] ( axis ) - p [axis] );
	}

	Vector3D	xyz () const
	{
		return Vector3D ( x, y, z );
	}

	Vector4D&	clamp ( float lower, float upper );

	static	Vector4D	getRandomVector ( float len = 1 );

	static	inline	Vector4D vmin ( const Vector4D& v1, const Vector4D& v2 )
	{
		return Vector4D ( v1.x < v2.x ? v1.x : v2.x, v1.y < v2.y ? v1.y : v2.y,
					      v1.z < v2.z ? v1.z : v2.z, v1.w < v2.w ? v1.w : v2.w );
	}

	static	inline	Vector4D vmax ( const Vector4D& v1, const Vector4D& v2 )
	{
		return Vector4D ( v1.x > v2.x ? v1.x : v2.x, v1.y > v2.y ? v1.y : v2.y,
						  v1.z > v2.z ? v1.z : v2.z, v1.w > v2.w ? v1.w : v2.w );
	}

	friend Vector4D operator + ( const Vector4D&, const Vector4D& );
	friend Vector4D operator - ( const Vector4D&, const Vector4D& );
	friend Vector4D operator * ( const Vector4D&, const Vector4D& );
	friend Vector4D operator * ( float,           const Vector4D& );
	friend Vector4D operator * ( const Vector4D&, float );
	friend Vector4D operator / ( const Vector4D&, float );
	friend Vector4D operator / ( const Vector4D&, const Vector4D& );

private:
    float   max2 ( float a, float b ) const
    {
        return a > b ? a : b;
    }

    float   min2 ( float a, float b ) const
    {
        return a < b ? a : b;
    }

    float   sqr ( float x ) const
    {
        return x*x;
    }

    static	Vector4D	zero;
    static	Vector4D	one;
};

inline Vector4D operator + ( const Vector4D& u, const Vector4D& v )
{
	return Vector4D ( u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w );
}

inline Vector4D operator - ( const Vector4D& u, const Vector4D& v )
{
	return Vector4D ( u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w );
}

inline Vector4D operator * ( const Vector4D& u, const Vector4D& v )
{
	return Vector4D ( u.x*v.x, u.y*v.y, u.z * v.z, u.w * v.w );
}

inline Vector4D operator * ( const Vector4D& v, float a )
{
	return Vector4D ( v.x*a, v.y*a, v.z*a, v.w*a );
}

inline Vector4D operator * ( float a, const Vector4D& v )
{
	return Vector4D ( v.x*a, v.y*a, v.z*a, v.w*a );
}

inline Vector4D operator / ( const Vector4D& u, const Vector4D& v )
{
	return Vector4D ( u.x/v.x, u.y/v.y, u.z/v.z, u.w/v.w );
}

inline Vector4D operator / ( const Vector4D& v, float a )
{
	return Vector4D ( v.x/a, v.y/a, v.z/a, v.w/a );
}

inline Vector4D operator / ( float a, const Vector4D& v )
{
	return Vector4D ( a / v.x, a / v.y, a / v.z, a / v.w );
}

inline float operator & ( const Vector4D& u, const Vector4D& v )
{
	return u.x*v.x + u.y*v.y + u.z*v.z + u.w*v.w;
}

inline float dot ( const Vector4D& a, const Vector4D& b )
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline	Vector4D lerp ( const Vector4D& a, const Vector4D& b, float t )
{
	return a + t * (b - a);
}

#endif
