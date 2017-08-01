//
// Simple Perlin noise class
//
// Author: Alex V. Boreskoff
//

#ifndef	__NOISE__
#define	__NOISE__

#include	"Vector3D.h"

class	Noise
{
private:
	enum
	{
		n = 256
	};

	int	      * p;
	Vector3D  * g;

	int	fold ( int i, int j, int k ) const
	{
		return p [(p [(p[i & (n-1)] + j) & (n-1)] + k) & (n-1)];
	}

	float	drop ( float t ) const
	{
		float	ta = (float) fabs ( t );

		if ( t <= 1 )
			return 1 + ta*ta*(2*ta - 3);

		return 0;
	}

	float	omega ( long i, long j, long k, const Vector3D& pt ) const
	{
		return (g [fold ( i, j, k )] & pt) * drop ( pt.x ) * drop ( pt.y ) * drop ( pt.z );
	}

	static	Vector3D	large;

public:
	Noise  ( int seed = -1 );
	~Noise ();

	float	noise ( const Vector3D& pt ) const
	{
		long	ip  = (long) floor ( pt.x );
		long	jp  = (long) floor ( pt.y );
		long	kp  = (long) floor ( pt.z );
		float	sum = 0;

		for ( register long i = ip; i <= ip + 1; i++ )
			for ( register long j = jp; j <= jp + 1; j++ )
				for ( register long k = kp; k <= kp + 1; k++ )
					sum += omega ( i, j, k, Vector3D ( pt.x - i, pt.y - j, pt.z - k ) );

		return sum;
	}

	Vector3D	dNoise ( const Vector3D& pt ) const;

	Vector3D	noise3D ( const Vector3D& pt ) const
	{
		return Vector3D ( noise ( pt - large ), noise ( pt ), noise ( pt + large ) );
	}

	float	turbulence       ( const Vector3D& pt, int octaves ) const;
	float	turbulence       ( const Vector3D& pt, float minFreq, float maxFreq ) const;
    float   signedTurbulence ( const Vector3D& pt, float minFreq, float maxFreq ) const;
	float	fBm              ( const Vector3D& pt, float h, float lacunarity, float octaves ) const;
};

#endif
