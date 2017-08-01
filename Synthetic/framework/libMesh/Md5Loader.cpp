//
// Simple class to load 3DStudio Max 3DS files
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#include	<assert.h>
#include	<stdio.h>

#include	"Md5Loader.h"
#include	"Mesh.h"
#include	"MeshNode.h"
#include	"Data.h"
#include	"Matrix3D.h"
#include	"Quaternion.h"
#include	"libTexture.h"
#include	"StringUtils.h"

#include	<map>

struct	Md5Joint
{
	string		name;
	int			parentIndex;
	Md5Joint  * parent;
	Vector3D	bindPos;
	Quaternion	orient;

	Vector3D	transform ( const Vector3D& v ) const
	{
		return orient.rotate ( v ) + bindPos;
	}

	Vector3D	invTransform ( const Vector3D& v ) const
	{
		Quaternion	c ( orient );

		return c.conj ().rotate ( v ) + bindPos;
	}
};


struct	Md5Vertex								// mesh vertex
{
	Vector2D	tex;
	int			weightIndex;
	int			weightCount;
};

struct	Md5Triangle
{
	int	index [3];
};

struct	Md5Weight
{
	int			jointIndex;					// for whuch bone this weight is for
	float		weight;						// looks like a weight of this pos
	Vector3D	pos;
};

class	Md5Mesh
{
	string			shader;
	int				numVertices;
	Md5Vertex     * vertices;
	int				numTriangles;
	Md5Triangle   * tris;
	int				numWeights;
	Md5Weight     * weights;
	unsigned		diffuseMap, specularMap, bumpMap;

public:
	Md5Mesh ()
	{
		numVertices  = 0;
		vertices     = NULL;
		numTriangles = 0;
		tris         = NULL;
		numWeights   = 0;
		weights      = NULL;
		diffuseMap   = 0;
		specularMap  = 0;
		bumpMap      = 0;
	}

	~Md5Mesh ()
	{
		delete vertices;
		delete tris;
		delete weights;
	}

	int	getNumVertices () const
	{
		return numVertices;
	}

	int	getNumTris () const
	{
		return numTriangles;
	}

	void	setShader ( const string& str )
	{
		shader = str;
	}

	void	setNumVertices ( int n )
	{
		numVertices = n;
		vertices    = new Md5Vertex [n];
	}

	void	setNumTris ( int n )
	{
		numTriangles = n;
		tris         = new Md5Triangle [n];
	}

	void	setNumWeights ( int n )
	{
		numWeights = n;
		weights    = new Md5Weight [n];
	}

	Md5Vertex&	vertex ( int index )
	{
		assert ( index >= 0 && index < numVertices );

		return vertices [index];
	}

	Md5Triangle&	triangle ( int index )
	{
		assert ( index >= 0 && index < numTriangles );

		return tris [index];
	}

	Md5Weight&	weight ( int index )
	{
		assert ( index >= 0 && index < numWeights );

		return weights [index];
	}

	void	setVertex ( int index, const Vector2D& tex, int weightIndex, int weightCount )
	{
		vertices [index].tex         = tex;
		vertices [index].weightIndex = weightIndex;
		vertices [index].weightCount = weightCount;
	}

	void	setTri ( int index, int i1, int i2, int i3 )
	{
		tris [index].index [0] = i1;
		tris [index].index [1] = i2;
		tris [index].index [2] = i3;
	}

	void	setWeight ( int index, int joint, float bias, const Vector3D& pos )
	{
		weights [index].jointIndex = joint;
		weights [index].weight     = bias;
		weights [index].pos        = pos;
	}

	bool	loadShader ( const string& modelName, Material& m );
	void	calcPoints ( Md5Joint * joints, Vertex * points );
};

