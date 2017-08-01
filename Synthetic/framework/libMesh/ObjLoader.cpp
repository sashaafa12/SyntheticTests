//
// Class to load Wavefront's obj files
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#include	<assert.h>
#include	<stdio.h>

#include	"ObjLoader.h"
#include	"Mesh.h"
#include	"MeshNode.h"
#include	"MeshUtils.h"
#include	"Data.h"
#include	"Matrix3D.h"
#include	"libTexture.h"
#include	"StringUtils.h"

#include	<map>

#define	DELTA	100

template <typename T>
void	reallocArray ( T *& ptr, int& size, int delta )
{
	T * newPtr = new T [size + delta];

	if ( ptr != NULL )
		memcpy ( newPtr, ptr, sizeof ( T ) * size );

	delete ptr;

	ptr   = newPtr;
	size += delta;
}

MeshNode * ObjLoader :: load ( Data * data )
{
	MeshNode  * out = new MeshNode ( NULL );
	Vector3D	v;
	Vector2D	t;
	int			vi [30];
	int			ti [30];
	string		str, cmd, args;
	bool		res           = true;
	bool		hasTexCoords  = false;
	char		lastCommand   = '\0';
	Vertex    * vertices      = NULL;
	int			numVertices   = 0;
	int			maxVertices   = 0;
	Vector2D  * texCoords     = NULL;
	int			numTexCoords  = 0;
	int			maxTexCoords  = 0;
	Face      * faces         = NULL;
	Face      * texFaces      = NULL;
	int			numFaces      = 0;
	int			maxFaces      = 0;
	int			vertexIndex   = 0;
	int			texIndex      = 0;
	int			faceIndex     = 0;
	int			startVertex   = 1;
	int			startTexCoord = 1;
	int			startFace     = 1;

	map <int, Material>	materials;

	while ( data -> getString ( str, '\n' ) )
	{
		str = trim ( str );

		if ( str.empty () || str [0] == '#' )			// skip empty lines and comments
			continue;

		parseString ( str, cmd, args );
														// check for start of next mesh
		if ( cmd [0] == 'v' && (lastCommand == 'f' || lastCommand == 'g') )	
		{
			Mesh * mesh;

			if ( texFaces == NULL )
				mesh = new Mesh ( "", vertices, numVertices, faces, numFaces, true );
			else
			{
				int		 n           = numVertices;
				Vertex * outVertices = fixFaces ( vertices, n, faces, texFaces, numFaces, texCoords, numTexCoords );

				mesh = new Mesh ( "", outVertices, n, faces, numFaces, true );

				delete outVertices;
			}

			out -> attach ( "", mesh, Vector3D ( 0, 0, 0 ), Matrix3D ( 1 ) );

			startVertex   += numVertices;
			startTexCoord += numTexCoords;
			startFace     += numFaces;
			numVertices    = 0;
			numTexCoords   = 0;
			numFaces       = 0;
			vertexIndex    = 0;
			texIndex       = 0;
			faceIndex      = 0;
		}

		if ( cmd == "v" )								// get vertex
		{
			sscanf ( args.c_str (), "%f %f %f", &v.x, &v.y, &v.z );

			lastCommand = 'v';

			if ( vertexIndex >= numVertices )
				numVertices ++;

			if ( numVertices >= maxVertices )
				reallocArray ( vertices, maxVertices, DELTA );

			vertices [vertexIndex++].pos = Vector4D ( v, 1 );
		}
		else
		if ( cmd == "vt" )								// get texcoord
		{
			hasTexCoords = true;

			sscanf ( args.c_str (), "%f %f", &t.x, &t.y );

			lastCommand = 'v';

			if ( texIndex >= numTexCoords )
				numTexCoords ++;

			if ( numTexCoords >= maxTexCoords )
				reallocArray ( texCoords, maxTexCoords, DELTA );

			texCoords [texIndex++] = t;
		}
		else
/*		if ( cmd == "vn" )								// get normal
		{
			sscanf ( args.c_str (), "%f %f %f", &v.x, &v.y, &v.z );

			lastCommand = 'v';

			if ( index >= numVertices )
				numVertices ++;

			if ( numVertices >= maxVertices )
				reallocArray ( vertices, maxVertices, DELTA );

			vertices [index++].n = v;
		}
		else
*/		if ( cmd == "f" )								// check for # of vertices
		{												// split by spaces into groups
														// nn/nn/nn group cannot contain spaces
			int	cnt  = parseF ( args, vi, hasTexCoords ? ti : NULL );
			int	fcnt = cnt - 2;							// number of faces to add

			lastCommand = 'f';

			if ( faceIndex + fcnt > numFaces )
				numFaces = faceIndex + fcnt;

			if ( numFaces >= maxFaces )
			{
				reallocArray ( faces, maxFaces, DELTA );

				if ( hasTexCoords )
				{
					reallocArray ( texFaces, maxFaces, DELTA );

					maxFaces -= DELTA;
				}
			}
											// add faces
			for ( int i = 0; i < fcnt; i++, faceIndex++ )
			{
				faces    [faceIndex].index [0] = vi [0]   - startVertex;
				faces    [faceIndex].index [1] = vi [i+1] - startVertex;
				faces    [faceIndex].index [2] = vi [i+2] - startVertex;

				if ( hasTexCoords )
				{
					texFaces [faceIndex].index [0] = ti [0] - startTexCoord;
					texFaces [faceIndex].index [1] = ti [i+1] - startTexCoord;
					texFaces [faceIndex].index [2] = ti [i+2] - startTexCoord;
				}
			}
		}
	}

	if ( numVertices > 0 && numFaces > 0  )				// check for last mesh
	{
		Mesh * mesh;

		if ( texFaces == NULL )
			mesh = new Mesh ( "", vertices, numVertices, faces, numFaces, true );
		else
		{
			int		 n           = numVertices;
			Vertex * outVertices = fixFaces ( vertices, n, faces, texFaces, numFaces, texCoords, numTexCoords );

			mesh = new Mesh ( "", outVertices, n, faces, numFaces, true );

			delete outVertices;
		}

		out -> attach ( "", mesh, Vector3D ( 0, 0, 0 ), Matrix3D ( 1 ) );
	}

	delete vertices;
	delete faces;
	delete texFaces;

	return out;
}

int	ObjLoader :: parseF ( const string& str, int * vi, int * ti ) const
{
	int		index = 0;
	string	s = str;
	string	arg1, args;

	for ( ; ; s = args, index++ )
	{
		parseString ( s, arg1, args );

		if ( arg1.empty () )
			return index;

		if ( ti != NULL )
			sscanf ( arg1.c_str (), "%d/%d", &vi [index], &ti [index] );
		else
			sscanf ( arg1.c_str (), "%d", &vi [index] );
	}
}
