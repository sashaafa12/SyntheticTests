//
// Hierarchical meshes
//
// Author: Alex V. Boreskoff
//

#ifndef	__HIERARCHICAL_MESH__
#define	__HIERARCHICAL_MESH__

#ifdef	_WIN32
	#pragma	warning (disable:4786)
#endif

#include	<list>
#include	<string>

using namespace std;

#include	"Vector3D.h"
#include	"Matrix3D.h"

class	Mesh;
class	NodeVisitor;

class	MeshNode
{
public:
	struct	MeshLink					// structure to represent a submesh with transform to it
	{
		MeshNode  * node;
		Vector3D	offset;				// do translation
		Matrix3D	matr;
		string		name;
	};

	typedef list <MeshLink *> Links;

protected:
	Mesh  * mesh;
	Links	links;

public:
	MeshNode  ( Mesh * theMesh );
	~MeshNode ();

										// attach subnode
	bool	attach ( const char * linkName, Mesh     * subMesh, const Vector3D& offs, const Matrix3D& m );
	bool	attach ( const char * linkName, MeshNode * node,    const Vector3D& offs, const Matrix3D& m );
	bool	attach ( MeshLink * node );

	Mesh * getMesh () const
	{
		return mesh;
	}

	void	setMesh ( Mesh * newMesh );
											// get direct subnode
	MeshNode :: MeshLink * nodeWithName ( const char * name ) const;

	void	render ();					// render all hierarchy

										// perform hierarchy traversal
	bool	visit ( NodeVisitor& visitor );

										// iterating through links
	Links :: const_iterator	begin () const
	{
		return links.begin ();
	}

	Links :: const_iterator end () const
	{
		return links.end ();
	}
};

#endif
