//
// Class for simple plane (p,n) + dist = 0
//
// Author: Alex V. Boreskoff
//

#include    "Plane.h"
#include	"Math3D.h"

void    Plane :: computeNearPointMaskAndMainAxis ()
{
	nearPointMask = computeNearPointMask ( n );
    mainAxis      = n.getMainAxis ();
}
