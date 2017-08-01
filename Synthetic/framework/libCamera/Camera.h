//
// Camera class for OpenGL
//
// Author: Alex V. Boreskoff <steps3d@narod.ru>
//
//

#ifndef __CAMERA__
#define __CAMERA__

#include	"../3D/Vector3D.h"
#include	"../3D/Matrix3D.h"

class	Plane;
class	Transform3D;
class	Quaternion;

class   Camera
{
    Vector3D		pos;				// camera position
    Vector3D		viewDir;			// viewing direction (normalized)
    Vector3D		upDir;				// up direction (normalized)
    Vector3D		sideDir;			// side direction (normalized) - may be left or right
    									// depending on the handedness of camera
    bool			rightHanded;		// whether camera is righthanded
    bool			infinite;			// whether it uses zFar equal infinity
    Matrix3D		transf;				// camera transform (from world to camera space)
    float			fov;				// field of view angle
	float			zNear;				// near clipping z-value
	float			zFar;				// far clipping z-value
	int				width;				// view width
	int				height;				// view height
	float			aspect;				// aspect ratio of camera
	float			proj [16];			// projection matrix, set in apply

public:
    Camera ( const Vector3D& p, float yaw, float pitch, float roll,
             float aFov = 60, float nearZ = 0.1, float farZ = 100, bool rHanded = true );

    Camera ( const Vector3D& p, const Quaternion& oerintation,
             float aFov = 60, float nearZ = 0.1, float farZ = 100, bool rHanded = true );

    Camera ( const Camera& camera );
	Camera ();

    const Vector3D&   getPos () const
    {
        return pos;
    }

    const Vector3D&	getViewDir () const
    {
        return viewDir;
    }

    const Vector3D&	getSideDir () const
    {
        return sideDir;
    }

    const Vector3D&	getUpDir () const
    {
        return upDir;
    }

	bool	isRightHanded () const
	{
		return rightHanded;
	}

	float	getZNear () const
	{
		return zNear;
	}

	float	getZFar () const
	{
		return zFar;
	}

    float   getFov () const
    {
        return fov;
    }

	int	getWidth () const
	{
		return width;
	}

	int	getHeight () const
	{
		return height;
	}

	float	getAspect () const
	{
		return aspect;
	}

	Quaternion	getOrientation () const;

											// map vector from world space to camera space
	Vector3D	mapFromWorld ( const Vector3D& p ) const
	{
		Vector3D	tmp ( p - pos );

		return Vector3D ( tmp & sideDir, tmp & upDir, -(tmp & viewDir) );
	}

											// map vector from camera space to world space
	Vector3D	mapToWorld ( const Vector3D& p ) const
	{
		return pos + p.x * sideDir + p.y * upDir + p.z * viewDir;
	}

											// map vector to screen space
	Vector3D	mapToScreen ( const Vector3D& p ) const
	{
		Vector3D	scr ( transf * ( p - pos ) );

		return scr /= scr.z;				// do perspective transform
	}


											// move camera (absolute & relative)
    void    moveTo ( const Vector3D& newPos )
    {
        pos = newPos;

		computeMatrix ();					// XXX-since clipping planes must be rebuild
    }

    void    moveBy ( const Vector3D& delta )
    {
        pos += delta;

		computeMatrix ();					// XXX-since clipping planes must be rebuild
    }

	float	getProjAt ( int i, int j ) const
	{
		return proj [i+j*4];
	}
											// set orientation either via Euler angles or
											// via quaternion
    void    setEulerAngles  ( float theYaw, float thePitch, float theRoll );
    void	setOrientation  ( const Quaternion& orientation );

    										// set viewport parameters
	void	setViewSize     ( int theWidth, int theHeight, float theFov );
    void    setFov          ( float newFovAngle );
    void	setRightHanded  ( bool flag );

    										// transform camera
    void    mirror          ( const Plane& );
	void	transform       ( const Transform3D& );

	void	apply ();						// sets camera transform for current context, fill proj matrix

											// return poly (quad) for intersection of plane paraller to zNear plane with given z
	void	getPlanePolyForZ ( float z, Vector3D * poly ) const;
private:
    void    computeMatrix ();				// compute vectors, transform matrix and build
    										// viewing frustrum
};

#endif
