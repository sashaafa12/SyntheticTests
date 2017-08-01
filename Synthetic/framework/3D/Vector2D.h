//
// Basic class for 2d vectors.
// Supports al basic vector operations
//
// Author: Alex V. Boreskoff
//

#ifndef	__VECTOR2D__
#define	__VECTOR2D__

#include	<math.h>

#ifndef	EPS
	#define	EPS	0.0001f
#endif

class	Vector2D
{
public:
	float	x, y;

	Vector2D () {}
	Vector2D ( float v ) : x ( v ), y ( v ) {}
	Vector2D ( float px, float py ) : x ( px  ), y ( py  ) {}
	Vector2D ( const Vector2D& v  ) : x ( v.x ), y ( v.y ) {}

	Vector2D& operator = ( const Vector2D& v )
	{
		x = v.x;
		y = v.y;

		return *this;
	}

	Vector2D operator + () const
	{
		return *this;
	}

	Vector2D operator - () const
	{
		return Vector2D ( -x, -y );
	}

	Vector2D& operator += ( const Vector2D& v )
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	Vector2D& operator -= ( const Vector2D& v )
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}

	Vector2D& operator *= ( const Vector2D& v )
	{
		x *= v.x;
		y *= v.y;

		return *this;
	}

	Vector2D& operator *= ( float f )
	{
		x *= f;
		y *= f;

		return *this;
	}

	Vector2D& operator /= ( const Vector2D& v )
	{
		x /= v.x;
		y /= v.y;

		return *this;
	}

	Vector2D& operator /= ( float f )
	{
		x /= f;
		y /= f;

		return *this;
	}

	float& operator [] ( int index )
	{
		return * ( index + &x );
	}

	int	operator == ( const Vector2D& v ) const
	{
		return x == v.x && y == v.y;
	}

	int	operator != ( const Vector2D& v ) const
	{
		return x != v.x || y != v.y;
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
		return (float) sqrt ( x * x + y * y );
	}

    float   lengthSq () const
    {
	    return x * x + y * y;
    }

	Vector2D&	normalize ()
	{
		return (*this) /= length ();
	}

    float   maxLength () const
    {
        return max2 ( (float) fabs (x), (float) fabs (y) );
    }

    float   distanceToSq ( const Vector2D& p ) const
    {
        return sqr ( x - p.x ) + sqr ( y - p.y );
    }

    float   distanceTo ( const Vector2D& p ) const
    {
        return (float) sqrt ( sqr ( x - p.x ) + sqr ( y - p.y ) );
    }

    Vector2D ort () const
    {
        return Vector2D ( -y, x );
    }

	Vector2D&	clamp ( float lower, float upper )
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

		return *this;
	}

	static	Vector2D	getRandomVector ( float len = 1 );

	static	inline	Vector2D vmin ( const Vector2D& v1, const Vector2D& v2 )
	{
		return Vector2D ( v1.x < v2.x ? v1.x : v2.x, v1.y < v2.y ? v1.y : v2.y );
	}

	static	inline	Vector2D vmax ( const Vector2D& v1, const Vector2D& v2 )
	{
		return Vector2D ( v1.x > v2.x ? v1.x : v2.x, v1.y > v2.y ? v1.y : v2.y );
	}

	friend Vector2D operator + ( const Vector2D&, const Vector2D& );
	friend Vector2D operator - ( const Vector2D&, const Vector2D& );
	friend Vector2D operator * ( const Vector2D&, const Vector2D& );
	friend Vector2D operator * ( float,           const Vector2D& );
	friend Vector2D operator * ( const Vector2D&, float );
	friend Vector2D operator / ( const Vector2D&, float );
	friend Vector2D operator / ( const Vector2D&, const Vector2D& );
	friend float    operator & ( const Vector2D&, const Vector2D& );

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
    
    static	Vector2D	zero;
    static	Vector2D	one;
};

inline Vector2D operator + ( const Vector2D& u, const Vector2D& v )
{
	return Vector2D ( u.x + v.x, u.y + v.y );
}

inline Vector2D operator - ( const Vector2D& u, const Vector2D& v )
{
	return Vector2D ( u.x - v.x, u.y - v.y );
}

inline Vector2D operator * ( const Vector2D& u, const Vector2D& v )
{
	return Vector2D ( u.x*v.x, u.y*v.y );
}

inline Vector2D operator * ( const Vector2D& v, float a )
{
	return Vector2D ( v.x*a, v.y*a );
}

inline Vector2D operator * ( float a, const Vector2D& v )
{
	return Vector2D ( v.x*a, v.y*a );
}

inline Vector2D operator / ( const Vector2D& u, const Vector2D& v )
{
	return Vector2D ( u.x/v.x, u.y/v.y );
}

inline Vector2D operator / ( const Vector2D& v, float a )
{
	return Vector2D ( v.x/a, v.y/a );
}

inline Vector2D operator / ( float a, const Vector2D& v )
{
	return Vector2D ( a / v.x, a / v.y );
}

inline float operator & ( const Vector2D& u, const Vector2D& v )
{
	return u.x*v.x + u.y*v.y;
}

inline	bool	areCollinear ( const Vector2D& a, const Vector2D& b )
{
	return (a.ort () & b) < EPS;
}

inline	Vector2D lerp ( const Vector2D& a, const Vector2D& b, float t )
{
	return a + t * (b - a);
}

inline float dot ( const Vector2D& a, const Vector2D& b, float t )
{
	return a.x * b.x + a.y * b.y;
}

#endif
