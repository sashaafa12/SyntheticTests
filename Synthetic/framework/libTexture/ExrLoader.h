//
// Class to load images from EXR files using OpenEXR library
//
// Author: Alex V. Boreskoff
//

#ifndef	__EXR_LOADER__
#define	__EXR_LOADER__

#include	"TextureLoader.h"

class	ExrLoader : public TextureLoader
{
public:
	ExrLoader () : TextureLoader () {}

	virtual	Texture * load        ( Data * data );
//	bool	  		  loadCubemap ( Data * data, Texture * images [6] );
};

#endif
