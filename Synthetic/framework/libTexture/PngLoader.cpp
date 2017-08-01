//
// Class to load images from PNG files
//
// Author: Alex V. Boreskoff
//

#ifdef	MACOSX
	#include	<stdlib.h>
#else
	#include	<malloc.h>
#endif

extern "C"
{
	#define		Byte	z_Byte					// to avoid conflcits in zconf.h
	#include	<zlib.h>
	#undef		Byte
	#include	<png.h>
}

#include	"PngLoader.h"
#include	"Data.h"
#include	"Texture.h"

inline	dword	rgbToInt ( dword red, dword green, dword blue )
{
	return (red << 16) | (green << 8) | (blue);
}

inline	dword	rgbaToInt ( dword red, dword green, dword blue, dword alpha )
{
	return (red << 16) | (green << 8) | (blue) | (alpha << 24);
}

/////////////////////////// libpng related stuff ////////////////////////////

struct	PngImageRawData
{
	byte  * data;
	size_t	size;
};

static	void	pngImageRead ( png_structp pngPtr, png_bytep data, png_size_t size )
{
	PngImageRawData * self = (PngImageRawData *) pngPtr -> io_ptr;

	if ( self -> size < size )
		png_error ( pngPtr, "Read error" );
	else
	{
		memcpy ( data, self -> data, size );

		self -> size -= size;
		self -> data += size;
	}
}

///////////////////////////////////// PngLoader methods /////////////////////////////

Texture * PngLoader :: load ( Data * data )
{
								// check signature
	if ( !png_check_sig ( (byte *)data -> getPtr (), data -> getLength () ) )
        return NULL;

	png_structp	pngPtr = png_create_read_struct ( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );

	if ( pngPtr == NULL )
		return NULL;

	png_infop	infoPtr = png_create_info_struct ( pngPtr );

	if ( infoPtr == NULL )
	{
		png_destroy_read_struct ( &pngPtr, (png_infopp) NULL, (png_infopp) NULL );

		return NULL;
	}

	png_infop	endInfo = png_create_info_struct ( pngPtr );

	if ( endInfo == NULL )
	{
		png_destroy_read_struct ( &pngPtr, &infoPtr, (png_infopp) NULL );

		return NULL;
	}

	PngImageRawData	raw;

	raw.data = (byte *) data -> getPtr ();
	raw.size = data -> getLength ();

	png_set_read_fn ( pngPtr, &raw, pngImageRead );
	png_read_info   ( pngPtr, infoPtr );

	int	width         = png_get_image_width  ( pngPtr, infoPtr );
	int	height        = png_get_image_height ( pngPtr, infoPtr );
	int	bpp           = png_get_bit_depth    ( pngPtr, infoPtr );
	int	colorType     = png_get_color_type   ( pngPtr, infoPtr );
	int	rowBytes      = png_get_rowbytes     ( pngPtr, infoPtr );
	int	numComponents = 0;

	switch ( colorType )
	{
		case PNG_COLOR_TYPE_RGB:
			numComponents = 3;
			break;

		case PNG_COLOR_TYPE_RGB_ALPHA:
			numComponents = 4;
			break;

		case PNG_COLOR_TYPE_GRAY:
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			numComponents = 1;
			break;

		case PNG_COLOR_TYPE_PALETTE:
			numComponents = 3;
			break;

		default:
			break;
	}

	if ( numComponents == 0 )
	{
		png_destroy_read_struct ( &pngPtr, &infoPtr, (png_infopp) NULL );

		return NULL;
	}


	Texture * texture = new Texture ( width, height, numComponents );

	if ( texture == NULL || texture -> getData () == NULL )
	{
		png_destroy_read_struct ( &pngPtr, &infoPtr, (png_infopp) NULL );

		return NULL;
	}


	png_color   pal [256];
	png_bytep * rowPtr  = new png_bytep [height];
	dword     * lineBuf = new dword     [width];
	png_color * pngPal     = NULL;
	int			numEntries = (1 << bpp) - 1;
	int			numColors;
	int			i;

	for ( i = 0; i < height; i++ )
		rowPtr [i] = (png_bytep) malloc ( rowBytes );

	png_read_image ( pngPtr, rowPtr );

										// get palette (if there is any)
	if ( colorType == PNG_COLOR_TYPE_PALETTE )
	{
		if ( !png_get_PLTE ( pngPtr, infoPtr, (png_colorp *)&pngPal, &numColors ) )
		{
					// This is a grayscale image, build a grayscale palette
			numColors = numEntries + 1;
			pngPal    = pal;

			for ( i = 0; i < numColors; i++)
				pngPal [i].red   =
				pngPal [i].green =
				pngPal [i].blue  = (byte)((i * 255) / numEntries);
		}
		else
		{
			for ( i = 0; i < numColors; i++ )
			{
				pngPal [i].red   = (byte)((pngPal [i].red   * 255) / numEntries);
				pngPal [i].green = (byte)((pngPal [i].green * 255) / numEntries);
				pngPal [i].blue  = (byte)((pngPal [i].blue  * 255) / numEntries);
			}
		}
	}

										// now rebuild the image
	for ( i = 0; i < height; i++ )
	{
		png_bytep	ptr = rowPtr [i];

		if ( colorType == PNG_COLOR_TYPE_RGB )
		{
			for ( int x = 0; x < width; x++, ptr += 3 )
				lineBuf [x] = rgbToInt ( ptr [2], ptr [1], ptr [0] );
		}
		else
		if ( colorType == PNG_COLOR_TYPE_RGB_ALPHA )
		{
			for ( int x = 0; x < width; x++, ptr += 4 )
				lineBuf [x] = rgbaToInt ( ptr [2], ptr [1], ptr [0], ptr [3] );
		}
		else
		if ( colorType == PNG_COLOR_TYPE_PALETTE )
		{
			for ( int x = 0; x < width; x++ )
			{
				int	ind = ptr [x];

				lineBuf [x] = rgbToInt ( pngPal [ind].blue, pngPal [ind].green, pngPal [ind].red );
			}
		}
		else
		{
			for ( int x = 0; x < width; x++, ptr++ )
				lineBuf [x] = ptr [0];
		}

		texture -> putLine ( height - 1 - i, lineBuf );

		free ( rowPtr [i] );
	}

	delete lineBuf;
	delete rowPtr;

	png_read_end            ( pngPtr, endInfo );
	png_destroy_read_struct ( &pngPtr, &infoPtr, &endInfo );

	return texture;
}

