//
// Simple Perlin noise class
//
// Author: Alex V. Boreskoff
//

#include    <stdlib.h>
#include    "libExt.h"
#include    "libTexture3D.h"
#include    "noise3D.h"


inline float rnd ()             // return random number in [-1,1]
{
    return 2*((float) rand () / (float) RAND_MAX) - 1;
}

NoiseTexture :: NoiseTexture  ( int seed )
{
    g = new Vector3D [n*n*n];

    srand ( seed );

    for ( int i = 0; i < n*n*n; i++ )
    {
        g [i].x = rnd ();
        g [i].y = rnd ();
        g [i].z = rnd ();
    }

}

NoiseTexture :: ~NoiseTexture ()
{
    delete g;
}

unsigned    NoiseTexture :: createNoiseTexture2D ( int width, int height ) const
{
    float   dx      = (float) n / (float) width;
    float   dy      = (float) n / (float) height;
    GLuint	texture = 0;

    byte * image = new byte [width*height*3];
    byte * ptr   = image;

    for ( int i = 0; i < width; i++ )
        for ( int j = 0; j < height; j++ )
        {
            Vector2D    p ( i * dx, j * dy );
            Vector3D    n ( noise2D ( p ) );

            *ptr++ = (byte)((n.x + 1)*0.5*255.0);
            *ptr++ = (byte)((n.x + 1)*0.5*255.0);
            *ptr++ = (byte)((n.x + 1)*0.5*255.0);
        }

    glGenTextures ( 1, &texture );
    glBindTexture ( GL_TEXTURE_2D, texture );

    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );

    glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, image );

    delete image;

    return texture;
}

unsigned    NoiseTexture :: createNoiseTexture3D ( int width, int height, int depth ) const
{
    float   dx      = (float) n / (float) width;
    float   dy      = (float) n / (float) height;
    float   dz      = (float) n / (float) depth;
    GLuint	texture = 0;

    byte * image = new byte [width*height*depth*3];
    byte * ptr   = image;

    for ( int i = 0; i < width; i++ )
        for ( int j = 0; j < height; j++ )
            for ( int k = 0; k < depth; k++ )
            {
                Vector3D    p ( i * dx, j * dy, k * dz );
                Vector3D    n ( noise3D ( p ) );

                *ptr++ = (byte)((n.x + 1)*0.5*255.0);
                *ptr++ = (byte)((n.y + 1)*0.5*255.0);
                *ptr++ = (byte)((n.y + 1)*0.5*255.0);
            }

    glGenTextures ( 1, &texture );
    glBindTexture ( GL_TEXTURE_3D_EXT, texture );

    glTexParameteri ( GL_TEXTURE_3D_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_3D_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_S,     GL_REPEAT );
    glTexParameteri ( GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_T,     GL_REPEAT );
    glTexParameteri ( GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_R,     GL_REPEAT );

    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );

    glTexImage3DEXT ( GL_TEXTURE_3D_EXT, 0, GL_RGB, width, height, depth, 0, GL_RGB,
                      GL_UNSIGNED_BYTE, image );

    delete image;

    return texture;
}

bool    NoiseTexture :: saveNoiseTexture3D   ( int width, int height, int depth, const char * fileName ) const
{
    float       dx      = (float) n / (float) width;
    float       dy      = (float) n / (float) height;
    float       dz      = (float) n / (float) depth;

    byte * image = new byte [width*height*depth*3];
    byte * ptr   = image;

    for ( int i = 0; i < width; i++ )
        for ( int j = 0; j < height; j++ )
            for ( int k = 0; k < depth; k++ )
            {
                Vector3D    p ( i * dx, j * dy, k * dz );
                Vector3D    n ( noise3D ( p ) );

                *ptr++ = (byte)((n.x + 1)*0.5*255.0);
                *ptr++ = (byte)((n.y + 1)*0.5*255.0);
                *ptr++ = (byte)((n.z + 1)*0.5*255.0);
            }

    bool    res = saveTexture3D ( width, height, depth, 3, image, fileName );

    delete image;

    return res;
}

unsigned    NoiseTexture :: createAbsNoiseTexture3D ( int width, int height, int depth ) const
{
    float   dx      = (float) n / (float) width;
    float   dy      = (float) n / (float) height;
    float   dz      = (float) n / (float) depth;
    GLuint	texture = 0;

    byte * image = new byte [width*height*depth*3];
    byte * ptr   = image;

    for ( int i = 0; i < width; i++ )
        for ( int j = 0; j < height; j++ )
            for ( int k = 0; k < depth; k++ )
            {
                Vector3D    p ( i * dx, j * dy, k * dz );
                Vector3D    n ( noise3D ( p ) );

                *ptr++ = (byte)( fabs ( n.x ) * 255.0 );
                *ptr++ = (byte)( fabs ( n.y ) * 255.0 );
                *ptr++ = (byte)( fabs ( n.z ) * 255.0 );
            }

    glGenTextures ( 1, &texture );
    glBindTexture ( GL_TEXTURE_3D_EXT, texture );

    glTexParameteri ( GL_TEXTURE_3D_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_3D_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_S,     GL_REPEAT );
    glTexParameteri ( GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_T,     GL_REPEAT );
    glTexParameteri ( GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_R,     GL_REPEAT );

    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );

    glTexImage3DEXT ( GL_TEXTURE_3D_EXT, 0, GL_RGB, width, height, depth, 0, GL_RGB,
                      GL_UNSIGNED_BYTE, image );

    delete image;

    return texture;

}

bool        NoiseTexture :: saveAbsNoiseTexture3D   ( int width, int height, int depth, const char * fileName ) const
{
    float       dx      = (float) n / (float) width;
    float       dy      = (float) n / (float) height;
    float       dz      = (float) n / (float) depth;

    byte * image = new byte [width*height*depth*3];
    byte * ptr   = image;

    for ( int i = 0; i < width; i++ )
        for ( int j = 0; j < height; j++ )
            for ( int k = 0; k < depth; k++ )
            {
                Vector3D    p ( i * dx, j * dy, k * dz );
                Vector3D    n ( noise3D ( p ) );

                *ptr++ = (byte)( fabs ( n.x ) * 255.0 );
                *ptr++ = (byte)( fabs ( n.y ) * 255.0 );
                *ptr++ = (byte)( fabs ( n.z ) * 255.0 );
            }

    bool    res = saveTexture3D ( width, height, depth, 3, image, fileName );

    delete image;

    return res;
}

