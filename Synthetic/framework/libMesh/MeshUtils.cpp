//
// Mesh utils - create some common meshes
//

#include	<assert.h>
#include	<string.h>
#include	"MeshUtils.h"

static	inline  Vector3D    cross ( const Vector4D& v1, const Vector4D& v2 )
{
    return Vector3D ( v1.x, v1.y, v1.z ) ^ Vector3D ( v2.x, v2.y, v2.z );
}

Mesh * makeTorus  ( float r1, float r2, int numRings, int numSides )
{
    float   ringDelta = 2.0f * M_PI / numRings;
    float   sideDelta = 2.0f * M_PI / numSides;
    float   invRings  = 1.0f / (float) numRings;
    float   invSides  = 1.0f / (float) numSides;
    int     i, j;

    Vertex * vertices = new Vertex [(numSides+1)*(numRings+1)];
    Face   * faces    = new Face   [numSides*numRings*2];
    int		 numVertices;
    int		 numFaces;

                                // create vertices
    int index = 0;

    for ( i = 0; i <= numRings; i++ )
    {
        float theta    = i * ringDelta;
        float cosTheta = cos ( theta );
        float sinTheta = sin ( theta );

        for ( j = 0; j <= numSides; j++ )
        {
            float phi    = j * sideDelta;
            float cosPhi = cos ( phi );
            float sinPhi = sin ( phi );
            float dist = r2 + r1 * cosPhi;

        			// (x,y,z) below is the parametric equation for a torus
			        // when theta and phi both vary from 0 to pi.
                    // x = cos(theta) * (R + r * cos(phi))
                    // y = -sin(theta) * (R + r * cos(phi))
                    // z = r * sin(phi)

            vertices [index].pos.x = cosTheta * dist;
            vertices [index].pos.y = -sinTheta * dist;
            vertices [index].pos.z = r1 * sinPhi;
            vertices [index].pos.w = 1;

			      	// now setup texture coordinates

            vertices [index].tex.x = j * invSides;
            vertices [index].tex.y = i * invRings;

			vertices [index].color = Vector4D ( 1, 1, 1, 1 );

			      	// normalize the partial derivative of (x,y,z) with respect to theta.
			      	// T = normalize([dx/dtheta,dy/dtheta,dz/dtheta])

            float   dxdtheta = -sinTheta * dist;
            float   dydtheta = -cosTheta * dist;
            float   d        = 1 / sqrt ( dxdtheta*dxdtheta + dydtheta*dydtheta );

            vertices [index].t.x = dxdtheta * d;
            vertices [index].t.y = dydtheta * d;
            vertices [index].t.z = 0;

			      	// compute unit normal

            vertices [index].n.x =  cosTheta * cosPhi;
            vertices [index].n.y = -sinTheta * cosPhi;
            vertices [index].n.z = sinPhi;

        			// B = N cross T

            vertices [index].b = vertices [index].n ^ vertices [index].t;

        			// Note:  by the construction above, [T, B, N] form an orthonormal space.

            index++;
        }
    }

    numVertices = index;

                                            // Create faces
    index = 0;

    for ( i = 0; i < numRings; i++ )
        for ( j = 0; j < numSides; j++, index += 2 )
        {
            faces [index].index [0] = (i       ) * (numSides+1) + j;
            faces [index].index [2] = ((i + 1) ) * (numSides+1) + j;
            faces [index].index [1] = ((i + 1) ) * (numSides+1) + (j + 1);
            faces [index].n         = cross ( vertices [faces[index].index [1]].pos - vertices [faces[index].index [0]].pos,
                                              vertices [faces[index].index [2]].pos - vertices [faces[index].index [0]].pos );

            faces [index+1].index [0] = (i       ) * (numSides+1) + j;
            faces [index+1].index [2] = ((i + 1) ) * (numSides+1) + (j + 1);
            faces [index+1].index [1] = (i       ) * (numSides+1) + (j + 1);
            faces [index+1].n         = cross ( vertices [faces[index+1].index [1]].pos - vertices [faces[index+1].index [0]].pos,
                                                vertices [faces[index+1].index [2]].pos - vertices [faces[index+1].index [0]].pos );

			assert ( (faces [index].n   & vertices [faces[index].index [0]].n)   > 0 );
			assert ( (faces [index+1].n & vertices [faces[index+1].index [0]].n) > 0 );
        }

    numFaces = index;

	Mesh * torus = new Mesh ( "torus", vertices, numVertices, faces, numFaces, false );

	return torus;
}

