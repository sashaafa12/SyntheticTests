//
// Frustum class for OpenGL
//
// Author: Alex V. Boreskoff <steps3d@narod.ru>, <alexboreskoff@mtu-net.ru>
//

#include	"libExt.h"
#include	"Frustum.h"
#include	"Math3D.h"
#include	"BoundingBox.h"

Frustum :: Frustum ()
{
	get ();
}

void	Frustum :: get ()				// acquire from current OpenGL context
{
	float	proj  [16];						// projection matrix
	float	model [16];						// modelview matrix
	float	clip  [16];						// transform to clip space matrix

	glGetFloatv ( GL_PROJECTION_MATRIX, proj  );
	glGetFloatv ( GL_MODELVIEW_MATRIX,  model );

											// now multiply them to get clip transform matrix
	clip[ 0] = model[ 0] * proj[ 0] + model[ 1] * proj[ 4] + model[ 2] * proj[ 8] + model[ 3] * proj[12];
	clip[ 1] = model[ 0] * proj[ 1] + model[ 1] * proj[ 5] + model[ 2] * proj[ 9] + model[ 3] * proj[13];
	clip[ 2] = model[ 0] * proj[ 2] + model[ 1] * proj[ 6] + model[ 2] * proj[10] + model[ 3] * proj[14];
	clip[ 3] = model[ 0] * proj[ 3] + model[ 1] * proj[ 7] + model[ 2] * proj[11] + model[ 3] * proj[15];

	clip[ 4] = model[ 4] * proj[ 0] + model[ 5] * proj[ 4] + model[ 6] * proj[ 8] + model[ 7] * proj[12];
	clip[ 5] = model[ 4] * proj[ 1] + model[ 5] * proj[ 5] + model[ 6] * proj[ 9] + model[ 7] * proj[13];
	clip[ 6] = model[ 4] * proj[ 2] + model[ 5] * proj[ 6] + model[ 6] * proj[10] + model[ 7] * proj[14];
	clip[ 7] = model[ 4] * proj[ 3] + model[ 5] * proj[ 7] + model[ 6] * proj[11] + model[ 7] * proj[15];

	clip[ 8] = model[ 8] * proj[ 0] + model[ 9] * proj[ 4] + model[10] * proj[ 8] + model[11] * proj[12];
	clip[ 9] = model[ 8] * proj[ 1] + model[ 9] * proj[ 5] + model[10] * proj[ 9] + model[11] * proj[13];
	clip[10] = model[ 8] * proj[ 2] + model[ 9] * proj[ 6] + model[10] * proj[10] + model[11] * proj[14];
	clip[11] = model[ 8] * proj[ 3] + model[ 9] * proj[ 7] + model[10] * proj[11] + model[11] * proj[15];

	clip[12] = model[12] * proj[ 0] + model[13] * proj[ 4] + model[14] * proj[ 8] + model[15] * proj[12];
	clip[13] = model[12] * proj[ 1] + model[13] * proj[ 5] + model[14] * proj[ 9] + model[15] * proj[13];
	clip[14] = model[12] * proj[ 2] + model[13] * proj[ 6] + model[14] * proj[10] + model[15] * proj[14];
	clip[15] = model[12] * proj[ 3] + model[13] * proj[ 7] + model[14] * proj[11] + model[15] * proj[15];

											// now extract clip planes params:
	Vector3D	n [6];
	float		d [6];
											// right plane
	n [0].x = clip [3]  - clip [0];
	n [0].y = clip [7]  - clip [4];
	n [0].z = clip [11] - clip [8];
	d [0]   = clip [15] - clip [12];

											// left plane
	n [1].x = clip [3]  + clip [0];
	n [1].y = clip [7]  + clip [4];
	n [1].z = clip [11] + clip [8];
	d [1]   = clip [15] + clip [12];

											// top plane
	n [2].x = clip [3]  - clip [1];
	n [2].y = clip [7]  - clip [5];
	n [2].z = clip [11] - clip [9];
	d [2]   = clip [15] - clip [13];

											// bottom plane
	n [3].x = clip [3]  + clip [1];
	n [3].y = clip [7]  + clip [5];
	n [3].z = clip [11] + clip [9];
	d [3]   = clip [15] + clip [13];

											// far plane
	n [4].x = clip [3]  - clip [2];
	n [4].y = clip [7]  - clip [6];
	n [4].z = clip [11] - clip [10];
	d [4]   = clip [15] - clip [14];

											// near plane
	n [5].x = clip [3]  + clip [2];
	n [5].y = clip [7]  + clip [6];
	n [5].z = clip [11] + clip [10];
	d [5]   = clip [15] + clip [14];

											// normalize
	for ( int i = 0; i < 6; i++ )
	{
		float	len = n [i].length ();

		if ( len > EPS )
		{
			n [i] /= len;
			d [i] /= len;
		}

		plane [i] = Plane ( n [i], d [i] );
	}	
}

bool	Frustum :: pointInFrustum ( const Vector3D& v ) const
{
	for ( register int i = 0; i < 6; i++ )
		if ( plane [i].classify ( v ) == IN_BACK )
			return false;

	return true;
}

bool	Frustum :: boxInFrustum ( const BoundingBox& box ) const
{
	for ( register int i = 0; i < 6; i++ )
		if ( box.classify ( plane [i] ) == IN_BACK )
			return false;
			
	return true;
}
