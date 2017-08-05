//
// Camera class for OpenGL
//
// Author: Alex V. Boreskoff <steps3d@narod.ru>
//

#include	"libExt.h"
#include    "Camera.h"
#include	"Quaternion.h"
#include	"Transform3D.h"
#include	"Plane.h"

Camera :: Camera ( const Vector3D& p, float yaw, float pitch, float roll,
                   float aFov, float nearZ, float farZ, bool rHanded )
{
    fov         = aFov;
	zNear       = nearZ;
	zFar        = farZ;
    pos         = p;
	width       = 640;					// default view size
	height      = 480;
	aspect      = (float)width / (float) height;
	rightHanded = rHanded;
	infinite    = false;

    setEulerAngles ( yaw, pitch, roll );
}

Camera :: Camera ( const Vector3D& p, const Quaternion& orientation,
                   float aFov, float nearZ, float farZ, bool rHanded )
{
    fov         = aFov;
	zNear       = nearZ;
	zFar        = farZ;
    pos         = p;
	width       = 640;					// default view size
	height      = 480;
	aspect      = (float)width / (float) height;
	rightHanded = rHanded;
	infinite    = false;

    setOrientation ( orientation );
}

Camera :: Camera ( const Camera& camera )
{
    zNear       = camera.zNear;
    zFar        = camera.zFar;
    fov         = camera.fov;
    pos         = camera.pos;
    viewDir     = camera.viewDir;
    upDir       = camera.upDir;
    rightHanded = camera.rightHanded;
	infinite    = camera.infinite;
	width       = camera.width;
	height      = camera.height;
	aspect      = camera.aspect;

    computeMatrix ();
}

void    Camera :: setEulerAngles ( float yaw, float pitch, float roll )
{
    viewDir = Vector3D ( 1, 0, 0 );
    sideDir = Vector3D ( 0, 1, 0 );
    upDir   = Vector3D ( 0, 0, 1 );

    Matrix3D  rot ( Matrix3D :: rotate ( yaw, pitch, roll ) );

    viewDir = rot * viewDir;
    upDir   = rot * upDir;
    sideDir = rot * sideDir;

    computeMatrix ();
}

void	Camera :: setOrientation ( const Quaternion& orientation )
{
    viewDir = Vector3D ( 1, 0, 0 );
    sideDir = Vector3D ( 0, 1, 0 );
    upDir   = Vector3D ( 0, 0, 1 );

    Matrix3D  rot = orientation.getMatrix ();

    viewDir = rot * viewDir;
    upDir   = rot * upDir;
    sideDir = rot * sideDir;

    computeMatrix ();
}

Quaternion	Camera :: getOrientation () const
{
							// build matrix from viewDir, sideDir, upDir
	Matrix3D	rot ( viewDir, sideDir, upDir );
	
							// build quaternion from it
	return Quaternion ( rot );
}


void	Camera :: setRightHanded  ( bool flag )
{
	if ( flag == rightHanded )
		return;

	rightHanded = flag;

	computeMatrix ();
}


void    Camera :: setFov ( float newFovAngle )
{
    fov = newFovAngle;

    computeMatrix ();
}

void	Camera :: setViewSize ( int theWidth, int theHeight, float theFov )
{
	width  = theWidth;
	height = theHeight;
	fov    = theFov;
	aspect = (float)width / (float) height;

	computeMatrix ();
}