Mesh * makeShape1 ( float r, float h, int n1, int n2, float factor )
{
    Vertex * vertices = new Vertex [(n1)*(n2)];
    Face   * faces    = new Face   [n1*n2*2];
    int		 numVertices;
    int		 numFaces;
    int		 i, j;
                                // create vertices
    int index = 0;

	for ( i = 0; i < n1; i++ )
	{
		float	x  = 2 * h * i / (float) n1;
		float	r1 = r * pow ( 1 - fabs ( 1 - x / h ), factor );

		for ( j = 0; j < n2; j++ )
		{
			float	phi = 2 * M_PI * j / (float) n2;
			float	s   = r1 * sin ( phi );
			float	c   = r1 * cos ( phi );

			vertices [index].pos   = Vector3D ( x - h, c, s );
            vertices [index].tex.x = i / (float) n1;
            vertices [index].tex.y = j / (float) n2;
			vertices [index].color = Vector4D ( 1, 1, 1, 1 );

			index++;
		}
	}

	numVertices = index;
                                            // create faces
    index = 0;

    for ( i = 0; i < n1 - 1; i++ )
        for ( j = 0; j < n2; j++, index += 2 )
        {
            faces [index].index [0] = i       * n2 + j;
            faces [index].index [2] = (i + 1) * n2 + j;
            faces [index].index [1] = (i + 1) * n2 + ((j + 1) % n2);
            faces [index].n         = cross ( vertices [faces[index].index [1]].pos - vertices [faces[index].index [0]].pos,
                                               vertices [faces[index].index [2]].pos - vertices [faces[index].index [0]].pos );

            faces [index+1].index [0] = i       * n2 + j;
            faces [index+1].index [2] = (i + 1) * n2 + ((j + 1) % n2);
            faces [index+1].index [1] = i       * n2 + ((j + 1) % n2);
            faces [index+1].n         = cross ( vertices [faces[index+1].index [1]].pos - vertices [faces[index+1].index [0]].pos,
                                                vertices [faces[index+1].index [2]].pos - vertices [faces[index+1].index [0]].pos );
        }

    numFaces = index;

	Mesh * mesh = new Mesh ( "mesh", vertices, numVertices, faces, numFaces, false );

	mesh -> computeTangents ();

	return mesh;
}

Mesh * makeRevSurface ( const Vector2D d [], float tex [], int n, int n2 )
{
    Vertex * vertices = new Vertex [n*n2];
    Face   * faces    = new Face   [n*n2*2];
    int		 numVertices;
    int		 numFaces;
    int		 i, j;
                                // create vertices
    int index = 0;

	for ( i = 0; i < n; i++ )
	{
		float	x = d [i].x;
		float	r = d [i].y;

		for ( j = 0; j < n2; j++ )
		{
			float	phi = 2 * M_PI * j / (float) n2;
			float	s   = r * sin ( phi );
			float	c   = r * cos ( phi );

			vertices [index].pos   = Vector3D ( x, c, s );
            vertices [index].tex.x = tex [i];
            vertices [index].tex.y = j / (float) n2;
			vertices [index].color = Vector4D ( 1, 1, 1, 1 );

			index++;
		}
	}

	numVertices = index;
                                            // create faces
    index = 0;

    for ( i = 0; i < n - 1; i++ )
        for ( j = 0; j < n2; j++, index += 2 )
        {
            faces [index].index [0] = i       * n2 + j;
            faces [index].index [2] = (i + 1) * n2 + j;
            faces [index].index [1] = (i + 1) * n2 + ((j + 1) % n2);
            faces [index].n         = cross ( vertices [faces[index].index [1]].pos - vertices [faces[index].index [0]].pos,
                                              vertices [faces[index].index [2]].pos - vertices [faces[index].index [0]].pos );

            faces [index+1].index [0] = i       * n2 + j;
            faces [index+1].index [2] = (i + 1) * n2 + ((j + 1) % n2);
            faces [index+1].index [1] = i       * n2 + ((j + 1) % n2);
            faces [index+1].n         = cross ( vertices [faces[index+1].index [1]].pos - vertices [faces[index+1].index [0]].pos,
                                                vertices [faces[index+1].index [2]].pos - vertices [faces[index+1].index [0]].pos );

//			assert ( (faces [index].n   & vertices [faces[index].index [0]].n)   > 0 );
//			assert ( (faces [index+1].n & vertices [faces[index+1].index [0]].n) > 0 );
        }

    numFaces = index;

	Mesh * mesh = new Mesh ( "mesh", vertices, numVertices, faces, numFaces, false );

	mesh -> computeTangents ();

	return mesh;
}

