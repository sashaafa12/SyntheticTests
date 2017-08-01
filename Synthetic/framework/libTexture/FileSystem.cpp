//
// Interface of a class to access data (virtual file system)
//

#include	"FileSystem.h"
#include	"Data.h"
#include	"libTexture.h"

Folder :: Folder ( const string& theFolder ) : FileSystem (), folder ( theFolder ) 
{

}

Data  * Folder :: getFile  ( const string& name )
{
	string	newName = buildFileName ( folder, name );

	return new Data ( newName.c_str () );
}