static	bool	loadJoints ( Data * data, int numJoints, Md5Joint * joints )
{
	string		str, cmd, args;
	Vector3D	pos, orient;
	int			index;
	Quaternion	q;
	int			no = 0;

	while ( data -> getString ( str, '\r' ) )
	{
		str = trim ( str );

		if ( str.empty () )
			continue;

		parseString ( str, cmd, args );

		if ( cmd == "//" )							// skip comments
			continue;

		if ( cmd == "}" )
			return true;

		sscanf ( args.c_str (), "%d ( %f %f %f ) ( %f %f %f )", &index, &pos.x, &pos.y, &pos.z, 
			                                                    &orient.x, &orient.y, &orient.z );

		q.x = orient.x;
		q.y = orient.y;
		q.z = orient.z;
		q.w = -sqrt ( 1 - q.x * q.x - q.y * q.y - q.z * q.z );

		joints [no].parentIndex = index;
		joints [no].parent      = index >= 0 ? &joints [index] : NULL;
		joints [no].bindPos     = pos;
		joints [no].orient      = q;

		no++;
	}

	return false;
}

static	bool	loadMesh ( Data * data, Md5Mesh& mesh )
{
	string	str, cmd, args;

	while ( data -> getString ( str, '\r' ) )
	{
		str = trim ( str );

		if ( str.empty () )
			continue;

		parseString ( str, cmd, args );

		if ( cmd.empty () )
			continue;

		if ( cmd == "//" )							// skip comments
			continue;

		if ( cmd == "shader" )
			mesh.setShader ( dequote ( args ) );
		else
		if ( cmd == "numverts" )
			mesh.setNumVertices ( atoi ( args.c_str () ) );
		else
		if ( cmd == "vert" )
		{
			int		index;
			float	u, v;
			int		weightIndex;
			int		weightCount;

			sscanf ( args.c_str (), "%d ( %f %f ) %d %d", &index, &u, &v, &weightIndex, 
				                                          &weightCount );

			mesh.setVertex ( index, Vector2D ( u, v ), weightIndex, weightCount );
		}
		else
		if ( cmd == "numtris" )
			mesh.setNumTris ( atoi ( args.c_str () ) );
		else
		if ( cmd == "tri" )
		{
			int	index, i1, i2, i3;

			sscanf ( args.c_str (), "%d %d %d %d", &index, &i1, &i2, &i3 );

			mesh.setTri ( index, i1, i2, i3 );
		}
		else
		if ( cmd == "numweights" )
			mesh.setNumWeights ( atoi ( args.c_str () ) );
		else
		if ( cmd == "weight" )
		{
			int		index;
			int		joint;
			float	bias;
			float	x, y, z;

			sscanf ( args.c_str (), "%d %d %f ( %f %f %f )", &index, &joint, &bias, &x, &y, &z );

			mesh.setWeight ( index, joint, bias, Vector3D ( x, y, z ) );
		}
		else
		if ( cmd == "}" )
			break;
	}

	return true;
}

bool	Md5Mesh :: loadShader ( const string& modelName, Material& m )	// load textures
{
	string	fileName        = shader;				// texture template
	string	ext             = "tga";
	string	diffuseMapName  = fileName + "_d." + ext;
	string	diffuseMapName2 = fileName + "_h." + ext;
	string	diffuseMapName3 = fileName + "_b." + ext;
	string	diffuseMapName4 = fileName + "."   + ext;

	if ( ( diffuseMap = createTexture2D ( true, diffuseMapName.c_str () ) ) != 0 )
		m.diffuse.mapName = diffuseMapName;
	else
	if ( ( diffuseMap = createTexture2D ( true, diffuseMapName2.c_str () ) ) != 0 )
		m.diffuse.mapName = diffuseMapName2;
	if ( ( diffuseMap = createTexture2D ( true, diffuseMapName3.c_str () ) ) != 0 )
		m.diffuse.mapName = diffuseMapName3;
	else
	if ( ( diffuseMap = createTexture2D ( true, diffuseMapName4.c_str () ) ) != 0 )
		m.diffuse.mapName = diffuseMapName4;
	else
		printf ( "File %s not found.\n", diffuseMapName4.c_str () );

												// locate specular map
	string	specularMapName  = fileName + "_s."  + ext;
	string	specularMapName2 = fileName + "_sp." + ext;

	if ( ( specularMap = createTexture2D ( true, specularMapName.c_str () ) ) != 0 )
		m.specular.mapName = specularMapName;
	else
		printf ( "File %s not found.\n", specularMapName.c_str () );

												// locate bumpmap
	string	bumpMapName = fileName + "_local." + ext;

	if ( ( bumpMap = createTexture2D ( true, bumpMapName.c_str () ) ) != 0 )
		m.bump.mapName = bumpMapName;
	else
		printf ( "File %s not found.\n", bumpMapName.c_str () );


	return true;
}
													// compute real points from bones data
