//
// Class to load images from TGA files
//
// Author: Alex V. Boreskoff
//

#ifndef	__TGA_LOADER__
#define	__TGA_LOADER__

#include	"TextureLoader.h"

class	TgaLoader : public TextureLoader
{
public:
	TgaLoader () : TextureLoader () {}

	virtual	Texture * load ( Data * data );

private:
	bool	loadRGB8  ( Data * data, Texture * tex, dword * buf, dword palette [], bool vertInvert );
	bool	loadRGB15 ( Data * data, Texture * tex, dword * buf, bool vertInvert );
	bool	loadRGB24 ( Data * data, Texture * tex, dword * buf, bool vertInvert );
	bool	loadRGB32 ( Data * data, Texture * tex, dword * buf, bool vertInvert );
	bool	loadRLE8  ( Data * data, Texture * tex, dword * buf, dword palette [], bool vertInvert );
	bool	loadRLE15 ( Data * data, Texture * tex, dword * buf, bool vertInvert );
	bool	loadRLE24 ( Data * data, Texture * tex, dword * buf, bool vertInvert );
	bool	loadRLE32 ( Data * data, Texture * tex, dword * buf, bool vertInvert );
};

#endif
