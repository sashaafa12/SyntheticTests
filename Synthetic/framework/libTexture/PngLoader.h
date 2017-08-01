//
// Class to load images from PNG files
//
// Author: Alex V. Boreskoff
//

#ifndef	__PNG_LOADER__
#define	__PNG_LOADER__

#include	"TextureLoader.h"

class	PngLoader : public TextureLoader
{
public:
	PngLoader () : TextureLoader () {}

	virtual	Texture * load ( Data * data );
};

#endif
