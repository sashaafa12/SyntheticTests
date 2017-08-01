//
// Simple mesh model and neccessary data structures
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#include	<assert.h>
#include	<string.h>
#include	"libExt.h"
#include	"Mesh.h"

static	int vertexStride = sizeof ( Vertex );
static	Vertex	_v;

static	int offsets [] =
{
	((int)&_v.pos)   - ((int)&_v),			// tagVertex
	((int)&_v.n)     - ((int)&_v),			// tagNormal
	((int)&_v.color) - ((int)&_v),			// tagColor
	((int)&_v.tex)   - ((int)&_v),			// tagTexCoord
	((int)&_v.t)     - ((int)&_v),			// tagTangent
	((int)&_v.b)     - ((int)&_v)			// tagBinormal
};

void	computeTangents ( Vertex& v0, const Vertex& v1, const Vertex& v2 );

////////////////////////// Mesh methods /////////////////////////////////

Mesh :: Mesh  ( const char * theName, Vertex * theVertices, int nv, Face * theFaces, int nf, bool copy  )
{
	name         = theName;
	numVertices  = nv;
	numFaces     = nf;
	numEdges     = 0;
	edges        = NULL;
	vertexBuffer = 0;
	indexBuffer  = 0;

	if ( copy )
	{
		vertices = new Vertex [numVertices];
		faces    = new Face   [numFaces   ];

		memcpy ( vertices, theVertices, numVertices * sizeof ( Vertex ) );
		memcpy ( faces,    theFaces,    numFaces    * sizeof ( Face   ) );
	}
	else
	{
		vertices = theVertices;
		faces    = theFaces;
	}

	computeEdges ();

	for ( int i = 0; i < numVertices; i++ )
		boundingBox.addVertex ( Vector3D ( vertices [i].pos.x, vertices [i].pos.y, vertices [i].pos.z ) );
}

Mesh :: ~Mesh ()
{
	delete vertices;
	delete faces;
	delete edges;

	if ( vertexBuffer != 0 )
		glDeleteBuffersARB ( 1, &vertexBuffer );

	if ( indexBuffer != 0 )
		glDeleteBuffersARB ( 1, &indexBuffer  );
}

											// add assignment for source data to outputs
bool	Mesh :: addCoordAssignment ( int from, int to )
{
	if ( from < tagVertex || from > tagBinormal )
		return false;

	if ( to == tagVertex || to == tagNormal || to == tagColor || (to >= tagTex0 && to <= tagTex7) )
	{
		mapping.push_back ( make_pair ( from, to ) );

		return true;
	}

	return false;
}

void	Mesh :: createBuffers ()
{
                                            // create vertex data VBO
    glGenBuffersARB ( 1, &vertexBuffer );
    glBindBufferARB ( GL_ARRAY_BUFFER_ARB, vertexBuffer );
    glBufferDataARB ( GL_ARRAY_BUFFER_ARB, numVertices * sizeof ( Vertex ), vertices, GL_STREAM_DRAW_ARB );

                                            // create index data VBO
	int * buffer = new int [numFaces * 3];

	for ( int i = 0, j = 0; i < numFaces; i++ )
		for ( int k = 0; k < 3; k++ )
			buffer [j++] = faces [i].index [k];

    glGenBuffersARB ( 1, &indexBuffer );
    glBindBufferARB ( GL_ELEMENT_ARRAY_BUFFER_ARB, indexBuffer );
    glBufferDataARB ( GL_ELEMENT_ARRAY_BUFFER_ARB, numFaces * 3 * sizeof ( int ), buffer, GL_STATIC_DRAW_ARB );

                                            // unbind buffers
    glBindBufferARB ( GL_ARRAY_BUFFER_ARB,         0 );
    glBindBufferARB ( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );

	delete buffer;
}

											// render the mesh
void	Mesh :: render ()
{
	preRender ();
                                                // request draw
    glDrawElements  ( GL_TRIANGLES, 3*numFaces, GL_UNSIGNED_INT, 0 );

	postRender ();
}

void	Mesh :: preRender ()
{
											// save state
    glPushClientAttrib  ( GL_CLIENT_VERTEX_ARRAY_BIT );

    										// setup vertex buffer
    glBindBufferARB     ( GL_ARRAY_BUFFER_ARB, vertexBuffer );

											// go through the list of mappings
	for ( list <pair <int, int> > :: iterator it = mapping.begin (); it != mapping.end (); ++it )
	{
		int	from = it -> first;
		int	to   = it -> second;

		if ( to == tagVertex )
		{
		    glEnableClientState ( GL_VERTEX_ARRAY );
		    glVertexPointer     ( 4, GL_FLOAT, vertexStride, (void *) offsets [from] );
		}
		else
		if ( to == tagNormal )
		{
		    glEnableClientState ( GL_NORMAL_ARRAY );
		    glNormalPointer     ( GL_FLOAT, vertexStride, (void *) offsets [from] );
		}
		else
		if ( to == tagColor )
		{
		    glEnableClientState ( GL_COLOR_ARRAY );
		    glColorPointer      ( 4, GL_FLOAT, vertexStride, (void *) offsets [from] );
		}
		else
		if ( to >= tagTex0 && to <= tagTex7 )
		{
		    glClientActiveTextureARB ( GL_TEXTURE0_ARB + to - tagTex0 );
		    glEnableClientState      ( GL_TEXTURE_COORD_ARRAY );

		    if ( from == tagTexCoord )
			    glTexCoordPointer ( 2, GL_FLOAT, vertexStride, (void *) offsets [tagTexCoord] );
			else
				glTexCoordPointer ( 3, GL_FLOAT, vertexStride, (void *) offsets [from] );
		}
	}
												// setup index buffer
    glBindBufferARB ( GL_ELEMENT_ARRAY_BUFFER_ARB, indexBuffer );

	material.bind ();
}