Mesh * makeLathedSurface ( const Vector3D line [], int n1, const Vector2D contour [], int n2, const Vector2D& center, const Vector3D& up )
{
    Vertex * vertices = new Vertex [n1*n2];
    Face   * faces    = new Face   [n1*n2*2];
    int		 numVertices;
    int		 numFaces;
    int		 i, j;
                                // create vertices
    int 		index = 0;
    Vector3D	t;				// tangent vector along line
    Vector3D	u;
    Vector3D	b;

	for ( i = 0; i < n1; i++ )
	{
		if ( i + 1 < n1 )		// find tangent
			t = (line [i+1] - line [i]).normalize ();

								// create up and binormal vectors, orthogonal to t
								// thus formaing ON basis
		u = ( up - t * ( t & up ) ).normalize ();
		b = t ^ u;
		
		for ( j = 0; j < n2; j++ )
		{
			Vector3D	v ( contour [j].x - center.x, contour [j].y - center.y, 0 );

			vertices [index].pos   = line [i] + Vector3D ( v & t, v & u, v & b );
            vertices [index].tex.x = float ( i ) / float ( n1 );
            vertices [index].tex.y = float ( j ) / float ( n2 );
			vertices [index].color = Vector4D ( 1, 1, 1, 1 );

			index++;
		}
	}

	numVertices = index;

                                            // create faces
    index = 0;

    for ( i = 0; i < n1 - 1; i++ )
        for ( j = 0; j < n2; j++, index += 2 )
        {
            faces [index].index [0] = i       * n2 + j;
            faces [index].index [2] = (i + 1) * n2 + j;
            faces [index].index [1] = (i + 1) * n2 + ((j + 1) % n2);
            faces [index].n         = cross ( vertices [faces[index].index [1]].pos - vertices [faces[index].index [0]].pos,
                                              vertices [faces[index].index [2]].pos - vertices [faces[index].index [0]].pos );

            faces [index+1].index [0] = i       * n2 + j;
            faces [index+1].index [2] = (i + 1) * n2 + ((j + 1) % n2);
            faces [index+1].index [1] = i       * n2 + ((j + 1) % n2);
            faces [index+1].n         = cross ( vertices [faces[index+1].index [1]].pos - vertices [faces[index+1].index [0]].pos,
                                                vertices [faces[index+1].index [2]].pos - vertices [faces[index+1].index [0]].pos );

//			assert ( (faces [index].n   & vertices [faces[index].index [0]].n)   > 0 );
//			assert ( (faces [index+1].n & vertices [faces[index+1].index [0]].n) > 0 );
        }

    numFaces = index;

	Mesh * mesh = new Mesh ( "mesh", vertices, numVertices, faces, numFaces, false );

	mesh -> computeTangents ();

	return mesh;
}

static	bool	reallocVertices ( Vertex *& outVertices, int numOutVertices, int& maxOutVertices, int delta )
{
	maxOutVertices += delta;

	Vertex * newVertices = new Vertex [maxOutVertices];

	memcpy ( newVertices, outVertices, numOutVertices * sizeof ( Vertex ) );

	delete outVertices;

	outVertices = newVertices;

	return true;
}

Vertex * fixFaces ( Vertex * vertices, int& numVertices, Face * faces, Face * texFaces, int numFaces, Vector2D * texVertices, int numTexVertices )
{
	int	     numOutVertices   = numTexVertices;
	Vertex * outVertices      = new Vertex [2*numTexVertices];
	int	   * vertexAssignment = new int    [numTexVertices];
	int		 maxOutVertices   = 2*numTexVertices;
	int		 i, j;

	for ( i = 0; i < numTexVertices; i++ )
		vertexAssignment [i] = -1;

	for ( i = 0; i < numFaces; i++ )
	{
		//faces [i].n = texFaces [i].n;

		for ( j = 0; j < 3; j++ )
		{
			int	vi = faces    [i].index [j];
			int	ti = texFaces [i].index [j];

											// map vertex to texVertex
											// if not assigned yet, do assignment ti -> vi
			if ( vertexAssignment [ti] == -1 )
			{
				outVertices [vi]       = vertices    [vi];
				outVertices [vi].tex   = texVertices [ti];
				outVertices [vi].color = Vector4D ( 1, 1, 1, 1 );
				vertexAssignment [ti]  = vi;
			}
			else						// if already assigned to smth different
										// create new vertex
			if ( outVertices [vi].tex != texVertices [ti] )
			{
				if ( numOutVertices >= maxOutVertices )
					reallocVertices ( outVertices, numOutVertices, maxOutVertices, numTexVertices );

				faces [i].index [j]                = numOutVertices;
				outVertices [numOutVertices]       = vertices    [vi];
				outVertices [numOutVertices].tex   = texVertices [ti];
				outVertices [numOutVertices].color = Vector4D ( 1, 1, 1, 1 );

				numOutVertices++;
			}
		}
	}

	delete vertexAssignment;

	numVertices = numOutVertices;

	return outVertices;
}


