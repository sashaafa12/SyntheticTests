//
// Simple library to load basic textures format
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#ifndef	__LIB_TEXTURE__
#define	__LIB_TEXTURE__

#ifdef	_WIN32
	#pragma	warning (disable:4786)
#endif

#include	<string>
#include	"TypeDefs.h"

using namespace std;

class	FileSystem;
class	Data;
class	Texture;
class	TextureLoader;
							// create corresponding textures from file (or build them)
unsigned	createNormalizationCubemap ( int cubeSize, bool mipmap = false );

unsigned	createCubeMap     ( bool mipmap, const char * maps [] );
unsigned	createCubeMap     ( bool mipmap, const char * fileName );
unsigned	createTexture2D   ( bool mipmap, const char * fileName );
unsigned	createTexture1D   ( bool mipmap, const char * fileName );
unsigned	createTextureRect ( const char * fileName );

							// create normalmaps from heightmap and from
							// from normal map textures
unsigned	createNormalMapFromHeightMap       ( bool mipmap, const char * fileName, float scale );
unsigned	createNormalMap                    ( bool mipmap, const char * fileName );
unsigned	createNormalHeightMapFromHeightMap ( bool mipmap, const char * fileName, float scale );

							// load texture from file into given texture target
bool		loadTexture ( int target, bool mipmap, const char * fileName );

							// load texture into object (not in OpenGL)
Texture   * getTexture ( const char * fileName );

							// simple check whether the file exists
bool		fileExist ( const char * fileName );

								// extract path from a filename
string		getPath ( const string& fullName );

							// extract filename and extension
string		getFileName ( const string& fullName );

							// get only name without extension
string		getName ( const string& fullName );

							// build filename from path and a name
string		buildFileName ( const string& path, const string& file );

							// save contents of the window in a TGA file
bool		saveScreenShot ( const char * fileName );

							// add file system
bool		addFileSystem ( FileSystem * fileSystem );

							// add zip file source
bool		addZipFileSystem ( const char * fileName );

							// add rar file source
bool		addRarFileSystem ( const char * fileName );

							// add new search path
bool		addSearchPath ( const char * path );

							// add decoder for texture format
bool		addDecoder ( const char * ext, TextureLoader * decoder );

							// get file contents (possibly scanning several sources)
Data * 		getFile ( const char * fileName );

#endif
