//
// Simple class to load 3DStudio Max 3DS files
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#include	<assert.h>
#include	<stdio.h>

#include	"3dsLoader.h"
#include	"Mesh.h"
#include	"MeshNode.h"
#include	"Data.h"
#include	"Matrix3D.h"
#include	"libTexture.h"

#include	<map>

#define MAIN3DS             0x4D4D
#define EDIT3DS             0x3D3D  // this is the start of the editor config
#define EDIT_MATERIAL       0xAFFF
#define EDIT_OBJECT         0x4000
#define OBJ_TRIMESH         0x4100
#define OBJ_LIGHT           0x4600
#define OBJ_CAMERA          0x4700
#define LIT_OFF             0x4620
#define LIT_SPOT            0x4610
#define TRI_VERTEXLIST      0x4110
#define TRI_VERTEXOPTIONS   0x4111
#define TRI_FACELIST        0x4120
#define TRI_FACEMAPPING     0x4140
#define TRI_MATRIX          0x4160
#define RGB_COLOR           0x0010
#define COLOR24             0x0011
#define SPOTLIGHT           0x4610

#define COLOR_F             0x0010
#define COLOR_24            0x0011
#define LIN_COLOR_24        0x0012
#define LIN_COLOR_F         0x0013
#define INT_PERCENTAGE      0x0030
#define FLOAT_PERCENTAGE    0x0031

#define MAT_ENTRY           0xAFFF
#define MAT_NAME            0xA000
#define MAT_AMBIENT         0xA010
#define MAT_DIFFUSE         0xA020
#define MAT_SPECULAR        0xA030
#define MAT_SHININESS       0xA040
#define MAT_SHIN2PCT        0xA041
#define MAT_TRANSPARENCY    0xA050
#define MAT_SHADING         0xA100
#define MAT_TWO_SIDE        0xA081
#define MAT_ADDITIVE        0xA083
#define MAT_WIRE            0xA085
#define MAT_FACEMAP         0xA088
#define MAT_WIRESIZE        0xA087
#define MAT_DECAL           0xA082
#define MAT_TEXMAP          0xA200
#define MAT_MAPNAME         0xA300
#define MAT_MAP_TILING      0xA351
#define MAT_MAP_USCALE      0xA354
#define MAT_MAP_VSCALE      0xA356
#define MAT_MAP_UOFFSET     0xA358
#define MAT_MAP_VOFFSET     0xA35A
#define MAT_MAP_ANG         0xA35C
#define MAT_TEX2MAP         0xA33A
#define MAT_OPACMAP         0xA210
#define MAT_BUMPMAP         0xA230
#define MAT_SPECMAP         0xA204
#define MAT_SHINMAP         0xA33C
#define MAT_REFLMAP         0xA220
#define MAT_ACUBIC          0xA310

#define EDIT_OBJECT         0x4000
#define OBJ_TRIMESH         0x4100
#define OBJ_LIGHT           0x4600
#define OBJ_CAMERA          0x4700
#define LIT_OFF             0x4620
#define LIT_SPOT            0x4610
#define TRI_VERTEXLIST      0x4110
#define TRI_VERTEXOPTIONS   0x4111

#define TRI_FACELIST        0x4120
#define TRI_MAT_GROUP       0x4130
#define TRI_SMOOTH_GROUP    0x4150

#define TRI_FACEMAPPING     0x4140
#define TRI_MATRIX          0x4160

#define SPOTLIGHT           0x4610

struct Chunk
{
    uint16	id;
    int32 	start;
    int32	end;
};

static	void	readChunk ( Chunk& chunk, Data * data )
{
	chunk.id = (uint16)data -> getShort ();

	int	len = data -> getLong ();

	chunk.start = data -> getPos ();
	chunk.end   = chunk.start + len - 6;
}

static	void	goToChunk ( const Chunk& chunk, Data * data )
{
	data -> seekAbs ( chunk.start );
}

static	void	skipChunk ( const Chunk& chunk, Data * data )
{
	data -> seekAbs ( chunk.end );
}

static	Vector3D	readVector3D ( Data * data )
{
	Vector3D	v;

	v.x = data -> getFloat ();
	v.y = data -> getFloat ();
	v.z = data -> getFloat ();

	return v;
}

