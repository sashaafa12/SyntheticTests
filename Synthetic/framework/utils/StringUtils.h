//
// Simple string utilities
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#ifndef	__STRING_UTILS__
#define	__STRING_UTILS__

#ifdef	_WIN32
	#pragma	warning (disable:4786)
#endif

#include	<string>

using namespace std;

string	trim        ( const string& str );
string	replaceTabs ( const string& str );
void	parseString ( const string& str, string& cmd, string& args );
string	dequote     ( const string& str );

#endif
