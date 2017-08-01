//
// Class to load images from DDS files
//
// Author: Alex V. Boreskoff
//

#ifndef	__DDS_LOADER__
#define	__DDS_LOADER__

#include	"TextureLoader.h"

struct	DDS_HEADER;

class	DdsLoader : public TextureLoader
{
public:
	DdsLoader () : TextureLoader () {}

	virtual	Texture * load        ( Data * data );
	bool	  		  loadCubemap ( Data * data, Texture * images [6] );

	bool	loadUncompressed8BitRGBData   ( Data * data, const DDS_HEADER& ddsd, int component, int bytesPerLine, byte * ptr );
	bool	loadUncompressed8BitAlphaData ( Data * data, const DDS_HEADER& ddsd, int bytesPerLine, byte * ptr );
	bool	loadUncompressed16BitRGBData  ( Data * data, const DDS_HEADER& ddsd, int bytesPerLine, byte * ptr );
	
protected:
	Texture * loadUncompressed          ( Data * data, const DDS_HEADER& ddsd, int numComponents );
	Texture * loadUncompressed8BitRGB   ( Data * data, const DDS_HEADER& ddsd, int component );
	Texture * loadUncompressed8BitAlpha ( Data * data, const DDS_HEADER& ddsd );
	Texture * loadUncompressed16BitRGB  ( Data * data, const DDS_HEADER& ddsd );
	void	  skipMipmaps               ( Data * data, int w, int h, int bytesPerLine, int cnt, int mipmapsCount );
};

#endif
