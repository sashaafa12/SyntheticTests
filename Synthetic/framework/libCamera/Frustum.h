//
// Frustum class for OpenGL
//
// Author: Alex V. Boreskoff <steps3d@narod.ru>, <alexboreskoff@mtu-net.ru>
//

#ifndef    __FRUSTUM__
#define    __FRUSTUM__

#include   "Vector3D.h"
#include	"Plane.h"

class	BoundingBox;

class      Frustum
{
	Plane	plane [6];
	
public:
	Frustum ();

	void	get ();				// acquire from current OpenGL context
	
								// checks
	bool	pointInFrustum ( const Vector3D&   v    ) const;
	bool	boxInFrustum   ( const BoundingBox& box ) const;
};

#endif

