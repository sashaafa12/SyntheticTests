//
// Abstract class to load texture images
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#ifndef	__TEXTURE_LOADER__
#define	__TEXTURE_LOADER__

#include	"TypeDefs.h"

class	Data;
class	Texture;

class	TextureLoader
{
public:
	TextureLoader () {}
	virtual ~TextureLoader () {}

	virtual	Texture * load ( Data * data ) = 0;
};

#endif