void    Camera :: computeMatrix ()
{
    viewDir.normalize ();       // normalize viewDir
                                // compute upDir perpendicular to viewDir
    upDir -= (upDir & viewDir) * viewDir;

                                // compute sideDir to form orthogonal basis
    sideDir = upDir ^ viewDir;

    if ( !rightHanded )			// invert side orientation: right -> left
       sideDir = -sideDir;

    upDir  .normalize ();      // orthonormalize upDir and sideDir
    sideDir.normalize ();

                                // now build transform matrix
	float	halfAngle = 0.5f * M_PI * fov / 180.0f;
    float	fovValueY = 1 / (float)tan ( halfAngle );
	float	fovValueX = fovValueY / aspect;

    transf [0][0] = (sideDir [0] * fovValueX + viewDir [0]) * 0.5f * width;	
    transf [0][1] = (sideDir [1] * fovValueX + viewDir [1]) * 0.5f * width;
    transf [0][2] = (sideDir [2] * fovValueX + viewDir [2]) * 0.5f * width;
    transf [1][0] = (upDir   [0] * fovValueY + viewDir [0]) * 0.5f * height;
    transf [1][1] = (upDir   [1] * fovValueY + viewDir [1]) * 0.5f * height;
    transf [1][2] = (upDir   [2] * fovValueY + viewDir [2]) * 0.5f * height;
    transf [2][0] = viewDir  [0];
    transf [2][1] = viewDir  [1];
    transf [2][2] = viewDir  [2];
}

void    Camera :: mirror ( const Plane& mirror )
{
    mirror.reflectPos   ( pos );
    mirror.reflectDir   ( viewDir );
    mirror.reflectDir   ( upDir );
    mirror.reflectDir   ( sideDir );

    rightHanded = !rightHanded;

    computeMatrix ();
}

void	Camera :: transform ( const Transform3D& tr )
{
	pos     = tr.transformPoint ( pos     );
	viewDir = tr.transformDir   ( viewDir );
	upDir   = tr.transformDir   ( upDir   );
	sideDir = tr.transformDir   ( sideDir );

							// now check for right/left handedness
	rightHanded = ((upDir ^ viewDir) & sideDir) > EPS;

	computeMatrix ();
}

void	Camera :: apply ()
{
	float		m [16];
													// create a coordinate space transform
													// (rotation) matrix
	m [0 ] = sideDir.x;
	m [1 ] = upDir.x;
	m [2 ] = -viewDir.x;
	m [3 ] = 0;

	m [4 ] = sideDir.y;
	m [5 ] = upDir.y;
	m [6 ] = -viewDir.y;
	m [7 ] = 0;

	m [8 ] = sideDir.z;
	m [9 ] = upDir.z;
	m [10] = -viewDir.z;
	m [11] = 0;

	m [12] = 0;
	m [13] = 0;
	m [14] = 0;
	m [15] = 1;

														// set current viewport
	glViewport     ( 0, 0, getWidth (), getHeight () );

	glMatrixMode   ( GL_PROJECTION );					// select projection matrix
	glLoadIdentity ();									// reset projection matrix

														// calculate aspect ratio of the window
	gluPerspective ( getFov (), getAspect (), getZNear (), getZFar () );

	glMatrixMode   ( GL_MODELVIEW );					// select modelview matrix
	glLoadIdentity ();									// reset modelview matrix

	glMultMatrixf ( m );
	glTranslatef  ( -pos.x, -pos.y, -pos.z );
	
	glGetFloatv ( GL_PROJECTION_MATRIX, proj );
}

void	Camera :: getPlanePolyForZ ( float z, Vector3D * poly ) const
{
	float		halfAngle = 0.5f * M_PI * fov / 180.0f;
    float		f         = 1 / (float)tan ( halfAngle );
	Vector3D	base ( z * aspect / f, z / f, z );
	
	poly [0] = pos + base.z * viewDir + base.x * sideDir + base.y * upDir;
	poly [1] = pos + base.z * viewDir + base.x * sideDir - base.y * upDir; //Vector3D (  base.x, -base.y, base.z );
	poly [2] = pos + base.z * viewDir - base.x * sideDir - base.y * upDir;//Vector3D ( -base.x, -base.y, base.z );
	poly [3] = pos + base.z * viewDir - base.x * sideDir + base.y * upDir;//Vector3D ( -base.x,  base.y, base.z );
}
