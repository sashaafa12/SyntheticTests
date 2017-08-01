//
// Simple class to load 3DS Max ASE files
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#include	<stdio.h>
#include	<ctype.h>

#include	"AseLoader.h"
#include	"Mesh.h"
#include	"MeshNode.h"
#include	"Data.h"
#include	"StringUtils.h"

#include	<map>

static	bool	getIndexVector ( const string& args, int& index, Vector3D& v )
{
	if ( sscanf ( args.c_str (), "%d %f %f %f", &index, &v.x, &v.y, &v.z ) != 4 )
		return false;

	return true;
}

static	bool	getVector ( const string& args, Vector3D& v )
{
	if ( sscanf ( args.c_str (), "%f %f %f", &v.x, &v.y, &v.z ) != 3 )
		return false;

	return true;
}

class	AseInfo
{
	Vertex   * vertices;
	int		   numVertices;
	Face     * faces;
	int		   numFaces;
	Face     * texFaces;
	int		   numTexFaces;
	Vector2D * texVertices;
	int		   numTexVertices;
	Vector3D   pos;
	Vector3D   rotAxis;
	float	   rotAngle;
	Vector3D   scale;
	Matrix3D   matr;
	string	   nodeName;
	int		   materialRef;

public:
	AseInfo ()
	{
		vertices       = NULL;
		numVertices    = 0;
		faces          = NULL;
		numFaces       = 0;
		texFaces       = NULL;
		numTexFaces    = 0;
		texVertices    = NULL;
		numTexVertices = 0;
		pos            = Vector3D ( 0, 0, 0 );
		rotAxis        = Vector3D ( 0, 0, 0 );
		rotAngle       = 0;
		scale          = Vector3D ( 1, 1, 1 );
		nodeName       = "";
		materialRef    = -1;
	}

	~AseInfo ()
	{
		clear ();
	}

	void	clear ()
	{
		delete vertices;
		delete faces;
		delete texFaces;
		delete texVertices;

		vertices       = NULL;
		numVertices    = 0;
		faces          = NULL;
		numFaces       = 0;
		texFaces       = NULL;
		numTexFaces    = 0;
		texVertices    = NULL;
		numTexVertices = 0;
		pos            = Vector3D ( 0, 0, 0 );
		rotAxis        = Vector3D ( 0, 0, 0 );
		rotAngle       = 0;
		scale          = Vector3D ( 1, 1, 1 );
		nodeName       = "";
		materialRef    = -1;
	}

	void	setNumVertices ( int n )
	{
		numVertices = n;
		vertices    = new Vertex [n];

		memset ( vertices, '\0', n * sizeof ( Vertex ) );
	}

	int		getNumVertices () const
	{
		return numVertices;
	}

	void	setNumFaces ( int n )
	{
		numFaces = n;
		faces    = new Face [n];

		memset ( faces, '\0', n * sizeof ( Face ) );
	}

	int		getNumFaces () const
	{
		return numFaces;
	}

	void	setNumTexVertices ( int n )
	{
		numTexVertices = n;
		texVertices    = new Vector2D [n];

		memset ( texVertices, '\0', n * sizeof ( Vector2D ) );
	}

	void	setNumTexFaces ( int n )
	{
		numTexFaces = n;
		texFaces    = new Face [n];

		memset ( texFaces, '\0', n * sizeof ( Face ) );
	}

	bool	setVertexPos ( int index, const Vector3D& v )
	{
		if ( index < 0 || index >= numVertices || vertices == NULL )
			return false;

		vertices [index].pos = v;

		return true;
	}

	bool	setVertexNormal ( int index, const Vector3D& v )
	{
		if ( index < 0 || index >= numVertices || vertices == NULL )
			return false;

		vertices [index].n = v;

		return true;
	}

	void	setNodeName ( const string& theName )
	{
		nodeName = theName;
	}

	bool	setTexVertex ( int index, const Vector3D& v )
	{
		if ( index < 0 || index >= numTexVertices || texVertices == NULL )
			return false;

		texVertices [index] = Vector2D ( v.x, v.y );

		return true;
	}

	bool	setFace ( int index, int i1, int i2, int i3 )
	{
		if ( index < 0 || index >= numFaces || faces == NULL )
			return false;

		faces [index].index [0] = i1;
		faces [index].index [1] = i2;
		faces [index].index [2] = i3;

		return true;
	}

	bool	setTexFace ( int index, int i1, int i2, int i3 )
	{
		if ( index < 0 || index >= numTexFaces || texFaces == NULL )
			return false;

		texFaces [index].index [0] = i1;
		texFaces [index].index [1] = i2;
		texFaces [index].index [2] = i3;

		return true;
	}

	bool	setFaceNormal ( int index, const Vector3D& n )
	{
		if ( index < 0 || index >= numFaces || faces == NULL )
			return false;

		faces [index].n = n;

		return true;
	}

	void	setPos ( const Vector3D& p )
	{
		pos = p;
	}