static	void	readMatrix ( Matrix3D& m, Data * data )
{
	for ( int i = 0; i < 3; i++ )
		for ( int j = 0; j < 3; j++ )
			m [i][j] = data -> getFloat ();
}

static	Vector3D	readColor ( const Chunk& chunk, Data * data )
{
	Vector3D	color ( 0, 0, 0 );

	goToChunk ( chunk, data );

	switch ( chunk.id )
	{
		case COLOR_F:
		case LIN_COLOR_F:
			color = readVector3D ( data ) ;
			break;

		case COLOR_24:
		case LIN_COLOR_24:
			color.x = data -> getByte () / 255.0f;
			color.y = data -> getByte () / 255.0f;
			color.z = data -> getByte () / 255.0f;
			break;
	}

	return color;
}

static	float	readPercentage ( const Chunk& chunk, Data * data )
{
	goToChunk ( chunk, data );

	switch ( chunk.id )
	{
		case INT_PERCENTAGE:
			return data -> getShort () / 100.0f;

		case FLOAT_PERCENTAGE:
			return data -> getFloat ();
	}

	return 0;		// XXX
}

static	bool	findChunk ( Chunk& target, const Chunk& parent, Data * data )
{
	if ( data -> getPos () >= parent.end )
		return false;

	Chunk	ch;

	readChunk ( ch, data );

	while ( (ch.id != target.id) && (ch.end <= parent.end) )
	{
		skipChunk ( ch, data );

		if ( ch.end >= parent.end )
			break;

		readChunk ( ch, data );
	}

	if ( ch.id != target.id )
		return false;

	target.start = ch.start;
	target.end   = ch.end;

	return true;
}

static	Face * readFaces ( const Chunk& chunk, int& count, Data * data, string& matName )
{
	Face  * faces = NULL;

	goToChunk ( chunk, data );

	matName = "";
	count   = data -> getShort ();
	faces   = new Face [count];

	for ( int i = 0; i < count; i++ )
	{
		for ( int j = 0; j < 3; j++ )
			faces [i].index [j] = data -> getShort ();

		data -> getShort ();
	}

	Chunk	ch;

	readChunk ( ch, data );

	while ( ch.end <= chunk.end )
	{
		if ( ch.id == TRI_MAT_GROUP )		// read material groups - use only first and apply
		{									// it's material to all mesh
			data -> getString ( matName, '\0' );

			break;
		}

		skipChunk ( ch, data );
		readChunk ( ch, data );
	}

	return faces;
}

static	void	readMap ( const Chunk& ch, Data * data, Map& theMap )
{
	Chunk	child;

	goToChunk ( ch,    data );
	readChunk ( child, data );

	while ( child.end <= ch.end )
	{
		switch ( child.id )
		{
			case MAT_MAPNAME:
				data -> getString ( theMap.mapName, '\0' );
				break;

			case MAT_MAP_USCALE:
				theMap.texScale.x = data -> getFloat ();
				break;

			case MAT_MAP_VSCALE:
				theMap.texScale.y = data -> getFloat ();
				break;

			case MAT_MAP_UOFFSET:
				theMap.texOffs.x = data -> getFloat ();
				break;

			case MAT_MAP_VOFFSET:
				theMap.texOffs.y = data -> getFloat ();
				break;
		}

		skipChunk ( child, data );
		readChunk ( child, data );
	}
}

static	void	readMaterial ( const Chunk& parent, Data * data, map <string, Material>& materials )
{
	Chunk		ch, ch2;
	Material	mat;

	goToChunk ( parent, data );
	readChunk ( ch, data );

	while ( ch.end <= parent.end )
	{
		switch ( ch.id )
		{
			case MAT_NAME:
				data -> getString ( mat.name, '\0' );

				break;

			case MAT_TEXMAP:
				readMap ( ch, data, mat.tex1 );
				break;

			case MAT_TEX2MAP:
				readMap ( ch, data, mat.tex2 );
				break;

	        case MAT_OPACMAP:
				readMap ( ch, data, mat.tex3 );
				break;

	        case MAT_BUMPMAP:
				readMap ( ch, data, mat.bump );
				break;

	        case MAT_SPECMAP:
				readMap ( ch, data, mat.specular );
				break;

	        case MAT_REFLMAP:
	        	readChunk ( ch2, data );
	        	skipChunk ( ch2, data );
	        	readChunk ( ch2, data );

            	if ( ch2.id == MAT_MAPNAME )
            		data -> getString ( mat.reflection.mapName, '\0' );

            	break;
		}

		skipChunk ( ch, data );
		readChunk ( ch, data );
	}

	materials [mat.name] = mat;
}

