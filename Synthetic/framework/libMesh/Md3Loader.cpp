//
// Class to load md3 files (Quake III)
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#include	"Md3Loader.h"
#include	"Mesh.h"
#include	"MeshNode.h"
#include	"Data.h"
#include	"Matrix3D.h"
#include	"libTexture.h"
#include	"StringUtils.h"
#include	<map>

										// .md3 file structures
#pragma pack (push, 1)

struct	Md3Header						// header of .md3 file
{
	char	id [4];						// should be "IDP3"
	int32	version;					// should be 15
	char	fileName [68];
	int32	numBoneFrames;
	int32	numTags;
	int32	numMeshes;
	int32	numMaxSkins;
	int32	headerLength;
	int32	tagStart;
	int32	tagEnd;
	int32	fileSize;
};

struct	Md3BoneFrame
{
	float	mins [3];
	float	maxs [3];
	float	pos  [3];
	float	scale;
	char	creator [16];
};

struct	Md3Tag
{
	char	name [64];
	float	pos  [3];
	float	rot  [3][3];
};

struct	Md3MeshHeader
{
	char	id [4];
	char	name [68];
	int32	numMeshFrames;
	int32	numSkins;
	int32	numVertices;
	int32	numTriangles;
	int32	triStart;
	int32	skinsStart;
	int32	texVectorStart;
	int32	vertexStart;
	int32	meshSize;
};

struct	Md3Skin
{
	char	name [64];
	int32	index;
};

struct	Md3Triangle
{
	int32	vertex [3];
};

struct	Md3TexCoord
{
	float	coord [2];
};

struct	Md3Vertex
{
	int16	vertex [3];
	uint8	normal [2];
};

#pragma	pack (pop)

Vector3D	Md3Loader :: normalTable [256][256];
bool		Md3Loader :: normalTableInited = false;

Md3Loader :: Md3Loader ()
{
	if ( normalTableInited )
		return;

	for ( int i = 0; i < 256; i++ )
		for ( int j = 0; j < 256; j++ )
		{
			float	alpha = 2.0f * M_PI * i / 256.0f;
			float	beta  = 2.0f * M_PI * j / 256.0f;

			normalTable [i][j] = Vector3D ( (float)(cos (beta) * sin (alpha)),
											(float)(sin (beta) * sin (alpha)),
											(float) cos (alpha) );
		}

	normalTableInited = true;
}

