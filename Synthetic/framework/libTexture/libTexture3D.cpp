//
// load and save 3D images from/to DDS file's
//
// Author: Alex V. Boreskoff
//

#ifdef	_WIN32
    #pragma	warning (disable:4786)		// FUCK these stupid bastards !!!
#endif

#include    "libExt.h"
#include    "libTexture.h"
#include    "libTexture3D.h"
#include    "DdsDefs.h"
#include	"DdsLoader.h"
#include    "Data.h"
#include    <stdio.h>
#include    <string.h>

static unsigned	loadTexture3DFromData ( int width, int height, int depth, int numComponents, byte * image, bool mipmap );

bool    saveTexture3D ( int width, int height, int depth, int components, const byte * data, const char * fileName )
{
    DDS_HEADER  ddsh;
    int         bytesPerLine = width * components;
    int         padding      = 0;
    char        zeroes [4]   = "\0\0\0";

    if ( (bytesPerLine & 3) != 0 )              // do dword alignment
    {
        padding       = 4 - (bytesPerLine & 3);
        bytesPerLine += padding;
    }

    memset ( &ddsh, 0, sizeof ( ddsh ) );

    ddsh.dwSize              = sizeof ( struct DDS_HEADER );
    ddsh.dwFlags             = DDS_CAPS | DDS_WIDTH | DDS_HEIGHT | DDS_DEPTH | DDS_PIXELFORMAT;
    ddsh.dwHeight            = height;
    ddsh.dwWidth             = width;
    ddsh.dwDepth             = depth;

    ddsh.ddspf.dwSize        = sizeof ( struct DDS_PIXELFORMAT );
    ddsh.ddspf.dwFlags       = (components == 4) ? DDS_RGBA : DDS_RGB;
    ddsh.ddspf.dwRGBBitCount = components * 8;
    ddsh.ddspf.dwRBitMask    = 0x00ff0000;
    ddsh.ddspf.dwGBitMask    = 0x0000ff00;
    ddsh.ddspf.dwBBitMask    = 0x000000ff;

    if ( components == 4 )
    {
        ddsh.ddspf.dwFlags   |= DDS_ALPHA_PIXELS;
        ddsh.ddspf.dwABitMask = 0xff000000;
    }

    ddsh.dwCaps1 = DDS_TEXTURE | DDS_COMPLEX;
    ddsh.dwCaps2 = DDS_VOLUME;

    FILE * fp = fopen ( fileName, "wb" );

    if ( fp == NULL )
        return false;

                                                // write file signature
    fwrite ( "DDS ", 1, 4, fp );

                                                // write dds header
    fwrite ( &ddsh, 1, sizeof ( DDS_HEADER ), fp );

                                                // write texture data
    int     rowsCount = depth * height;
    byte  * buf       = new byte [width * components];

    for ( int i = 0; i < rowsCount; i++ )
    {
                                                // copy bytes to temp. buffer
        memcpy ( buf, data, bytesPerLine );

                                                // now swap red and blue channels
        for ( int j = 0; j < width; j += components )
        {
            buf [j  ] = data [j+2];
            buf [j+1] = data [j+1];
            buf [j+2] = data [j  ];

            if ( components == 4 )
                buf [j+3] = data [j+3];
        }

        fwrite ( buf, 1, bytesPerLine, fp );

        if ( padding > 0 )                      // pad with zeroes
            fwrite ( zeroes, 1, padding, fp );

        data += width * components;
    }

    delete buf;

    fclose ( fp );

    return true;
}

unsigned    createTexture3D ( bool mipmap, const char * fileName )
{
    Data * data = getFile ( fileName );

    if ( data == NULL )
        return 0;

    unsigned    texture = createTexture3D ( mipmap, data );

    delete data;

    return texture;
}

