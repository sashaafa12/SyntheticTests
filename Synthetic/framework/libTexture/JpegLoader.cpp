//
// Class to load images from jpeg files
//
// Author: Alex V. Boreskoff
//

#include	<stdio.h>
#include	<setjmp.h>

#include	"JpegLoader.h"
#include	"Texture.h"
#include	"Data.h"

extern "C" {
	#include	<jpeglib.h>
	#include	<jerror.h>
}

struct my_error_mgr
{
	jpeg_error_mgr	pub;			// "public" fields
	jmp_buf			setjmp_buffer;	// for return to caller
};

											// callbacks required by jpeglib

//
// Initialize source --- called by jpeg_read_header before any data is actually read.
//

static	void	initSource ( j_decompress_ptr )
{

}

//
// Fill the input buffer --- called whenever buffer is emptied. should never happen
//

static	boolean	fillInputBuffer ( j_decompress_ptr )
{
	return TRUE;
}

//
// Skip data --- used to skip over a potentially large amount of
// uninteresting data (such as an APPn marker).
//

static	void	skipInputData ( j_decompress_ptr cinfo, long count )
{
	jpeg_source_mgr * src = cinfo -> src;

	if ( count > 0 )
	{
		src -> bytes_in_buffer -= count;
		src -> next_input_byte += count;
	}
}

//
// Terminate source --- called by jpeg_finish_decompress
// after all data has been read.  Often a no-op.
//
// NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
// application must deal with any cleanup that should happen even
// for error exit.
//

static	void	termSource ( j_decompress_ptr )
{

}

static void myErrorExit (j_common_ptr cinfo)
{
					// cinfo->err really points to a my_error_mgr struct, so coerce pointer
	my_error_mgr * myerr = (my_error_mgr *) cinfo->err;

					// Return control to the setjmp point
	longjmp ( myerr -> setjmp_buffer, 1 );
}

inline	dword	rgbToInt ( dword red, dword green, dword blue )
{
	return (red << 16) | (green << 8) | (blue);
}

Texture * JpegLoader :: load ( Data * data )
{
	jpeg_decompress_struct	cinfo;
	my_error_mgr			jerr;

							// allocate and initialize JPEG decompression object
							// we set up the normal JPEG error routines, then override error_exit
	cinfo.err           = jpeg_std_error ( &jerr.pub );
	jerr.pub.error_exit = myErrorExit;

	if ( setjmp ( jerr.setjmp_buffer ) )
	{
		jpeg_destroy_decompress ( &cinfo );

		return NULL;
	}

							// now we can initialize the JPEG decompression object
	jpeg_create_decompress ( &cinfo );

							// specify data source (memory buffer, in this case)

	jpeg_source_mgr	jsrc;

							// set up data pointer
	jsrc.bytes_in_buffer   = data -> getLength ();
	jsrc.next_input_byte   = (JOCTET *) data -> getPtr ();
	jsrc.init_source       = initSource;
	jsrc.fill_input_buffer = fillInputBuffer;
	jsrc.skip_input_data   = skipInputData;
	jsrc.resync_to_restart = jpeg_resync_to_restart;			// use default
	jsrc.term_source       = termSource;

	cinfo.src            = &jsrc;


							// read file parameters with jpeg_read_header()
	jpeg_read_header ( &cinfo, TRUE );

							// We almost always want RGB output (no grayscale, yuv etc)
	if ( cinfo.jpeg_color_space != JCS_GRAYSCALE )
		cinfo.out_color_space = JCS_RGB;

							// Recalculate output image dimensions
	jpeg_calc_output_dimensions ( &cinfo );

							// Start decompressor
	jpeg_start_decompress ( &cinfo );

	int		width       = cinfo.output_width;
	int		height      = cinfo.output_height;
	int		rowSpan     = cinfo.image_width * cinfo.num_components;
//	bool	isGreyScale = cinfo.jpeg_color_space == JCS_GRAYSCALE;
	dword	palette [256];

							// Get palette
	if ( cinfo.quantize_colors )
	{
		int shift = 8 - cinfo.data_precision;

		if ( cinfo.jpeg_color_space != JCS_GRAYSCALE )
		{
			for ( int i = 0; i < cinfo.actual_number_of_colors; i++ )
				palette [i] = rgbToInt ( cinfo.colormap [0][i] << shift, cinfo.colormap [1] [i] << shift, cinfo.colormap [2][i] << shift );
		}
		else
		{
			for ( int i = 0; i < cinfo.actual_number_of_colors; i++ )
				palette [i] = rgbToInt ( cinfo.colormap [0][i] << shift, cinfo.colormap [0] [i] << shift, cinfo.colormap [0][i] << shift );
		}
	}

	Texture * texture = new Texture ( width, height, 3 );

	dword * lineBuf = new dword [width];

							// Make a one-row-high sample array that will go away when done with image
	JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, rowSpan, 1);

							// while (scan lines remain to be read)
							//    jpeg_read_scanlines(...);

							// Here we use the library's state variable cinfo.output_scanline as the
							// loop counter, so that we don't have to keep track ourselves.

	int	y = 0;

	while ( cinfo.output_scanline < cinfo.output_height )
	{
							// jpeg_read_scanlines expects an array of pointers to scanlines.
							// Here the array is only one element long, but you could ask for
							// more than one scanline at a time if that's more convenient.

		jpeg_read_scanlines ( &cinfo, buffer, 1 );

		if ( cinfo.output_components == 1 )		// paletted or grayscale image
		{
			if ( cinfo.quantize_colors )		// paletted image
			{
				for ( int i = 0; i < width; i++ )
					lineBuf [i] = palette [buffer [0][i]];
			}
			else								// grayscale image
			{
				for ( int i = 0; i < width; i++ )
					lineBuf [i] = rgbToInt ( buffer [0][i], buffer [0][i], buffer [0][i] );
			}
		}
		else									// rgb image
		{
			unsigned char * ptr = buffer [0];

			for ( int i = 0; i < width; i++, ptr += 3 )
				lineBuf [i] = rgbToInt ( ptr [2], ptr [1], ptr [0] );
		}

		texture -> putLine ( height - 1 - y, lineBuf );

		y++;
	}

	delete lineBuf;

						// Finish decompression
	jpeg_finish_decompress ( &cinfo );

						// Release JPEG decompression object
	jpeg_destroy_decompress ( &cinfo );

	return texture;
}

