//
// Mesh utils - create some common meshes
//

#ifndef	__MESH_UTILS__
#define	__MESH_UTILS__

#include	"Mesh.h"

Mesh * makeTorus         ( float r1, float r2, int n1, int n2 );
Mesh * makeShape1        ( float r, float h, int n1, int n2, float factor );
Mesh * makeRevSurface    ( const Vector2D d [], float tex [], int n, int n2 );
Mesh * makeLathedSurface ( const Vector3D line [], int n1, const Vector2D contour [], int n2, const Vector2D& center, const Vector3D& up );

								// rebuild vertices array
Vertex * fixFaces ( Vertex * vertices, int& numVertices, Face * faces, Face * texFaces, int numFaces, Vector2D * texCoord, int numTexCoords );

#endif