void	Mesh :: postRender ()
{
                                                // unbind array buffer
    glBindBufferARB ( GL_ARRAY_BUFFER_ARB,         0 );
    glBindBufferARB ( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );

    glPopClientAttrib ();
}

											// compute tangents and binormals from texCoords
void	Mesh :: computeTangents ()
{
	for ( int i = 0; i < numFaces; i++ )
	{
		Face&	f = faces [i];

		:: computeTangents ( vertices [f.index [0]], vertices [f.index [1]], vertices [f.index [2]] );
	}
}
											// compute edge data from vertices and faces
void	Mesh :: computeEdges ()
{
	int	maxEdges = 3 * numFaces;

	if ( edges != NULL )
		delete edges;

	numEdges = 0;
	edges    = new Edge [maxEdges];

	for ( int i = 0; i < numFaces; i++ )
		for ( int j = 0; j < 3; j++ )
			addEdge ( faces [i].index [j], faces [i].index [(j+1) % 3], i );
}

void	Mesh :: addEdge ( int v1, int v2, int face )
{
	for ( int i = 0; i < numEdges; i++ )
	{
		bool	b1 = ( edges [i].a == v1) && (edges [i].b == v2 );
		bool	b2 = ( edges [i].a == v2) && (edges [i].b == v1 );

		if ( b1 || b2 )
		{
			edges [i].f2 = (b1 ? face : face | 0x80000000);

			return;
		}
	}

	Edge&	edge = edges [numEdges];

	edge.a  = v1;
	edge.b  = v2;
	edge.f1 = face;
	edge.f2 = -1;

	numEdges++;
}

static	inline  Vector3D    cross ( const Vector4D& v1, const Vector4D& v2 )
{
    return Vector3D ( v1.x, v1.y, v1.z ) ^ Vector3D ( v2.x, v2.y, v2.z );
}

void	Mesh :: computeFaceNormals ()
{
	if ( faces == NULL || vertices == NULL )
		return;

	for ( int i = 0 ; i < numFaces; i++ )
	{
		Vector3D	n = cross ( vertices [faces[i].index [1]].pos - vertices [faces[i].index [0]].pos,
                                vertices [faces[i].index [2]].pos - vertices [faces[i].index [0]].pos );

        faces [i].n = n.normalize ();
	}
}

void	Mesh :: computeVertexNormals ()
{
	int	i;
											// set vertex normal to zero
	for ( i = 0; i < numVertices; i++ )
		vertices [i].n = Vector3D ( 0, 0, 0 );
											// sum normals for all faces passing throgh vertex
	for ( i = 0; i < numFaces; i++ )
		for ( int j = 0; j < 3; j++ )
		{
			int	index = faces [i].index [j];
			
			vertices [index].n += faces [i].n;
		}
											// normalize sums
	for ( i = 0; i < numVertices; i++ )
		vertices [i].n.normalize ();
}

////////////////////////////////////////////////////////////////////////////

void	computeTangents ( Vertex& v0, const Vertex& v1, const Vertex& v2 )
{
	Vector3D	e0 ( v1.pos.x - v0.pos.x, v1.tex.x - v0.tex.x, v1.tex.y - v0.tex.y );
	Vector3D	e1 ( v2.pos.x - v0.pos.x, v2.tex.x - v0.tex.x, v2.tex.y - v0.tex.y );
	Vector3D	cp = e0 ^ e1;

	if ( fabs ( cp.x ) > EPS )
	{
		v0.t.x = -cp.y / cp.x;
		v0.b.x = -cp.z / cp.x;
	}
	else
	{
		v0.t.x = 0;
		v0.b.x = 0;
	}

	e0.x = v1.pos.y - v0.pos.y;
	e1.x = v2.pos.y - v0.pos.y;
	cp   = e0 ^ e1;

	if ( fabs ( cp.x ) > EPS )
	{
		v0.t.y = -cp.y / cp.x;
		v0.b.y = -cp.z / cp.x;
	}
	else
	{
		v0.t.y = 0;
		v0.b.y = 0;
	}

	e0.x = v1.pos.z - v0.pos.z;
	e1.x = v2.pos.z - v0.pos.z;
	cp   = e0 ^ e1;

	if ( fabs ( cp.x ) > EPS )
	{
		v0.t.z = -cp.y / cp.x;
		v0.b.z = -cp.z / cp.x;
	}
	else
	{
		v0.t.z = 0;
		v0.b.z = 0;
	}

	if ( ( ( v0.t ^ v0.b ) & v0.n ) < 0 )
		v0.t = -v0.t;
}
