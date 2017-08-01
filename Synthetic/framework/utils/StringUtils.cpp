//
// Simple string utilities
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#include	<ctype.h>
#include	"StringUtils.h"

string	trim ( const string& str )
{
	int	len = str.length ();
	int	i, j;

	for ( i = 0; i < len && isspace ( str [i] ); i++ )
		;

	for ( j = len - 1; j >= 0 && isspace ( str [j] ); j-- )
		;

	return str.substr ( i, j - i + 1 );
}

string	replaceTabs ( const string& str )
{
	int		len = str.length ();
	string	s   = str;

	for ( int i = 0; i < len; i++ )
		if ( s [i] == '\t' )
			s [i] = ' ';

	return s;
}

void	parseString ( const string& str, string& cmd, string& args )
{
	int	len = str.length ();
	int	pos;

	for ( pos = 0; pos < len && str [pos] != ' ' && str [pos] != '\t'; pos++ )
		;

	cmd  = str.substr ( 0, pos );
	args = replaceTabs ( trim ( str.substr ( pos ) ) );
}

string	dequote ( const string& str )
{
	if ( str [0] == '\"' || str [0] == '\'' )			// remove quotes
		return str.substr ( 1, str.length () - 2 );

	return str;
}
