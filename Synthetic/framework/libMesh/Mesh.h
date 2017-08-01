//
// Simple mesh model and neccessary data structures
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#ifndef	__MESH__
#define	__MESH__

#ifdef	_WIN32
	#pragma	warning (disable:4786)
#endif

#include	<list>
#include	<string>

using namespace std;

#include	"Vector2D.h"
#include	"Vector3D.h"
#include	"Vector4D.h"
#include	"BoundingBox.h"
#include	"Material.h"

struct  Vertex
{
    Vector4D    pos;                        // position of vertex
    Vector3D    n;                          // unit normal
    Vector3D    t, b;                       // tangent and binormal
    Vector2D    tex;                        // texture coordinates
    Vector4D    color;                      // vertex color
};

struct  Face                                // triangular face
{
    int         index [3];                  // indices to Vertex array
    Vector3D    n;                          // face normal
};

struct  Edge
{
    int     a, b;                           // indices into array of Vertex
    int     f1, f2;                         // indices into array of Face's
    										// hi-order bit means for this face reverse
    										// endpoints
};

class	Mesh
{
protected:
	string	 name;
	int		 numVertices;
	int		 numFaces;
	int		 numEdges;
	Vertex * vertices;
	Face   * faces;
	Edge   * edges;
	GLuint   vertexBuffer;					// VBO for vertices
	GLuint   indexBuffer;					// VBO for face indices

	list <pair <int,int> >	mapping;		// mapping from vertex data to OpenGL data
	BoundingBox				boundingBox;	// bounding box for mesh
	Material				material;		// set of textures for mesh

public:
	Mesh  ( const char * theName, Vertex * theVertices, int nv, Face * theFaces, int nf,
	        bool copy = true );
	~Mesh ();

	const string&	getName () const
	{
		return name;
	}
											// access to verices
	int	getNumVertices () const
	{
		return numVertices;
	}

	const Vertex&	getVertex ( int index ) const
	{
		return vertices [index];
	}

	Vertex&	getVertex ( int index )
	{
		return vertices [index];
	}

											// access to faces
	int	getNumFaces () const
	{
		return numFaces;
	}

	const Face&	getFace ( int index ) const
	{
		return faces [index];
	}

	Face&	getFace ( int index )
	{
		return faces [index];
	}

											// access to edges
	int	getNumEdges () const
	{
		return numEdges;
	}

	const Edge&	getEdge ( int index ) const
	{
		return edges [index];
	}

	Edge&	getEdge ( int index )
	{
		return edges [index];
	}
	
	const Material&	getMaterial () const
	{
		return material;
	}
	
	Material&	getMaterial ()
	{
		return material;
	}
	
	void	setMaterial ( const Material& mat )
	{
		material = mat;						// component-wise assignment is ok
	}
											// add assignment for source data to outputs
											// from in (tagVertex,...,tagBinormal)
											// to   in (tagVertex,tagNormal,tagColor,tagTex0,...
											// ...,tagTex7)
	bool	addCoordAssignment ( int from, int to );

											// XXX: add map bindings !!!
											
											// create VBO's
	void	createBuffers ();
											// render the mesh
	void	render ();

	void	preRender  ();					// setup all buffers for rendering
	void	postRender ();					// restore state
	
											// compute tangents and binormals from texCoords
	void	computeTangents ();

											// compute edge data from vertices and faces
	void	computeEdges ();

											// compute face normals
	void	computeFaceNormals ();

											// compute vertex normal from face normals
	void	computeVertexNormals ();
	
	enum									// enums for what
	{
		tagVertex   = 0,
		tagNormal   = 1,
		tagColor    = 2,
		tagTexCoord = 3,
		tagTangent  = 4,
		tagBinormal = 5
	};

	enum									// enums for where
	{										// (plus tagVertex, tagColor, tagNormal)
		tagTex0 = 10,						// tex coords for unit 0
		tagTex1 = 11,						// tex coords for unit 1
		tagTex2 = 12,
		tagTex3 = 13,
		tagTex4 = 14,
		tagTex5 = 15,
		tagTex6 = 16,
		tagTex7 = 17
	};

protected:
	void	addEdge ( int v1, int v2, int face );
};

#endif

