//
// Simple class to load 3DStudio Max 3DS files
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#ifndef	__3DS_LOADER__
#define	__3DS_LOADER__

class	Data;
class	Mesh;
class	MeshNode;

class	ThreeDsLoader
{
public:
	ThreeDsLoader () {}

	MeshNode * load ( Data * data );
};

#endif