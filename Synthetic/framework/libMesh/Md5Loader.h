//
// Simple class to load Doom III md5mesh files
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#ifndef	__MD5_LOADER__
#define	__MD5_LOADER__

#ifdef	_WIN32
	#pragma	warning (disable:4786)
#endif

#include	<string>

using namespace std;

class	Data;
class	Mesh;
class	MeshNode;

class	Md5Loader
{
public:
	Md5Loader () {}

	MeshNode * load ( Data * data, const string& modelName );
};

#endif