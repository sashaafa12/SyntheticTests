#pragma once

#include	"../3D/Vector3D.h"
#include	"../3D/Matrix3D.h"

#include	"../3D/Quaternion.h"
#include	"../3D/Transform3D.h"
#include	"../3D/Plane.h"

class SphericalCamera
{
public:
	void setViewSize ( int i_width, int i_height, float i_fov );
	void apply ();
	void    computeMatrix ();		

private:

    Vector3D		pos;				// camera position
    Vector3D		viewDir;			// viewing direction (normalized)
    Vector3D		upDir;				// up direction (normalized)
    Vector3D		sideDir;			// side direction (normalized) - may be left or right
	float			proj [16];			// projection matrix, set in apply

    int width, height, fov;
	float aspect;

    Matrix3D		transf;				// camera transform (from world to camera space)

};