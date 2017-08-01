//
// AttrList class definition
//

#ifndef	__ATTR_LIST__
#define	__ATTR_LIST__

#define	MAX_ATTRIBS		20
#define	MAX_PFORMATS	20

template <typename T>
class	AttrList
{
	T	attrs [2*MAX_ATTRIBS];
	int	numAttrs;
public:
	AttrList ()
	{
		numAttrs = 0;

		clear ();
	}

	void	clear ()
	{
		for ( int i = 0; i < 2*MAX_ATTRIBS; i++ )
			attrs [i] = 0;
	}

	void	add ( int attr, T value )
	{
		attrs [2*numAttrs    ] = attr;
		attrs [2*numAttrs + 1] = value;
		numAttrs++;
	}

	int	getNumAttrs () const
	{
		return numAttrs;
	}

	const T * getAttrs () const
	{
		return attrs;
	}
};

#endif
