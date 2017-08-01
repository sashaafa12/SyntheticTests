//
// Basic typedefs for data types
//
// Author: Alex V. Boreskoff
//

#ifndef	__TYPEDEFS__
#define	__TYPEDEFS__

typedef	unsigned char		uint8;
typedef	signed	 char		int8;
typedef	unsigned short		uint16;
typedef	signed	 short		int16;
typedef	unsigned long		uint32;
typedef	signed	 long		int32;

#ifdef	_WIN32
typedef	unsigned __int64	uint64;
typedef	signed	 __int64	int64;
#else
typedef	unsigned long long	uint64;
typedef signed   long long	int64;
#endif

typedef	unsigned char		byte;
typedef	uint16				word;
typedef	uint32				dword;

#endif
