//
// Class to load images from DDS files,
// based on ogl_dds_texture_loader.cpp by Kevin Harris (kevin@codesampler.com)
//
// Author: Alex V. Boreskoff
//

#include    <stdio.h>

#ifdef  _WIN32
    #include    <windows.h>
#endif

#ifdef	MACOSX
	#include	<OpenGL/gl.h>
	#include	<OpenGL/glu.h>
	#include	<OpenGL/glext.h>
#else
#include    <GL/gl.h>
	#include    <GL/glu.h>
	#include    "../glext.h"
#endif

//#include    <string.h>
#include    "Data.h"
#include    "ExrLoader.h"
#include    "CompressedTexture.h"
#include	"ImfIO.h"
#include	"ImfRgbaFile.h"
#include	"ImfArray.h"
#include	"ImfRgba.h"
#include	"ImathFun.h"

class	DataStream : public Imf::IStream
{
	Data * data;
public:
	DataStream ( Data * theData ) : Imf::IStream ( theData -> getFileName ().c_str () )
	{
		data = theData;
	}
	
    virtual bool    isMemoryMapped () const
	{
		return false;
	}
	
    virtual bool	read ( char c [], int n )
	{
		return data -> getBytes ( c, n ) == n;
	}
	
    virtual Imf::Int64	tellg ()
	{
		return data -> getPos ();
	}
	
    virtual void	seekg ( Imf::Int64 pos )
	{
		data -> seekAbs ( (int) pos );
	}
};


Texture * ExrLoader :: load ( Data * data )
{
	DataStream			  dataStream ( data );
    Imf::RgbaInputFile    inputImage ( dataStream );
    Imf::Array<Imf::Rgba> pixels;
    Imath::Box2i 		  dataWindow = inputImage.dataWindow ();
    float 				  pixelRatio = inputImage.pixelAspectRatio();
    unsigned 			  width      = dataWindow.max.x - dataWindow.min.x + 1;
    unsigned 			  height     = dataWindow.max.y - dataWindow.min.y + 1;  
	
    pixels.resizeErase        ( width * height);   
    inputImage.setFrameBuffer ( pixels + (height-1) * width, 1, -static_cast<int>(width));
	
    try
    {
        inputImage.readPixels ( dataWindow.min.y, dataWindow.max.y );
    }
    catch(const std::exception &)
    {
        return NULL;
    }

	Texture * texture = new Texture ( width, height, 4, 2 );		// 2 bytes per channel
	
	texture -> setFormat ( GL_RGBA16F_ARB );
	
	memcpy ( texture -> getData (), pixels, texture -> getBytesPerLine () * texture -> getHeight () );
	
    return texture;
}
