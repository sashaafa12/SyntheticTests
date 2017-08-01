//
// Simple class to load 3DStudio Max ASE files
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#ifndef	__ASE_LOADER__
#define	__ASE_LOADER__

class	Data;
class	Mesh;
class	MeshNode;

class	AseLoader
{
public:
	AseLoader () {}

	MeshNode * load ( Data * data );
};

#endif
