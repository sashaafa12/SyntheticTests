//
// Simple class to access data in a zip file
//

#ifndef	__ZIP_FILE_SYSTEM__
#define	__ZIP_FILE_SYSTEM__

#ifdef	_WIN32
    #pragma	warning (disable:4786)		// FUCK these stuppid bastards !!!
#endif    

#include	"FileSystem.h"
#include	<string>
#include	<list>

using namespace std;

class	Data;

#pragma pack (push, 1)					// save current pack, set 1-byte packing

struct	ZipLocalFileHeader
{
	unsigned long	signature;
	unsigned short	versionNeededToExtract;
	unsigned short	generalPurposeBitFlag;
	unsigned short	compressionMethod;
	unsigned short	lastModFileTime;
	unsigned short	lastModFileDate;
	unsigned long	crc32;
	long			compressedSize;
	long			uncompressedSize;
	unsigned short	filenameLength;
	unsigned short	extraFieldLength;
};

struct	ZipDataDescriptor
{
	unsigned long	crc32;
	unsigned long	compressedSize;
	unsigned long	uncompressedSize;
};

struct	ZipCentralHeader				// of central header
{
	unsigned long	signature;
	unsigned short	versionMadeBy;
	unsigned short	versionNeededToExtract;
	unsigned short	generalPurposeBitFlag;
	unsigned short	compressionMethod;
	unsigned short	lastModFileTime;
	unsigned short	lastModFileDate;
	unsigned long	crc32;
	long			compressedSize;
	long			uncompressedSize;
	unsigned short	filenameLength;
	unsigned short	extraFieldLength;
	unsigned short  commentLength;
	unsigned short	diskNumberStart;
	unsigned short	internalFileAttibutes;
	unsigned long	externalFileAttributes;
	long			relativeLocalHeaderOffset;
};

struct	ZipEndOfCentralDir
{
	unsigned long	signature;
	unsigned short	diskNo;
	unsigned short	centralDirDiskNo;
	unsigned short	numEntriesOnDisk;
	unsigned short	numEntries;
	unsigned long	centralDirSize;
	long			centralDirOffset;
	unsigned short	commentLength;
};

#pragma	pack (pop)

class	ZipFileSystem : public FileSystem
{
private:
	typedef	list <pair<string,ZipCentralHeader> >	ZipDir;

	string	fileName;								// name of zip file
	ZipDir	dir;									// contains directory of zip file
	bool	broken;

public:
	ZipFileSystem ( const string& zipName );

	virtual	bool	isOk () const
	{
		return !broken;
	}

	virtual	Data  * getFile  ( const string& name );

private:
	void	readDirectory ( int file );
	Data  * readEntry     ( const ZipCentralHeader& hdr );
};

#endif