void	Md5Mesh :: calcPoints ( Md5Joint * joints, Vertex * points )
{
	for ( int i = 0; i < numVertices; i++ )
	{
		Md5Vertex&	v = vertices [i];				// current vertex

		Vector3D	p ( 0, 0, 0 );

		for ( int k = 0; k < v.weightCount; k++ )
		{
			Md5Weight&	weight = weights [v.weightIndex + k];
			Md5Joint&	joint  = joints  [weight.jointIndex];

												// transform weight.pos by bone with weight
			p += joint.transform ( weight.pos ) * weight.weight;
		}

		points [i].pos = Vector3D ( p.y, p.x, p.z );
		points [i].tex = v.tex;
	}
}

MeshNode *	Md5Loader :: load ( Data * data, const string& modelName )
{
	if ( data == NULL )
		return NULL;

	MeshNode  * root      = new MeshNode ( NULL );
	int			meshNo    = 0;
	int			numMeshes = 0;
	int			numJoints = 0;
	Md5Mesh   * meshes    = NULL;
	Md5Joint  * joints    = NULL;
	string		str, cmd, args;

	while ( data -> getString ( str, '\r' ) )
	{
		str = trim ( str );

		if ( str.empty () )
			continue;

		parseString ( str, cmd, args );

		if ( cmd.empty () )
			continue;

		if ( cmd == "//" )							// skip comments
			continue;

		if ( cmd == "MD5Version" )
			;
		else
		if ( cmd == "commandline" )
			;
		else
		if ( cmd == "numJoints" )
			joints = new Md5Joint [numJoints = atoi ( args.c_str () )];
		else
		if ( cmd == "joints" )
			loadJoints ( data, numJoints, joints );
		else
		if ( cmd == "numMeshes" )
			meshes = new Md5Mesh [numMeshes = atoi ( args.c_str () )];
		else
		if ( cmd == "mesh" )
			loadMesh ( data, meshes [meshNo++] );
	}
											// load all textures
	for ( int i = 0; i < numMeshes; i++ )
	{
		Material	m;

		meshes [i].loadShader ( modelName, m );

		int		 numVertices = meshes [i].getNumVertices ();
		int		 numFaces    = meshes [i].getNumTris     ();
		Vertex * vertices    = new Vertex [numVertices];
		Face   * faces       = new Face   [numFaces   ];

		meshes [i].calcPoints ( joints, vertices );

		for ( int j = 0; j < numFaces; j++ )
			for ( int k = 0; k < 3; k++ )
				faces [j].index [k] = meshes [i].triangle ( j ).index [k];

		Mesh   * mesh = new Mesh ( "", vertices, numVertices, faces, numFaces, false );

		mesh -> computeFaceNormals ();
		mesh -> computeEdges       ();
		mesh -> computeTangents    ();
		mesh -> setMaterial        ( m );
		root -> attach             ( "", mesh, Vector3D ( 0, 0, 0 ), Matrix3D ( 1 ) );
	}

	delete joints;
	delete meshes;

	return root;
}