static	MeshNode :: MeshLink * readMesh ( const Chunk& parent, const string& meshName, map <string, Material>& materials, Data * data )
{
	Chunk		ch;
	int			count, i;
	Matrix3D	localAxes   ( 1 );
	Vector3D	localCenter ( 0, 0, 0 );
	Vertex    * vertices    = NULL;
	int			numVertices = 0;
	Face      * faces       = NULL;
	int			numFaces    = 0;
	string		matName;

	goToChunk ( parent, data );
	readChunk ( ch, data );

	while ( ch.end <= parent.end )
	{
		switch ( ch.id )
		{
			case TRI_VERTEXLIST:
				count       = data -> getShort ();
				vertices    = new Vertex [count];
				numVertices = count;

				for ( i = 0; i < count; i++ )
					vertices [i].pos = readVector3D ( data );

				break;

			case TRI_FACEMAPPING:
				count = data -> getShort ();

				if ( numVertices == 0 )
				{
					numVertices = count;
					vertices    = new Vertex [count];
				}

				assert ( count <= numVertices );

				for ( i = 0; i < count; i++ )
				{
					vertices [i].tex.x = data -> getFloat ();
					vertices [i].tex.y = data -> getFloat ();
				}

				break;

			case TRI_FACELIST:
				faces = readFaces ( ch, numFaces, data, matName );
				break;

			case TRI_MATRIX:				// get local axis and center
				readMatrix ( localAxes, data );
				localCenter = readVector3D ( data );
				break;

			default:
				break;
		}

		skipChunk ( ch, data );

		if ( ch.end >= parent.end )
			break;

		readChunk ( ch, data );
	}

	Mesh                 * mesh = new Mesh ( meshName.c_str (), vertices, numVertices, faces, numFaces, false );
	MeshNode :: MeshLink * node = new MeshNode :: MeshLink;

	if ( materials.find ( matName ) != materials.end () )
		mesh -> setMaterial ( materials [matName] );

	mesh -> computeFaceNormals ();
	mesh -> computeTangents    ();

	node -> node     = new MeshNode ( mesh );
	node -> name     = meshName;
	node -> offset   = Vector3D ( 0, 0, 0 );			// since offs is just local center of object
	node -> matr     = Matrix3D ( 1 );					// since m is just local coordinates for object, not its placement

	return node;
}

MeshNode * ThreeDsLoader :: load ( Data * data )
{
	MeshNode              * root = new MeshNode ( NULL );
	Chunk					main, edit, obj, mtl;
	map <string, Material>	materials;
	string					path;

	if ( data -> getFileName () != "" )
		path = getPath ( data -> getFileName () );

	readChunk ( main, data );

	if ( main.id != MAIN3DS )
		return NULL;

	edit.id = EDIT3DS;
	obj.id  = MAT_ENTRY;

	if ( !findChunk ( edit, main, data ) )
		return NULL;

	goToChunk ( edit, data );

	while ( findChunk ( obj, edit, data ) )
	{
		readMaterial ( obj, data, materials );
		skipChunk    ( obj, data );
	}

	if ( !path.empty () )
		for ( map <string, Material> :: iterator it = materials.begin (); it != materials.end (); ++it )
			it -> second.setPath ( path );

	goToChunk ( edit, data );

	obj.id = EDIT_OBJECT;

	while ( findChunk ( obj, edit, data ) )
	{
		string	objName;

		data -> getString ( objName, '\0' );

		readChunk ( mtl, data );

		if ( mtl.id == OBJ_TRIMESH )
		{
			MeshNode :: MeshLink * node = readMesh ( mtl, objName, materials, data );

			if ( node == NULL )
			{
				delete root;

				return NULL;
			}

			root -> attach ( node );
		}

		skipChunk ( obj, data );
	}

	return root;
}