unsigned    createTexture3D ( bool mipmap, Data * data )
{
    DDS_HEADER      ddsd;
    char            filecode [4];
    int 			numComponents = 0, bytesPerLine = 0;
                                                        // verify the file is a true .dds file
    data -> seekAbs  ( 0 );
    data -> getBytes ( filecode, 4 );

    if ( strncmp ( filecode, "DDS ", 4 ) != 0 )
        return 0;
                                                        // get the surface descriptor
    data -> getBytes ( &ddsd, sizeof ( ddsd ) );

    if ( (ddsd.ddspf.dwFlags & DDS_FOURCC) != 0 )       // we do not handle compressed 3D files
        return 0;

    if ( (ddsd.dwFlags & DDS_DEPTH) == 0 )              // not a 3D texture
        return 0;

    int 		width     = ddsd.dwWidth;
    int 		height    = ddsd.dwHeight;
    int 		depth     = ddsd.dwDepth;
    int 		rowsCount = depth * height;
	DdsLoader	loader;
	
    if ( ddsd.ddspf.dwFlags & DDS_ALPHA_PIXELS )        // ARGB
        numComponents = 4;
    else                                                // RGB
	if ( ddsd.ddspf.dwRGBBitCount == 24 )
        numComponents = 3;
	else
	if ( ddsd.ddspf.dwRGBBitCount == 8 )				// 8 -bit
	{
		if ( ddsd.ddspf.dwABitMask == 255 )
			numComponents = 1;
		else
			numComponents = 3;
	
		bytesPerLine = ddsd.dwWidth * 3;

		if ( (bytesPerLine & 3) != 0 )                      // do dword alignment
			bytesPerLine += 4 - (bytesPerLine & 3);

		byte    * image = new byte [width*height*depth*numComponents];
	
		if ( ddsd.ddspf.dwABitMask == 255 )
			loader.loadUncompressed8BitAlphaData ( data, ddsd, bytesPerLine, image );
		else
		if ( ddsd.ddspf.dwRBitMask == 255 )
			loader.loadUncompressed8BitRGBData ( data, ddsd, 0, bytesPerLine, image );
		else
		if ( ddsd.ddspf.dwGBitMask == 255 )
			loader.loadUncompressed8BitRGBData ( data, ddsd, 1, bytesPerLine, image );
		else
		if ( ddsd.ddspf.dwBBitMask == 255 )
			loader.loadUncompressed8BitRGBData ( data, ddsd, 2, bytesPerLine, image );
			
		unsigned	tex = loadTexture3DFromData ( width, height, depth, numComponents, image, mipmap );
		
		delete image;
		
		return tex;
			
	}
	else
	if ( ddsd.ddspf.dwRGBBitCount == 16 )
	{
		bytesPerLine = ddsd.dwWidth * 3;

		if ( (bytesPerLine & 3) != 0 )                      // do dword alignment
			bytesPerLine += 4 - (bytesPerLine & 3);

		byte    * image = new byte [width*height*depth*numComponents];
	
		loader.loadUncompressed16BitRGBData ( data, ddsd, bytesPerLine, image );
		
		unsigned	tex = loadTexture3DFromData ( width, height, depth, numComponents, image, mipmap );
		
		delete image;
		
		return tex;
	}
		
    bytesPerLine = ddsd.dwWidth * numComponents;

    if ( (bytesPerLine & 3) != 0 )                      // do dword alignment
        bytesPerLine += 4 - (bytesPerLine & 3);

    byte    * buf   = new byte [bytesPerLine];
    byte    * image = new byte [width*height*depth*numComponents];
    byte    * dest  = image;

    for ( int i = 0; i < rowsCount; i++ )
    {
        data -> getBytes ( buf, bytesPerLine );
                                                        // rearrange components
        byte * src  = buf;

        for ( register int j = 0; j < (int)ddsd.dwWidth; j++ )
        {
            dest [0] = src [2];                     // red
            dest [1] = src [1];                     // green
            dest [2] = src [0];                     // blue

            if ( numComponents == 4 )
                dest [3] = src [3];                 // alpha

            dest += numComponents;
            src  += numComponents;
        }
    }

	unsigned	tex = loadTexture3DFromData ( width, height, depth, numComponents, image, mipmap );
	
    delete image;
    delete buf;

	return tex;
}

static	unsigned	loadTexture3DFromData ( int width, int height, int depth, int numComponents, byte * image, bool mipmap )
{
    GLuint    texture;

    glGenTextures ( 1, &texture );
    glBindTexture ( GL_TEXTURE_3D_EXT, texture );

    glTexImage3DEXT ( GL_TEXTURE_3D_EXT, 0, numComponents, width, height, depth, 0,
                      numComponents == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, image );

    glTexParameteri ( GL_TEXTURE_3D_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_3D_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_S,     GL_REPEAT );
    glTexParameteri ( GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_T,     GL_REPEAT );
    glTexParameteri ( GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_R,     GL_REPEAT );

                                                    // use SGIS_generate_mipmap extensions
    if ( mipmap )
    {
        glTexParameteri ( GL_TEXTURE_3D_EXT, GL_GENERATE_MIPMAP_SGIS, GL_TRUE );
        glTexParameteri ( GL_TEXTURE_3D_EXT, GL_TEXTURE_MIN_FILTER,   GL_LINEAR_MIPMAP_LINEAR );
    }

    return texture;
}

