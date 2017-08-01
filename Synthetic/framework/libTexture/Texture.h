//
// Simple texture class
//

#ifndef __TEXTURE__
#define __TEXTURE__

#include    "TypeDefs.h"

class   Texture
{
protected:
    int         width;
    int         height;
    int         numComponents;
    int         bytesPerChannel;
    int         format;
    byte      * data;
    int         levels;
    bool        compressed;

public:
    Texture  ();
    Texture  ( int theWidth, int theHeight, int theNumComponents, int theBytesPerChannel = 1 );
    virtual ~Texture ();

    int getWidth () const
    {
        return width;
    }

    int getHeight () const
    {
        return height;
    }

    int getNumComponents () const
    {
        return numComponents;
    }

    int getFormat () const
    {
        return format;
    }

    int getLevels () const
    {
        return levels;
    }

    bool    isCompressed () const
    {
        return compressed;
    }

    int getBytesPerLine () const
    {
        return width * numComponents;
    }

    byte * getData () const
    {
        return data;
    }

    void    setFormat ( int newFormat )
    {
        format = newFormat;
    }

    void    putLine    ( int y, dword * bits );

    virtual bool    upload ( int target, bool mipmap = true );
};

inline  dword   makeRgba ( int red, int green, int blue, int alpha = 255 )
{
    return red + (green << 8) + (blue << 16) + (alpha << 24);
}

#endif
