//
// Simple class to access data in a rar file using unrarlib
//

#ifndef	__RAR_FILE_SYSTEM__
#define	__RAR_FILE_SYSTEM__

#ifdef	_WIN32
    #pragma	warning (disable:4786)		// FUCK these stuppid bastards !!!
#endif    

#include	"FileSystem.h"
#include	<string>
#include	<list>

using namespace std;

class	Data;

class	RarFileSystem : public FileSystem
{
private:
	struct	RarFileInfo
	{
		string	name;
		long	size;
	};

	typedef	list <RarFileInfo>	RarDir;

	string	fileName;								// name of zip file
	RarDir	dir;									// contains directory of zip file
	bool	broken;

public:
	RarFileSystem ( const string& rarName );

	virtual	bool	isOk () const
	{
		return !broken;
	}

	virtual	Data  * getFile  ( const string& name );
};

#endif
