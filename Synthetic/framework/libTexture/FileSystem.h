//
// Interface of a class to access data (virtual file system)
//

#ifndef	__FILE_SYSTEM__
#define	__FILE_SYSTEM__

#include	<string>

using namespace std;

#ifdef	_WIN32
	#pragma warning (disable:4996)
#endif

class	Data;

class	FileSystem
{
public:
	FileSystem () {}
	virtual ~FileSystem () {}

	virtual	bool	isOk () const
	{
		return false;
	}

	virtual	Data  * getFile  ( const string& name ) = 0;
};

class	Folder : public FileSystem
{
	string	folder;

public:
	Folder ( const string& theFolder );

	virtual	bool	isOk () const
	{
		return true;
	}

	virtual	Data  * getFile  ( const string& name );
};

#endif
