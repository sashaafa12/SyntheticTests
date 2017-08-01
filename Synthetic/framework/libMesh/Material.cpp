//
// Basic wrapper about material (as a set of textures with mapping)
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#ifdef	_WIN32
	#include	<windows.h>
#endif

#ifdef	MACOSX
	#include	<OpenGL/gl.h>
#else
	#include	<GL/gl.h>
#endif

#include	<stdio.h>
#include	"libExt.h"
#include	"libTexture.h"
#include	"libTexture3D.h"
#include	"Material.h"

Map :: Map ()
{
	unit     = 0;						// assign to unit 0
	id       = 0;						// illegal id
	target   = GL_TEXTURE_2D;			// 2D texture by default
	mipmap   = true;
	texOffs  = Vector2D ( 0, 0 );		// empty texture transform
	texScale = Vector2D ( 1, 1 );
}

Map :: ~Map ()
{
	if ( id != 0 )
		glDeleteTextures ( 1, &id );
}

bool	Map :: load ()
{
	id = 0;

	if ( mapName.empty () )			// since no texture specified it's ok
		return true;

	if ( target == GL_TEXTURE_1D )
		id = createTexture1D ( mipmap, mapName.c_str () );
	else
	if ( target == GL_TEXTURE_2D )
		id = createTexture2D ( mipmap, mapName.c_str () );
	else
	if ( target == GL_TEXTURE_3D )
		id = createTexture3D ( mipmap, mapName.c_str () );

	return id != 0;
}

bool	Map :: setTextureMatrix ()
{
	if ( id == 0 || unit == -1 )
		return false;

	glMatrixMode   ( GL_TEXTURE );
	glLoadIdentity ();
	glScalef       ( texScale.x, texScale.y, 1 );
	glTranslatef   ( texOffs.x,  texOffs.y,  0 );

	return true;
}

bool	Map :: bind ()
{
	if ( id == 0 || unit == -1 )
		return false;

	glActiveTextureARB ( GL_TEXTURE0_ARB + unit );
	glEnable           ( target );
	glBindTexture      ( target, id );

	return setTextureMatrix ();
}

Material :: Material ()
{
	color = Vector4D ( 1, 1, 1, 1 );
}

void	Material :: load ()						// load all maps
{
	diffuse.   load ();
	specular.  load ();
	reflection.load ();
	bump.      load ();
	tex1.      load ();
	tex2.      load ();
	tex3.      load ();
	tex4.      load ();
}

void	Material :: bind ()						// bind all maps to texture units
{
	glColor4fv ( color );
	
	diffuse.   bind ();
	specular.  bind ();
	reflection.bind ();
	bump.      bind ();
	tex1.      bind ();
	tex2.      bind ();
	tex3.      bind ();
	tex4.      bind ();
}

void	Material :: setPath ( const string& path )	// update texture file ref with correct path
{
	if ( diffuse.mapName != "" )
		diffuse.mapName = buildFileName ( path, diffuse.mapName );

	if ( specular.mapName != "" )
		specular.mapName = buildFileName ( path, specular.mapName );

	if ( reflection.mapName != "" )
		reflection.mapName = buildFileName ( path, reflection.mapName );

	if ( bump.mapName != "" )
		bump.mapName = buildFileName ( path, bump.mapName );

	if ( tex1.mapName != "" )
		tex1.mapName = buildFileName ( path, tex1.mapName );

	if ( tex2.mapName != "" )
		tex2.mapName = buildFileName ( path, tex2.mapName );

	if ( tex3.mapName != "" )
		tex3.mapName = buildFileName ( path, tex3.mapName );

	if ( tex4.mapName != "" )
		tex4.mapName = buildFileName ( path, tex4.mapName );
}

