//
// Noise texture generator class
//
// Author: Alex V. Boreskoff
//

#ifndef __NOISE_3D__
#define __NOISE_3D__

#include    "Vector2D.h"
#include    "Vector3D.h"

class   NoiseTexture
{
private:
    enum
    {
        n = 8                              // size of matrix
    };

    Vector3D  * g;

    float   drop ( float t ) const
    {
        float   ta = (float) fabs ( t );

        if ( ta <= 1 )
            //return 1 + ta*ta*(2*ta - 3);
            return 1 - ta*ta*ta*(10 - 15*ta + 6*ta*ta);

        return 0;
    }

    const Vector3D&     n2D ( int x, int y, int z ) const
    {
        x &= n - 1;
        y &= n - 1;
        z &= n - 1;

        return g [z*n*n + y*n + x];
    }

    Vector3D    omega ( long i, long j, long k, const Vector3D& pt ) const
    {
        return n2D ( i, j, k ) * drop ( pt.x ) * drop ( pt.y ) * drop ( pt.z );
    }

public:
    NoiseTexture  ( int seed = -1 );
    ~NoiseTexture ();

    float   noise ( const Vector3D& pt ) const
    {
        long    ip  = (long) floor ( pt.x );
        long    jp  = (long) floor ( pt.y );
        long    kp  = (long) floor ( pt.z );
        float   sum = 0;

        for ( register long i = ip; i <= ip + 1; i++ )
            for ( register long j = jp; j <= jp + 1; j++ )
                for ( register long k = kp; k <= kp + 1; k++ )
                    sum += omega ( i, j, k, Vector3D ( pt.x - i, pt.y - j, pt.z - k ) ).x;

        return sum;
    }

    Vector3D    noise2D ( const Vector2D& pt ) const
    {
        long        ip  = (long) floor ( pt.x );
        long        jp  = (long) floor ( pt.y );
        Vector3D    sum ( 0, 0, 0 );

        for ( register long i = ip; i <= ip + 1; i++ )
            for ( register long j = jp; j <= jp + 1; j++ )
                  sum += omega ( i, j, 0, Vector3D ( pt.x - i, pt.y - j, 0 ) );

        return sum;
    }

    Vector3D    noise3D ( const Vector3D& pt ) const
    {
        long        ip  = (long) floor ( pt.x );
        long        jp  = (long) floor ( pt.y );
        long        kp  = (long) floor ( pt.z );
        Vector3D    sum ( 0, 0, 0 );

        for ( register long i = ip; i <= ip + 1; i++ )
            for ( register long j = jp; j <= jp + 1; j++ )
                for ( register long k = kp; k <= kp + 1; k++ )
                    sum += omega ( i, j, k, Vector3D ( pt.x - i, pt.y - j, pt.z - k ) );

        return sum;
    }

    unsigned    createNoiseTexture2D ( int width, int height ) const;
    unsigned    createNoiseTexture3D ( int width, int height, int depth ) const;
    bool        saveNoiseTexture3D   ( int width, int height, int depth, const char * fileName ) const;

    unsigned    createAbsNoiseTexture3D ( int width, int height, int depth ) const;
    bool        saveAbsNoiseTexture3D   ( int width, int height, int depth, const char * fileName ) const;
};

#endif
