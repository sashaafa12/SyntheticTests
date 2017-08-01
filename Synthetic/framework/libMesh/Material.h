//
// Basic wrapper about material (as a set of textures with mapping)
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#ifndef	__MATERIAL__
#define	__MATERIAL__

#ifdef	_WIN32
	#pragma	warning (disable:4786)
	#include	<windows.h>
#endif

#ifdef	MACOSX
	#include	<OpenGL/gl.h>
#else
	#include	<GL/gl.h>
#endif

#include	<string>
#include	"Vector2D.h"
#include	"Vector4D.h"

using namespace std;

class Map									// map info
{
public:
	string		mapName;					// file with texture
	int			unit;						// texture unit to place this texture into
	GLuint	    id;							// texture id
	GLenum		target;						// texture target
	bool		mipmap;						// whether to use mipmapping
	Vector2D	texOffs;
	Vector2D	texScale;

	Map  ();
	~Map ();

	bool	load             ();
	bool	setTextureMatrix ();
	bool	bind             ();

	void	bindToUnit ( int theUnit )
	{
		unit = theUnit;
	}

};

class	Material
{
public:
	string		name;
	Vector4D	color;						// defalt color
	Map			diffuse;
	Map			specular;
	Map			reflection;
	Map			bump;
	Map			tex1;
	Map			tex2;
	Map			tex3;
	Map			tex4;

	Material ();
	
	void	load ();						// load all maps
	void	bind ();						// bind all maps to texture units
	void	setPath ( const string& path );	// update texture file ref with correct path
};

#endif