	void	setRotAxis ( const Vector3D& v )
	{
		rotAxis = v;
	}

	void	setRotAngle ( float a )
	{
		rotAngle = a;
	}

	void	setScale ( const Vector3D& s )
	{
		scale = s;
	}

	void	setMatrixRow ( int row, const Vector3D& v )
	{
		matr [0][row] = v.x;
		matr [1][row] = v.y;
		matr [2][row] = v.z;
	}
	
	void	setMaterialRef ( int ref )
	{
		materialRef = ref;
	}

	int		getMaterialRef () const
	{
		return materialRef;
	}

	const string&	getName () const
	{
		return nodeName;
	}

	const Vector3D&	getPos () const
	{
		return pos;
	}

	const Vector3D&	getRotAxis () const
	{
		return rotAxis;
	}

	const Vector3D&	getScale () const
	{
		return scale;
	}

	float	getRotAngle () const
	{
		return rotAngle;
	}

	const Matrix3D&	getMatrix () const
	{
		return matr;
	}
	
	bool	reallocVertices ( Vertex *& outVertices, int numOutVertices, int& maxOutVertices, int delta )
	{
		maxOutVertices += delta;

		Vertex * newVertices = new Vertex [maxOutVertices];

		memcpy ( newVertices, outVertices, numOutVertices * sizeof ( Vertex ) );

		delete outVertices;

		outVertices = newVertices;

		return true;
	}

	Mesh * buildMesh ( const Material& m )
	{
		if ( numFaces != numTexFaces )
			return NULL;

											// recompute vertices based on faces and texFaces arrays
		int	     numOutVertices   = numTexVertices;
		Vertex * outVertices      = new Vertex [2*numTexVertices];
		int	   * vertexAssignment = new int    [numTexVertices];
		int		 maxOutVertices   = 2*numTexVertices;
		int		 i, j;

		for ( i = 0; i < numTexVertices; i++ )
			vertexAssignment [i] = -1;

		for ( i = 0; i < numFaces; i++ )
		{
			texFaces [i].n = faces [i].n;

			for ( j = 0; j < 3; j++ )
			{
				int	vi = faces    [i].index [j];
				int	ti = texFaces [i].index [j];

											// map vertex to texvertex
											// if not assigned yet, do assignment ti -> vi
				if ( vertexAssignment [ti] == -1 )
				{
					outVertices [ti]       = vertices    [vi];
					outVertices [ti].tex   = texVertices [ti];
					outVertices [ti].color = Vector4D ( 1, 1, 1, 1 );
					vertexAssignment [ti]  = vi;
				}
				else						// if already assigned to smth different
											// create new vertex
				if ( outVertices [vi].tex != texVertices [ti] )
				{
					if ( numOutVertices >= maxOutVertices )
						reallocVertices ( outVertices, numOutVertices, maxOutVertices, numTexVertices );

					texFaces [i].index [j]             = numOutVertices;
					outVertices [numOutVertices]       = vertices    [vi];
					outVertices [numOutVertices].tex   = texVertices [ti];
					outVertices [numOutVertices].color = Vector4D ( 1, 1, 1, 1 );

					numOutVertices++;
				}
			}
		}

		delete vertexAssignment;
/*
											// now apply texture coordinate transform
		for ( i = 0; i < numOutVertices; i++ )
			outVertices [i].tex = outVertices [i].tex * m.texTile + m.texOffs;
*/

		Mesh * mesh = new Mesh ( nodeName.c_str (), outVertices, numOutVertices, texFaces, numTexFaces, true );

		mesh -> setMaterial ( m );

		delete outVertices;

		return mesh;
	}
};

