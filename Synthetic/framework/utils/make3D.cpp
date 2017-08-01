//
// Simple utility to build 3D texture (in dds format) from a set of 2D textures
//
// Author: Alex V. Boreskoff <steps3d@narod.ru>, <alexboreskoff@mtu-net.ru>
//

#ifdef	_WIN32
    #pragma	warning (disable:4786)		// FUCK these stuppid bastards !!!
#endif

#include	<memory.h>
#include	<stdio.h>
#include	"libTexture.h"
#include	"libTexture3D.h"
#include	"Texture.h"

#include	<list>

using namespace	std;

void	usage ()
{
	printf ( "Usage:\n\tmake3D out-name in-format\n\ng.e. make3D tex3D tex2D-%02d\n" );
}
	
int main ( int argc, char * argv [] )
{
	if ( argc != 3 )
	{
		usage ();
		
		return 1;
	}

	char 			name [256];
	list<Texture *>	textures;
	int				i;
	
	for ( i = 0; ; i++ )
	{
		sprintf ( name, argv [2], i );
		
		if ( !fileExist ( name ) )
			break;
			
		Texture * inTexture = getTexture ( name );
		
		if ( inTexture == NULL )
			break;
			
		textures.push_front ( inTexture );
	}
	
	if ( i < 1 )
	{
		printf ( "No textures found.\n" );
		
		return 2;
	}
	
	Texture * tex0          = *textures.begin ();
	int		  width         = tex0 -> getWidth  ();
	int 	  height        = tex0 -> getHeight ();
	int		  numComponents = tex0 -> getNumComponents ();
	
										// validate all textures have the same size and numComponents
	list <Texture *> :: iterator it;
	
	for ( it = textures.begin (); it != textures.end (); ++it )
	{
		Texture * tex = *it;
		
		if ( tex -> getWidth () != width || tex -> getHeight () != height || tex -> getNumComponents () != numComponents )
		{
			printf ( "Not all textures have the same format.\n" );
			
			return 3;
		}
	}
	
	int	   depth   = textures.size  ();
	byte * buf     = new byte [width * height * depth * numComponents];
	byte * ptr     = buf;
	int	   texSize = width * height * numComponents;
	
	for ( it = textures.begin (); it != textures.end (); ++it )
	{
		Texture * tex = *it;
		
		memcpy ( ptr, tex -> getData (), texSize );
		
		ptr += texSize;
	}
	
	if ( !saveTexture3D ( width, height, depth, numComponents, buf, argv [1] ) )
	{
		printf ( "Error making %s.\n", argv [1] );
		
		return 4;
	}
	
	delete buf;

	printf ( "3D-texture %s successfully created. Size %dx%dx%d\n", argv [1], width, height, depth );
	
	return 0;
}
