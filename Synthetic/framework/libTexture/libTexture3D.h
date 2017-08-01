//
// load and save 3D images from/to DDS file's
//
// Author: Alex V. Boreskoff
//

#ifndef	__LIB_TEXTURE_3D__
#define	__LIB_TEXTURE_3D__

#include	"TypeDefs.h"

class	Data;

bool		saveTexture3D   ( int width, int height, int depth, int components, const byte * data, const char * fileName );
unsigned	createTexture3D ( bool mipmap, const char * fileName );
unsigned	createTexture3D ( bool mipmap, Data * data );

#endif

