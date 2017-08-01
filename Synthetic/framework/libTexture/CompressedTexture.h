//
// Compressed texture class
//

#ifndef	__COMPRESSED_TEXTURE__
#define	__COMPRESSED_TEXTURE__

#include	"Texture.h"

class	CompressedTexture : public Texture
{
public:
	CompressedTexture ( int theWidth, int theHeight, int theNumComponents, int theFormat, int theLevels, int theSize );

	virtual	bool	upload ( int target, bool mipmap = true );
};

#endif
