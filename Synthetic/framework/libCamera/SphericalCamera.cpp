#include	"libExt.h"
#include    "SphericalCamera.h"
#include	"Quaternion.h"
#include	"Transform3D.h"
#include	"Plane.h"
#include "../3D/Matrix4x4.h"


void	SphericalCamera :: setViewSize ( int theWidth, int theHeight, float theFov )
{
	width  = theWidth;
	height = theHeight;
	fov    = theFov;
	aspect = (float)width / (float) height;

	computeMatrix ();
}

SphericalCamera :: SphericalCamera(float i_ro, float i_phi, float i_theta)
{
	ro = i_ro;
	phi = i_phi;
	theta = i_theta;

	updateCamera();
	setViewSize (800, 600, 45);
}

void SphericalCamera :: updateCamera()
{
	float x = sin(phi) * cos(theta) * ro;
	float z = cos(phi) * cos(theta) * ro;
	float y = sin(theta) * ro;

	pos = Vector3D(x, y, z);
	viewDir = (-pos);
	viewDir.normalize();
	upDir = Vector3D(0, 1, 0);
	sideDir = (viewDir ^ upDir).normalize();
	upDir = (sideDir ^ viewDir).normalize();
}

void    SphericalCamera :: computeMatrix ()
{
    viewDir.normalize ();       // normalize viewDir
                                // compute upDir perpendicular to viewDir
    upDir -= (upDir & viewDir) * viewDir;

                                // compute sideDir to form orthogonal basis
    sideDir = upDir ^ viewDir;
	    
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

extern float transformProjection[16];
extern float transformModelView[16];
extern Matrix4x4 transformMatrix;

void	SphericalCamera :: apply ()
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
	glViewport     ( 0, 0, width, height);

	glMatrixMode   ( GL_PROJECTION );					// select projection matrix
	glLoadIdentity ();									// reset projection matrix
															// calculate aspect ratio of the window
	gluPerspective ( fov, aspect, 0.1, 100);

	glGetFloatv ( GL_PROJECTION_MATRIX, transformProjection );

	glMatrixMode   ( GL_MODELVIEW );					// select modelview matrix
	glLoadIdentity ();									// reset modelview matrix

	glMultMatrixf ( m );
	glTranslatef  ( -pos.x, -pos.y, -pos.z );
	
	glGetFloatv ( GL_MODELVIEW_MATRIX, transformModelView);

	/*for(int i = 0; i < 4; i++)
		memcpy(transformMatrix.m[i], &transformModelView[4 * i], 4 * sizeof(float));
	*/

	glGetFloatv ( GL_MODELVIEW_PROJECTION_NV, transformModelView);

	//transformMatrix = transformMatrix.inverse();
}

void SphericalCamera :: changePhi(float i_delta)
{
	phi += i_delta;
	updateCamera();
	apply ();
}

void SphericalCamera :: changeTheta(float i_delta)
{
	theta += i_delta;
	updateCamera();
	apply ();
}