MeshNode * AseLoader :: load ( Data * data )
{
	MeshNode  * out = new MeshNode ( NULL );
	AseInfo		info;
	string		str, cmd, args;
	bool		res = true;
	int			curMaterial;

	map <int, Material>	materials;

	while ( data -> getString ( str, '\r' ) )
	{
		str = trim ( str );

		if ( str.empty () )
			continue;

		parseString ( str, cmd, args );

		if ( cmd == "*MATERIAL" )
			curMaterial = atoi ( args.c_str () );
		else
		if ( cmd == "*BITMAP" )
			materials [curMaterial].diffuse.mapName = dequote ( args );
		else
		if ( cmd == "*UVW_U_OFFSET" )
			materials [curMaterial].diffuse.texOffs.x = atof ( args.c_str () );
		else
		if ( cmd == "*UVW_V_OFFSET" )
			materials [curMaterial].diffuse.texOffs.y = atof ( args.c_str () );
		else
		if ( cmd == "*UVW_U_TILING" )
			materials [curMaterial].diffuse.texScale.x = atof ( args.c_str () );
		else
		if ( cmd == "*UVW_V_TILING" )
			materials [curMaterial].diffuse.texScale.y = atof ( args.c_str () );
		else
		if ( cmd == "*GEOMOBJECT" )			// we're starting new objects
		{
			if ( info.getNumVertices () < 1 || info.getNumFaces () < 1 )
				continue;
											// flush current data as new node
			Mesh * mesh = info.buildMesh ( materials [info.getMaterialRef ()] );

			out -> attach ( info.getName ().c_str (), mesh, info.getPos (), info.getMatrix () );

											// free all allocated data
			info.clear ();
		}
		else
		if ( cmd == "*NODE_NAME" )
			info.setNodeName ( dequote ( args ) );
		else
		if ( cmd == "*MESH_NUMVERTEX" )
			info.setNumVertices ( atoi ( args.c_str () ) );
		else
		if ( cmd == "*MESH_NUMFACES" )
			info.setNumFaces ( atoi ( args.c_str () ) );
		else
		if ( cmd == "*MESH_VERTEX" )
		{
			int			index;
			Vector3D	v;

			if ( !getIndexVector ( args, index, v ) )
			{
				res = false;

				break;
			}

			if ( !info.setVertexPos ( index, v ) )
			{
				res = false;

				break;
			}
		}
		else
		if ( cmd == "*MESH_VERTEXNORMAL" )
		{
			int			index;
			Vector3D	v;

			if ( !getIndexVector ( args, index, v ) )
			{
				res = false;

				break;
			}

			if ( !info.setVertexNormal ( index, v ) )
			{
				res = false;

				break;
			}
		}
		else
		if ( cmd == "*MESH_FACE" )
		{
			int	index, i1, i2, i3;

			if ( sscanf ( args.c_str (), "%d: A: %d B: %d C: %d", &index, &i1, &i2, &i3 ) != 4 )
			{
				res = false;

				break;
			}

			if ( !info.setFace ( index, i1, i2, i3 ) )
			{
				res = false;

				break;
			}
		}
		else
		if ( cmd == "*MESH_FACENORMAL" )
		{
			int			index;
			Vector3D	n;

			if ( !getIndexVector ( args, index, n ) )
			{
				res = false;

				break;
			}

			if ( !info.setFaceNormal ( index, n ) )
			{
				res = false;

				break;
			}
		}
		else
		if ( cmd == "*MESH_NUMTVERTEX" )
			info.setNumTexVertices ( atoi ( args.c_str () ) );
		else
		if ( cmd == "*MESH_TVERT" )
		{
			int			index;
			Vector3D	v;

			if ( !getIndexVector ( args, index, v ) )
			{
				res = false;

				break;
			}

			if ( !info.setTexVertex ( index, v ) )
			{
				res = false;

				break;
			}
		}
		else
		if ( cmd == "*MESH_NUMTVFACES" )
			info.setNumTexFaces ( atoi ( args.c_str () ) );
		else
		if ( cmd == "*MESH_TFACE" )
		{
			int	index, i1, i2, i3;

			if ( sscanf ( args.c_str (), "%d %d %d %d", &index, &i1, &i2, &i3 ) != 4 )
			{
				res = false;

				break;
			}

			if ( !info.setTexFace ( index, i1, i2, i3 ) )
			{
				res = false;

				break;
			}
		}
		else
		if ( cmd == "*TM_POS" )
		{
			Vector3D	v;

			if ( !getVector ( args, v ) )
			{
				res = false;

				break;
			}

			info.setPos ( v );
		}
		else
		if ( cmd == "*TM_ROTAXIS" )
		{
			Vector3D	v;

			if ( !getVector ( args, v ) )
			{
				res = false;

				break;
			}

			info.setRotAxis ( v );
		}
		else
		if ( cmd == "*TM_ROTANGLE" )
			info.setRotAngle ( atof ( args.c_str () ) );
		else
		if ( cmd == "*TM_SCALE" )
		{
			Vector3D	v;

			if ( !getVector ( args, v ) )
			{
				res = false;

				break;
			}

			info.setScale ( v );
		}
		else
		if ( cmd == "*TM_ROW0" )
		{
			Vector3D	v;
			
			if ( !getVector ( args, v ) )
			{
				res = false;
				
				break;
			}
			
			info.setMatrixRow ( 0, v );
		}
		else
		if ( cmd == "*TM_ROW1" )
		{
			Vector3D	v;
			
			if ( !getVector ( args, v ) )
			{
				res = false;
				
				break;
			}
			
			info.setMatrixRow ( 1, v );
		}
		else
		if ( cmd == "*TM_ROW2" )
		{
			Vector3D	v;
			
			if ( !getVector ( args, v ) )
			{
				res = false;
				
				break;
			}
			
			info.setMatrixRow ( 2, v );
		}
		else
		if ( cmd == "*MATERIAL_REF" )
			info.setMaterialRef ( atoi ( args.c_str () ) );
	}

											// flush current data as new node
	Mesh * mesh = info.buildMesh ( materials [info.getMaterialRef ()] );

	out -> attach ( info.getName ().c_str (), mesh, info.getPos (), info.getMatrix () );

	if ( res )
		return out;

	delete out;

	return NULL;
}

