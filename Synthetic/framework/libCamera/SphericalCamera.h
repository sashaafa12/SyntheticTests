#pragma once

#include	"../3D/Vector3D.h"
#include	"../3D/Matrix3D.h"

#include	"../3D/Quaternion.h"
#include	"../3D/Transform3D.h"
#include	"../3D/Plane.h"

class SphericalCamera
{
public:
	//SphericalCamera(Vector3D i_pos, Vector3D i_viewDir, Vector3D i_upDir, Vector3D i_sideDir)
	//{
	//	setViewSize (800, 600, 45);
	//	
	//	viewDir = i_viewDir; 
	//	upDir = i_upDir; 
	//	sideDir = i_sideDir;
	//	pos = i_pos;
	//}

	SphericalCamera(float i_ro, float i_phi, float i_theta);

	void setViewSize ( int i_width, int i_height, float i_fov );
	void apply ();
	void    computeMatrix ();		
	
	void changePhi(float i_delta);
	void changeTheta(float i_delta);

private:

    Vector3D		pos;				// camera position
    Vector3D		viewDir;			// viewing direction (normalized)
    Vector3D		upDir;				// up direction (normalized)
    Vector3D		sideDir;			// side direction (normalized) - may be left or right
	float			proj [16];			// projection matrix, set in apply

    int width, height, fov;
	float aspect;

	float ro, phi, theta;
	void updateCamera();

    Matrix3D		transf;				// camera transform (from world to camera space)

};