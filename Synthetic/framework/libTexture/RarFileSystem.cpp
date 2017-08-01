//
// Simple class to access data in a rar file using unrarlib
//

#include	"unrarlib.h"
#include	"RarFileSystem.h"
#include	"Data.h"

RarFileSystem :: RarFileSystem ( const string& rarName ) : fileName ( rarName )
{
	int                   fileCount = 0;
	ArchiveList_struct  * lst       = NULL;
	ArchiveList_struct  * item      = NULL;
	RarFileInfo			  info;

	fileCount = urarlib_list ( (void *)rarName.c_str (), (ArchiveList_struct*)&lst );
	item      = lst;

	if ( lst == NULL )
	{
		broken = true;

		return;
	}

	for ( int i = 0; i < fileCount; i++ )
	{
		info.name = item -> item.Name;
		info.size = item -> item.UnpSize;
		item      = (ArchiveList_struct*) item -> next;

		dir.push_back ( info );
	}

	urarlib_freelist ( lst );
}

Data  * RarFileSystem :: getFile ( const string& name )
{
	char            * dataPtr;			// pointers for the decompressed data
	unsigned long   dataSize;           // size of the decompressed

	if ( urarlib_get ( &dataPtr, &dataSize, (char *)name.c_str (), (void *)fileName.c_str (), "" ) )
		return new Data ( dataPtr, dataSize );

	return NULL;
}

