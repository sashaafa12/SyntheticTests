//
// Class to load images from jpeg files
//
// Author: Alex V. Boreskoff
//

#ifndef	__JPEG_LOADER__
#define	__JPEG_LOADER__

#include	"TextureLoader.h"

class	JpegLoader : public TextureLoader
{
public:
	JpegLoader () : TextureLoader () {}

	virtual	Texture * load ( Data * data );
};

#endif
