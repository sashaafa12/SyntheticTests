//
// Simple interface for MeshNode visitor class
//

#ifndef	__MESHNODE_VISITOR__
#define	__MESHNODE_VISITOR__

#include	"MeshNode.h"

class	NodeVisitor
{
public:
	NodeVisitor () {}
	virtual ~NodeVisitor () {}
	
	virtual	bool	visit     ( MeshNode * node ) = 0;
	virtual	bool	visitLink ( MeshNode :: MeshLink * node ) = 0;
};

#endif