MeshNode * Md3Loader :: load ( Data * data, Data * skin )
{
	Md3Header * hdr = (Md3Header *) data -> getPtr ();
	int	    i, j;

	if ( strncmp ( hdr -> id, "IDP3", 4 ) )
		return NULL;

	if ( hdr -> version != 15 )
		return NULL;

	map <string, string>	meshTextures;

	parseSkin ( skin, meshTextures );
										// get bone frames ptr
	data -> seekCur ( sizeof ( Md3Header ) );

	Md3BoneFrame * boneFramesPtr = (Md3BoneFrame *) data -> getPtr ();

										// get tags ptr
	data -> seekCur ( hdr -> numBoneFrames * sizeof ( Md3BoneFrame ) );

	Md3Tag * tagsPtr = (Md3Tag *) data -> getPtr ();

										// read meshes
	int	meshOffs = sizeof ( Md3Header ) +
		           hdr -> numBoneFrames * sizeof ( Md3BoneFrame ) +
		           hdr -> numBoneFrames * hdr -> numTags * sizeof ( Md3Tag );

	MeshNode * root = new MeshNode ( NULL );

	for ( int meshNo = 0; meshNo < hdr -> numMeshes; meshNo++ )
	{
		Md3MeshHeader * meshHdr = (Md3MeshHeader *) data -> getPtr ( meshOffs );

										// get triangles ptr
		Md3Triangle * triPtr = (Md3Triangle *) data -> getPtr ( meshOffs + meshHdr -> triStart );

										// get tex. coords ptr
		Md3TexCoord * texCoordPtr = (Md3TexCoord *) data -> getPtr ( meshOffs + meshHdr -> texVectorStart );

										// get vertices ptr
		Md3Vertex * vertPtr = (Md3Vertex *) data -> getPtr ( meshOffs + meshHdr -> vertexStart );

										// get skins ptr
		Md3Skin * skinsPtr = (Md3Skin *) data -> getPtr ( meshOffs + meshHdr -> skinsStart );

		meshOffs += meshHdr -> meshSize;

										// now prepare to build mesh from this data
		string	name ( meshHdr -> name );
		int		numSkins     = meshHdr -> numSkins;
		int		numVertices  = meshHdr -> numVertices;
		int		numFaces     = meshHdr -> numTriangles;

		Vertex   * vertices  = new Vertex [numVertices *  meshHdr -> numMeshFrames];
		Face     * faces     = new Face   [numFaces];

										// convert vertices, change coordinate system (scale, swap y & z)
		for ( i = 0; i < numVertices * meshHdr -> numMeshFrames; i++ )
		{
			vertices [i].pos.x = vertPtr [i].vertex [0] / 64.0f;
			vertices [i].pos.y = vertPtr [i].vertex [2] / 64.0f;
			vertices [i].pos.z = vertPtr [i].vertex [1] / 64.0f;
			vertices [i].pos.w = 1;
			vertices [i].n     = normalTable [vertPtr [i].normal [0]][vertPtr [i].normal [1]];
			vertices [i].color = Vector4D ( 1, 1, 1, 1 );
		}

										// convert uv coordinates
		for ( i = 0; i < numVertices * meshHdr -> numMeshFrames; i++ )
		{
			vertices [i].tex.x = texCoordPtr [i % numVertices].coord [0];
			vertices [i].tex.y = 1 - texCoordPtr [i % numVertices].coord [1];
		}

										// now build faces
		for ( i = 0; i < numFaces; i++ )
			for ( j = 0; j < 3; j++ )
				faces [i].index [j] = triPtr [i].vertex [j];

										// build mesh
		Mesh * mesh = new Mesh ( name.c_str (), vertices, numVertices, faces, numFaces, false );

										// now load skins
										// NB: we use only first skin (if present)
		if ( numSkins > 0 )
		{
			mesh -> getMaterial ().diffuse.mapName = getFileName ( skinsPtr -> name );

/*
			if ( texture == NULL )		// Quake III quirk: sometimes they use wrong extension
			{
				String	texName ( skinsPtr -> name );

										// try using jpg extension
				String :: setExtension ( texName, "jpg" );

				texture = getResourceManager () -> getTexture ( texName );

				if ( texture == NULL )
				{
										// try using tga extension
					String :: setExtension ( texName, "tga" );

					texture = getResourceManager () -> getTexture ( texName );
				}
			}
*/
		}

		if ( meshTextures.find ( meshHdr -> name ) != meshTextures.end () )
			mesh -> getMaterial ().diffuse.mapName = meshTextures [meshHdr -> name];

		mesh -> computeFaceNormals ();
		mesh -> computeTangents    ();

		root -> attach ( meshHdr -> name, mesh, Vector3D ( 0, 0, 0 ), Matrix3D ( 1 ) );

		Matrix3D	m;

		for ( i = 0; i < 3; i++ )
			for ( j = 0; j < 3; j++ )
				m [i][j] = tagsPtr [i].rot [i][j];

										// add dummy nodes for tags
		for ( i = 0; i < hdr -> numTags; i++ )
			root -> attach ( tagsPtr [i].name, (MeshNode *)NULL,
			                 Vector3D ( tagsPtr [i].pos [0], tagsPtr [i].pos [2], tagsPtr [i].pos [1] ), m );
	}

	return root;
}

										// get mapping from mesh names to textures
void	Md3Loader :: parseSkin ( Data * skin, map <string,string>& textures )
{
	string	line;

	if ( skin == NULL )
		return;

	while ( skin -> getString ( line, '\n' ) )
	{
		if ( line.empty () )
			continue;

		int	pos = line.find ( ',' );

		if ( pos == string :: npos )
			continue;

		string	meshName  = line.substr ( 0, pos );
		string	imageName = line.substr ( pos + 1 );

		if ( getName ( imageName ) != "null" )
			textures [meshName] = trim ( imageName );
	}
}
