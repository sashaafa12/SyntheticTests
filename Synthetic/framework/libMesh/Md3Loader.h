//
// Class to load md3 files (Quake III)
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#ifndef	__MD3_LOADER__
#define	__MD3_LOADER__

#ifdef	_WIN32
	#pragma	warning (disable:4786)
#endif

#include	<map>
#include	<string>

using namespace std;

#include	"Vector3D.h"

class	Data;
class	Mesh;
class	MeshNode;

class	Md3Loader
{
public:
	Md3Loader ();

	MeshNode * load ( Data * data, Data * skin );

private:
	static	Vector3D	normalTable [256][256];
	static	bool		normalTableInited;

										// get mapping from mesh names to textures
	void	parseSkin ( Data * skin, map <string,string>& textures );
};

#endif
