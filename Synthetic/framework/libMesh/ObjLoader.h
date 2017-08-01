//
// Class to load Wavefront's obj files
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#ifndef	__OBJ_LOADER__
#define	__OBJ_LOADER__

#include	<string>

using namespace std;

class	Data;
class	Mesh;
class	MeshNode;

class	ObjLoader
{
public:
	ObjLoader () {}

	MeshNode * load ( Data * data );

protected:
	int	parseF ( const string& args, int * vi, int * ti ) const;
};

#endif
