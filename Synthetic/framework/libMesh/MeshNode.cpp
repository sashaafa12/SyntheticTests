//
// Hierarchical meshes
//
// Author: Alex V. Boreskoff
//

#ifdef	_WIN32
	#include	<windows.h>
#endif

#include	"libExt.h"
#include	"MeshNode.h"
#include	"Mesh.h"
#include	"NodeVisitor.h"

MeshNode :: MeshNode  ( Mesh * theMesh )
{
	mesh = theMesh;
}

MeshNode :: ~MeshNode ()
{
	delete mesh;

	for ( list <MeshLink *> :: iterator it = links.begin (); it != links.end (); ++it )
	{
		delete (*it) -> node;
		delete *it;
	}
}

void	MeshNode :: setMesh ( Mesh * newMesh )
{
    delete mesh;
    
    mesh = newMesh;
}
										// attach subnode
bool	MeshNode :: attach ( const char * linkName, Mesh * subMesh, const Vector3D& offs, const Matrix3D& m )
{
	MeshLink * link = new MeshLink;

	link -> name     = linkName;
	link -> node     = new MeshNode ( subMesh );
	link -> offset   = offs;
	link -> matr     = m;

	links.push_front ( link );

	return true;
}

bool	MeshNode :: attach ( const char * linkName, MeshNode * node, const Vector3D& offs, const Matrix3D& m )
{
	MeshLink * link = new MeshLink;

	link -> name     = linkName;
	link -> node     = node;
	link -> offset   = offs;
	link -> matr     = m;

	links.push_front ( link );

	return true;
}

bool	MeshNode :: attach ( MeshLink * node )
{
	links.push_front ( node );

	return true;
}

										// get direct subnode
MeshNode :: MeshLink * MeshNode :: nodeWithName ( const char * name ) const
{
	for ( list <MeshLink *> :: const_iterator it = links.begin (); it != links.end (); ++it )
		if ( (*it) -> name == name )
			return *it;

	return NULL;
}
										// render all hierarchy
void	MeshNode :: render ()
{
	if ( mesh != NULL )
		mesh -> render ();

	glMatrixMode ( GL_MODELVIEW );

	for ( list <MeshLink *> :: iterator it = links.begin (); it != links.end (); ++it )
	{
		MeshLink  * link = *it;
		float		m [16];

										// create appropriate transform matrix
		link -> matr.getHomMatrix ( m, link -> offset );

		glPushMatrix  ();
		glMultMatrixf ( m );

		if ( link -> node != NULL )
			link -> node -> render ();

		glMatrixMode ( GL_MODELVIEW );
		glPopMatrix  ();
	}
}

										// perform hierarchy traversal
bool	MeshNode :: visit ( NodeVisitor& visitor )
{
	if ( !visitor.visit ( this ) )
		return false;

	for ( list <MeshLink *> :: iterator it = links.begin (); it != links.end (); ++it )
		if ( !visitor.visitLink ( *it ) )
			return false;

	return true;
}

